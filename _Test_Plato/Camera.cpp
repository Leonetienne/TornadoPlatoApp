#include "CppUnitTest.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/Camera.h"
#include <random>
#include <sstream>
#include <thread>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define DUMMY_CAM_ARGUMENTS Vector2i(10,10), 90, 0.1, 100

#define TEST_START WorldObjectManager::Free();
#define TEST_END WorldObjectManager::Free();

namespace Components
{
	TEST_CLASS(_Camera)
	{
	private:
		std::mt19937 rng;

	public:
		// Constructor
		_Camera()
		{
			rng = std::mt19937((std::random_device())());
			return;
		}

		// Tests that Camera::GetMainCamera() returns the correct (main) camera.
		TEST_METHOD(GetMainCamera)
		{
			TEST_START

			// Setup
			Camera* cam0 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
			Camera* cam1 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
			Camera* cam2 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
			Camera* cam3 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);

			cam1->SetAsMainCamera();

			// Exercise
			Camera* mainCamera = Camera::GetMainCamera();

			// Verify
			Assert::IsTrue(cam1 == mainCamera);

			TEST_END
			return;
		}

		// Tests that a main camera can be set
		TEST_METHOD(Can_Set_Main_Camera)
		{
			TEST_START

			// Setup
			Camera* cam0 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
			Camera* cam1 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
			Camera* cam2 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
			Camera* cam3 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);

			// Exercise
			cam1->SetAsMainCamera();
			// Verify
			Assert::IsTrue(cam1 == Camera::GetMainCamera());

			// Exercise
			cam3->SetAsMainCamera();
			// Verify
			Assert::IsTrue(cam3 == Camera::GetMainCamera());

			TEST_END
			return;
		}

		// Tests that the main camera does always have the id "main_camera"
		TEST_METHOD(Main_Camera_Always_Has_Id_main_camera)
		{
			TEST_START

			// Setup
			Camera* cam0 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
			Camera* cam1 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
			Camera* cam2 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
			Camera* cam3 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);

			// Exercise
			cam1->SetAsMainCamera();
			// Verify
			Assert::IsTrue(cam1->worldObject == WorldObjectManager::FindObjectById("main_camera"));

			// Exercise
			cam2->SetAsMainCamera();
			// Verify
			Assert::IsTrue(cam2->worldObject == WorldObjectManager::FindObjectById("main_camera"));

			// Exercise
			cam3->SetAsMainCamera();
			// Verify
			Assert::IsTrue(cam3->worldObject == WorldObjectManager::FindObjectById("main_camera"));

			TEST_END
			return;
		}

		// Tests that the main camera gets set to nullptr, when it gets deleted via WorldObjectManager::Free()
		TEST_METHOD(Deleted_Main_Camera_Is_Nullptr_Free)
		{
			TEST_START;

			// Setup
			Camera* cam0 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
			Camera* cam1 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
			
			Assert::IsTrue(cam0 == Camera::GetMainCamera(), L"Wrong main camera to begin with... wtf");

			// Exercise
			WorldObjectManager::Free();

			// Verify
			Assert::IsNull(Camera::GetMainCamera());

			TEST_END;
			return;
		}

		// Tests that the main camera gets set to nullptr, when it gets deleted via Destroy
		TEST_METHOD(Deleted_Main_Camera_Is_Nullptr_Destroy)
		{
			TEST_START;

			// Setup
			Camera* cam0 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
			Camera* cam1 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);

			Assert::IsTrue(cam0 == Camera::GetMainCamera(), L"Wrong main camera to begin with... wtf");

			// Exercise
			cam0->worldObject->Destroy();
			WorldObjectManager::DeleteFlaggedObjects();

			// Verify
			Assert::IsNull(Camera::GetMainCamera());

			TEST_END;
			return;
		}

		// Tests that the first camera created is automatically the main camera
		TEST_METHOD(First_Camera_Is_Main_Camera)
		{
			TEST_START

			// Setup, Exercise
			Camera* cam0 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
			Camera* cam1 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
			Camera* cam2 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);
			Camera* cam3 = WorldObjectManager::NewWorldObject()->AddComponent<Camera>(DUMMY_CAM_ARGUMENTS);

			// Verify
			Assert::IsTrue(cam0 == Camera::GetMainCamera());

			TEST_END
			return;
		}

	};
}
