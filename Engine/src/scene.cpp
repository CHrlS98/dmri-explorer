#include <scene.h>
#include <sh_field.h>
#include <texture.h>
#include <glm/gtx/transform.hpp>
#include <utils.hpp>
#include <application_state.h>

namespace Slicer
{
Scene::Scene(const std::shared_ptr<ApplicationState>& state)
:mState(state)
,mCoordinateSystem(new CoordinateSystem())
{}

Scene::~Scene()
{}

void Scene::AddSHField()
{
    // create a SH Field model
    mModels.push_back(std::shared_ptr<SHField>(new SHField(mState, mCoordinateSystem)));
}

void Scene::AddTexture()
{
    // create a Texture model
    mModels.push_back(std::shared_ptr<Texture>(new Texture(mState, mCoordinateSystem)));
}

void Scene::Render()
{
    // TODO: Stencil test for discarding texture
    // occluding ODF on same plane.
    //
    // ** Draw planes one at a time:
    //    1. FODF x, then texture x
    //    2. FODF y, then texture y
    //    3. FODF z, then texture z
    glStencilMask(0xFF);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    for(auto model : mModels)
    {
        model->Draw();
    }
}
} // namespace Slicer
