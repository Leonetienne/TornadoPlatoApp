#include "CppUnitTest.h"
#include "../Tornado/Texture.h"
#include "../_TestingUtilities/Similar.h"
#include "../_TestingUtilities/HandyMacros.h"
#include "../_TestingUtilities/MemoryLeakDetector.h"
#include <random>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TexturingAndStuff
{
	TEST_CLASS(_Texture)
	{
	private:
		std::mt19937 rng;

	public:
		// Constructor
		_Texture()
		{
			rng = std::mt19937((std::random_device())());
			return;
		}

		// Tests that a texture constructed only by color is of size 1x1
		TEST_METHOD(Color_Only_Is_Size_1x1)
		{
			Texture txt(Color::green);
			Assert::AreEqual(1, (int)txt.GetPixelBuffer().GetDimensions().x);
			Assert::AreEqual(1, (int)txt.GetPixelBuffer().GetDimensions().y);

			return;
		}

		// Tests that a texture constructed only by color does match that color
		TEST_METHOD(Color_Only_Color_Matches)
		{
			// Test 100 timews
			for (std::size_t i = 0; i < 100; i++)
			{
				// Generate random color
				const int r = rng() % 255;
				const int g = rng() % 255;
				const int b = rng() % 255;
				const int a = rng() % 255;
				const Color textureCol(Color(r, g, b, a));

				// Generate texture of random size from it
				Texture txt(textureCol);
				
				// Check that the pixel color matches the generated color
				Color pixelCol(
					*txt.GetPixelBuffer().GetPixel({ 0, 0 }, 0),
					*txt.GetPixelBuffer().GetPixel({ 0, 0 }, 1),
					*txt.GetPixelBuffer().GetPixel({ 0, 0 }, 2),
					*txt.GetPixelBuffer().GetPixel({ 0, 0 }, 3)
				);
				Assert::IsTrue(Color(r,g,b,a) == pixelCol);
			}

			return;
		}

		// Tests when a texture is constructed from a color and a size, that it matches the size
		TEST_METHOD(Color_Size_Matches_Size)
		{
			// Test 10 times
			for (std::size_t i = 0; i < 10; i++)
			{
				int x = LARGE_RAND_POSITIVE_INT;
				int y = LARGE_RAND_POSITIVE_INT;

				Texture txt(Color::green, { x, y });
				Assert::AreEqual(x, (int)txt.GetPixelBuffer().GetDimensions().x);
				Assert::AreEqual(y, (int)txt.GetPixelBuffer().GetDimensions().y);
			}

			return;
		}

		// Tests when a texture is constructed from a color and a size, that all pixels match that color
		TEST_METHOD(Color_Size_All_Pixels_Match_Color)
		{
			// Test 10 times
			for (std::size_t i = 0; i < 10; i++)
			{
				// Generate random color
				const int r = rng() % 255;
				const int g = rng() % 255;
				const int b = rng() % 255;
				const int a = rng() % 255;
				const Color textureCol(Color(r, g, b, a));

				// Generate texture of random size from it
				// Don't make the textures too large, so the unit tests won't take decades to run
				int x = rng() % 33 + 1;
				int y = rng() % 33 + 1;
				Texture txt(textureCol, { x, y });

				// Check for each pixel, that it matches the generated color
				const std::size_t numPixels = txt.GetPixelBuffer().GetSizeofBuffer();
				for (std::size_t j = 0; j < numPixels; j += 4)
				{
					Color pixelCol(
						txt.GetPixelBuffer().GetRawData()[j + 0],
						txt.GetPixelBuffer().GetRawData()[j + 1],
						txt.GetPixelBuffer().GetRawData()[j + 2],
						txt.GetPixelBuffer().GetRawData()[j + 3]
					);

					Assert::IsTrue(textureCol == pixelCol);
				}

			}

			return;
		}

		// Tests that when constructing from a pixel buffer, that it clones that pixel buffer correctly
		TEST_METHOD(Construct_From_Pixelbuffer_Values_Matches)
		{
			PixelBuffer<4> pxb({ 128, 128 });
			pxb.GetRawData()[0] = 128;

			Texture txt(pxb);

			Assert::AreEqual(pxb.GetRawData()[0], txt.GetPixelBuffer().GetRawData()[0]);

			return;
		}

		// Tests that when constructing from a pixel buffer, that changing the new buffer does not affect the old one
		TEST_METHOD(Construct_From_Pixelbuffer_Values_Are_Independent)
		{
			PixelBuffer<4> pxb({ 128, 128 });
			pxb.GetRawData()[0] = 128;

			Texture txt(pxb);
			txt.GetPixelBuffer().GetRawData()[0] = 255;

			Assert::AreNotEqual(pxb.GetRawData()[0], txt.GetPixelBuffer().GetRawData()[0]);

			return;
		}


		// =========== MEMORY LEAK TESTS ===========
		// These tests depends on debug-mode for memory insights.
		// Thus, they only works in debug mode.
		#ifdef _DEBUG

		// Tests that when just creating a color texture, no memory leak occurs
		TEST_METHOD(No_Memleak_Color)
		{
			MemoryLeakDetector mld;
			mld.Init();
		
			{
				Texture txt(Color::red);
			}

			Assert::IsFalse(mld.DetectLeak());
			return;
		}

		// Tests that when creating sized color texture, no memory leak occurs
		TEST_METHOD(No_Memleak_Color_Size)
		{
			MemoryLeakDetector mld;
			mld.Init();

			{
				Texture txt(Color::red, { 125, 125 });
			}

			Assert::IsFalse(mld.DetectLeak());
			return;
		}

		// Tests that when creating a texture from a pixel buffer, no memory leak occurs
		TEST_METHOD(No_Memleak_ExistingBuffer)
		{
			PixelBuffer<4> pxb({125, 125});

			MemoryLeakDetector mld;
			mld.Init();

			{
				Texture txt(pxb);
			}

			Assert::IsFalse(mld.DetectLeak());
			return;
		}

		// Tests that when swapping the pixel buffer, no memory leak occurs
		TEST_METHOD(No_Memleak_SwappingPixelBuffer)
		{
			PixelBuffer<4> pxb_a({ 125, 125 });
			PixelBuffer<4> pxb_b({ 125, 125 });

			MemoryLeakDetector mld;
			mld.Init();

			{
				Texture txt(pxb_a);
				txt.SetPixelBuffer(pxb_b);
			}

			Assert::IsFalse(mld.DetectLeak());
			return;
		}

		// Tests that when using the copy constructor, no memory leak occurs
		TEST_METHOD(No_Memleak_CopyConstructor)
		{
			Texture txt_a(PixelBuffer<4>({ 125, 125 }));

			MemoryLeakDetector mld;
			mld.Init();

			{
				Texture txt_b(txt_a);
			}

			Assert::IsFalse(mld.DetectLeak());
			return;
		}

		#endif
	};
}
