#include "CppUnitTest.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/Component.h"
#include "../Plato/WorldObject.h"
#include "../_TestingUtilities/HandyMacros.h"
#include "../_TestingUtilities/MemoryLeakDetector.h"
#include <random>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define SETUP_TEST WorldObjectManager::Free();
#define CLEAN_TEST WorldObjectManager::Free();

class TestComponent : public Component
{
public:
	int GetI() { return  *i; }
	std::string GetGOName() { return worldObject->GetName(); }

private:
	int* i;

	TestComponent(WorldObject* worldObject, int* i)
		:
		Component(worldObject)
	{
		this->i = i;
		return;
	}

	friend class WorldObject;
};

namespace WorldObjects
{
	TEST_CLASS(_WorldObject)
	{
	private:
		std::mt19937 rng;

	public:
		// Constructor
		_WorldObject()
		{
			rng = std::mt19937((std::random_device())());
			return;
		}

		// Tests that a name can be set
		TEST_METHOD(Can_Set_Name)
		{
			SETUP_TEST

			WorldObject* wo = WorldObjectManager::NewWorldObject();

			wo->SetName("Northern Star");
			Assert::AreEqual(std::string("Northern Star"), wo->GetName());

			CLEAN_TEST
			return;
		}

		// Tests that an id can be set
		TEST_METHOD(Can_Set_Id)
		{
			SETUP_TEST

			WorldObject* wo = WorldObjectManager::NewWorldObject();

			wo->SetId("Northern Star");
			Assert::AreEqual(std::string("Northern Star"), wo->GetId());

			CLEAN_TEST
			return;
		}

		// Tests the same id cannot be assigned twice!
		TEST_METHOD(Cant_Set_Id_Twice)
		{
			SETUP_TEST

			WorldObject* wo1 = WorldObjectManager::NewWorldObject();
			WorldObject* wo2 = WorldObjectManager::NewWorldObject();

			wo1->SetId("Northern Star");
			wo2->SetId("Northern Star");
			Assert::AreNotEqual(std::string("Northern Star"), wo2->GetId());

			CLEAN_TEST
			return;
		}

		// Tests that the SetId method returns the correct value
		TEST_METHOD(Set_Id_Return_Value)
		{
			SETUP_TEST

			WorldObject* wo1 = WorldObjectManager::NewWorldObject();
			WorldObject* wo2 = WorldObjectManager::NewWorldObject();

			Assert::IsTrue(wo1->SetId("Northern Star"), L"Failed on initial setting");
			Assert::IsFalse(wo2->SetId("Northern Star"), L"Failed on second setting");

			CLEAN_TEST
		}

		// Tests that clearing an objects id works
		TEST_METHOD(ClearId_Works)
		{
			SETUP_TEST

				WorldObject* wo1 = WorldObjectManager::NewWorldObject();
			WorldObject* wo2 = WorldObjectManager::NewWorldObject();

			wo1->SetId("MY_ID");
			wo1->ClearId();
			Assert::AreEqual(std::string(""), wo1->GetId());

			wo2->SetId("MY_ID");
			Assert::AreEqual(std::string("MY_ID"), wo2->GetId());


			CLEAN_TEST
			return;
		}

		// Tests that no tags are associated by default
		TEST_METHOD(No_Tags_By_Default)
		{
			SETUP_TEST

			WorldObject* wo = WorldObjectManager::NewWorldObject();

			Assert::AreEqual(std::size_t(0), wo->GetTags().size());

			CLEAN_TEST
			return;
		}

		// Tests that after setting a tag, the tag is associated
		TEST_METHOD(Tag_Associated_After_Setting)
		{
			SETUP_TEST

			WorldObject* wo = WorldObjectManager::NewWorldObject();

			wo->AddTag("Northern Star");
			Assert::IsTrue(wo->HasTag("Northern Star"));

			CLEAN_TEST
			return;
		}

		// Tests that the HasTag method returns false if an object is not associated to a given tag
		TEST_METHOD(HasTag_False_On_Wrong_Tag)
		{
			SETUP_TEST

			WorldObject* wo = WorldObjectManager::NewWorldObject();

			wo->AddTag("Northern Star");
			wo->AddTag("Grabbity");
			wo->AddTag("Gnome");

			Assert::IsFalse(wo->HasTag("Pistol"));

			CLEAN_TEST
			return;
		}

		// Tests that associating a new tag will increase the size of the tag set
		TEST_METHOD(Associating_Tag_Increases_Num_Tags)
		{
			SETUP_TEST

			WorldObject* wo = WorldObjectManager::NewWorldObject();

			wo->AddTag("Northern Star");
			Assert::AreEqual(std::size_t(1), wo->GetTags().size(), L"Size 1 mismatch");
			wo->AddTag("Tiger mask");
			Assert::AreEqual(std::size_t(2), wo->GetTags().size(), L"Size 2 mismatch");
			wo->AddTag("Gum");
			Assert::AreEqual(std::size_t(3), wo->GetTags().size(), L"Size 3 mismatch");
			wo->AddTag("Enemy");
			Assert::AreEqual(std::size_t(4), wo->GetTags().size(), L"Size 4 mismatch");

			CLEAN_TEST
			return;
		}

		// Tests that deleting a tag association decreases the size of the tag set
		TEST_METHOD(Unassociating_Tag_Increases_Num_Tags)
		{
			SETUP_TEST

			WorldObject* wo = WorldObjectManager::NewWorldObject();

			wo->AddTag("Northern Star");
			wo->AddTag("Tiger mask");
			wo->AddTag("Gum");
			wo->AddTag("Enemy");

			wo->ClearTag("Gum");
			wo->ClearTag("Tiger mask");

			Assert::AreEqual(std::size_t(2), wo->GetTags().size(), L"Size 4 mismatch");

			CLEAN_TEST
			return;
		}

		// Tests that having multiple tags works
		TEST_METHOD(Multiple_Tag_Association_Working)
		{
			SETUP_TEST

			WorldObject* wo = WorldObjectManager::NewWorldObject();

			wo->AddTag("Northern Star");
			wo->AddTag("Tiger mask");
			wo->AddTag("Enemy");

			Assert::IsTrue(wo->HasTag("Northern Star"));
			Assert::IsTrue(wo->HasTag("Tiger mask"));
			Assert::IsTrue(wo->HasTag("Enemy"));

			CLEAN_TEST
			return;
		}

		// Tests that a tag is longer associated with the world object after calling ClearTag()
		TEST_METHOD(Tag_No_Longer_Associated_After_Calling_ClearTag)
		{
			SETUP_TEST

			WorldObject* wo = WorldObjectManager::NewWorldObject();

			wo->AddTag("Northern Star");
			wo->ClearTag("Northern Star");

			Assert::IsFalse(wo->HasTag("Northern Star"));

			CLEAN_TEST
			return;
		}

		// Tests that ClearTag() only removes the matching tag
		TEST_METHOD(ClearTag_Removes_Only_Matching_Tag)
		{
			SETUP_TEST

			WorldObject* wo = WorldObjectManager::NewWorldObject();

			wo->AddTag("Northern Star");
			wo->AddTag("Tiger mask");
			wo->AddTag("Enemy");
			wo->AddTag("Glue");
			wo->AddTag("Interactable");

			wo->ClearTag("Glue");

			Assert::IsTrue(wo->HasTag("Northern Star"), L"Northern Star");
			Assert::IsTrue(wo->HasTag("Tiger mask"), L"Tiger mask");
			Assert::IsTrue(wo->HasTag("Enemy"), L"Enemy");
			Assert::IsFalse(wo->HasTag("Glue"), L"Glue");
			Assert::IsTrue(wo->HasTag("Interactable"), L"Interactable");
			
			CLEAN_TEST
			return;
		}

		TEST_METHOD(No_Tag_Associated_After_Calling_ClearAllTags)
		{
			SETUP_TEST

			WorldObject* wo = WorldObjectManager::NewWorldObject();

			wo->AddTag("Northern Star");
			wo->AddTag("Tiger mask");
			wo->AddTag("Enemy");

			wo->ClearAllTags();

			Assert::IsFalse(wo->HasTag("Northern Star"));
			Assert::IsFalse(wo->HasTag("Tiger mask"));
			Assert::IsFalse(wo->HasTag("Enemy"));
			
			CLEAN_TEST
			return;
		}

		// Tests that adding the same tag twice does nothing
		TEST_METHOD(Adding_Same_Tag_Twice_Does_Nothing)
		{
			SETUP_TEST

			WorldObject* wo = WorldObjectManager::NewWorldObject();

			wo->AddTag("Northern Star");
			wo->AddTag("Northern Star");
			Assert::AreEqual(std::size_t(1), wo->GetTags().size(), L"Size mismatch");
			Assert::IsTrue(wo->HasTag("Northern Star"), L"Tag no longer associated");

			CLEAN_TEST
			return;
		}

		// Tests that adding a component works
		TEST_METHOD(Can_Add_Component)
		{
			SETUP_TEST

			int i = 33;
			TestComponent* tc = WorldObjectManager::NewWorldObject("myname")->AddComponent<TestComponent>(&i);
			
			Assert::IsNotNull(tc, L"TestComponent was null!");
			Assert::AreEqual(i, tc->GetI(), L"Pointer not carried correctly");
			Assert::AreEqual(std::string("myname"), tc->GetGOName(), L"WorldObject name does not match");

			CLEAN_TEST
			return;
		}

		// Tests that multiple objects can have components simultaniusly
		TEST_METHOD(Can_Add_Components_to_Multiple_Objects)
		{
			SETUP_TEST

			int i = 33;
			
			std::vector<TestComponent*> comps;

			for (std::size_t j = 0; j < 50; j++)
				comps.push_back(
					WorldObjectManager::NewWorldObject("myname")->AddComponent<TestComponent>(&i)
				);

			for (std::size_t j = 0; j < 50; j++)
			{
				Assert::IsNotNull(comps[j], L"TestComponent was null!");
				Assert::AreEqual(i, comps[j]->GetI(), L"Pointer not carried correctly");
				Assert::AreEqual(std::string("myname"), comps[i]->GetGOName(), L"WorldObject name does not match");
			}

			CLEAN_TEST
			return;
		}

		// Tests that adding multiple components to the same object works
		TEST_METHOD(Can_Add_Multiple_Components_To_Same_Object)
		{
			SETUP_TEST

			int i = 33;
			
			std::vector<TestComponent*> comps;
			WorldObject* wo = WorldObjectManager::NewWorldObject("myname");

			for (std::size_t j = 0; j < 50; j++)
				comps.push_back(
					wo->AddComponent<TestComponent>(&i)
				);

			for (std::size_t j = 0; j < 50; j++)
			{
				Assert::IsNotNull(comps[j], L"TestComponent was null!");
				Assert::AreEqual(i, comps[j]->GetI(), L"Pointer not carried correctly");
				Assert::AreEqual(std::string("myname"), comps[i]->GetGOName(), L"WorldObject name does not match");
			}

			CLEAN_TEST
			return;
		}

		// Tests that any world object is enabled by default
		TEST_METHOD(Enabled_By_Default)
		{
			SETUP_TEST
			
			// Setup, Exercise
			WorldObject* wo = WorldObjectManager::NewWorldObject();

			// Verify
			Assert::IsTrue(wo->GetIsEnabled());

			CLEAN_TEST
			return;
		}

		// Tests that any world object is globally enabled by default
		TEST_METHOD(Globally_Enabled_By_Default)
		{
			SETUP_TEST
			
			// Setup, Exercise
			WorldObject* wo = WorldObjectManager::NewWorldObject();

			// Verify
			Assert::IsTrue(wo->GetIsGloballyEnabled());

			CLEAN_TEST
			return;
		}

		// Tests that any world object is globally disabled by default, if the parent is disabled aswell
		TEST_METHOD(Globally_Disabled_By_Default_If_Instanciated_On_Disabled_Parent)
		{
			SETUP_TEST
			
			// Setup, Exercise
			WorldObject* parent = WorldObjectManager::NewWorldObject();
			parent->Disable();
			WorldObject* wo = WorldObjectManager::NewWorldObject("Child", parent->transform);

			// Verify
			Assert::IsFalse(wo->GetIsGloballyEnabled());

			CLEAN_TEST
			return;
		}

		// Tests that any world object can be disabled
		TEST_METHOD(Can_Disable_WorldObject)
		{
			SETUP_TEST;

			// Setup
			WorldObject* wo = WorldObjectManager::NewWorldObject();

			// Exercise
			wo->Disable();

			// Verify
			Assert::IsFalse(wo->GetIsEnabled());

			CLEAN_TEST;
		}

		// Tests that any world object can be enabled again
		TEST_METHOD(Can_Enable_WorldObject)
		{
			SETUP_TEST;

			// Setup
			WorldObject* wo = WorldObjectManager::NewWorldObject();

			// Exercise
			wo->Disable();
			wo->Enable();

			// Verify
			Assert::IsTrue(wo->GetIsEnabled());

			CLEAN_TEST;
		}

		// Tests that disabling any world object, also globally disables it
		TEST_METHOD(Disabling_WorldObject_Causes_It_To_Be_Globally_Disabled)
		{
			SETUP_TEST;

			// Setup
			WorldObject* wo = WorldObjectManager::NewWorldObject();

			// Exercise
			wo->Disable();

			// Verify
			Assert::IsFalse(wo->GetIsGloballyEnabled());

			CLEAN_TEST;
			return;
		}

		// Tests that disabling any world objects parent, also globally disables itself
		TEST_METHOD(Disabling_WorldObjects_Parent_Causes_It_To_Be_Globally_Disabled)
		{
			SETUP_TEST;

			// Setup
			WorldObject* parent = WorldObjectManager::NewWorldObject();
			WorldObject* wo = WorldObjectManager::NewWorldObject("Child", parent->transform);

			// Exercise
			parent->Disable();

			// Verify
			Assert::IsFalse(wo->GetIsGloballyEnabled());

			CLEAN_TEST;
			return;
		}

		// =========== MEMORY LEAK TESTS ===========
		// These tests depends on debug-mode for memory insights.
		// Thus, they only works in debug mode.
		#ifdef _DEBUG


		// Tests that adding a component does not create a memory leak
		TEST_METHOD(Create_Component_No_MemoryLeak)
		{
			SETUP_TEST

			MemoryLeakDetector mld;
			mld.Init();

			{
				int i = 33;
				TestComponent* tc = WorldObjectManager::NewWorldObject("myname")->AddComponent<TestComponent>(&i);

				WorldObjectManager::Free();

				CLEAN_TEST
			}

			Assert::IsFalse(mld.DetectLeak());
			return;
		}

		#endif
	};
}
