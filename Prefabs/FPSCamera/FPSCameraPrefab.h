#pragma once
#include "../../Plato/Prefab.h"
#include "../../HandyComponents/CameraFPSKeyboardControl.h"
#include "../../HandyComponents/CameraExclKeyboardControl.h"
#include "../../Plato/WorldObjectManager.h"
#include "../../Plato/Camera.h"

class FPSCameraPrefab : public Plato::Prefab
{
public:
    enum class CONTROL_TYPE {
        LOOK_MOUSE,
        LOOK_HJKL,
    };

    FPSCameraPrefab(CONTROL_TYPE controlType) : controlType{controlType} {};

    void Instantiate(Plato::Transform* parent = nullptr) override {
        // Necessary for camera rotation
        Plato::Transform* cameraYPivot = Plato::WorldObjectManager::NewWorldObject("Camera y pivot", parent)->transform;
        Components::Camera* camera = WorldObjectManager::NewWorldObject("Main Camera", cameraYPivot)->AddComponent<Components::Camera>(90, 0.001, 10);
        cameraYPivot->worldObject->SetId("main_camera_ypiv");
        camera->SetAsMainCamera();

        // Let's add a control component to the camera by default
        if (controlType == CONTROL_TYPE::LOOK_MOUSE) {
            camera->worldObject->AddComponent<CameraFPSKeyboardControl>(cameraYPivot, camera->transform, 0.2, 0.6, 4);
        }
        else if (controlType == CONTROL_TYPE::LOOK_HJKL) {

            camera->worldObject->AddComponent<CameraExclKeyboardControl>(cameraYPivot, camera->transform, 0.2, 0.6, 4);
        }
    }

private:
    CONTROL_TYPE controlType;
};

