
#include "../Tornado/Texture.h"
#include "../_TestingUtilities/Catch2.h"
#include "../_TestingUtilities/HandyMacros.h"
#include <random>

using namespace TorGL;

// Tests that a texture constructed only by color is of size 1x1
TEST_CASE("Color Only Is Size 1x1", "[Texture]")
{
    Texture txt(Color::green);
    REQUIRE(txt.GetPixelBuffer().GetDimensions().x == 1);
    REQUIRE(txt.GetPixelBuffer().GetDimensions().y == 1);
}

// Tests that a texture constructed only by color does match that color
TEST_CASE("Color Only Color Matches", "[Texture]")
{
    std::mt19937 rng((std::random_device())());

    // Test 100 times
    for (std::size_t i = 0; i < 100; i++)
    {
        // Generate random color
        const int r = rng() % 255;
        const int g = rng() % 255;
        const int b = rng() % 255;
        const int a = rng() % 255;
        const Color textureCol(Color(r, g, b, a));

        // Generate texture from the color
        Texture txt(textureCol);

        // Check that the pixel color matches the generated color
        Color pixelCol(
            *txt.GetPixelBuffer().GetPixel({ 0, 0 }, 0),
            *txt.GetPixelBuffer().GetPixel({ 0, 0 }, 1),
            *txt.GetPixelBuffer().GetPixel({ 0, 0 }, 2),
            *txt.GetPixelBuffer().GetPixel({ 0, 0 }, 3)
        );
        REQUIRE(textureCol == pixelCol);
    }
}

// Tests when a texture is constructed from a color and a size, that it matches the size
TEST_CASE("Color and Size Matches Size", "[Texture]")
{
    std::mt19937 rng((std::random_device())());

    // Test 10 times
    for (std::size_t i = 0; i < 10; i++)
    {
        int x = LARGE_RAND_POSITIVE_INT + 1; // Pls no texture of size 0
        int y = LARGE_RAND_POSITIVE_INT + 1;

        Texture txt(Color::green, { x, y });
        REQUIRE((int)txt.GetPixelBuffer().GetDimensions().x == x);
        REQUIRE((int)txt.GetPixelBuffer().GetDimensions().y == y);
    }
}

// Tests when a texture is constructed from a color and a size, that all pixels match that color
TEST_CASE("Color and Size All Pixels Match Color", "[Texture]")
{
    std::mt19937 rng((std::random_device())());

    // Test 10 times
    for (std::size_t i = 0; i < 10; i++)
    {
        // Generate random color
        const int r = rng() % 255;
        const int g = rng() % 255;
        const int b = rng() % 255;
        const int a = rng() % 255;
        const Color textureCol(Color(r, g, b, a));

        // Generate texture of random size
        int x = rng() % 33 + 1;
        int y = rng() % 33 + 1;
        Texture txt(textureCol, { x, y });

        // Check for each pixel that it matches the generated color
        const std::size_t numPixels = txt.GetPixelBuffer().GetSizeofBuffer();
        for (std::size_t j = 0; j < numPixels; j += 4)
        {
            Color pixelCol(
                txt.GetPixelBuffer().GetRawData()[j + 0],
                txt.GetPixelBuffer().GetRawData()[j + 1],
                txt.GetPixelBuffer().GetRawData()[j + 2],
                txt.GetPixelBuffer().GetRawData()[j + 3]
            );

            REQUIRE(textureCol == pixelCol);
        }
    }
}

// Tests that when constructing from a pixel buffer, it clones the pixel buffer correctly
TEST_CASE("Construct From Pixelbuffer Values Matches", "[Texture]")
{
    PixelBuffer<4> pxb({ 128, 128 });
    pxb.GetRawData()[0] = 128;

    Texture txt(pxb);
    REQUIRE(pxb.GetRawData()[0] == txt.GetPixelBuffer().GetRawData()[0]);
}

// Tests that when constructing from a pixel buffer, changing the new buffer does not affect the old one
TEST_CASE("Construct From Pixelbuffer Values Are Independent", "[Texture]")
{
    PixelBuffer<4> pxb({ 128, 128 });
    pxb.GetRawData()[0] = 128;

    Texture txt(pxb);
    txt.GetPixelBuffer().GetRawData()[0] = 255;

    REQUIRE(pxb.GetRawData()[0] != txt.GetPixelBuffer().GetRawData()[0]);
}

