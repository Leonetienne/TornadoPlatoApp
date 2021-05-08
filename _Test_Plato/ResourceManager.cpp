#include "CppUnitTest.h"
#include "../Plato/ResourceManager.h"
#include "../Tornado/Similar.h"
#include "../_TestingUtilities/HandyMacros.h"
#include "../_TestingUtilities/MemoryLeakDetector.h"
#include <random>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define SETUP_TEST ResourceManager::Free();
#define CLEAN_TEST ResourceManager::Free();

namespace Misc
{
	TEST_CLASS(_ResourceManager)
	{
	private:
		std::mt19937 rng;

	public:
		// Constructor
		_ResourceManager()
		{
			rng = std::mt19937((std::random_device())());
			return;
		}


		/// MATERIALS

		// Tests that a new material can be created
		TEST_METHOD(Can_Create_New_Material)
		{
			SETUP_TEST;
			// Setup, Exercise
			ResourceManager::NewMaterial("new material");

			// Verify
			Assert::IsNotNull(ResourceManager::FindMaterial("new material"));

			CLEAN_TEST;
			return;
		}

		// Tests that multiple materials can be created
		TEST_METHOD(Can_Create_Multiple_Materials)
		{
			SETUP_TEST;
			// Setup, Exercise
			ResourceManager::NewMaterial("new material 1");
			ResourceManager::NewMaterial("new material 2");

			// Verify
			Assert::IsNotNull(ResourceManager::FindMaterial("new material 1"));
			Assert::IsNotNull(ResourceManager::FindMaterial("new material 2"));

			CLEAN_TEST;
			return;
		}

		// Tests that trying to register the same material name twice results in nullptr
		TEST_METHOD(Same_Material_Name_Twice_Exception)
		{
			SETUP_TEST;

			// Setup
			ResourceManager::NewMaterial("new material 1");

			// Verify
			Assert::ExpectException<std::exception>([]()
			{
				ResourceManager::NewMaterial("new material 1");
			});

			CLEAN_TEST;
			return;
		}

		// Tests that trying to find a non-existent material throws an exception
		TEST_METHOD(Unknown_Material_Nullptr)
		{
			SETUP_TEST;

			// Setup
			ResourceManager::NewMaterial("new material");

			// Verify
			Assert::IsNull(ResourceManager::FindMaterial("new material 2"));

			CLEAN_TEST;
			return;
		}

		// Tests that the correct material will be returned
		TEST_METHOD(Returns_Correct_Material)
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

				Assert::IsTrue(resources[rndIdx] == ResourceManager::FindMaterial(
					((std::stringstream()) << rndIdx).str()
				));
			}


			CLEAN_TEST;
			return;
		}

		/// TEXTURES

		// Tests that a new texture can be created
		TEST_METHOD(Can_Create_New_Texture)
		{
			SETUP_TEST;
			// Setup, Exercise
			ResourceManager::NewTexture("new texture", { 1,1 });

			// Verify
			Assert::IsNotNull(ResourceManager::FindTexture("new texture"));

			CLEAN_TEST;
			return;
		}

		// Tests that multiple textures can be created
		TEST_METHOD(Can_Create_Multiple_Textures)
		{
			SETUP_TEST;
			// Setup, Exercise
			ResourceManager::NewTexture("new texture 1", {1,1});
			ResourceManager::NewTexture("new texture 2", {1,1});

			// Verify
			Assert::IsNotNull(ResourceManager::FindTexture("new texture 1"));
			Assert::IsNotNull(ResourceManager::FindTexture("new texture 2"));

			CLEAN_TEST;
			return;
		}

		// Tests that trying to register the same texture name twice results in nullptr
		TEST_METHOD(Same_Texture_Name_Twice_Exception)
		{
			SETUP_TEST;

			// Setup
			ResourceManager::NewTexture("new texture 1", { 1,1 });

			// Verify
			Assert::ExpectException<std::exception>([]()
				{
					ResourceManager::NewTexture("new texture 1", {1,1});
				});

			CLEAN_TEST;
			return;
		}

		// Tests that trying to find a non-existent texture throws an exception
		TEST_METHOD(Unknown_Texture_Nullptr)
		{
			SETUP_TEST;

			// Setup
			ResourceManager::NewTexture("new texture", { 1,1 });

			// Verify
			Assert::IsNull(ResourceManager::FindTexture("new texture 2"));

			CLEAN_TEST;
			return;
		}

		// Tests that the correct texture will be returned
		TEST_METHOD(Returns_Correct_Texture)
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

				Assert::IsTrue(resources[rndIdx] == ResourceManager::FindTexture(
					((std::stringstream()) << rndIdx).str()
				));
			}


			CLEAN_TEST;
			return;
		}


		/// MESHES

		// Tests that a new mesh can be created
		TEST_METHOD(Can_Create_New_Mesh)
		{
			SETUP_TEST;
			// Setup, Exercise
			ResourceManager::NewMesh("new mesh");

			// Verify
			Assert::IsNotNull(ResourceManager::FindMesh("new mesh"));

			CLEAN_TEST;
			return;
		}

		// Tests that multiple meshs can be created
		TEST_METHOD(Can_Create_Multiple_Meshs)
		{
			SETUP_TEST;
			// Setup, Exercise
			ResourceManager::NewMesh("new mesh 1");
			ResourceManager::NewMesh("new mesh 2");

			// Verify
			Assert::IsNotNull(ResourceManager::FindMesh("new mesh 1"));
			Assert::IsNotNull(ResourceManager::FindMesh("new mesh 2"));

			CLEAN_TEST;
			return;
		}

		// Tests that trying to register the same mesh name twice results in nullptr
		TEST_METHOD(Same_Mesh_Name_Twice_Exception)
		{
			SETUP_TEST;

			// Setup
			ResourceManager::NewMesh("new mesh 1");

			// Verify
			Assert::ExpectException<std::exception>([]()
				{
					ResourceManager::NewMesh("new mesh 1");
				});

			CLEAN_TEST;
			return;
		}

		// Tests that trying to find a non-existent mesh throws an exception
		TEST_METHOD(Unknown_Mesh_Nullptr)
		{
			SETUP_TEST;

			// Setup
			ResourceManager::NewMesh("new mesh");

			// Verify
			Assert::IsNull(ResourceManager::FindMesh("new mesh 2"));

			CLEAN_TEST;
			return;
		}

		// Tests that the correct mesh will be returned
		TEST_METHOD(Returns_Correct_Mesh)
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

				Assert::IsTrue(resources[rndIdx] == ResourceManager::FindMesh(
					((std::stringstream()) << rndIdx).str()
				));
			}


			CLEAN_TEST;
			return;
		}

		// MISC

		// Tests that the same name can be used for different classes of resources
		TEST_METHOD(Same_Name_For_Different_Classes_Valid)
		{
			SETUP_TEST;

			// Setup, Exercise
			Material* mat = ResourceManager::NewMaterial("test");
			Texture* txt = ResourceManager::NewTexture("test", { 1,1 });
			Mesh* msh = ResourceManager::NewMesh("test");

			// Verify
			Assert::IsTrue(mat == ResourceManager::FindMaterial("test"));
			Assert::IsTrue(txt == ResourceManager::FindTexture("test"));
			Assert::IsTrue(msh == ResourceManager::FindMesh("test"));

			return;
			CLEAN_TEST;
		}

		
		// MEMORY LEAK TESTING
		#ifdef _DEBUG
		
		// Tests that no memory leaks occur
		TEST_METHOD(No_Memoryleak)
		{
			SETUP_TEST;

			// Setup
			MemoryLeakDetector mld;
			mld.Init();

			// Exercise
			{
				ResourceManager::NewMaterial("test");
				ResourceManager::NewTexture("test", { 1,1 });
				ResourceManager::NewMesh("test");
				ResourceManager::NewMaterial("test1");
				ResourceManager::NewTexture("test2", { 1,1 });
				ResourceManager::NewMesh("test3");

				ResourceManager::Free();
			}

			// Verify
			Assert::IsFalse(mld.DetectLeak());

			CLEAN_TEST;
			return;
		}

		#endif
	};
}
