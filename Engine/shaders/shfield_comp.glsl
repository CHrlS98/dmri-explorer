#version 460
#extension GL_ARB_shading_language_include : require

#include "/include/shfield_util.glsl"
#include "/include/orthogrid_util.glsl"

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding=0) buffer allRadiisBuffer
{
    uint allRadiis[];
};

// TODO: Investigate ARB_shading_language_packing for compressing normals.
// https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_shading_language_packing.txt
layout(std430, binding=1) buffer allSpheresNormalsBuffer
{
    vec4 allNormals[];
};

layout(std430, binding=12) buffer allMaxAmplitudeBuffer
{
    float allMaxAmplitude[];
};

const float FLOAT_EPS = 1e-4;
const float PI = 3.14159265358979323;

// 00000000 00000000 00000000 11111111
const uint bitMask8 = 255;
// 00000000 00000000 11111111 00000000
const uint bitMask16 = bitMask8 << 8;
// 00000000 11111111 00000000 00000000
const uint bitMask24 = bitMask16 << 8;
// 11111111 00000000 00000000 00000000
const uint bitMask32 = bitMask24 << 8;

void zeroInitRadius(uint index)
{
    // 1. convert index to **true index**
    const uint trueIndex = index / 4;
    const uint bitOffset = index - trueIndex * 4;
    uint notMask;
    if(bitOffset == 0)
    {
        notMask = ~bitMask8;
    }
    else if(bitOffset == 1)
    {
        notMask = ~bitMask16;
    }
    else if(bitOffset == 2)
    {
        notMask = ~bitMask24;
    }
    else if(bitOffset == 3)
    {
        notMask = ~bitMask32;
    }

    // 2. set to 0 using atomicAnd operator
    atomicAnd(allRadiis[trueIndex], notMask);
    memoryBarrier();
}

void writeRadius(uint index, float sfEval, float maxAmplitude)
{
    const uint trueIndex = index / 4;
    const uint bitOffset = index - trueIndex * 4;
    uint value = uint(sfEval / maxAmplitude * 255.0f);  // bounded between 0-255
    if(bitOffset == 1)
    {
        value = value << 8;
    }
    else if(bitOffset == 2)
    {
        value = value << 16;
    }
    else if(bitOffset == 3)
    {
        value = value << 24;
    }
    atomicOr(allRadiis[trueIndex], value);
    memoryBarrier();
}

float readRadius(uint dirIndex, uint voxIndex)
{
    const uint trueIndex = dirIndex / 4;
    const uint bitOffset = dirIndex - trueIndex * 4;
    uint mask;
    uint radius = allRadiis[trueIndex];
    if(bitOffset == 0)
    {
        mask = bitMask8;
        radius = radius & mask;
    }
    else if(bitOffset == 1)
    {
        mask = bitMask16;
        radius = radius & mask;
        radius = radius >> 8;
    }
    else if(bitOffset == 2)
    {
        mask = bitMask24;
        radius = radius & mask;
        radius = radius >> 16;
    }
    else if(bitOffset == 3)
    {
        mask = bitMask32;
        radius = radius & mask;
        radius = radius >> 24;
    }

    return float(radius) / 255.0f * allMaxAmplitude[voxIndex];
}

bool scaleSphere(uint voxID, uint firstVertID)
{
    float sfEval;
    vec3 normal;
    float rmax;
    float maxAmplitude = 0.0f;
    const float sh0 = shCoeffs[voxID * nbCoeffs];
    const bool nonZero = sh0 > FLOAT_EPS;
    // iterate through all sphere directions to
    // find maximum amplitude
    for(uint sphVertID = 0; sphVertID < nbVertices; ++sphVertID)
    {
        sfEval = 0.0f;
        for(int i = 0; i < nbCoeffs; ++i)
        {
            sfEval += shCoeffs[voxID * nbCoeffs + i]
                    * shFuncs[sphVertID * nbCoeffs + i];
        }

        // Evaluate the max amplitude for all vertices.
        maxAmplitude = max(maxAmplitude, sfEval);
    }

    for(uint sphVertID = 0; sphVertID < nbVertices; ++sphVertID)
    {
        zeroInitRadius(firstVertID + sphVertID);
        if(maxAmplitude > FLOAT_EPS)
        {
            sfEval = 0.0f;
            for(int i = 0; i < nbCoeffs; ++i)
            {
                sfEval += shCoeffs[voxID * nbCoeffs + i]
                        * shFuncs[sphVertID * nbCoeffs + i];
            }
            writeRadius(firstVertID + sphVertID, sfEval, maxAmplitude);
        }
    }
    maxAmplitude = maxAmplitude > 0.0f ? maxAmplitude : 1.0f;
    allMaxAmplitude[firstVertID / nbVertices] = maxAmplitude;
    return nonZero;
}

void updateNormals(uint firstNormalID)
{
    vec3 ab, ac, n;
    vec3 a, b, c;

    // reset normals for sphere
    for(uint i = 0; i < nbVertices; ++i)
    {
        allNormals[firstNormalID + i] = vec4(0.0, 0.0, 0.0, 0.0);
    }

    const uint voxelIndex = firstNormalID / nbVertices;
    for(uint i = 0; i < nbIndices; i += 3)
    {
        a = readRadius(indices[i] + firstNormalID, voxelIndex) * vertices[indices[i]].xyz;
        b = readRadius(indices[i + 1] + firstNormalID, voxelIndex) * vertices[indices[i + 1]].xyz;
        c = readRadius(indices[i + 2] + firstNormalID, voxelIndex) * vertices[indices[i + 2]].xyz;
        ab = b - a;
        ac = c - a;
        if(length(ab) > FLOAT_EPS && length(ac) > FLOAT_EPS)
        {
            ab = normalize(ab);
            ac = normalize(ac);
            if(abs(dot(ab, ac)) < 1.0)
            {
                n = normalize(cross(ab, ac));
                allNormals[indices[i] + firstNormalID] += vec4(n, 0.0);
                allNormals[indices[i + 1] + firstNormalID] += vec4(n, 0.0);
                allNormals[indices[i + 2] + firstNormalID] += vec4(n, 0.0);
            }
        }
    }
}

void main()
{
    uint i, j, k, outSphereID;
    if(currentSlice == 0) // x-slice
    {
        outSphereID = gl_GlobalInvocationID.x + gridDims.x * gridDims.y;
        i = sliceIndex.x;
        j = gl_GlobalInvocationID.x / gridDims.z;
        k = gl_GlobalInvocationID.x - j * gridDims.z;
    }
    else if(currentSlice == 1) // y-slice
    {
        outSphereID = gl_GlobalInvocationID.x + gridDims.x * gridDims.y + gridDims.y * gridDims.z;
        j = sliceIndex.y;
        k = gl_GlobalInvocationID.x / gridDims.x;
        i = gl_GlobalInvocationID.x - k * gridDims.x;
    }
    else if(currentSlice == 2) // z-slice
    {
        outSphereID = gl_GlobalInvocationID.x;
        k = sliceIndex.z;
        j = gl_GlobalInvocationID.x / gridDims.x;
        i = gl_GlobalInvocationID.x - j * gridDims.x;
    }

    const uint voxID = convertSHCoeffsIndex3DToFlatVoxID(i, j, k);
    const uint firstVertID = outSphereID * nbVertices;
    if(scaleSphere(voxID, firstVertID))
    {
        updateNormals(firstVertID);
    }
}
