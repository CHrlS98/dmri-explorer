#include <sh_field.h>
#include <glad/glad.h>
#include <timer.h>

namespace
{
const int NB_THREADS_FOR_SPHERES = 4;
}

namespace Slicer
{
SHField::SHField(const std::shared_ptr<ApplicationState>& state,
                 std::shared_ptr<CoordinateSystem> parent)
:Model(state)
,mIndices()
,mVAO(0)
,mIndicesBO(0)
,mIndirectBO(0)
,mNonZeroVoxels()
,mSphHarmCoeffsData()
,mSphHarmFuncsData()
,mSphereVerticesData()
,mSphereIndicesData()
,mSphereInfoData()
,mAllSpheresNormalsData()
,mNonZeroVoxelsData()
,mIndirectCmd()
,mSphere(nullptr)
{
    resetCS(std::shared_ptr<CoordinateSystem>(new CoordinateSystem(glm::mat4(1.0f), parent)));
    initializeModel();
    initializeMembers();
    initializeGPUData();

    scaleAllSpheres();
}

SHField::~SHField()
{
}

void SHField::updateApplicationStateAtInit()
{
}

void SHField::registerStateCallbacks()
{
    mState->Sphere.IsNormalized.RegisterCallback(
        [this](bool p, bool n)
        {
            this->setNormalized(p, n);
        }
    );
    mState->Sphere.ColorMapMode.RegisterCallback(
        [this](int p, int n)
        {
            this->setColorMapMode(p, n);
        }
    );
    mState->Sphere.SH0Threshold.RegisterCallback(
        [this](float p, float n)
        {
            this->setSH0Threshold(p, n);
        }
    );
    mState->Sphere.Scaling.RegisterCallback(
        [this](float p, float n)
        {
            this->setSphereScaling(p, n);
        }
    );
    mState->Sphere.FadeIfHidden.RegisterCallback(
        [this](bool p, bool n)
        {
            this->setFadeIfHidden(p, n);
        }
    );
    mState->ViewMode.Mode.RegisterCallback(
        [this](State::CameraMode p, State::CameraMode n)
        {
            this->setVisibleSlices(p, n);
        }
    );
}

void SHField::initProgramPipeline()
{
    const std::string vsPath = DMRI_EXPLORER_BINARY_DIR + std::string("/shaders/shfield_vert.glsl");
    const std::string fsPath = DMRI_EXPLORER_BINARY_DIR + std::string("/shaders/shfield_frag.glsl");

    std::vector<GPU::ShaderProgram> shaders;
    shaders.push_back(GPU::ShaderProgram(vsPath, GL_VERTEX_SHADER));
    shaders.push_back(GPU::ShaderProgram(fsPath, GL_FRAGMENT_SHADER));
    mProgramPipeline = GPU::ProgramPipeline(shaders);
}

void SHField::initializeMembers()
{
    // Initialize compute shader
    const std::string csRadiisPath = DMRI_EXPLORER_BINARY_DIR + std::string("/shaders/shradiis_comp.glsl");
    const std::string csNormalsPath = DMRI_EXPLORER_BINARY_DIR + std::string("/shaders/shnormals_comp.glsl");
    mComputeRadiisShader = GPU::ShaderProgram(csRadiisPath, GL_COMPUTE_SHADER);
    mComputeNormalsShader = GPU::ShaderProgram(csNormalsPath, GL_COMPUTE_SHADER);

    // Initialize a sphere for SH to SF projection
    const auto& image = mState->FODFImage.Get();
    const auto& dims = image.GetDims();
    mSphere.reset(new Primitive::Sphere(mState->Sphere.Resolution.Get(), dims.w));

    // Compute non-zero voxels table
    computeNonZeroVoxels(image);

    // Preallocate buffers for draw call
    const auto& nbIndices = mSphere->GetIndices().size();
    const auto& nbSpheres = mNonZeroVoxels.size();
    mIndices.resize(nbSpheres * nbIndices);
    mIndirectCmd.resize(nbSpheres);

    // Copy sphere indices and instantiate draw commands.
    std::vector<std::thread> threads;
    dispatchSubsetCommands(&SHField::initializeSubsetDrawCommand,
                           nbSpheres, NB_THREADS_FOR_SPHERES, threads);

    // wait for all threads to finish
    for(auto& t : threads)
    {
        t.join();
    }

    // Bind primitives to GPU
    glCreateVertexArrays(1, &mVAO);
    mIndicesBO = genVBO<GLuint>(mIndices);
    mIndirectBO = genVBO<DrawElementsIndirectCommand>(mIndirectCmd);
}

void SHField::computeNonZeroVoxels(const NiftiImageWrapper<float>& image)
{
    const auto& dims = image.GetDims();
    const size_t nCoeffs = dims.w;
    const std::vector<float>& voxelData = image.GetVoxelData();

    unsigned int flatIndex3D = 0;
    unsigned int nbNonZeroVoxels = 0;
    mNonZeroVoxels.resize(dims.x*dims.y*dims.z); // upper bound
    for(size_t i = 0; i < voxelData.size(); i += nCoeffs)
    {
        if(voxelData[i] > 0.0f)
        {
            mNonZeroVoxels[nbNonZeroVoxels++] = flatIndex3D;
        }
        ++flatIndex3D;
    }
    mNonZeroVoxels.resize(nbNonZeroVoxels);
}

void SHField::dispatchSubsetCommands(void(SHField::*fn)(size_t, size_t), size_t nbElements,
                                     size_t nbThreads, std::vector<std::thread>& threads)
{
    size_t nbElementsPerThread = nbElements / nbThreads;
    size_t startIndex = 0;
    size_t stopIndex = nbElementsPerThread;
    for(int i = 0; i < nbThreads - 1; ++i)
    {
        threads.push_back(std::thread(fn, this, startIndex, stopIndex));
        startIndex = stopIndex;
        stopIndex += nbElementsPerThread;
    }
    threads.push_back(std::thread(fn, this, startIndex, nbElements));
}

void SHField::initializeSubsetDrawCommand(size_t firstIndex, size_t lastIndex)
{
    const auto& indices = mSphere->GetIndices();
    const auto numIndices = indices.size();
    const auto numVertices = mSphere->GetPoints().size();

    size_t i, j;
    for(i = firstIndex; i < lastIndex; ++i)
    {
        // Add sphere faces
        for(j = 0; j < numIndices; ++j)
        {
            mIndices[i*numIndices+j] = indices[j];
        }

        // Add indirect draw command for current sphere
        mIndirectCmd[i] =
            DrawElementsIndirectCommand(
                static_cast<unsigned int>(numIndices), // num of elements to draw per drawID
                1, // number of identical instances
                0, // offset in VBO
                static_cast<unsigned int>(i * numVertices), // offset in vertices array
                0);
    }
}

void SHField::initializeGPUData()
{
    // The SH coefficients image to copy on the GPU.
    const auto& image = mState->FODFImage.Get();

    const int nbSpheres = mNonZeroVoxels.size();
    const size_t nbRadiis = nbSpheres * mSphere->GetPoints().size();

    // temporary zero-filled array for all normals
    std::vector<GLuint> allNormals(nbRadiis, 0);

    // to compress the SF amplitudes, we will pack 8 values per int
    const size_t nbIntegersForRadiis = ceil(static_cast<float>(nbRadiis) / 4.0f);
    std::vector<GLuint> allRadiis(nbIntegersForRadiis, 0);

    std::vector<float> allMaxAmplitude(nbSpheres);

    // Sphere data GPU buffer
    SphereData sphereData;
    sphereData.NumVertices = static_cast<unsigned int>(mSphere->GetPoints().size());
    sphereData.NumIndices = static_cast<unsigned int>(mSphere->GetIndices().size());
    sphereData.IsNormalized = mState->Sphere.IsNormalized.Get();
    sphereData.MaxOrder = mSphere->GetMaxSHOrder();
    sphereData.SH0threshold = mState->Sphere.SH0Threshold.Get();
    sphereData.Scaling = mState->Sphere.Scaling.Get();
    sphereData.NbCoeffs = mState->FODFImage.Get().GetDims().w;
    sphereData.FadeIfHidden = mState->Sphere.FadeIfHidden.Get();
    sphereData.ColorMapMode = mState->Sphere.ColorMapMode.Get();

    // Grid data GPU buffer
    // TODO: Move out of SHField. Should be in a standalone class.
    GridData gridData;
    gridData.SliceIndices = glm::ivec4(mState->VoxelGrid.SliceIndices.Get(), 0);
    gridData.VolumeShape = glm::ivec4(mState->VoxelGrid.VolumeShape.Get(), 0);
    gridData.IsVisible = glm::ivec4(1, 1, 1, 0);
    gridData.CurrentSlice = 0;

    // read/write
    mAllSpheresNormalsData = GPU::ShaderData(allNormals.data(), GPU::Binding::allSpheresNormals,
                                             sizeof(GLuint) * allNormals.size());
    mAllRadiisData = GPU::ShaderData(allRadiis.data(), GPU::Binding::allRadiis,
                                     sizeof(GLuint) * allRadiis.size());
    mSphHarmCoeffsData = GPU::ShaderData(image.GetVoxelData().data(), GPU::Binding::shCoeffs,
                                         sizeof(float) * image.GetVoxelData().size());
    mAllMaxAmplitudeData = GPU::ShaderData(allMaxAmplitude.data(), GPU::Binding::allMaxAmplitude,
                                     sizeof(float) * allMaxAmplitude.size());

    // TODO: readonly (GL_STATIC_READ?)
    mSphHarmFuncsData = GPU::ShaderData(mSphere->GetSHFuncs().data(), GPU::Binding::shFunctions,
                                        sizeof(float) * mSphere->GetSHFuncs().size());
    mSphereVerticesData = GPU::ShaderData(mSphere->GetPoints().data(), GPU::Binding::sphereVertices,
                                          sizeof(glm::vec4) * mSphere->GetPoints().size());
    mSphereIndicesData = GPU::ShaderData(mSphere->GetIndices().data(), GPU::Binding::sphereIndices,
                                         sizeof(unsigned int) * mSphere->GetIndices().size());
    mSphereInfoData = GPU::ShaderData(&sphereData, GPU::Binding::sphereInfo,
                                      sizeof(SphereData));
    mGridInfoData = GPU::ShaderData(&gridData, GPU::Binding::gridInfo,
                                    sizeof(GridData));
    mNonZeroVoxelsData = GPU::ShaderData(mNonZeroVoxels.data(), GPU::Binding::nonZeroMapping,
                                         sizeof(unsigned int) * mNonZeroVoxels.size());

    // push all data to GPU
    mSphHarmCoeffsData.ToGPU();
    mSphHarmFuncsData.ToGPU();
    mSphereVerticesData.ToGPU();
    mSphereIndicesData.ToGPU();
    mSphereInfoData.ToGPU();
    mGridInfoData.ToGPU();
    mAllRadiisData.ToGPU();
    mAllSpheresNormalsData.ToGPU();
    mAllMaxAmplitudeData.ToGPU();
    mNonZeroVoxelsData.ToGPU();
}

template <typename T>
GLuint SHField::genVBO(const std::vector<T>& data) const
{
    GLuint vbo;
    glCreateBuffers(1, &vbo);
    glNamedBufferData(vbo, data.size() * sizeof(T), &data[0], GL_STATIC_DRAW);
    return vbo;
}

void SHField::setNormalized(bool previous, bool isNormalized)
{
    if(previous != isNormalized)
    {
        unsigned int isNormalizedInt = isNormalized ? 1 : 0;
        mSphereInfoData.Update(sizeof(unsigned int)*2, sizeof(unsigned int), &isNormalizedInt);
    }
}

void SHField::setColorMapMode(int previous, int mode)
{
    if(previous != mode)
    {
        mSphereInfoData.Update(6*sizeof(unsigned int) + 2*sizeof(float), sizeof(unsigned int), &mode);
    }
}

void SHField::setSH0Threshold(float previous, float threshold)
{
    if(previous != threshold)
    {
        mSphereInfoData.Update(4*sizeof(unsigned int), sizeof(float), &threshold);
    }
}

void SHField::setSphereScaling(float previous, float scaling)
{
    if(previous != scaling)
    {
        mSphereInfoData.Update(4*sizeof(unsigned int) + sizeof(float),
                               sizeof(float),
                               &scaling);
    }
}

void SHField::setFadeIfHidden(bool previous, bool fadeEnabled)
{
    if(previous != fadeEnabled)
    {
        unsigned int uintFadeEnabled = fadeEnabled ? 1 : 0;
        mSphereInfoData.Update(5*sizeof(unsigned int) + 2*sizeof(float),
                               sizeof(unsigned int),
                               &uintFadeEnabled);
    }
}

void SHField::setVisibleSlices(State::CameraMode previous, State::CameraMode next)
{
    if(previous != next)
    {
        glm::ivec4 isVisible;
        switch(next)
        {
            case State::CameraMode::projectiveX:
                isVisible = glm::ivec4(1, 0, 0, 0);
                break;
            case State::CameraMode::projectiveY:
                isVisible = glm::ivec4(0, 1, 0, 0);
                break;
            case State::CameraMode::projectiveZ:
                isVisible = glm::ivec4(0, 0, 1, 0);
                break;
            case State::CameraMode::projective3D:
            default:
                isVisible = glm::ivec4(1, 1, 1, 0);
                break;
        }
        mGridInfoData.Update(2*sizeof(glm::ivec4), sizeof(glm::ivec4), &isVisible);
    }
}

void SHField::drawSpecific()
{
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndicesBO);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, mIndirectBO);
    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, (GLvoid*)0,
                                static_cast<int>(mIndirectCmd.size()), 0);
}

void SHField::scaleAllSpheres()
{
    const auto& image = mState->FODFImage.Get();
    const auto& dims = image.GetDims();

    // compute radiis
    glUseProgram(mComputeRadiisShader.ID());
    glDispatchCompute(mNonZeroVoxels.size(), 1, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    // compute normals
    glUseProgram(mComputeNormalsShader.ID());
    glDispatchCompute(mNonZeroVoxels.size(), 1, 1);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    glUseProgram(0);
}
} // namespace Slicer
