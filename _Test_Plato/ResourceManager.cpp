#include "../_TestingUtilities/Catch2.h"
#include "../Plato/ResourceManager.h"
#include <random>
#include <sstream>
#include <stdexcept>

using namespace Plato;

#define SETUP_TEST ResourceManager::Free();
#define CLEAN_TEST ResourceManager::Free();

namespace {
    static std::mt19937 rng = std::mt19937((std::random_device())());
}

/// MATERIALS

// Tests that a new material can be created
TEST_CASE(__FILE__"/Can_Create_New_Material", "[ResourceManager]")
{
    SETUP_TEST;
    // Setup, Exercise
    ResourceManager::NewMaterial("new material");

    // Verify
    REQUIRE(ResourceManager::FindMaterial("new material") != nullptr);

    CLEAN_TEST;
    return;
}

// Tests that multiple materials can be created
TEST_CASE(__FILE__"/Can_Create_Multiple_Materials", "[ResourceManager]")
{
    SETUP_TEST;
    // Setup, Exercise
    ResourceManager::NewMaterial("new material 1");
    ResourceManager::NewMaterial("new material 2");

    // Verify
    REQUIRE(ResourceManager::FindMaterial("new material 1") != nullptr);
    REQUIRE(ResourceManager::FindMaterial("new material 2") != nullptr);

    CLEAN_TEST;
    return;
}

// Tests that trying to register the same material name twice results in nullptr
TEST_CASE(__FILE__"/Same_Material_Name_Twice_Exception", "[ResourceManager]")
{
    SETUP_TEST;

    // Setup
    ResourceManager::NewMaterial("new material 1");

    // Verify
    REQUIRE_THROWS_AS(ResourceManager::NewMaterial("new material 1"), std::runtime_error);

    CLEAN_TEST;
    return;
}

// Tests that trying to find a non-existent material throws a runtime_error
TEST_CASE(__FILE__"/Unknown_Material_Nullptr", "[ResourceManager]")
{
    SETUP_TEST;

    // Setup
    ResourceManager::NewMaterial("new material");

    // Verify
    REQUIRE(ResourceManager::FindMaterial("new material 2") == nullptr);

    CLEAN_TEST;
    return;
}

// Tests that the correct material will be returned
TEST_CASE(__FILE__"/Returns_Correct_Material", "[ResourceManager]")
{
    SETUP_TEST;

    // Setup
    // Create 100 resources
    Material* resources[100];
    for (std::size_t j = 0; j < 100; j++)
    {
        // Gotta have unique, predictable names
        resources[j] = ResourceManager::NewMaterial(
            ((std::stringstream()) << j).str()
        );
    }

    // Exercise, verify
    // Test 50 random picks
    for (std::size_t j = 0; j < 50; j++)
    {
        const std::size_t rndIdx = rng() % 100;

        REQUIRE(resources[rndIdx] == ResourceManager::FindMaterial(
            ((std::stringstream()) << rndIdx).str()
        ));
    }


    CLEAN_TEST;
    return;
}

/// TEXTURES

// Tests that a new texture can be created
TEST_CASE(__FILE__"/Can_Create_New_Texture", "[ResourceManager]")
{
    SETUP_TEST;
    // Setup, Exercise
    ResourceManager::NewTexture("new texture", { 1,1 });

    // Verify
    REQUIRE(ResourceManager::FindTexture("new texture") != nullptr);

    CLEAN_TEST;
    return;
}

// Tests that multiple textures can be created
TEST_CASE(__FILE__"/Can_Create_Multiple_Textures", "[ResourceManager]")
{
    SETUP_TEST;
    // Setup, Exercise
    ResourceManager::NewTexture("new texture 1", {1,1});
    ResourceManager::NewTexture("new texture 2", {1,1});

    // Verify
    REQUIRE(ResourceManager::FindTexture("new texture 1") != nullptr);
    REQUIRE(ResourceManager::FindTexture("new texture 2") != nullptr);

    CLEAN_TEST;
    return;
}

// Tests that trying to register the same texture name twice results in nullptr
TEST_CASE(__FILE__"/Same_Texture_Name_Twice_Exception", "[ResourceManager]")
{
    SETUP_TEST;

    // Setup
    ResourceManager::NewTexture("new texture 1", { 1,1 });

    // Verify
    REQUIRE_THROWS_AS(ResourceManager::NewTexture("new texture 1", {1,1}), std::runtime_error);

    CLEAN_TEST;
    return;
}

// Tests that trying to find a non-existent texture throws a runtime_error
TEST_CASE(__FILE__"/Unknown_Texture_Nullptr", "[ResourceManager]")
{
    SETUP_TEST;

    // Setup
    ResourceManager::NewTexture("new texture", { 1,1 });

    // Verify
    REQUIRE(ResourceManager::FindTexture("new texture 2") == nullptr);

    CLEAN_TEST;
    return;
}

// Tests that the correct texture will be returned
TEST_CASE(__FILE__"/Returns_Correct_Texture", "[ResourceManager]")
{
    SETUP_TEST;

    // Setup
    // Create 100 resources
    Texture* resources[100];
    for (std::size_t j = 0; j < 100; j++)
    {
        // Gotta have unique, predictable names
        resources[j] = ResourceManager::NewTexture(
            ((std::stringstream()) << j).str(),
            { 1,1 }
        );
    }

    // Exercise, verify
    // Test 50 random picks
    for (std::size_t j = 0; j < 50; j++)
    {
        const std::size_t rndIdx = rng() % 100;

        REQUIRE(resources[rndIdx] == ResourceManager::FindTexture(
            ((std::stringstream()) << rndIdx).str()
        ));
    }


    CLEAN_TEST;
    return;
}


/// MESHES

// Tests that a new mesh can be created
TEST_CASE(__FILE__"/Can_Create_New_Mesh", "[ResourceManager]")
{
    SETUP_TEST;
    // Setup, Exercise
    ResourceManager::NewMesh("new mesh");

    // Verify
    REQUIRE(ResourceManager::FindMesh("new mesh") != nullptr);

    CLEAN_TEST;
    return;
}

// Tests that multiple meshs can be created
TEST_CASE(__FILE__"/Can_Create_Multiple_Meshs", "[ResourceManager]")
{
    SETUP_TEST;
    // Setup, Exercise
    ResourceManager::NewMesh("new mesh 1");
    ResourceManager::NewMesh("new mesh 2");

    // Verify
    REQUIRE(ResourceManager::FindMesh("new mesh 1") != nullptr);
    REQUIRE(ResourceManager::FindMesh("new mesh 2") != nullptr);

    CLEAN_TEST;
    return;
}

// Tests that trying to register the same mesh name twice results in nullptr
TEST_CASE(__FILE__"/Same_Mesh_Name_Twice_Exception", "[ResourceManager]")
{
    SETUP_TEST;

    // Setup
    ResourceManager::NewMesh("new mesh 1");

    // Verify
    REQUIRE_THROWS_AS(ResourceManager::NewMesh("new mesh 1"), std::runtime_error);

    CLEAN_TEST;
    return;
}

// Tests that trying to find a non-existent mesh throws a runtime_error
TEST_CASE(__FILE__"/Unknown_Mesh_Nullptr", "[ResourceManager]")
{
    SETUP_TEST;

    // Setup
    ResourceManager::NewMesh("new mesh");

    // Verify
    REQUIRE(ResourceManager::FindMesh("new mesh 2") == nullptr);

    CLEAN_TEST;
    return;
}

// Tests that the correct mesh will be returned
TEST_CASE(__FILE__"/Returns_Correct_Mesh", "[ResourceManager]")
{
    SETUP_TEST;

    // Setup
    // Create 100 resources
    Mesh* resources[100];
    for (std::size_t j = 0; j < 100; j++)
    {
        // Gotta have unique, predictable names
        resources[j] = ResourceManager::NewMesh(
            ((std::stringstream()) << j).str()
        );
    }

    // Exercise, verify
    // Test 50 random picks
    for (std::size_t j = 0; j < 50; j++)
    {
        const std::size_t rndIdx = rng() % 100;

        REQUIRE(resources[rndIdx] == ResourceManager::FindMesh(
            ((std::stringstream()) << rndIdx).str()
        ));
    }


    CLEAN_TEST;
    return;
}

// MISC

// Tests that the same name can be used for different classes of resources
TEST_CASE(__FILE__"/Same_Name_For_Different_Classes_Valid", "[ResourceManager]")
{
    SETUP_TEST;

    // Setup, Exercise
    Material* mat = ResourceManager::NewMaterial("test");
    Texture* txt = ResourceManager::NewTexture("test", { 1,1 });
    Mesh* msh = ResourceManager::NewMesh("test");

    // Verify
    REQUIRE(mat == ResourceManager::FindMaterial("test"));
    REQUIRE(txt == ResourceManager::FindTexture("test"));
    REQUIRE(msh == ResourceManager::FindMesh("test"));

    return;
    CLEAN_TEST;
}

#undef TEST_START
#undef TEST_END

