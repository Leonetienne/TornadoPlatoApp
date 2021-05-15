#include "CppUnitTest.h"
#include "../Plato/Quaternion.h"
#include "../Tornado/Math.h"
#include "../_TestingUtilities/HandyMacros.h"
#include <random>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TransformRelated
{
	TEST_CLASS(_Quaternion)
	{
	private:
		std::mt19937 rng;

	public:
		// Constructor
		_Quaternion()
		{
			rng = std::mt19937((std::random_device())());
			return;
		}

		// Tests that if constructed with the default constructor, that all values are 0 (but w should be 1)
		TEST_METHOD(Default_Constructor_All_0)
		{
			Quaternion q;
			Assert::IsTrue(Vector4d(0, 0, 0, 1) == q.GetRawValues());

			return;
		}

		// Tests that getting and setting raw values works
		TEST_METHOD(Can_Set_Get_Raw_Values)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				Vector4d v(
					rng() % 90,
					rng() % 90,
					rng() % 90,
					rng() % 90
				);

				Quaternion q(Vector4d(0, 0, 0, 0)); // Garbage values
				
				q.SetRawValues(v);
				Assert::IsTrue(v.Similar(q.GetRawValues()));
			}

			return;
		}

		// Tests that retreiving euler angles (without gimbal lock) results in the same values as put in
		TEST_METHOD(To_Euler_From_Euler)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				// Create vector
				Vector3d eul(
					rng() % 90,
					rng() % 90,
					rng() % 90
				);

				// Create quaternion from vector
				Quaternion q(eul);

				// Create debug output
				std::wstringstream wss;
				wss << std::endl
					<< "Actual vals: " << q.ToEulerAngles() << std::endl
					<< "Target vals: " << eul << std::endl;

				// Assertion
				Assert::IsTrue(eul.Similar(q.ToEulerAngles()), wss.str().c_str());
			}

			return;
		}

		// Tests that adding angles (0,0,0) does not modify the quaternion
		TEST_METHOD(Add_Angles_0_Does_Nothing)
		{
			Quaternion a(Vector3d(0, -45, 45));
			Quaternion b(Vector3d(0, 0, 0));

			Assert::IsTrue(Vector3d(0, -45, 45).Similar((a * b).ToEulerAngles()));

			return;
		}

		// Tests that subtracting angles (0,0,0) does not modify the quaternion
		TEST_METHOD(Sub_Angles_0_Does_Nothing)
		{
			Quaternion a(Vector3d(0, -45, 45));
			Quaternion b(Vector3d(0, 0, 0));

			Assert::IsTrue(Vector3d(0, -45, 45).Similar((a / b).ToEulerAngles()));

			return;
		}

		// Tests that subtracting by itself always returns (0,0,0)
		TEST_METHOD(Sub_Itself_Is_0)
		{
			// Run test 100 times
			for (std::size_t i = 0; i < 100; i++)
			{
				Quaternion a(Vector3d(LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE));
				Assert::IsTrue(Vector3d(0,0,0).Similar((a / a).ToEulerAngles()));
			}

			return;
		}

		// Tests that rotating a vector is equal to multiplying it with the inverted rotation matrix
		TEST_METHOD(RotateVector_Equal_to_RotationMatrix)
		{
			// Run test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				Quaternion a(Vector3d(LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE));

				Vector3d point(32, 19, -14);

				Assert::IsTrue((point * a.ToRotationMatrix()).Similar(a * point));
			}

			return;
		}

		// Tests that a *= b will result in the exact same outcome as a = a * b
		TEST_METHOD(MultiplyEquals_Operator_Same_Result_As_Multiply_Operator)
		{
			// Run tests 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				// Setup
				Quaternion a(Vector3d(rng() % 360, rng() % 360, rng() % 360));
				Quaternion b(Vector3d(rng() % 360, rng() % 360, rng() % 360));
				
				// Exercise
				Quaternion ref = a * b;
				a *= b;

				// Verify
				Assert::IsTrue(a.GetRawValues().Similar(ref.GetRawValues()));
			}

			return;
		}

		// Tests that a /= b will result in the exact same outcome as a = a / b
		TEST_METHOD(DivideEquals_Operator_Same_Result_As_Divide_Operator)
		{
			// Run tests 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				// Setup
				Quaternion a(Vector3d(rng() % 360, rng() % 360, rng() % 360));
				Quaternion b(Vector3d(rng() % 360, rng() % 360, rng() % 360));

				// Exercise
				Quaternion ref = a / b;
				a /= b;

				// Verify
				Assert::IsTrue(a.GetRawValues().Similar(ref.GetRawValues()));
			}

			return;
		}

		// Tests basic equals comparison -> true
		TEST_METHOD(Basic_EqualsComparison_True)
		{
			// Run tests 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				// Setup
				Vector3d e(rng() % 360, rng() % 360, rng() % 360);
				Quaternion a(e);
				Quaternion b(e);

				// Exercise and verify
				Assert::IsTrue(a == b);
			}

			return;
		}

		// Tests basic equals comparison -> true
		TEST_METHOD(Basic_EqualsComparison_False)
		{
			// Run tests 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				// Setup
				Vector3d ae(rng() % 360, rng() % 360, rng() % 360);
				Vector3d be(rng() % 360, rng() % 360, rng() % 360);

				// Abort if both vectors are equal
				if (ae == be)
				{
					i--;
					continue;
				}

				Quaternion a(ae);
				Quaternion b(be);

				// Exercise and verify
				Assert::IsFalse(a == b);
			}

			return;
		}

		// Tests that different euler angles return true, if the angle is the same.
		// Like [30, -10, 59] == [390, 350, 419]
		TEST_METHOD(Equals_Comparison_Same_Rotation_Different_EulerAngles)
		{
			// Run tests 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				// Setup
				//    Create random rotation
				Vector3d ae(rng() % 360, rng() % 360, rng() % 360);
				
				// add or subtract a random multiple of 360
				#define keep_rot_change_values (360.0 * (double)(rng() % 20) * ((rng()%2) ? 1.0 : -1.0))
				Vector3d be(ae.x + keep_rot_change_values, ae.y + keep_rot_change_values, ae.z + keep_rot_change_values);
				#undef keep_rot_change_values

				// Create quaternions
				Quaternion a(ae);
				Quaternion b(be);

				// Exercise & Verify
				//    Create debug output

				std::wstringstream wss;
				wss << "ae: " << ae << std::endl
					<< "be: " << be << std::endl
					<< "a: " << a << std::endl
					<< "b: " << b << std::endl;

				//    Assertion
				Assert::IsTrue(a == b, wss.str().c_str());
			}

			return;
		}

		// Tests basic not-equals comparison -> false
		TEST_METHOD(Basic_NotEqualsComparison_False)
		{
			// Run tests 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				// Setup
				Vector3d e(rng() % 360, rng() % 360, rng() % 360);
				Quaternion a(e);
				Quaternion b(e);

				// Exercise and verify
				Assert::IsFalse(a != b);
			}

			return;
		}

		// Tests basic not-equals comparison -> true
		TEST_METHOD(Basic_NotEqualsComparison_True)
		{
			// Run tests 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				// Setup
				Vector3d ae(rng() % 360, rng() % 360, rng() % 360);
				Vector3d be(rng() % 360, rng() % 360, rng() % 360);

				// Abort if both vectors are equal
				if (ae == be)
				{
					i--;
					continue;
				}

				Quaternion a(ae);
				Quaternion b(be);

				// Exercise and verify
				Assert::IsTrue(a != b);
			}

			return;
		}
	};
}
