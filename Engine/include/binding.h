# pragma once

namespace Slicer
{
namespace GPU
{
/// Enumeration of GPU bindings.
enum class Binding
{
    allRadiis = 0,
    allSpheresNormals = 1,
    shCoeffs = 3,
    shFunctions = 4,
    sphereVertices = 5,
    sphereIndices = 6,
    sphereInfo = 7,
    gridInfo = 8,
    camera = 9,
    modelTransform = 10,
    allMaxAmplitude = 12,
    nonZeroMapping = 13,
    none = 30
};
} // namespace GPU
} // namespace Slicer
