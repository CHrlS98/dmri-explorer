#pragma once

#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "nifti1_io.h"

namespace Image
{
enum class DataType
{
    unknown = DT_UNKNOWN,
    binary = DT_BINARY,
    int8 = DT_INT8,
    uint8 = DT_UINT8,
    int16 = DT_INT16,
    uint16 = DT_UINT16,
    int32 = DT_INT32,
    uint32 = DT_UINT32,
    int64 = DT_INT64,
    uint64 = DT_UINT64,
    float32 = DT_FLOAT32,
    float64 = DT_FLOAT64,
    float128 = DT_FLOAT128,
    complex64 = DT_COMPLEX64,
    complex128 = DT_COMPLEX128,
    complex256 = DT_COMPLEX256,
    rgb24 = DT_RGB24,
    rgba32 = DT_RGBA32
};

class NiftiImageWrapper
{
public:
    NiftiImageWrapper(const std::string& path);
    ~NiftiImageWrapper();
    std::shared_ptr<nifti_image> getNiftiImage() const;

    DataType dtype() const;
    glm::vec<4, int> dims() const;
    uint length() const;
    glm::vec<3, uint> unravelIndex3d(size_t flatIndex) const;
    size_t flattenIndex(uint i, uint j, uint k, uint l) const;

    // pixel values getters
    double at(uint i, uint j, uint k, uint l) const;

    template<typename T> std::vector<T*> at(uint i, uint j, uint k) const
    {
        std::vector<T*> voxChannels;
        voxChannels.resize(mDims.w);
        for(uint l = 0; l < mDims.w; ++l)
        {
            voxChannels[l] = this->at<T>(i, j, k, l);
        }
        return voxChannels;
    };

    template<typename T> T* at(uint i, uint j, uint k, uint l) const
    {
        const uint flatIndex = flattenIndex(i, j, k, l);

        if(flatIndex > mLength - 1)
        {
            throw std::runtime_error("Index is out of bound for image.");
        }

        T* v = &((T*)(mData->data))[flatIndex];
        return v;
    };
private:
    std::shared_ptr<nifti_image> mData;
    glm::vec<4, int> mDims;
    uint mLength;
};
} // Image
