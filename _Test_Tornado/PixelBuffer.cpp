#include "../_TestingUtilities/Catch2.h"
#include "../Tornado/PixelBuffer.h"
#include "../_TestingUtilities/HandyMacros.h"
#include <random>

using namespace TorGL;

namespace {
    static std::mt19937 rng = std::mt19937((std::random_device())());
}

// The Refit() tests cover the Refit() method, the PixelBuffer(uint8_t* existingData, Vector2i size) constructor, and the copy constructor!
// They all use the Refit() method!


// Tests if template argument T is indeed the channel width
TEST_CASE(__FILE__"./T_Is_ChannelWidth", "[Pixelbuffer]")
{
    PixelBuffer<6547> pxb({100, 100});

    REQUIRE(6547 == pxb.GetChannelWidth());
}

// Tests, if the the pixel buffer is indeed of the supplied dimensions
TEST_CASE(__FILE__"/Dimensions_Are_Correct", "[Pixelbuffer]")
{
    // Test 10 times
    for (std::size_t i = 0; i < 10; i++)
    {
        int x = LARGE_RAND_POSITIVE_INT;
        int y = LARGE_RAND_POSITIVE_INT;

        PixelBuffer<77> pxb({ x, y });

        REQUIRE(Vector2i(x, y) == pxb.GetDimensions());
    }
    return;
}

// Tests if a runtime_error is thrown if an invalid size is supplied
TEST_CASE(__FILE__"/Expect_Exception_Invalid_Size", "[Pixelbuffer]")
{
    REQUIRE_THROWS_AS(PixelBuffer<4>({ 0, 0 }), std::runtime_error);
    REQUIRE_THROWS_AS(PixelBuffer<4>({ 199, 0 }), std::runtime_error);
    REQUIRE_THROWS_AS(PixelBuffer<4>({ 0, 199 }), std::runtime_error);
    REQUIRE_THROWS_AS(PixelBuffer<0>({ 199, 199 }), std::runtime_error);
    REQUIRE_THROWS_AS(PixelBuffer<0>({ 0, 199 }), std::runtime_error);
    REQUIRE_THROWS_AS(PixelBuffer<0>({ 0, 0 }), std::runtime_error);
    REQUIRE_THROWS_AS(PixelBuffer<4>({ -199, 199 }), std::runtime_error);
    REQUIRE_THROWS_AS(PixelBuffer<4>({ 199, -199 }), std::runtime_error);
    REQUIRE_THROWS_AS(PixelBuffer<4>({ -199, -199 }), std::runtime_error);
}

// Tests if the buffer size is correct
TEST_CASE(__FILE__"/Buffersize_Is_Correct", "[Pixelbuffer]")
{
    // Test 10 times
    for (std::size_t i = 0; i < 10; i++)
    {
        int x = LARGE_RAND_POSITIVE_INT;
        int y = LARGE_RAND_POSITIVE_INT;

        PixelBuffer<4> pxb({ x, y });

        // Check function call
        REQUIRE(x*y*4 == (int)pxb.GetSizeofBuffer());

        // Check that the last position is indeed accessible
        pxb.GetRawData()[pxb.GetSizeofBuffer() - 1] = 39;
        REQUIRE(39 == (int)pxb.GetRawData()[pxb.GetSizeofBuffer() - 1]);
    }
}

// Tests if the buffer size is correct using the refit-constructor
TEST_CASE(__FILE__"/Buffersize_Is_Correct_Refit", "[Pixelbuffer]")
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
        REQUIRE(x * y * (std::size_t)5 == (std::size_t)pxb.GetSizeofBuffer());

        // Check that the last position is indeed accessible
        pxb.GetRawData()[pxb.GetSizeofBuffer() - 1] = 39;
        REQUIRE(39 == (int)pxb.GetRawData()[pxb.GetSizeofBuffer() - 1]);
    
        // Cleanup
        delete[] existingData;
    }
}

// Tests if the value returned by GetDimensions is correct with the blank-constructor
TEST_CASE(__FILE__"/GetDimensions_Blank", "[Pixelbuffer]")
{
    // Test 10 times
    for (std::size_t i = 0; i < 10; i++)
    {
        int x = LARGE_RAND_POSITIVE_INT;
        int y = LARGE_RAND_POSITIVE_INT;

        PixelBuffer<4> pxb({ x, y });

        // Check function call
        REQUIRE(x == pxb.GetDimensions().x);
        REQUIRE(y == pxb.GetDimensions().y);

        return;
    }
}

// Tests if the value returned by GetDimensions is correct with the blank-constructor
TEST_CASE(__FILE__"/GetDimensions_Refit", "[Pixelbuffer]")
{
    // Test 10 times
    for (std::size_t i = 0; i < 100; i++)
    {
        std::size_t x = LARGE_RAND_POSITIVE_INT | 1;
        std::size_t y = LARGE_RAND_POSITIVE_INT | 1; // anding with 1 to ensure it's never 0

        // Create existing buffer
        uint8_t* existingData = new uint8_t[x * y * (std::size_t)2];

        PixelBuffer<2> pxb(existingData, { (int)x, (int)y });

        // Cleanup
        delete[] existingData;

        // Check function call
        REQUIRE((int)x == pxb.GetDimensions().x);
        REQUIRE((int)y == pxb.GetDimensions().y);
    }
}


// Tests if a new pixel buffer is zeroed
TEST_CASE(__FILE__"/New_Pxb_Is_Zeroed", "[Pixelbuffer]")
{
    PixelBuffer<5> pxb({ 50, 50 });

    constexpr std::size_t size = 50*50*5;
    for (std::size_t i = 0; i < size; i++)
        REQUIRE(0 == (int)pxb.GetRawData()[i]);

    return;
}

// Tests if the pixel buffer can copy existing data via Refit(), matching by value
TEST_CASE(__FILE__"/Refit_Can_Copy_Existing_Data_Values_Match", "[Pixelbuffer]")
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
        REQUIRE(existingData[i] == pxb.GetRawData()[i]);

    // Cleanup
    delete[] existingData;

    return;
}

// Tests if the pixel buffer can copy existing data via Refit(), when the pixelbuffer is already initialized, matching by value
TEST_CASE(__FILE__"/Refit_Again_Can_Copy_Existing_Data_Values_Match",  "[Pixelbuffer]")
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
        REQUIRE(existingData[i] == pxb.GetRawData()[i]);

    // Cleanup
    delete[] existingData;

    return;
}

// Tests that if copied via Refit() from an existing data segment, changing the resulting Object does not change the initial data
TEST_CASE(__FILE__"/Refit_Can_Copy_Existing_Data_Actual_Copy",  "[Pixelbuffer]")
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
        REQUIRE_FALSE(existingData[i] == pxb.GetRawData()[i]);

    // Cleanup
    delete[] existingData;

    return;
}

// Tests that if copied via Refit() from an existing data segment, when the pixelbuffer is already initialized, changing the resulting Object does not change the initial data
TEST_CASE(__FILE__"/Refit_Again_Can_Copy_Existing_Data_Actual_Copy",  "[Pixelbuffer]")
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
        REQUIRE_FALSE(existingData[i] == pxb.GetRawData()[i]);

    // Cleanup
    delete[] existingData;

    return;
}

// Tests that refitting a pixel buffer actually changes the dimensions
TEST_CASE(__FILE__"/Refit_Changes_Dimensions",  "[Pixelbuffer]")
{
    // Create pixel buffer of different size
    PixelBuffer<3> pxb({ 128, 256 });

    REQUIRE(128 == pxb.GetDimensions().x);
    REQUIRE(256 == pxb.GetDimensions().y);

    // Now refit to data of size 512x1024
    // This is really not the usecase of Refit().
    // It should not be used just to change the dimensions.
    // It should be used to fit the pixel buffer to existing pixel data of arbitrary size.
    uint8_t* existingData = new uint8_t[512 * 1024 * 3];
    pxb.Refit(existingData, { 512, 1024 });

    REQUIRE(512 == pxb.GetDimensions().x);
    REQUIRE(1024 == pxb.GetDimensions().y);

    return;
}

// Tests that GetPixel returns the correct data
TEST_CASE(__FILE__"/GetPixel",  "[Pixelbuffer]")
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
            REQUIRE(x == pxb.GetPixel({ (int)x, (int)y })[0]);
            REQUIRE(y == pxb.GetPixel({ (int)x, (int)y })[1]);
        }

    return;
}

// Tests that GetPixel const returns the correct data
TEST_CASE(__FILE__"/GetPixel_Const",  "[Pixelbuffer]")
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
            REQUIRE(x == const_pxb.GetPixel({ (int)x, (int)y })[0]);
            REQUIRE(y == const_pxb.GetPixel({ (int)x, (int)y })[1]);
        }

    return;
}

// Tests that GetPixel with channel offset returns the correct data
TEST_CASE(__FILE__"/GetPixel_ChannelOffset",  "[Pixelbuffer]")
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
            REQUIRE(x == *pxb.GetPixel({ (int)x, (int)y }, 0));
            REQUIRE(y == *pxb.GetPixel({ (int)x, (int)y }, 1));
        }

    return;
}

// Tests that GetPixel const returns the correct data
TEST_CASE(__FILE__"/GetPixel_ChannelOffset_Const",  "[Pixelbuffer]")
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
            REQUIRE(x == *const_pxb.GetPixel({ (int)x, (int)y }, 0));
            REQUIRE(y == *const_pxb.GetPixel({ (int)x, (int)y }, 1));
        }

    return;
}

// Tests that refitting a pixel buffer actually changes the dimensions
TEST_CASE(__FILE__"/Can_Access_Refitted_Data",  "[Pixelbuffer]")
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
TEST_CASE(__FILE__"/Clear_Grayscale_3chn",  "[Pixelbuffer]")
{
    // Setup
    PixelBuffer<3> pxb({ 69, 69 });

    // Exercise
    pxb.Clear({ 129, 129, 129 });

    // Verify
    for (int x = 0; x < pxb.GetDimensions().x; x++)
        for (int y = 0; y < pxb.GetDimensions().y; y++)
        {
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 0));
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 1));
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 2));
        }

    return;
}

// Tests that clearing the pixel buffer with a solid grayscale color works with 4 channels
TEST_CASE(__FILE__"/Clear_Grayscale_4chn",  "[Pixelbuffer]")
{
    // Setup
    PixelBuffer<4> pxb({ 69, 69 });

    // Exercise
    pxb.Clear({ 129, 129, 129, 129 });

    // Verify
    for (int x = 0; x < pxb.GetDimensions().x; x++)
        for (int y = 0; y < pxb.GetDimensions().y; y++)
        {
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 0));
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 1));
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 2));
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 3));
        }

    return;
}

// Tests that clearing the pixel buffer with a solid color works with 1 channel
TEST_CASE(__FILE__"/Clear_1chn",  "[Pixelbuffer]")
{
    // Setup
    PixelBuffer<1> pxb({ 69, 69 });

    // Exercise
    pxb.Clear({ 129 });

    // Verify
    for (int x = 0; x < pxb.GetDimensions().x; x++)
        for (int y = 0; y < pxb.GetDimensions().y; y++)
        {
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 0));
        }

    return;
}

// Tests that clearing the pixel buffer with a solid Rg color works with 4 channels
TEST_CASE(__FILE__"/Clear_Rg_2chn",  "[Pixelbuffer]")
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
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 0));
            REQUIRE(uint8_t(72) == *pxb.GetPixel({ x, y }, 1));
        }

    return;
}

// Tests that clearing the pixel buffer with a solid rgb color works with 3 channels
TEST_CASE(__FILE__"/Clear_Rgb_3chn",  "[Pixelbuffer]")
{
    // Setup
    PixelBuffer<3> pxb({ 69, 69 });

    // Exercise
    pxb.Clear({ 129, 72, 69 });

    // Verify
    for (int x = 0; x < pxb.GetDimensions().x; x++)
        for (int y = 0; y < pxb.GetDimensions().y; y++)
        {
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 0));
            REQUIRE(uint8_t(72) == *pxb.GetPixel({ x, y }, 1));
            REQUIRE(uint8_t(69) == *pxb.GetPixel({ x, y }, 2));
        }

    return;
}

// Tests that clearing the pixel buffer with a solid rgba color works with 4 channels
TEST_CASE(__FILE__"/Clear_Rgba_4chn",  "[Pixelbuffer]")
{
    // Setup
    PixelBuffer<4> pxb({ 69, 69 });

    // Exercise
    pxb.Clear({ 129, 72, 69, 89 });

    // Verify
    for (int x = 0; x < pxb.GetDimensions().x; x++)
        for (int y = 0; y < pxb.GetDimensions().y; y++)
        {
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 0));
            REQUIRE(uint8_t(72) == *pxb.GetPixel({ x, y }, 1));
            REQUIRE(uint8_t(69) == *pxb.GetPixel({ x, y }, 2));
            REQUIRE(uint8_t(89) == *pxb.GetPixel({ x, y }, 3));
        }

    return;
}

// Tests that clearing the pixel buffer with a solid rgbax color works with 5 channels
TEST_CASE(__FILE__"/Clear_Rgbax_5chn",  "[Pixelbuffer]")
{
    // Setup
    PixelBuffer<5> pxb({ 69, 69 });

    // Exercise
    pxb.Clear({ 129, 72, 69, 89, 32 });

    // Verify
    for (int x = 0; x < pxb.GetDimensions().x; x++)
        for (int y = 0; y < pxb.GetDimensions().y; y++)
        {
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 0));
            REQUIRE(uint8_t(72) == *pxb.GetPixel({ x, y }, 1));
            REQUIRE(uint8_t(69) == *pxb.GetPixel({ x, y }, 2));
            REQUIRE(uint8_t(89) == *pxb.GetPixel({ x, y }, 3));
            REQUIRE(uint8_t(32) == *pxb.GetPixel({ x, y }, 4));
        }

    return;
}

// Tests that clearing the pixel buffer with a solid rgbaxxxx color works with 8 channels
TEST_CASE(__FILE__"/Clear_Rgbaxxxx_8chn",  "[Pixelbuffer]")
{
    // Setup
    PixelBuffer<8> pxb({ 69, 69 });

    // Exercise
    pxb.Clear({ 129, 72, 69, 89, 32, 19, 15, 3 });

    // Verify
    for (int x = 0; x < pxb.GetDimensions().x; x++)
        for (int y = 0; y < pxb.GetDimensions().y; y++)
        {
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 0));
            REQUIRE(uint8_t(72) == *pxb.GetPixel({ x, y }, 1));
            REQUIRE(uint8_t(69) == *pxb.GetPixel({ x, y }, 2));
            REQUIRE(uint8_t(89) == *pxb.GetPixel({ x, y }, 3));
            REQUIRE(uint8_t(32) == *pxb.GetPixel({ x, y }, 4));
            REQUIRE(uint8_t(19) == *pxb.GetPixel({ x, y }, 5));
            REQUIRE(uint8_t(15) == *pxb.GetPixel({ x, y }, 6));
            REQUIRE(uint8_t( 3) == *pxb.GetPixel({ x, y }, 7));
        }

    return;
}

// Tests that clearing the pixel buffer with a solid grayscale color works with 8 channels
TEST_CASE(__FILE__"/Clear_Grayscale_8chn",  "[Pixelbuffer]")
{
    // Setup
    PixelBuffer<8> pxb({ 69, 69 });

    // Exercise
    pxb.Clear({ 129, 129, 129, 129, 129, 129, 129, 129 });

    // Verify
    for (int x = 0; x < pxb.GetDimensions().x; x++)
        for (int y = 0; y < pxb.GetDimensions().y; y++)
        {
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 0));
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 1));
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 2));
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 3));
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 4));
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 5));
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 6));
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 7));
        }

    return;
}

// Tests that clearing the pixel buffer with a solid rgbaxxxxxx color works with 10 channels
TEST_CASE(__FILE__"/Clear_Rgbaxxxxxx_10chn",  "[Pixelbuffer]")
{
    // Setup
    PixelBuffer<10> pxb({ 69, 69 });

    // Exercise
    pxb.Clear({ 129, 72, 69, 89, 32, 19, 15, 3, 5, 95 });

    // Verify
    for (int x = 0; x < pxb.GetDimensions().x; x++)
        for (int y = 0; y < pxb.GetDimensions().y; y++)
        {
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 0));
            REQUIRE(uint8_t(72) == *pxb.GetPixel({ x, y }, 1));
            REQUIRE(uint8_t(69) == *pxb.GetPixel({ x, y }, 2));
            REQUIRE(uint8_t(89) == *pxb.GetPixel({ x, y }, 3));
            REQUIRE(uint8_t(32) == *pxb.GetPixel({ x, y }, 4));
            REQUIRE(uint8_t(19) == *pxb.GetPixel({ x, y }, 5));
            REQUIRE(uint8_t(15) == *pxb.GetPixel({ x, y }, 6));
            REQUIRE(uint8_t( 3) == *pxb.GetPixel({ x, y }, 7));
            REQUIRE(uint8_t( 5) == *pxb.GetPixel({ x, y }, 8));
            REQUIRE(uint8_t(95) == *pxb.GetPixel({ x, y }, 9));
        }

    return;
}

// Tests that clearing the pixel buffer with a solid grayscale color works with 10 channels
TEST_CASE(__FILE__"/Clear_Grayscale_10chn",  "[Pixelbuffer]")
{
    // Setup
    PixelBuffer<10> pxb({ 69, 69 });

    // Exercise
    pxb.Clear({ 129, 129, 129, 129, 129, 129, 129, 129, 129, 129 });

    // Verify
    for (int x = 0; x < pxb.GetDimensions().x; x++)
        for (int y = 0; y < pxb.GetDimensions().y; y++)
        {
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 0));
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 1));
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 2));
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 3));
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 4));
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 5));
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 6));
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 7));
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 8));
            REQUIRE(uint8_t(129) == *pxb.GetPixel({ x, y }, 9));
        }

    return;
    }
