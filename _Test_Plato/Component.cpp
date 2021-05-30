#include "CppUnitTest.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/WorldObject.h"
#include "../Plato/Component.h"
#include "../Plato/Math.h"
#include "../_TestingUtilities/HandyMacros.h"
#include "../_TestingUtilities/MemoryLeakDetector.h"
#include <random>
#include <sstream>
#include <thread>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Plato;

typedef std::chrono::milliseconds ms;

class TestComponent_ : public Component
{
public:

	void Update(double) override
	{
		if (failAtHookCall)
			Assert::Fail(L"Update-hook got called!");

		return;
	}

	void LateUpdate(double) override
	{
		if (failAtHookCall)
			Assert::Fail(L"LateUpdate-hook got called!");

		return;
	}

	void Render(Renderer*) override
	{
		if (failAtHookCall)
			Assert::Fail(L"Render-hook got called!");

		return;
	}

	void OnDestroy() override
	{
		if (failAtHookCall)
			Assert::Fail(L"OnDestroy-hook got called!");
	}

private:
	bool failAtHookCall = false;

	TestComponent_(WorldObject* worldObject, bool failAtHook)
		:
		Component(worldObject),
		failAtHookCall{ failAtHook }
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

		// Tests that any component is globally enabled by default
		TEST_METHOD(Globally_Enabled_By_Default)
		{
			// Setup, Exercise
			Component* comp = WorldObjectManager::NewWorldObject()->AddComponent<TestComponent_>(false);

			// Verify
			Assert::IsTrue(comp->GetIsGloballyEnabled());

			// Teardown
			WorldObjectManager::Free();

			return;
		}

		// Tests that any component is globally disabled by default, if instanciated on disabled world object
		TEST_METHOD(Globally_Disabled_By_Default_If_Instanciated_On_Disabled_Wobject)
		{
			// Setup, Exercise
			WorldObject* wo = WorldObjectManager::NewWorldObject();
			wo->Disable();
			Component* comp = wo->AddComponent<TestComponent_>(false);

			// Verify
			Assert::IsFalse(comp->GetIsGloballyEnabled());

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

		// Tests that disabling a component locally will also disable it globally
		TEST_METHOD(Disabling_Locally_Also_Disables_Globally)
		{
			// Setup
			Component* comp = WorldObjectManager::NewWorldObject()->AddComponent<TestComponent_>(false);

			// Excersise
			comp->Disable();

			// Verify
			Assert::IsFalse(comp->GetIsGloballyEnabled());

			// Teardown
			WorldObjectManager::Free();

			return;
		}

		// Tests that disabling the components world object also disables it globally
		TEST_METHOD(Disabling_WorldObject_Also_Disables_Component_Globally)
		{
			// Setup
			Component* comp = WorldObjectManager::NewWorldObject()->AddComponent<TestComponent_>(false);

			// Excersise
			comp->worldObject->Disable();

			// Verify
			Assert::IsFalse(comp->GetIsGloballyEnabled());

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

			comp->worldObject->Destroy(); // This will cause the WOM to delete it, calling OnDestroy
			WorldObjectManager::CallHook__Update(1);
			WorldObjectManager::CallHook__LateUpdate(1);
			WorldObjectManager::CallHook__Render(nullptr);
			WorldObjectManager::DeleteFlaggedObjects();

			// Assert::Fail() is called in the components hooks!

			// Teardown
			WorldObjectManager::Free();

			return;
		}

		// Tests that a component will not receive any hook calls if just globally disabled (not locally)
		TEST_METHOD(No_Hook_When_Globally_Disabled)
		{
			// Setup
			Component* comp = WorldObjectManager::NewWorldObject()->AddComponent<TestComponent_>(true);

			// Exercise
			comp->worldObject->Disable();

			comp->worldObject->Destroy(); // This will cause the WOM to delete it, calling OnDestroy
			WorldObjectManager::CallHook__Update(1);
			WorldObjectManager::CallHook__LateUpdate(1);
			WorldObjectManager::CallHook__Render(nullptr);
			WorldObjectManager::DeleteFlaggedObjects();

			// Assert::Fail() is called in the components hooks!

			// Teardown
			WorldObjectManager::Free();

			return;
		}

	};
}
