#include "CppUnitTest.h"
#include "../Plato/WorldObjectManager.h"
#include "../_TestingUtilities/HandyMacros.h"
#include "../_TestingUtilities/MemoryLeakDetector.h"
#include <random>
#include <sstream>

Vector3d ScaleVec(Vector3d a, Vector3d b)
{
	return Vector3d
	(
		a.x * b.x,
		a.y * b.y,
		a.z * b.z
	);
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// Will create a new WorldObject and fetch its transform
#define NEW_TRANSFORM WorldObjectManager::NewWorldObject()->transform

namespace TransformRelated
{
	TEST_CLASS(_Transform)
	{
	private:
		std::mt19937 rng;

	public:
		// Constructor
		_Transform()
		{
			rng = std::mt19937((std::random_device())());
			return;
		}

		// Tests that a transform and its world object are linked
		TEST_METHOD(WorldObject_And_Transform_Link)
		{
			// Free any rubbish previously failed tests left behind
			WorldObjectManager::Free();

			WorldObject* wo = WorldObjectManager::NewWorldObject();

			Assert::IsTrue(wo == wo->transform->worldObject);
			Assert::IsTrue(wo->transform == wo->transform->worldObject->transform);
			
			WorldObjectManager::Free();

			return;
		}

		// Tests that the default parent is nullptr
		TEST_METHOD(Default_Parent_Is_Null)
		{
			// Free any rubbish previously failed tests left behind
			WorldObjectManager::Free();

			Transform* tr = NEW_TRANSFORM;

			Assert::IsTrue(tr->GetParent() == nullptr);

			WorldObjectManager::Free();
		}

		// Tests that the default number of children is 0
		TEST_METHOD(No_Children_By_Default)
		{
			Transform* tr = NEW_TRANSFORM;

			Assert::AreEqual(std::size_t(0), tr->GetNumChildren());

			WorldObjectManager::Free();
		}

		// Tests that a transform can't have itself as a parent
		TEST_METHOD(Cant_Assign_Self_As_Parent)
		{
			// Free any rubbish previously failed tests left behind
			WorldObjectManager::Free();

			Transform* tr = NEW_TRANSFORM;

			tr->SetParent(tr);

			Assert::IsTrue(tr->GetParent() == nullptr);

			WorldObjectManager::Free();
		}

		// Tests if the parent can be set
		TEST_METHOD(Can_Set_Parent)
		{
			// Free any rubbish previously failed tests left behind
			WorldObjectManager::Free();

			Transform* trParent = NEW_TRANSFORM;
			Transform* trChild = NEW_TRANSFORM;

			trChild->SetParent(trParent);

			Assert::IsTrue(trParent == trChild->GetParent());
			Assert::IsTrue(trParent->operator[](0) == trChild);

			WorldObjectManager::Free();
			return;
		}

		// Tests if a child can be added
		TEST_METHOD(Add_Child)
		{
			// Free any rubbish previously failed tests left behind
			WorldObjectManager::Free();

			Transform* trParent = NEW_TRANSFORM;
			Transform* trChild = NEW_TRANSFORM;

			trParent->AddChild(trChild);

			Assert::IsTrue(trParent == trChild->GetParent());

			WorldObjectManager::Free();
			return;
		}


		// Tests that adding children actually incrases the size of the set of childs
		TEST_METHOD(Adding_Child_Increases_SetSize)
		{
			// Free any rubbish previously failed tests left behind
			WorldObjectManager::Free();

			Transform* trParent = NEW_TRANSFORM;
			Transform* trChild = NEW_TRANSFORM;

			Assert::AreEqual(std::size_t(0), trParent->GetNumChildren(), L"Initial size wrong!");

			trChild->SetParent(trParent);

			Assert::AreEqual(std::size_t(1), trParent->GetNumChildren(), L"New size wrong!");

			WorldObjectManager::Free();
			return;
		}

		// Tests that assigning a child to another parent removes it from the original parent
		TEST_METHOD(Removing_Child_Decreases_SetSize)
		{
			// Free any rubbish previously failed tests left behind
			WorldObjectManager::Free();

			Transform* trParent = NEW_TRANSFORM;
			Transform* trOtherParent = NEW_TRANSFORM;
			Transform* trChild = NEW_TRANSFORM;

			trParent->AddChild(trChild);
			Assert::AreEqual(std::size_t(1), trParent->GetNumChildren(), L"Child not added properly!");

			trChild->SetParent(trOtherParent);
			Assert::AreEqual(std::size_t(0), trParent->GetNumChildren(), L"New size wrong!");

			WorldObjectManager::Free();
			return;
		}
		
		// Tests that re-assinging children removes the correct children
		TEST_METHOD(Removes_Correct_Child)
		{
			// Free any rubbish previously failed tests left behind
			WorldObjectManager::Free();

			Transform* trParent = NEW_TRANSFORM;
			Transform* trChild = NEW_TRANSFORM;
			Transform* trOtherChild = NEW_TRANSFORM;

			trChild->SetParent(trParent);
			trOtherChild->SetParent(trParent);

			trChild->SetParent(nullptr);

			Assert::AreEqual(std::size_t(1), trParent->GetNumChildren(), L"Number of children wrong!");
			Assert::IsTrue(trOtherChild == trParent->operator[](0), L"Wrong child removed!");

			WorldObjectManager::Free();
			return;
		}

		// Tests that a recursive hirachy tree cannot be created
		TEST_METHOD(Cant_Create_Alabama_HirachyTree)
		{
			// Free any rubbish previously failed tests left behind
			WorldObjectManager::Free();

			// Setup
			Transform* a = NEW_TRANSFORM;
			Transform* b = NEW_TRANSFORM;
			Transform* c = NEW_TRANSFORM;

			b->SetParent(a);
			c->SetParent(b);

			// Current tree:
			// a
			//  \
			//   b
			//    \
			//     c
			//

			// Now let's try to set c as parent of a
			a->SetParent(c);

			// Check that this did nothing
			Assert::IsTrue(a->GetParent() == nullptr);

			WorldObjectManager::Free();
			return;
		}

		// Tests that the local transformation matrix works with only translation
		TEST_METHOD(Local_Transformation_Matrix_TranslationOnly)
		{
			// Free any rubbish previously failed tests left behind
			WorldObjectManager::Free();

			// Setup
			Vector3d originalPoint(17, 33, 5);
			Transform* a = NEW_TRANSFORM;
			a->SetPosition(Vector3d(255, 12, -23));

			// Exercise
			Vector3d transformedPoint = originalPoint * a->GetLocalTransformationMatrix();

			// Verify
			Assert::IsTrue(transformedPoint.Similar(originalPoint + Vector3d(255, 12, -23)));

			WorldObjectManager::Free();
			return;
		}

		// Tests that the local transformation matrix works with only rotation
		TEST_METHOD(Local_Transformation_Matrix_RotationOnly)
		{
			// Free any rubbish previously failed tests left behind
			WorldObjectManager::Free();

			// Setup
			Vector3d originalPoint(12, 314, -99);
			Transform* a = NEW_TRANSFORM;
			a->SetRotation(Quaternion::FromEuler(Vector3d(0, -90, 90)));

			// Exercise
			Vector3d transformedPoint = originalPoint * a->GetLocalTransformationMatrix();

			// Verify
			Vector3d targetPosition = originalPoint * a->GetRotation().ToRotationMatrix();

			//    Create debug output
			std::wstringstream wss;
			wss << "Original position   : " << originalPoint << std::endl
				<< "Transformed position: " << transformedPoint << std::endl
				<< "Target position     : " << targetPosition << std::endl;

			//    Assertion
			Assert::IsTrue(transformedPoint.Similar(targetPosition), wss.str().c_str());

			WorldObjectManager::Free();
			return;
		}

		// Tests that the local transformation matrix works with only scale
		TEST_METHOD(Local_Transformation_Matrix_ScalingOnly)
		{
			// Free any rubbish previously failed tests left behind
			WorldObjectManager::Free();

			// Setup
			Vector3d originalPoint(-2, 12, 33);
			Transform* a = NEW_TRANSFORM;
			a->SetScale(Vector3d(-0.22, 22, 1.5));

			// Exercise
			Vector3d transformedPoint = originalPoint * a->GetLocalTransformationMatrix();

			// Verify
			Vector3d targetPosition = Vector3d(originalPoint.x * -0.22, originalPoint.y * 22, originalPoint.z * 1.5);

			//    Create debug output
			std::wstringstream wss;
			wss << "Original position   : " << originalPoint << std::endl
				<< "Transformed position: " << transformedPoint << std::endl
				<< "Target position     : " << targetPosition << std::endl;

			//    Assertion
			Assert::IsTrue(transformedPoint.Similar(targetPosition), wss.str().c_str());

			WorldObjectManager::Free();
			return;
		}

		// Tests that the local transformation matrix works with all factors
		//TEST_METHOD(Local_Transformation_Matrix_Combined)
		//{
		//	// Free any rubbish previously failed tests left behind
		//	WorldObjectManager::Free();

		//	// Setup
		//	Vector3d originalPoint(-2, 12, 33);
		//	Quaternion rot(Vector3d(45, 13, 0));
		//	Vector3d trl(19.99, 12, -50);
		//	Vector3d scl(-0.22, 22, 1.5);

		//	Transform* a = NEW_TRANSFORM;
		//	a->SetPosition(Vector3d(trl));
		//	a->SetRotation(rot);
		//	a->SetScale(scl);

		//	// Exercise
		//	Vector3d transformedPoint = originalPoint * a->GetLocalTransformationMatrix();

		//	// Verify
		//	Vector3d targetPosition;
		//	{
		//		targetPosition =
		//			ScaleVec(rot.Inverse() * originalPoint, scl) +
		//			trl
		//		;
		//	}

		//	//    Create debug output
		//	std::wstringstream wss;
		//	wss << "Original position   : " << originalPoint << std::endl
		//		<< "Transformed position: " << transformedPoint << std::endl
		//		<< "Target position     : " << targetPosition << std::endl;

		//	//    Assertion
		//	Assert::IsTrue(transformedPoint.Similar(targetPosition), wss.str().c_str());

		//	WorldObjectManager::Free();
		//	return;
		//}
	};
}
