#include "Scene.h"

using namespace Plato;

Scene::Scene(const std::string& sceneName):
    sceneName{sceneName}
{

}

const std::string& Scene::GetSceneName() const
{
    return sceneName;
}

