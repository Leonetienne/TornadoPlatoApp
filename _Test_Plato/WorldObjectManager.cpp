#include "CppUnitTest.h"
#include "../Plato/WorldObjectManager.h"
#include "../_TestingUtilities/HandyMacros.h"
#include "../_TestingUtilities/MemoryLeakDetector.h"
#include <random>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define SETUP_TEST WorldObjectManager::Free();
#define CLEAN_TEST WorldObjectManager::Free();

namespace WorldObjects
{
	TEST_CLASS(_WorldObjectManager)
	{
	private:
		std::mt19937 rng;

	public:
		// Constructor
		_WorldObjectManager()
		{
			rng = std::mt19937((std::random_device())());
			return;
		}

		// Tests that the initial number/count of world objects is 0
		TEST_METHOD(Initial_Number_Of_WorldObjects_Is_0)
		{
			SETUP_TEST;

			Assert::AreEqual(std::size_t(0), WorldObjectManager::GetNumObjects());

			CLEAN_TEST;
		}

		// Tests if a new object can be created
		TEST_METHOD(Can_Create_New_Object)
		{
			SETUP_TEST

			WorldObject* wo = WorldObjectManager::NewWorldObject();
			Assert::IsNotNull(wo);

			CLEAN_TEST
			return;
		}

		// Tests if a LOT of objects can be created
		TEST_METHOD(Can_Create_Many_Objects)
		{
			SETUP_TEST

			// Tries to create a million objects. None is allowed to be nullptr
			for (std::size_t i = 0; i < 1000000; i++)
			{
				WorldObject* wo = WorldObjectManager::NewWorldObject();
				Assert::IsNotNull(wo);
			}

			CLEAN_TEST
			return;
		}

		// Tests if creating objects with metadata works
		TEST_METHOD(Create_Objects_With_Metadata_Works)
		{
			SETUP_TEST

			// Create parent object, with a name supplied
			WorldObject* parent = WorldObjectManager::NewWorldObject("Parent");

			// Check that the name got applied
			Assert::AreEqual(std::string("Parent"), parent->GetName());

			// Create child object, with a name and parent transform supplied
			WorldObject* child = WorldObjectManager::NewWorldObject("Child", parent->transform);

			// Check that the name got applied
			Assert::AreEqual(std::string("Child"), child->GetName());

			// Check that the child-parent relationship got set up
			Assert::IsTrue(parent->transform == child->transform->GetParent(), L"Child-Parent relationship not set");

			CLEAN_TEST
			return;
		}

		// Tests that objects can be found by name
		TEST_METHOD(FindObjectsByName)
		{
			SETUP_TEST

			WorldObject* wo1	 = WorldObjectManager::NewWorldObject("Northern Star");
			WorldObject* wo2	 = WorldObjectManager::NewWorldObject("Burrenhow High");
			WorldObject* bucket1 = WorldObjectManager::NewWorldObject("Bucket");
			WorldObject* bucket2 = WorldObjectManager::NewWorldObject("Bucket");
			WorldObject* bucket3 = WorldObjectManager::NewWorldObject("Bucket");
			WorldObject* wo6	 = WorldObjectManager::NewWorldObject("Floor");

			// Query objects
			std::vector<WorldObject*> foundObjects = WorldObjectManager::FindObjectsByName("Bucket");

			// Check that the amount of found objects matches
			Assert::AreEqual(std::size_t(3), foundObjects.size(), L"Size found mismatch!");

			// Check that we have no duplicates
			Assert::AreEqual((std::unordered_set<WorldObject*>(foundObjects.begin(), foundObjects.end())).size(), foundObjects.size(), L"Size found mismatch!");

			// Check that all objects found fall into our target set
			for (WorldObject* wo : foundObjects)
			{
				if ((wo != bucket1) && (wo != bucket2) && (wo != bucket3))
					Assert::Fail(L"Found non-bucket object!");
			}

			CLEAN_TEST
			return;
		}

		// Tests that FindObjectsByName returns an empty vector if no object is named accordingly
		TEST_METHOD(FindObjectsByName_None)
		{
			SETUP_TEST

			WorldObject* wo1	 = WorldObjectManager::NewWorldObject("Northern Star");
			WorldObject* wo2	 = WorldObjectManager::NewWorldObject("Burrenhow High");
			WorldObject* bucket1 = WorldObjectManager::NewWorldObject("Bucket");
			WorldObject* bucket2 = WorldObjectManager::NewWorldObject("Bucket");
			WorldObject* bucket3 = WorldObjectManager::NewWorldObject("Bucket");
			WorldObject* wo6	 = WorldObjectManager::NewWorldObject("Floor");

			std::vector<WorldObject*> foundObjects = WorldObjectManager::FindObjectsByName("Metro card");

			Assert::AreEqual(std::size_t(0), foundObjects.size());

			CLEAN_TEST
			return;
		}

		// Tests that FindObjectsByTag works
		TEST_METHOD(FindObjectsByTag)
		{
			SETUP_TEST

			WorldObject* wo1 = WorldObjectManager::NewWorldObject("Northern Star");
			WorldObject* wo2 = WorldObjectManager::NewWorldObject("Burrenhow High");
			WorldObject* bucket1 = WorldObjectManager::NewWorldObject("Bucket");
			WorldObject* bucket2 = WorldObjectManager::NewWorldObject("Bucket");
			WorldObject* bucket3 = WorldObjectManager::NewWorldObject("Bucket");
			WorldObject* figurine = WorldObjectManager::NewWorldObject("CrimsonChin Action Figure");
			WorldObject* wo6 = WorldObjectManager::NewWorldObject("Floor");
			WorldObject* wo7 = WorldObjectManager::NewWorldObject("Broom");

			// Assign a lot of overlapping tags
			wo1->AddTag("Object");
			wo1->AddTag("Building");
			wo1->AddTag("Hotel");
			wo1->AddTag("Static");
			wo2->AddTag("Object");
			wo2->AddTag("Building");
			wo2->AddTag("School");
			wo2->AddTag("Static");
			figurine->AddTag("Object");
			figurine->AddTag("Collectable");
			bucket1->AddTag("Object");
			bucket1->AddTag("Prop");
			bucket1->AddTag("Static");
			bucket1->AddTag("Collectable");
			bucket2->AddTag("Object");
			bucket2->AddTag("Prop");
			bucket2->AddTag("Collectable");
			bucket3->AddTag("Object");
			bucket3->AddTag("Prop");
			bucket3->AddTag("Collectable");
			wo6->AddTag("Terrain");
			wo6->AddTag("Object");
			wo6->AddTag("Static");
			wo7->AddTag("Prop");
			wo7->AddTag("Static");

			// Query objects
			std::vector<WorldObject*> foundObjects = WorldObjectManager::FindObjectsByTag("Collectable");

			// Check that the amount of found objects matches
			Assert::AreEqual(std::size_t(4), foundObjects.size(), L"Size found mismatch!");

			// Check that we have no duplicates
			Assert::AreEqual((std::unordered_set<WorldObject*>(foundObjects.begin(), foundObjects.end())).size(), foundObjects.size(), L"Size found mismatch!");

			// Check that all objects found fall into our target set
			for (WorldObject* wo : foundObjects)
			{
				if ((wo != bucket1) && (wo != bucket2) && (wo != bucket3) && (wo != figurine))
					Assert::Fail(L"Found non-collectable object!");
			}

			CLEAN_TEST
			return;
		}

		// Tests that finding an object by its id works
		TEST_METHOD(FindObjectById)
		{
			SETUP_TEST

			WorldObject* wo1 = WorldObjectManager::NewWorldObject("Northern Star");
			WorldObject* wo2 = WorldObjectManager::NewWorldObject("Burrenhow High");
			WorldObject* goldenBucket = WorldObjectManager::NewWorldObject("Golden bucket");
			WorldObject* bucket2 = WorldObjectManager::NewWorldObject("Bucket");
			WorldObject* bucket3 = WorldObjectManager::NewWorldObject("Bucket");
			WorldObject* wo6 = WorldObjectManager::NewWorldObject("Floor");

			goldenBucket->SetId("GOLDEN_BUCKET");

			WorldObject* foundObject = WorldObjectManager::FindObjectById("GOLDEN_BUCKET");

			Assert::IsTrue(goldenBucket == foundObject);

			CLEAN_TEST
			return;
		}

		// Tests that FindObjectById returns nullptr if the id is not set
		TEST_METHOD(FindObjectById_Nullptr)
		{
			SETUP_TEST

			WorldObject* wo1 = WorldObjectManager::NewWorldObject("Northern Star");
			WorldObject* wo2 = WorldObjectManager::NewWorldObject("Burrenhow High");
			WorldObject* goldenBucket = WorldObjectManager::NewWorldObject("Golden bucket");
			WorldObject* bucket2 = WorldObjectManager::NewWorldObject("Bucket");
			WorldObject* bucket3 = WorldObjectManager::NewWorldObject("Bucket");
			WorldObject* wo6 = WorldObjectManager::NewWorldObject("Floor");

			goldenBucket->SetId("GOLDEN_BUCKET");

			WorldObject* foundObject = WorldObjectManager::FindObjectById("SILVER_BUCKET");

			Assert::IsNull(foundObject);

			CLEAN_TEST
			return;
		}

		// Tests that FindObjectById returns nullptr if the supplied id is empty
		TEST_METHOD(FindObjectById_NoId)
		{
			SETUP_TEST

			// This object technically has the id ""
			WorldObject* wo1 = WorldObjectManager::NewWorldObject("Northern Star");
			// This one too
			WorldObject* wo2 = WorldObjectManager::NewWorldObject("Northern Burrow");

			WorldObject* foundObject = WorldObjectManager::FindObjectById("");
			Assert::IsNull(foundObject);

			CLEAN_TEST
			return;
		}

		// Tests th at the Free() call does not throw an exception/error when manually having deleted world objects before
		TEST_METHOD(Can_Call_Free_After_Manually_Destroying_Objects)
		{
			SETUP_TEST;

			// Setup
			WorldObject* wo1 = WorldObjectManager::NewWorldObject("Northern Star");
			WorldObject* wo2 = WorldObjectManager::NewWorldObject("Burrenhow High");
			WorldObject* goldenBucket = WorldObjectManager::NewWorldObject("Golden bucket");
			WorldObject* bucket2 = WorldObjectManager::NewWorldObject("Bucket");
			WorldObject* bucket3 = WorldObjectManager::NewWorldObject("Bucket");
			WorldObject* wo6 = WorldObjectManager::NewWorldObject("Floor");

			// Exercise
			wo2->Destroy();
			bucket2->Destroy();
			goldenBucket->Destroy();
			WorldObjectManager::DeleteFlaggedObjects();

			// Verify
			// This is not allowed to crash the program
			WorldObjectManager::Free();

			CLEAN_TEST
			return;
		}

		// Tests that deleting an objects parent before its children does not cause a nullpointer-dereference
		TEST_METHOD(Deleting_Parent_Before_Children_Does_Not_Cause_NullpointerDereference)
		{
			// If this bug would be present, it would only occur if the contents
			// of the WorldObjectManagers std::unordered_set would be just in the right order.
			// To ensure this gets tested at least once, we'll run the test 100000 times
			// Even with this many tries, it still does not occur every single time.

			Logger::WriteMessage("If this test fails even once, the bug is present. It might not occur very often.");

			for (std::size_t i = 0; i < 100000; i++)
			{
				SETUP_TEST;
			
				// Setup
				WorldObject* parent = WorldObjectManager::NewWorldObject("Parent");
				WorldObject* wo = WorldObjectManager::NewWorldObject("Child", parent->transform);

				// Exercise
				parent->Destroy();
				WorldObjectManager::DeleteFlaggedObjects();

				// Verify
				WorldObjectManager::Free();

				CLEAN_TEST;
			}

			return;
		}

		// Tests that creating a new object will increase the object count
		TEST_METHOD(Creating_Object_Increases_Object_Count)
		{
			SETUP_TEST;

			// Setup, Exercise
			WorldObjectManager::NewWorldObject();

			// Verify
			Assert::AreEqual(std::size_t(1), WorldObjectManager::GetNumObjects());

			CLEAN_TEST;
			return;
		}

		// Tests that destroying an object will decrease the object count
		TEST_METHOD(Destroying_Object_Increases_Object_Count)
		{
			SETUP_TEST;

			// Setup
			WorldObject* wo = WorldObjectManager::NewWorldObject();

			// Exercise
			wo->Destroy();
			WorldObjectManager::DeleteFlaggedObjects();

			// Verify
			Assert::AreEqual(std::size_t(0), WorldObjectManager::GetNumObjects());

			CLEAN_TEST;
			return;
		}

		// =========== MEMORY LEAK TESTS ===========
		// These tests depends on debug-mode for memory insights.
		// Thus, they only works in debug mode.
		#ifdef _DEBUG


		// Tests that simply creating a new world object / transform does not create a memory leak
		TEST_METHOD(Creating_No_Memory_Leak)
		{
			SETUP_TEST

			MemoryLeakDetector mld;
			mld.Init();

			{
				WorldObject* wo = WorldObjectManager::NewWorldObject();

				CLEAN_TEST
			}

			Assert::IsFalse(mld.DetectLeak());
			return;
		}

		// Tests that calling Destroy() on a world object does not cause any memory leak
		TEST_METHOD(Destroy_Object_Causes_No_MemoryLeak)
		{
			SETUP_TEST

			MemoryLeakDetector mld;
			mld.Init();

			{
				// Create world object
				WorldObject* wo = WorldObjectManager::NewWorldObject();

				// Mark object to be destroyed
				wo->Destroy();

				// Poll delete flagged
				// This does free the memory and remove the element of the vector.
				WorldObjectManager::DeleteFlaggedObjects();

				// Did it reduce the element count?
				Assert::AreEqual(std::size_t(0), WorldObjectManager::GetNumObjects(), L"Size mismatch");

				// What this method does NOT do is actually reducing the memory usage, since the vector will keep its allocated memory
				// Lets force the vector to shrink itself by calling Free. This will delete all elements, but we already know that
				// wo got removed from the vector, so it's fine
				WorldObjectManager::Free();
			}

			Assert::IsFalse(mld.DetectLeak());

			CLEAN_TEST
			return;
		}

		#endif
	};
}
