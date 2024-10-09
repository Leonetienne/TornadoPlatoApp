#include "../_TestingUtilities/Catch2.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/Component.h"
#include "../Plato/WorldObject.h"

using namespace Plato;

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

// Tests that a name can be set
TEST_CASE(__FILE__"/Can_Set_Name", "[WorldObject]")
{
    SETUP_TEST

    WorldObject* wo = WorldObjectManager::NewWorldObject();

    wo->SetName("Northern Star");
    REQUIRE(std::string("Northern Star") == wo->GetName());

    CLEAN_TEST
    return;
}

// Tests that an id can be set
TEST_CASE(__FILE__"/Can_Set_Id", "[WorldObject]")
{
    SETUP_TEST

    WorldObject* wo = WorldObjectManager::NewWorldObject();

    wo->SetId("Northern Star");
    REQUIRE(std::string("Northern Star") == wo->GetId());

    CLEAN_TEST
    return;
}

// Tests the same id cannot be assigned twice!
TEST_CASE(__FILE__"/Cant_Set_Id_Twice", "[WorldObject]")
{
    SETUP_TEST

    WorldObject* wo1 = WorldObjectManager::NewWorldObject();
    WorldObject* wo2 = WorldObjectManager::NewWorldObject();

    wo1->SetId("Northern Star");
    wo2->SetId("Northern Star");
    REQUIRE(std::string("Northern Star") != wo2->GetId());

    CLEAN_TEST
    return;
}

// Tests that the SetId method returns the correct value
TEST_CASE(__FILE__"/Set_Id_Return_Value", "[WorldObject]")
{
    SETUP_TEST

    WorldObject* wo1 = WorldObjectManager::NewWorldObject();
    WorldObject* wo2 = WorldObjectManager::NewWorldObject();

    REQUIRE(wo1->SetId("Northern Star"));
    REQUIRE_FALSE(wo2->SetId("Northern Star"));

    CLEAN_TEST
}

// Tests that clearing an objects id works
TEST_CASE(__FILE__"/ClearId_Works", "[WorldObject]")
{
    SETUP_TEST

    WorldObject* wo1 = WorldObjectManager::NewWorldObject();
    WorldObject* wo2 = WorldObjectManager::NewWorldObject();

    wo1->SetId("MY_ID");
    wo1->ClearId();
    REQUIRE(std::string("") == wo1->GetId());

    wo2->SetId("MY_ID");
    REQUIRE(std::string("MY_ID") == wo2->GetId());


    CLEAN_TEST
    return;
}

// Tests that no tags are associated by default
TEST_CASE(__FILE__"/No_Tags_By_Default", "[WorldObject]")
{
    SETUP_TEST

    WorldObject* wo = WorldObjectManager::NewWorldObject();

    REQUIRE(std::size_t(0) == wo->GetTags().size());

    CLEAN_TEST
    return;
}

// Tests that after setting a tag, the tag is associated
TEST_CASE(__FILE__"/Tag_Associated_After_Setting", "[WorldObject]")
{
    SETUP_TEST

    WorldObject* wo = WorldObjectManager::NewWorldObject();

    wo->AddTag("Northern Star");
    REQUIRE(wo->HasTag("Northern Star"));

    CLEAN_TEST
    return;
}

// Tests that the HasTag method returns false if an object is not associated to a given tag
TEST_CASE(__FILE__"/HasTag_False_On_Wrong_Tag", "[WorldObject]")
{
    SETUP_TEST

    WorldObject* wo = WorldObjectManager::NewWorldObject();

    wo->AddTag("Northern Star");
    wo->AddTag("Grabbity");
    wo->AddTag("Gnome");

    REQUIRE_FALSE(wo->HasTag("Pistol"));

    CLEAN_TEST
    return;
}

// Tests that associating a new tag will increase the size of the tag set
TEST_CASE(__FILE__"/Associating_Tag_Increases_Num_Tags", "[WorldObject]")
{
    SETUP_TEST

    WorldObject* wo = WorldObjectManager::NewWorldObject();

    wo->AddTag("Northern Star");
    REQUIRE(std::size_t(1) == wo->GetTags().size());

    wo->AddTag("Tiger mask");
    REQUIRE(std::size_t(2) == wo->GetTags().size());

    wo->AddTag("Gum");
    REQUIRE(std::size_t(3) == wo->GetTags().size());

    wo->AddTag("Enemy");
    REQUIRE(std::size_t(4) == wo->GetTags().size());

    CLEAN_TEST
    return;
}

// Tests that deleting a tag association decreases the size of the tag set
TEST_CASE(__FILE__"/Unassociating_Tag_Increases_Num_Tags", "[WorldObject]")
{
    SETUP_TEST

    WorldObject* wo = WorldObjectManager::NewWorldObject();

    wo->AddTag("Northern Star");
    wo->AddTag("Tiger mask");
    wo->AddTag("Gum");
    wo->AddTag("Enemy");

    wo->ClearTag("Gum");
    wo->ClearTag("Tiger mask");

    REQUIRE(std::size_t(2) == wo->GetTags().size());

    CLEAN_TEST
    return;
}

// Tests that having multiple tags works
TEST_CASE(__FILE__"/Multiple_Tag_Association_Working", "[WorldObject]")
{
    SETUP_TEST

    WorldObject* wo = WorldObjectManager::NewWorldObject();

    wo->AddTag("Northern Star");
    wo->AddTag("Tiger mask");
    wo->AddTag("Enemy");

    REQUIRE(wo->HasTag("Northern Star"));
    REQUIRE(wo->HasTag("Tiger mask"));
    REQUIRE(wo->HasTag("Enemy"));

    CLEAN_TEST
    return;
}

// Tests that a tag is longer associated with the world object after calling ClearTag()
TEST_CASE(__FILE__"/Tag_No_Longer_Associated_After_Calling_ClearTag", "[WorldObject]")
{
    SETUP_TEST

    WorldObject* wo = WorldObjectManager::NewWorldObject();

    wo->AddTag("Northern Star");
    wo->ClearTag("Northern Star");

    REQUIRE_FALSE(wo->HasTag("Northern Star"));

    CLEAN_TEST
    return;
}

// Tests that ClearTag() only removes the matching tag
TEST_CASE(__FILE__"/ClearTag_Removes_Only_Matching_Tag", "[WorldObject]")
{
    SETUP_TEST

    WorldObject* wo = WorldObjectManager::NewWorldObject();

    wo->AddTag("Northern Star");
    wo->AddTag("Tiger mask");
    wo->AddTag("Enemy");
    wo->AddTag("Glue");
    wo->AddTag("Interactable");

    wo->ClearTag("Glue");

    REQUIRE(wo->HasTag("Northern Star"));
    REQUIRE(wo->HasTag("Tiger mask"));
    REQUIRE(wo->HasTag("Enemy"));
    REQUIRE_FALSE(wo->HasTag("Glue"));
    REQUIRE(wo->HasTag("Interactable"));
    
    CLEAN_TEST
    return;
}

TEST_CASE(__FILE__"/No_Tag_Associated_After_Calling_ClearAllTags", "[WorldObject]")
{
    SETUP_TEST

    WorldObject* wo = WorldObjectManager::NewWorldObject();

    wo->AddTag("Northern Star");
    wo->AddTag("Tiger mask");
    wo->AddTag("Enemy");

    wo->ClearAllTags();

    REQUIRE_FALSE(wo->HasTag("Northern Star"));
    REQUIRE_FALSE(wo->HasTag("Tiger mask"));
    REQUIRE_FALSE(wo->HasTag("Enemy"));
    
    CLEAN_TEST
    return;
}

// Tests that adding the same tag twice does nothing
TEST_CASE(__FILE__"/Adding_Same_Tag_Twice_Does_Nothing", "[WorldObject]")
{
    SETUP_TEST

    WorldObject* wo = WorldObjectManager::NewWorldObject();

    wo->AddTag("Northern Star");
    wo->AddTag("Northern Star");
    REQUIRE(std::size_t(1) == wo->GetTags().size());
    REQUIRE(wo->HasTag("Northern Star"));

    CLEAN_TEST
    return;
}

// Tests that adding a component works
TEST_CASE(__FILE__"/Can_Add_Component", "[WorldObject]")
{
    SETUP_TEST

    int i = 33;
    TestComponent* tc = WorldObjectManager::NewWorldObject("myname")->AddComponent<TestComponent>(&i);
    
    REQUIRE(tc != nullptr);
    REQUIRE(i == tc->GetI());
    REQUIRE(std::string("myname") == tc->GetGOName());

    CLEAN_TEST
    return;
}

// Tests that multiple objects can have components simultaniusly
TEST_CASE(__FILE__"/Can_Add_Components_to_Multiple_Objects", "[WorldObject]")
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
        REQUIRE(comps[j]);
        REQUIRE(i == comps[j]->GetI());
        REQUIRE(std::string("myname") == comps[i]->GetGOName());
    }

    CLEAN_TEST
    return;
}

// Tests that adding multiple components to the same object works
TEST_CASE(__FILE__"/Can_Add_Multiple_Components_To_Same_Object", "[WorldObject]")
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
        REQUIRE(comps[j] != nullptr);
        REQUIRE(i == comps[j]->GetI());
        REQUIRE(std::string("myname") == comps[i]->GetGOName());
    }

    CLEAN_TEST
    return;
}

// Tests that any world object is enabled by default
TEST_CASE(__FILE__"/Enabled_By_Default", "[WorldObject]")
{
    SETUP_TEST
    
    // Setup, Exercise
    WorldObject* wo = WorldObjectManager::NewWorldObject();

    // Verify
    REQUIRE(wo->GetIsEnabled());

    CLEAN_TEST
    return;
}

// Tests that any world object is globally enabled by default
TEST_CASE(__FILE__"/Globally_Enabled_By_Default", "[WorldObject]")
{
    SETUP_TEST
    
    // Setup, Exercise
    WorldObject* wo = WorldObjectManager::NewWorldObject();

    // Verify
    REQUIRE(wo->GetIsGloballyEnabled());

    CLEAN_TEST
    return;
}

// Tests that any world object is globally disabled by default, if the parent is disabled aswell
TEST_CASE(__FILE__"/Globally_Disabled_By_Default_If_Instanciated_On_Disabled_Parent", "[WorldObject]")
{
    SETUP_TEST
    
    // Setup, Exercise
    WorldObject* parent = WorldObjectManager::NewWorldObject();
    parent->Disable();
    WorldObject* wo = WorldObjectManager::NewWorldObject("Child", parent->transform);

    // Verify
    REQUIRE_FALSE(wo->GetIsGloballyEnabled());

    CLEAN_TEST
    return;
}

// Tests that any world object can be disabled
TEST_CASE(__FILE__"/Can_Disable_WorldObject", "[WorldObject]")
{
    SETUP_TEST;

    // Setup
    WorldObject* wo = WorldObjectManager::NewWorldObject();

    // Exercise
    wo->Disable();

    // Verify
    REQUIRE_FALSE(wo->GetIsEnabled());

    CLEAN_TEST;
    return;
}

// Tests that any world object can be enabled again
TEST_CASE(__FILE__"/Can_Enable_WorldObject", "[WorldObject]")
{
    SETUP_TEST;

    // Setup
    WorldObject* wo = WorldObjectManager::NewWorldObject();

    // Exercise
    wo->Disable();
    wo->Enable();

    // Verify
    REQUIRE(wo->GetIsEnabled());

    CLEAN_TEST;
    return;
}

// Tests that destroying a world object will also destroy all of its children
TEST_CASE(__FILE__"/Destroying_WorldObject_Also_Destroys_Children", "[WorldObject]")
{
    SETUP_TEST;

    // Setup
    WorldObject* wo00 = WorldObjectManager::NewWorldObject();
    WorldObject* wo10 = WorldObjectManager::NewWorldObject(wo00->transform);
    WorldObject* wo11 = WorldObjectManager::NewWorldObject(wo00->transform);
    WorldObject* wo12 = WorldObjectManager::NewWorldObject(wo00->transform);
    WorldObject* wo20 = WorldObjectManager::NewWorldObject(wo12->transform);
    WorldObject* wo21 = WorldObjectManager::NewWorldObject(wo12->transform);
    WorldObject* wo22 = WorldObjectManager::NewWorldObject(wo12->transform);
    WorldObject* wo30 = WorldObjectManager::NewWorldObject(wo22->transform);
    WorldObject* wo31 = WorldObjectManager::NewWorldObject(wo22->transform);
    WorldObject* wo32 = WorldObjectManager::NewWorldObject(wo22->transform);

    // Little sanity-check
    REQUIRE(std::size_t(10) == WorldObjectManager::GetNumObjects());

    // Exercise
    wo12->Destroy(); // <-- Has 6 total children
    WorldObjectManager::DeleteFlaggedObjects();

    // Verify
    REQUIRE(std::size_t(3) == WorldObjectManager::GetNumObjects());

    CLEAN_TEST;
    return;
}

// Tests that disabling any world object, also globally disables it
TEST_CASE(__FILE__"/Disabling_WorldObject_Causes_It_To_Be_Globally_Disabled", "[WorldObject]")
{
    SETUP_TEST;

    // Setup
    WorldObject* wo = WorldObjectManager::NewWorldObject();

    // Exercise
    wo->Disable();

    // Verify
    REQUIRE_FALSE(wo->GetIsGloballyEnabled());

    CLEAN_TEST;
    return;
}

// Tests that disabling any world objects parent, also globally disables itself
TEST_CASE(__FILE__"/Disabling_WorldObjects_Parent_Causes_It_To_Be_Globally_Disabled", "[WorldObject]")
{
    SETUP_TEST;

    // Setup
    WorldObject* parent = WorldObjectManager::NewWorldObject();
    WorldObject* wo = WorldObjectManager::NewWorldObject("Child", parent->transform);

    // Exercise
    parent->Disable();

    // Verify
    REQUIRE_FALSE(wo->GetIsGloballyEnabled());

    CLEAN_TEST;
    return;
}

#undef TEST_START
#undef TEST_END

