#include "CppUnitTest.h"
#include "../Tornado/PixelBuffer.h"
#include "../Eule/Math.h"
#include "../_TestingUtilities/HandyMacros.h"
#include "../_TestingUtilities/MemoryLeakDetector.h"
#include <random>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// The Refit() tests cover the Refit() method, the PixelBuffer(uint8_t* existingData, Vector2i size) constructor, and the copy constructor!
// They all use the Refit() method!

namespace PixelBuffers
{
	TEST_CLASS(_PixelBuffer)
	{
	private:
		std::mt19937 rng;

	public:
		// Constructor
		_PixelBuffer()
		{
			rng = std::mt19937((std::random_device())());
			return;
		}

		// Tests if template argument T is indeed the channel width
		TEST_METHOD(T_Is_ChannelWidth)
		{
			PixelBuffer<6547> pxb({100, 100});

			Assert::AreEqual(6547, (int)pxb.GetChannelWidth());

			return;
		}

		// Tests, if the the pixel buffer is indeed of the supplied dimensions
		TEST_METHOD(Dimensions_Are_Correct)
		{
			// Test 10 times
			for (std::size_t i = 0; i < 10; i++)
			{
				int x = LARGE_RAND_POSITIVE_INT;
				int y = LARGE_RAND_POSITIVE_INT;

				PixelBuffer<77> pxb({ x, y });

				Assert::IsTrue(Vector2i(x, y) == pxb.GetDimensions());
			}
			return;
		}

		// Tests if a runtime_error is thrown if an invalid size is supplied
		TEST_METHOD(Expect_Exception_Invalid_Size)
		{
			Assert::ExpectException<std::runtime_error&>([]()
				{
					PixelBuffer<4> pxb({ 0, 0 });
				}, L"No runtime_error thrown for <4> (0, 0)!");

			Assert::ExpectException<std::runtime_error&>([]()
				{
					PixelBuffer<4> pxb({ 199, 0 });
				}, L"No runtime_error thrown for <4> (199, 0)!");

			Assert::ExpectException<std::runtime_error&>([]()
				{
					PixelBuffer<4> pxb({ 0, 199 });
				}, L"No runtime_error thrown for <4> (0, 199)!");

			Assert::ExpectException<std::runtime_error&>([]()
				{
					PixelBuffer<0> pxb({ 199, 199 });
				}, L"No runtime_error thrown for <0> (199, 199)!");

			Assert::ExpectException<std::runtime_error&>([]()
				{
					PixelBuffer<0> pxb({ 0, 199 });
				}, L"No runtime_error thrown for <0> (0, 199)!");

			Assert::ExpectException<std::runtime_error&>([]()
				{
					PixelBuffer<0> pxb({ 0, 0 });
				}, L"No runtime_error thrown for <0> (0, 0)!");

			Assert::ExpectException<std::runtime_error&>([]()
				{
					PixelBuffer<4> pxb({ -199, 199 });
				}, L"No runtime_error thrown for <4> (-199, 199)!");

			Assert::ExpectException<std::runtime_error&>([]()
				{
					PixelBuffer<4> pxb({ 199, -199 });
				}, L"No runtime_error thrown for <4> (199, -199)!");

			Assert::ExpectException<std::runtime_error&>([]()
				{
					PixelBuffer<4> pxb({ -199, -199 });
				}, L"No runtime_error thrown for <4> (-199, -199)!");

			return;
		}

		// Tests if the buffer size is correct
		TEST_METHOD(Buffersize_Is_Correct)
		{
			// Test 10 times
			for (std::size_t i = 0; i < 10; i++)
			{
				int x = LARGE_RAND_POSITIVE_INT;
				int y = LARGE_RAND_POSITIVE_INT;

				PixelBuffer<4> pxb({ x, y });

				// Check function call
				Assert::AreEqual(x*y*4, (int)pxb.GetSizeofBuffer(), L"Check function call failed");

				// Check that the last position is indeed accessible
				pxb.GetRawData()[pxb.GetSizeofBuffer() - 1] = 39;
				Assert::AreEqual(39, (int)pxb.GetRawData()[pxb.GetSizeofBuffer() - 1], L"Set value not equal");
			}
		}

		// Tests if the buffer size is correct using the refit-constructor
		TEST_METHOD(Buffersize_Is_Correct_Refit)
		{
			// Test 10 times
			for (std::size_t i = 0; i < 10; i++)
			{
				std::size_t x = LARGE_RAND_POSITIVE_INT;
				std::size_t y = LARGE_RAND_POSITIVE_INT;

				// Create existing buffer
				uint8_t* existingData = new uint8_t[x * y * (std::size_t)5];

				// Create pixelbuffer object
				PixelBuffer<5> pxb(existingData, { (int)x, (int)y });

				// Check function call
				Assert::AreEqual(x * y * (std::size_t)5, (std::size_t)pxb.GetSizeofBuffer(), L"Check function call failed");

				// Check that the last position is indeed accessible
				pxb.GetRawData()[pxb.GetSizeofBuffer() - 1] = 39;
				Assert::AreEqual(39, (int)pxb.GetRawData()[pxb.GetSizeofBuffer() - 1], L"Set value not equal");
			
				// Cleanup
				delete[] existingData;
			}
		}

		// Tests if the value returned by GetDimensions is correct with the blank-constructor
		TEST_METHOD(GetDimensions_Blank)
		{
			// Test 10 times
			for (std::size_t i = 0; i < 10; i++)
			{
				int x = LARGE_RAND_POSITIVE_INT;
				int y = LARGE_RAND_POSITIVE_INT;

				PixelBuffer<4> pxb({ x, y });

				// Check function call
				Assert::AreEqual(x, pxb.GetDimensions().x, L"Width failed");
				Assert::AreEqual(y, pxb.GetDimensions().y, L"Height failed");

				return;
			}
		}

		// Tests if the value returned by GetDimensions is correct with the blank-constructor
		TEST_METHOD(GetDimensions_Refit)
		{
			// Test 10 times
			for (std::size_t i = 0; i < 10; i++)
			{
				std::size_t x = LARGE_RAND_POSITIVE_INT;
				std::size_t y = LARGE_RAND_POSITIVE_INT;

				// Create existing buffer
				uint8_t* existingData = new uint8_t[x * y * (std::size_t)2];

				PixelBuffer<2> pxb(existingData, { (int)x, (int)y });

				// Cleanup
				delete[] existingData;

				// Check function call
				Assert::AreEqual((int)x, pxb.GetDimensions().x, L"Width failed");
				Assert::AreEqual((int)y, pxb.GetDimensions().y, L"Height failed");
			}
		}


		// Tests if a new pixel buffer is zeroed
		TEST_METHOD(New_Pxb_Is_Zeroed)
		{
			PixelBuffer<5> pxb({ 50, 50 });

			constexpr std::size_t size = 50*50*5;
			for (std::size_t i = 0; i < size; i++)
				Assert::AreEqual(0, (int)pxb.GetRawData()[i]);

			return;
		}

		// Tests if the pixel buffer can copy existing data via Refit(), matching by value
		TEST_METHOD(Refit_Can_Copy_Existing_Data_Values_Match)
		{
			constexpr std::size_t size = 128*128*3;

			// Create random existing data
			uint8_t* existingData = new uint8_t[size];
			
			for (std::size_t i = 0; i < size; i++)
				existingData[i] = rng() % 255;

			// Create pixelbuffer from it
			PixelBuffer<3> pxb(existingData, { 128, 128 });

			// Assert, that the two data segments are identical
			for (std::size_t i = 0; i < size; i++)
				Assert::AreEqual(existingData[i], pxb.GetRawData()[i]);

			// Cleanup
			delete[] existingData;

			return;
		}

		// Tests if the pixel buffer can copy existing data via Refit(), when the pixelbuffer is already initialized, matching by value
		TEST_METHOD(Refit_Again_Can_Copy_Existing_Data_Values_Match)
		{
			constexpr std::size_t size = 128*128*2;

			// Create pixel buffer of different size
			PixelBuffer<2> pxb({ 64, 64 });

			// Create random existing data
			uint8_t* existingData = new uint8_t[size];
			for (std::size_t i = 0; i < size; i++)
				existingData[i] = rng() % 255;

			// Fit buffer to new data
			pxb.Refit(existingData, { 128, 128 });

			// Assert, that the two data segments are identical
			for (std::size_t i = 0; i < size; i++)
				Assert::AreEqual(existingData[i], pxb.GetRawData()[i]);

			// Cleanup
			delete[] existingData;

			return;
		}

		// Tests that if copied via Refit() from an existing data segment, changing the resulting Object does not change the initial data
		TEST_METHOD(Refit_Can_Copy_Existing_Data_Actual_Copy)
		{
			constexpr std::size_t size = 128*128*1;

			// Create existing data
			uint8_t* existingData = new uint8_t[size];

			for (std::size_t i = 0; i < size; i++)
				existingData[i] = 0;

			// Create pixelbuffer from it
			PixelBuffer<1> pxb(existingData, { 128, 128 });

			// Modify existing data, changing every single element
			for (std::size_t i = 0; i < size; i++)
				existingData[i] = rng() % 250 + 5;

			// Assert, that none two data segments are identical
			for (std::size_t i = 0; i < size; i++)
				Assert::AreNotEqual(existingData[i], pxb.GetRawData()[i]);

			// Cleanup
			delete[] existingData;

			return;
		}

		// Tests that if copied via Refit() from an existing data segment, when the pixelbuffer is already initialized, changing the resulting Object does not change the initial data
		TEST_METHOD(Refit_Again_Can_Copy_Existing_Data_Actual_Copy)
		{
			constexpr std::size_t size = 128*128*2;

			// Create pixel buffer of different size
			PixelBuffer<2> pxb({ 64, 64 });

			// Create existing data
			uint8_t* existingData = new uint8_t[size];
			for (std::size_t i = 0; i < size; i++)
				existingData[i] = 0;

			// Fit buffer to new data
			pxb.Refit(existingData, { 128, 128 });

			// Modify existing data, changing every single element
			for (std::size_t i = 0; i < size; i++)
				existingData[i] = rng() % 250 + 5;

			// Assert, that none two data segments are identical
			for (std::size_t i = 0; i < size; i++)
				Assert::AreNotEqual(existingData[i], pxb.GetRawData()[i]);

			// Cleanup
			delete[] existingData;

			return;
		}

		// Tests that refitting a pixel buffer actually changes the dimensions
		TEST_METHOD(Refit_Changes_Dimensions)
		{
			// Create pixel buffer of different size
			PixelBuffer<3> pxb({ 128, 256 });

			Assert::AreEqual(128, pxb.GetDimensions().x, L"Failed at x=128");
			Assert::AreEqual(256, pxb.GetDimensions().y, L"Failed at y=256");

			// Now refit to data of size 512x1024
			// This is really not the usecase of Refit().
			// It should not be used just to change the dimensions.
			// It should be used to fit the pixel buffer to existing pixel data of arbitrary size.
			uint8_t* existingData = new uint8_t[512 * 1024 * 3];
			pxb.Refit(existingData, { 512, 1024 });

			Assert::AreEqual(512, pxb.GetDimensions().x, L"Failed at x=512");
			Assert::AreEqual(1024, pxb.GetDimensions().y, L"Failed at y=1024");

			return;
		}

		// Tests that GetPixel returns the correct data
		TEST_METHOD(GetPixel)
		{
			// Create pixel buffer
			PixelBuffer<2> pxb({ 64, 64 });

			// Set pixel values manually
			for (uint8_t x = 0; x < 64; x++)
				for (uint8_t y = 0; y < 64; y++)
				{
					pxb.GetRawData()[2*(y*64 + x) + 0] = x;
					pxb.GetRawData()[2*(y*64 + x) + 1] = y;
				}

			// Compare to the values returned from GetPixel()
			for (uint8_t x = 0; x < 64; x++)
				for (uint8_t y = 0; y < 64; y++)
				{
					Assert::AreEqual(x, pxb.GetPixel({ (int)x, (int)y })[0]);
					Assert::AreEqual(y, pxb.GetPixel({ (int)x, (int)y })[1]);
				}

			return;
		}

		// Tests that GetPixel const returns the correct data
		TEST_METHOD(GetPixel_Const)
		{
			// Create pixel buffer
			PixelBuffer<2> pxb({ 64, 64 });


			// Set pixel values manually
			for (uint8_t x = 0; x < 64; x++)
				for (uint8_t y = 0; y < 64; y++)
				{
					pxb.GetRawData()[2 * (y * 64 + x) + 0] = x;
					pxb.GetRawData()[2 * (y * 64 + x) + 1] = y;
				}

			const PixelBuffer<2>& const_pxb = pxb;
			// Compare to the values returned from GetPixel()
			for (uint8_t x = 0; x < 64; x++)
				for (uint8_t y = 0; y < 64; y++)
				{
					Assert::AreEqual(x, const_pxb.GetPixel({ (int)x, (int)y })[0]);
					Assert::AreEqual(y, const_pxb.GetPixel({ (int)x, (int)y })[1]);
				}

			return;
		}

		// Tests that GetPixel with channel offset returns the correct data
		TEST_METHOD(GetPixel_ChannelOffset)
		{
			// Create pixel buffer
			PixelBuffer<2> pxb({ 64, 64 });

			// Set pixel values manually
			for (uint8_t x = 0; x < 64; x++)
				for (uint8_t y = 0; y < 64; y++)
				{
					pxb.GetRawData()[2 * (y * 64 + x) + 0] = x;
					pxb.GetRawData()[2 * (y * 64 + x) + 1] = y;
				}

			// Compare to the values returned from GetPixel()
			for (uint8_t x = 0; x < 64; x++)
				for (uint8_t y = 0; y < 64; y++)
				{
					Assert::AreEqual(x, *pxb.GetPixel({ (int)x, (int)y }, 0));
					Assert::AreEqual(y, *pxb.GetPixel({ (int)x, (int)y }, 1));
				}

			return;
		}

		// Tests that GetPixel const returns the correct data
		TEST_METHOD(GetPixel_ChannelOffset_Const)
		{
			// Create pixel buffer
			PixelBuffer<2> pxb({ 64, 64 });


			// Set pixel values manually
			for (uint8_t x = 0; x < 64; x++)
				for (uint8_t y = 0; y < 64; y++)
				{
					pxb.GetRawData()[2 * (y * 64 + x) + 0] = x;
					pxb.GetRawData()[2 * (y * 64 + x) + 1] = y;
				}

			const PixelBuffer<2>& const_pxb = pxb;
			// Compare to the values returned from GetPixel()
			for (uint8_t x = 0; x < 64; x++)
				for (uint8_t y = 0; y < 64; y++)
				{
					Assert::AreEqual(x, *const_pxb.GetPixel({ (int)x, (int)y }, 0));
					Assert::AreEqual(y, *const_pxb.GetPixel({ (int)x, (int)y }, 1));
				}

			return;
		}

		// Tests that refitting a pixel buffer actually changes the dimensions
		TEST_METHOD(Can_Access_Refitted_Data)
		{
			// Create pixel buffer of different size
			PixelBuffer<8> pxb({ 32, 32 });

			// This should not work now.
			// Write x50, y50
			// pxb.GetRawData()[50*32 + 50] = 128;

			// Create existing data
			uint8_t* existingData = new uint8_t[64 * 64 * 8];

			// Fit buffer to new data
			pxb.Refit(existingData, { 64, 64 });

			// This should work now
			// Write to x50, y50
			pxb.GetRawData()[50*64 + 50] = 128;

			// Cleanup
			delete[] existingData;
		}


		// We need this many tests to test Clear() because it is highly optimized for specific setups

		// Tests that clearing the pixel buffer with a solid grayscale color works with 3 channels
		TEST_METHOD(Clear_Grayscale_3chn)
		{
			// Setup
			PixelBuffer<3> pxb({ 69, 69 });

			// Exercise
			pxb.Clear({ 129, 129, 129 });

			// Verify
			for (int x = 0; x < pxb.GetDimensions().x; x++)
				for (int y = 0; y < pxb.GetDimensions().y; y++)
				{
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 0));
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 1));
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 2));
				}

			return;
		}

		// Tests that clearing the pixel buffer with a solid grayscale color works with 4 channels
		TEST_METHOD(Clear_Grayscale_4chn)
		{
			// Setup
			PixelBuffer<4> pxb({ 69, 69 });

			// Exercise
			pxb.Clear({ 129, 129, 129, 129 });

			// Verify
			for (int x = 0; x < pxb.GetDimensions().x; x++)
				for (int y = 0; y < pxb.GetDimensions().y; y++)
				{
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 0));
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 1));
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 2));
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 3));
				}

			return;
		}

		// Tests that clearing the pixel buffer with a solid color works with 1 channel
		TEST_METHOD(Clear_1chn)
		{
			// Setup
			PixelBuffer<1> pxb({ 69, 69 });

			// Exercise
			pxb.Clear({ 129 });

			// Verify
			for (int x = 0; x < pxb.GetDimensions().x; x++)
				for (int y = 0; y < pxb.GetDimensions().y; y++)
				{
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 0));
				}

			return;
		}

		// Tests that clearing the pixel buffer with a solid Rg color works with 4 channels
		TEST_METHOD(Clear_Rg_2chn)
		{
			// Setup
			PixelBuffer<2> pxb({ 69, 69 });

			// Exercise
			pxb.Clear({ 129, 72 });

			// Verify
			for (int x = 0; x < pxb.GetDimensions().x; x++)
				for (int y = 0; y < pxb.GetDimensions().y; y++)
				{
					// Generate debug outout
					std::wstringstream wss;
					wss << std::endl
						<< "x: " << x << std::endl
						<< "y: " << y << std::endl
						<< "chn0: " << *pxb.GetPixel({ x, y }, 0) << std::endl
						<< "chn1: " << *pxb.GetPixel({ x, y }, 1) << std::endl
						<< std::endl;

					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 0), wss.str().c_str());
					Assert::AreEqual(uint8_t(72), *pxb.GetPixel({ x, y }, 1), wss.str().c_str());
				}

			return;
		}

		// Tests that clearing the pixel buffer with a solid rgb color works with 3 channels
		TEST_METHOD(Clear_Rgb_3chn)
		{
			// Setup
			PixelBuffer<3> pxb({ 69, 69 });

			// Exercise
			pxb.Clear({ 129, 72, 69 });

			// Verify
			for (int x = 0; x < pxb.GetDimensions().x; x++)
				for (int y = 0; y < pxb.GetDimensions().y; y++)
				{
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 0));
					Assert::AreEqual(uint8_t(72), *pxb.GetPixel({ x, y }, 1));
					Assert::AreEqual(uint8_t(69), *pxb.GetPixel({ x, y }, 2));
				}

			return;
		}

		// Tests that clearing the pixel buffer with a solid rgba color works with 4 channels
		TEST_METHOD(Clear_Rgba_4chn)
		{
			// Setup
			PixelBuffer<4> pxb({ 69, 69 });

			// Exercise
			pxb.Clear({ 129, 72, 69, 89 });

			// Verify
			for (int x = 0; x < pxb.GetDimensions().x; x++)
				for (int y = 0; y < pxb.GetDimensions().y; y++)
				{
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 0));
					Assert::AreEqual(uint8_t(72), *pxb.GetPixel({ x, y }, 1));
					Assert::AreEqual(uint8_t(69), *pxb.GetPixel({ x, y }, 2));
					Assert::AreEqual(uint8_t(89), *pxb.GetPixel({ x, y }, 3));
				}

			return;
		}

		// Tests that clearing the pixel buffer with a solid rgbax color works with 5 channels
		TEST_METHOD(Clear_Rgbax_5chn)
		{
			// Setup
			PixelBuffer<5> pxb({ 69, 69 });

			// Exercise
			pxb.Clear({ 129, 72, 69, 89, 32 });

			// Verify
			for (int x = 0; x < pxb.GetDimensions().x; x++)
				for (int y = 0; y < pxb.GetDimensions().y; y++)
				{
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 0));
					Assert::AreEqual(uint8_t(72), *pxb.GetPixel({ x, y }, 1));
					Assert::AreEqual(uint8_t(69), *pxb.GetPixel({ x, y }, 2));
					Assert::AreEqual(uint8_t(89), *pxb.GetPixel({ x, y }, 3));
					Assert::AreEqual(uint8_t(32), *pxb.GetPixel({ x, y }, 4));
				}

			return;
		}

		// Tests that clearing the pixel buffer with a solid rgbaxxxx color works with 8 channels
		TEST_METHOD(Clear_Rgbaxxxx_8chn)
		{
			// Setup
			PixelBuffer<8> pxb({ 69, 69 });

			// Exercise
			pxb.Clear({ 129, 72, 69, 89, 32, 19, 15, 3 });

			// Verify
			for (int x = 0; x < pxb.GetDimensions().x; x++)
				for (int y = 0; y < pxb.GetDimensions().y; y++)
				{
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 0));
					Assert::AreEqual(uint8_t(72), *pxb.GetPixel({ x, y }, 1));
					Assert::AreEqual(uint8_t(69), *pxb.GetPixel({ x, y }, 2));
					Assert::AreEqual(uint8_t(89), *pxb.GetPixel({ x, y }, 3));
					Assert::AreEqual(uint8_t(32), *pxb.GetPixel({ x, y }, 4));
					Assert::AreEqual(uint8_t(19), *pxb.GetPixel({ x, y }, 5));
					Assert::AreEqual(uint8_t(15), *pxb.GetPixel({ x, y }, 6));
					Assert::AreEqual(uint8_t(3), *pxb.GetPixel({ x, y }, 7));
				}

			return;
		}

		// Tests that clearing the pixel buffer with a solid grayscale color works with 8 channels
		TEST_METHOD(Clear_Grayscale_8chn)
		{
			// Setup
			PixelBuffer<8> pxb({ 69, 69 });

			// Exercise
			pxb.Clear({ 129, 129, 129, 129, 129, 129, 129, 129 });

			// Verify
			for (int x = 0; x < pxb.GetDimensions().x; x++)
				for (int y = 0; y < pxb.GetDimensions().y; y++)
				{
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 0));
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 1));
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 2));
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 3));
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 4));
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 5));
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 6));
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 7));
				}

			return;
		}

		// Tests that clearing the pixel buffer with a solid rgbaxxxxxx color works with 10 channels
		TEST_METHOD(Clear_Rgbaxxxxxx_10chn)
		{
			// Setup
			PixelBuffer<10> pxb({ 69, 69 });

			// Exercise
			pxb.Clear({ 129, 72, 69, 89, 32, 19, 15, 3, 5, 95 });

			// Verify
			for (int x = 0; x < pxb.GetDimensions().x; x++)
				for (int y = 0; y < pxb.GetDimensions().y; y++)
				{
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 0));
					Assert::AreEqual(uint8_t(72), *pxb.GetPixel({ x, y }, 1));
					Assert::AreEqual(uint8_t(69), *pxb.GetPixel({ x, y }, 2));
					Assert::AreEqual(uint8_t(89), *pxb.GetPixel({ x, y }, 3));
					Assert::AreEqual(uint8_t(32), *pxb.GetPixel({ x, y }, 4));
					Assert::AreEqual(uint8_t(19), *pxb.GetPixel({ x, y }, 5));
					Assert::AreEqual(uint8_t(15), *pxb.GetPixel({ x, y }, 6));
					Assert::AreEqual(uint8_t(3), *pxb.GetPixel({ x, y }, 7));
					Assert::AreEqual(uint8_t(5), *pxb.GetPixel({ x, y }, 8));
					Assert::AreEqual(uint8_t(95), *pxb.GetPixel({ x, y }, 9));
				}

			return;
		}

		// Tests that clearing the pixel buffer with a solid grayscale color works with 10 channels
		TEST_METHOD(Clear_Grayscale_10chn)
		{
			// Setup
			PixelBuffer<10> pxb({ 69, 69 });

			// Exercise
			pxb.Clear({ 129, 129, 129, 129, 129, 129, 129, 129, 129, 129 });

			// Verify
			for (int x = 0; x < pxb.GetDimensions().x; x++)
				for (int y = 0; y < pxb.GetDimensions().y; y++)
				{
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 0));
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 1));
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 2));
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 3));
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 4));
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 5));
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 6));
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 7));
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 8));
					Assert::AreEqual(uint8_t(129), *pxb.GetPixel({ x, y }, 9));
				}

			return;
		}


		// =========== MEMORY LEAK TESTS ===========
		// These tests depends on debug-mode for memory insights.
		// Thus, they only works in debug mode.
		#ifdef _DEBUG

		// Tests for the PixelBuffer not creating a memory leak when construction via the blank constructor
		TEST_METHOD(Blank_No_Memoryleak)
		{
			MemoryLeakDetector mld;
			mld.Init();

			{
				PixelBuffer<8> pxb({ 256, 256 });
			}

			Assert::IsFalse(mld.DetectLeak());

			return;
		}

		// Tests for the PixelBuffer not creating a memory leak when construction via the refit constructor
		TEST_METHOD(Refit_No_Memoryleak)
		{
			// Create existing buffer
			uint8_t* existingData = new uint8_t[128*128*8];

			MemoryLeakDetector mld;
			mld.Init();
			{
				PixelBuffer<8> pxb(existingData, { 128, 128 });
			}
			Assert::IsFalse(mld.DetectLeak());

			// Cleanup
			delete[] existingData;

			return;
		}

		// Tests for the PixelBuffer not creating a memory leak when refitting an existing pixel buffer
		TEST_METHOD(Refit_Again_No_Memoryleak)
		{
			// Create existing buffer
			uint8_t* existingData = new uint8_t[128*128*8];

			MemoryLeakDetector mld;
			mld.Init();
			{
				// Create pixel buffer of different size
				// This will allocate memory for a blank buffer
				PixelBuffer<8> pxb({ 32, 32 });

				// Now refit it to a completely new buffer
				pxb.Refit(existingData, { 128, 128 });
			}
			Assert::IsFalse(mld.DetectLeak());

			// Cleanup
			delete[] existingData;

			return;
		}

		#endif
	};
}
