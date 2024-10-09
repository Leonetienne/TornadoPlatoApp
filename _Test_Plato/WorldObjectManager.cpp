#include "../_TestingUtilities/Catch2.h"
#include "../Plato/WorldObjectManager.h"
#include "../_TestingUtilities/HandyMacros.h"
#include <random>

using namespace Plato;

#define SETUP_TEST WorldObjectManager::Free();
#define CLEAN_TEST WorldObjectManager::Free();

// Tests that the initial number/count of world objects is 0
TEST_CASE(__FILE__"/Initial_Number_Of_WorldObjects_Is_0", "[WorldObjectManager]")
{
    SETUP_TEST;

    REQUIRE(std::size_t(0) == WorldObjectManager::GetNumObjects());

    CLEAN_TEST;
}

// Tests if a new object can be created
TEST_CASE(__FILE__"/Can_Create_New_Object", "[WorldObjectManager]")
{
    SETUP_TEST

    WorldObject* wo = WorldObjectManager::NewWorldObject();
    REQUIRE(wo != nullptr);

    CLEAN_TEST
    return;
}

// Tests if a LOT of objects can be created
TEST_CASE(__FILE__"/Can_Create_Many_Objects", "[WorldObjectManager]")
{
    SETUP_TEST

    // Tries to create a million objects. None is allowed to be nullptr
    // Different value for debug mode, because it takes FAAAAAR too long
    #ifndef _DEBUG
    for (std::size_t i = 0; i < 1000000; i++)
    #else
    for (std::size_t i = 0; i < 10000; i++)
    #endif
    {
        WorldObject* wo = WorldObjectManager::NewWorldObject();
        REQUIRE(wo != nullptr);
    }

    CLEAN_TEST
    return;
}

// Tests if creating objects with metadata works
TEST_CASE(__FILE__"/Create_Objects_With_Metadata_Works", "[WorldObjectManager]")
{
    SETUP_TEST

    // Create parent object, with a name supplied
    WorldObject* parent = WorldObjectManager::NewWorldObject("Parent");

    // Check that the name got applied
    REQUIRE(std::string("Parent") == parent->GetName());

    // Create child object, with a name and parent transform supplied
    WorldObject* child = WorldObjectManager::NewWorldObject("Child", parent->transform);

    // Check that the name got applied
    REQUIRE(std::string("Child") == child->GetName());

    // Check that the child-parent relationship got set up
    REQUIRE(parent->transform == child->transform->GetParent());

    CLEAN_TEST
    return;
}

// Tests that objects can be found by name
TEST_CASE(__FILE__"/FindObjectsByName", "[WorldObjectManager]")
{
    SETUP_TEST

    WorldObject* wo1	 = WorldObjectManager::NewWorldObject("Northern Star");
    WorldObject* wo2	 = WorldObjectManager::NewWorldObject("Burrenhow High");
    WorldObject* bucket1 = WorldObjectManager::NewWorldObject("Bucket");
    WorldObject* bucket2 = WorldObjectManager::NewWorldObject("Bucket");
    WorldObject* bucket3 = WorldObjectManager::NewWorldObject("Bucket");
    WorldObject* wo6	 = WorldObjectManager::NewWorldObject("Floor");

    // Query objects
    std::unordered_set<WorldObject*> foundObjects = WorldObjectManager::FindObjectsByName("Bucket");

    // Check that the amount of found objects matches
    REQUIRE(std::size_t(3) == foundObjects.size());

    // Check that we have no duplicates
    REQUIRE((std::unordered_set<WorldObject*>(foundObjects.begin(), foundObjects.end())).size() == foundObjects.size());

    // Check that all objects found fall into our target set
    for (WorldObject* wo : foundObjects)
    {
        if ((wo != bucket1) && (wo != bucket2) && (wo != bucket3))
            FAIL("Found non-bucket object!");
    }

    CLEAN_TEST
    return;
}

// Tests that FindObjectsByName returns an empty vector if no object is named accordingly
TEST_CASE(__FILE__"/FindObjectsByName_None", "[WorldObjectManager]")
{
    SETUP_TEST

    WorldObject* wo1	 = WorldObjectManager::NewWorldObject("Northern Star");
    WorldObject* wo2	 = WorldObjectManager::NewWorldObject("Burrenhow High");
    WorldObject* bucket1 = WorldObjectManager::NewWorldObject("Bucket");
    WorldObject* bucket2 = WorldObjectManager::NewWorldObject("Bucket");
    WorldObject* bucket3 = WorldObjectManager::NewWorldObject("Bucket");
    WorldObject* wo6	 = WorldObjectManager::NewWorldObject("Floor");

    std::unordered_set<WorldObject*> foundObjects = WorldObjectManager::FindObjectsByName("Metro card");

    REQUIRE(std::size_t(0) == foundObjects.size());

    CLEAN_TEST
    return;
}

// Tests that FindObjectsByTag works
TEST_CASE(__FILE__"/FindObjectsByTag", "[WorldObjectManager]")
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
    std::unordered_set<WorldObject*> foundObjects = WorldObjectManager::FindObjectsByTag("Collectable");

    // Check that the amount of found objects matches
    REQUIRE(std::size_t(4) == foundObjects.size());

    // Check that we have no duplicates
    REQUIRE((std::unordered_set<WorldObject*>(foundObjects.begin(), foundObjects.end())).size() == foundObjects.size());

    // Check that all objects found fall into our target set
    for (WorldObject* wo : foundObjects)
    {
        if ((wo != bucket1) && (wo != bucket2) && (wo != bucket3) && (wo != figurine))
            FAIL("Found non-collectable object!");
    }

    CLEAN_TEST
    return;
}

// Tests that finding an object by its id works
TEST_CASE(__FILE__"/FindObjectById", "[WorldObjectManager]")
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

    REQUIRE(goldenBucket == foundObject);

    CLEAN_TEST
    return;
}

// Tests that FindObjectById returns nullptr if the id is not set
TEST_CASE(__FILE__"/FindObjectById_Nullptr", "[WorldObjectManager]")
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

    REQUIRE(foundObject == nullptr);

    CLEAN_TEST
    return;
}

// Tests that FindObjectById returns nullptr if the supplied id is empty
TEST_CASE(__FILE__"/FindObjectById_NoId", "[WorldObjectManager]")
{
    SETUP_TEST

    // This object technically has the id ""
    WorldObject* wo1 = WorldObjectManager::NewWorldObject("Northern Star");
    // This one too
    WorldObject* wo2 = WorldObjectManager::NewWorldObject("Northern Burrow");

    WorldObject* foundObject = WorldObjectManager::FindObjectById("");
    REQUIRE(foundObject == nullptr);

    CLEAN_TEST
    return;
}

// Tests th at the Free() call does not throw a runtime_error/error when manually having deleted world objects before
TEST_CASE(__FILE__"/Can_Call_Free_After_Manually_Destroying_Objects", "[WorldObjectManager]")
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
TEST_CASE(__FILE__"/Deleting_Parent_Before_Children_Does_Not_Cause_NullpointerDereference", "[WorldObjectManager]")
{
    // If this bug would be present, it would only occur if the contents
    // of the WorldObjectManagers std::unordered_set would be just in the right order.
    // To ensure this gets tested at least once, we'll run the test 100000 times
    // Even with this many tries, it still does not occur every single time.

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
TEST_CASE(__FILE__"/Creating_Object_Increases_Object_Count", "[WorldObjectManager]")
{
    SETUP_TEST;

    // Setup, Exercise
    WorldObjectManager::NewWorldObject();

    // Verify
    REQUIRE(std::size_t(1) == WorldObjectManager::GetNumObjects());

    CLEAN_TEST;
    return;
}

// Tests that destroying an object will decrease the object count
TEST_CASE(__FILE__"/Destroying_Object_Increases_Object_Count", "[WorldObjectManager]")
{
    SETUP_TEST;

    // Setup
    WorldObject* wo = WorldObjectManager::NewWorldObject();

    // Exercise
    wo->Destroy();
    WorldObjectManager::DeleteFlaggedObjects();

    // Verify
    REQUIRE(std::size_t(0) == WorldObjectManager::GetNumObjects());

    CLEAN_TEST;
    return;
}

#undef TEST_START
#undef TEST_END

