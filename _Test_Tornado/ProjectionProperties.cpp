#include "CppUnitTest.h"
#include "../Tornado/ProjectionProperties.h"
#include "../_TestingUtilities/HandyMacros.h"
#include "../Tornado/Similar.h"
#include <random>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Rendering
{
	TEST_CLASS(_ProjectionProperties)
	{
	private:
		std::mt19937 rng;

	public:
		// Constructor
		_ProjectionProperties()
		{
			rng = std::mt19937((std::random_device())());
			return;
		}

		// Tests if it sets the correct constructor values
		TEST_METHOD(Constructor_Values_Match)
		{
			ProjectionProperties props(
				{ 1920, 1080 },
				90,
				2,
				100
			);

			Assert::AreEqual(1920, props.GetResolution().x);
			Assert::AreEqual(1080, props.GetResolution().y);
			Assert::AreEqual(1920.0 / 1080.0, props.GetAspectRatio());
			Assert::AreEqual(90.0, props.GetFov());
			Assert::AreEqual(100.0, props.GetFarclip());
			Assert::IsTrue(10000.0l == props.GetSqrFarclip());
			Assert::IsTrue(Vector2d(960, 540) == props.GetHalfResolution());
			Assert::AreEqual(2.0, props.GetNearclip());

			return;
		}

		// Tests if the resolution can be set afterwards
		TEST_METHOD(Can_Set_Resolution)
		{
			ProjectionProperties props(
				{ 1920, 1080 },
				90,
				2,
				100
			);

			props.SetResolution({ 3840, 1440 });

			Assert::AreEqual(3840, props.GetResolution().x);
			Assert::AreEqual(1440, props.GetResolution().y);
			Assert::AreEqual(3840.0 / 1440.0, props.GetAspectRatio());
			Assert::IsTrue(Vector2d(1920, 720) == props.GetHalfResolution());

			return;
		}

		// Tests if the fov can be set afterwards
		TEST_METHOD(Can_Set_Fov)
		{
			ProjectionProperties props(
				{ 1920, 1080 },
				90,
				2,
				100
			);

			props.SetFov(65);

			Assert::AreEqual(65.0, props.GetFov());

			return;
		}

		// Tests if the nearclip can be set afterwards
		TEST_METHOD(Can_Set_Nearclip)
		{
			ProjectionProperties props(
				{ 1920, 1080 },
				90,
				2,
				100
			);

			props.SetNearclip(6);

			Assert::AreEqual(6.0, props.GetNearclip());

			return;
		}

		// Tests if the farclip can be set afterwards
		TEST_METHOD(Can_Set_Farclip)
		{
			ProjectionProperties props(
				{ 1920, 1080 },
				90,
				2,
				100
			);

			props.SetFarclip(200);

			Assert::AreEqual(200.0, props.GetFarclip());
			Assert::IsTrue(40000.0l == props.GetSqrFarclip());

			return;
		}

		// Tests if the generation projection matrix is correct
		TEST_METHOD(Is_ProjectionMatrix_Correct)
		{
			ProjectionProperties props(
				{ 1920, 1080 },
				90,
				2,
				100
			);

			Matrix4x4 comparison;
			comparison[0] = { 0.5625, 0,            0,            0 };
			comparison[1] = {      0, 1,            0,            0 };
			comparison[2] = {      0, 0, -1.040816327, -4.081632653 };
			comparison[3] = {      0, 0,           -1,            1 };

			for (std::size_t y = 0; y < 4; y++)
				for (std::size_t x = 0; x < 4; x++)
				{
					Assert::IsTrue(Similar(comparison[y][x], props.GetProjectionMatrix()[y][x]));
				}

			return;
		}

		// Tests that setting ANY value updates the projection matrix
		TEST_METHOD(Setting_Anything_Updates_Matrix)
		{
			// Prepare original data
			ProjectionProperties props(
				{ 1920, 1080 },
				90,
				2,
				100
			);
			Matrix4x4 origMat;

			// Change resolution
			origMat = props.GetProjectionMatrix();
			props.SetResolution({ 800, 600 });
			Assert::IsFalse(origMat == props.GetProjectionMatrix(), L"Failed at resolution");

			// Change fov
			origMat = props.GetProjectionMatrix();
			props.SetFov(60);
			Assert::IsFalse(origMat == props.GetProjectionMatrix(), L"Failed at fov");

			// Change nearclip
			origMat = props.GetProjectionMatrix();
			props.SetNearclip(99);
			Assert::IsFalse(origMat == props.GetProjectionMatrix(), L"Failed at nearclip");

			// Change farclip
			origMat = props.GetProjectionMatrix();
			props.SetFarclip(8888);
			Assert::IsFalse(origMat == props.GetProjectionMatrix(), L"Failed at farclip");

			return;
		}
	};
}
