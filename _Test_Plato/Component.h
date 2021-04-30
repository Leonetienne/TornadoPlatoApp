#include "CppUnitTest.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/WorldObject.h"
#include "../Plato/Component.h"
#include "../Tornado/Similar.h"
#include "../_TestingUtilities/HandyMacros.h"
#include "../_TestingUtilities/MemoryLeakDetector.h"
#include <random>
#include <sstream>
#include <thread>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
typedef std::chrono::milliseconds ms;

class TestComponent_ : public Component
{
public:

	void Update(double)
	{
		if (failAtHookCall)
			Assert::Fail(L"Update-hook got called!");

		return;
	}

	void Render(Renderer*)
	{
		if (failAtHookCall)
			Assert::Fail(L"Render-hook got called!");

		return;
	}

	void OnDestroy()
	{
		if (failAtHookCall)
			Assert::Fail(L"OnDestroy-hook got called!");
	}

private:
	bool failAtHookCall = false;

	TestComponent_(WorldObject* worldObject, bool failAtHook)
		:
		Component(worldObject),
		failAtHookCall {failAtHook}
	{
		return;
	}

	friend class WorldObject;
};

namespace WorldObjects
{
	TEST_CLASS(_Component)
	{
	private:
		std::mt19937 rng;

	public:
		// Constructor
		_Component()
		{
			rng = std::mt19937((std::random_device())());
			return;
		}

		// Tests that any component is enabled by default
		TEST_METHOD(Enabled_By_Default)
		{
			// Setup, Exercise
			Component* comp = WorldObjectManager::NewWorldObject()->AddComponent<TestComponent_>(false);

			// Verify
			Assert::IsTrue(comp->GetIsEnabled());

			// Teardown
			WorldObjectManager::Free();

			return;
		}

		// Tests that a component can be disabled
		TEST_METHOD(Can_Disable_Component)
		{
			// Setup
			Component* comp = WorldObjectManager::NewWorldObject()->AddComponent<TestComponent_>(false);

			// Excersise
			comp->Disable();

			// Verify
			Assert::IsFalse(comp->GetIsEnabled());

			// Teardown
			WorldObjectManager::Free();

			return;
		}

		// Tests that a component can be enabled again
		TEST_METHOD(Can_Enable_Component)
		{
			// Setup
			Component* comp = WorldObjectManager::NewWorldObject()->AddComponent<TestComponent_>(false);

			// Excersise
			comp->Disable();
			comp->Enable();

			// Verify
			Assert::IsTrue(comp->GetIsEnabled());

			// Teardown
			WorldObjectManager::Free();

			return;
		}

		// Tests that no hooks get called when an object is disabled.
		TEST_METHOD(No_Hook_When_Disabled)
		{
			// Setup
			Component* comp = WorldObjectManager::NewWorldObject()->AddComponent<TestComponent_>(true);

			// Exercise
			comp->Disable();
			WorldObjectManager::CallHook__Update(1);
			WorldObjectManager::CallHook__Render(nullptr);

			// Assert::Fail() is called in the components hooks!

			// Teardown
			WorldObjectManager::Free();

			return;
		}
	};
}
