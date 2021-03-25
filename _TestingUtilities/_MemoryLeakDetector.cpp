#include "CppUnitTest.h"
#include "MemoryLeakDetector.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestingUtilities
{
	TEST_CLASS(_MemoryLeakDetector)
	{
	public:

		// Tests to detect no memory leak, if the test does nothing at all
		TEST_METHOD(No_Memleak_For_Nothing)
		{
			MemoryLeakDetector mld;
			mld.Init();

			{
				// Do nothing here
			}

			Assert::IsFalse(mld.DetectLeak());
			return;
		}

		// Tests to detect no memory leak when not even touching pointers
		TEST_METHOD(No_Memleak_For_No_Pointer_Action)
		{
			MemoryLeakDetector mld;
			mld.Init();

			{
				int i = 33;
				int c = i * 9;
			}

			Assert::IsFalse(mld.DetectLeak());
			return;
		}

		// Tests to detect no memory leak when correctly cleaning up pointers
		TEST_METHOD(No_Memleak_For_Cleaned_Up_Pointers)
		{
			MemoryLeakDetector mld;
			mld.Init();

			{
				int* ptr = new int[333];
				delete[] ptr;
			}

			Assert::IsFalse(mld.DetectLeak());
			return;
		}

		// Tests to detect a memory leak when not cleaning up pointers
		TEST_METHOD(Memleak_For_No_Pointer_Cleanup)
		{
			MemoryLeakDetector mld;
			mld.Init();

			{
				int* ptr = new int[333];
			}

			Assert::IsTrue(mld.DetectLeak());
			return;
		}

		// Tests to detect no memory leak when correctly cleaning up pointers, using C-Methods
		TEST_METHOD(No_Memleak_For_Cleaned_Up_Pointers_C_Like)
		{
			MemoryLeakDetector mld;
			mld.Init();

			{
				int* ptr = (int*)malloc(sizeof(int) * 333);
				free(ptr);
			}

			Assert::IsFalse(mld.DetectLeak());
			return;
		}

		// Tests to detect a memory leak when not cleaning up pointers, using C-Methods
		TEST_METHOD(Memleak_For_No_Pointer_Cleanup_C_Like)
		{
			MemoryLeakDetector mld;
			mld.Init();

			{
				int* ptr = (int*)malloc(sizeof(int) * 333);
			}

			Assert::IsTrue(mld.DetectLeak());
			return;
		}
	};
}
