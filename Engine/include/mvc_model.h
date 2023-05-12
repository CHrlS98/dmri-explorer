#pragma once
#include <memory>
#include <string>
#include <sh_model.h>
#include <scalar_model.h>
#include <grid_model.h>

namespace Slicer
{
enum class LoadRoutineStatus
{
    IDLING,
    FILE_IS_CHOSEN,
    SKIPPED_ONCE,
    HEADER_IS_VALID,
    HEADER_IS_INVALID,
    IMAGE_DATA_LOADED
};

class MVCModel
{
public:
    MVCModel(int winWidth, int winHeight);
    LoadRoutineStatus AddSHModel(const std::string& imageFilePath);
    LoadRoutineStatus AddScalarModel(const std::shared_ptr<NiftiImageWrapper<float>>& niftiImage,
                                     const LoadRoutineStatus& status);

    inline std::shared_ptr<GridModel> GetGridModel() const { return mGridModel; };

    inline std::shared_ptr<SHModel> GetSHModel() { return mSHModel; };
    inline std::shared_ptr<ScalarModel> GetScalarModel() { return mScalarModel; };

    inline int GetWindowWidth() const { return mWinWidth; };
    inline int GetWindowHeight() const { return mWinHeight; };
private:
    int mWinWidth = 0;
    int mWinHeight = 0;

    std::shared_ptr<GridModel> mGridModel = nullptr;
    std::shared_ptr<SHModel> mSHModel = nullptr;
    std::shared_ptr<ScalarModel> mScalarModel = nullptr;
};
} // namespace Slicer
