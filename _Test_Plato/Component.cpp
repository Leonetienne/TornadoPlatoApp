#include "../_TestingUtilities/Catch2.h"
#include "../Plato/WorldObjectManager.h"
#include "../Plato/WorldObject.h"
#include "../Plato/Component.h"
#include <random>

using namespace Plato;

class TestComponent_ : public Component
{
public:

	void Update(double) override
	{
		if (failAtHookCall)
			FAIL("Update-hook got called!");

		return;
	}

	void LateUpdate(double) override
	{
		if (failAtHookCall)
			FAIL("LateUpdate-hook got called!");

		return;
	}

	void Render(Renderer*) override
	{
		if (failAtHookCall)
			FAIL("Render-hook got called!");

		return;
	}

	void OnDestroy() override
	{
		if (failAtHookCall)
			FAIL("OnDestroy-hook got called!");
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

// Tests that any component is enabled by default
TEST_CASE(__FILE__"/Enabled_By_Default", "[Component]")
{
    // Setup, Exercise
    Component* comp = WorldObjectManager::NewWorldObject()->AddComponent<TestComponent_>(false);

    // Verify
    REQUIRE(comp->GetIsEnabled());

    // Teardown
    WorldObjectManager::Free();

    return;
}

// Tests that any component is globally enabled by default
TEST_CASE(__FILE__"/Globally_Enabled_By_Default", "[Component]")
{
    // Setup, Exercise
    Component* comp = WorldObjectManager::NewWorldObject()->AddComponent<TestComponent_>(false);

    // Verify
    REQUIRE(comp->GetIsGloballyEnabled());

    // Teardown
    WorldObjectManager::Free();

    return;
}

// Tests that any component is globally disabled by default, if instanciated on disabled world object
TEST_CASE(__FILE__"/Globally_Disabled_By_Default_If_Instanciated_On_Disabled_Wobject", "[Component]")
{
    // Setup, Exercise
    WorldObject* wo = WorldObjectManager::NewWorldObject();
    wo->Disable();
    Component* comp = wo->AddComponent<TestComponent_>(false);

    // Verify
    REQUIRE_FALSE(comp->GetIsGloballyEnabled());

    // Teardown
    WorldObjectManager::Free();
    return;
}

// Tests that a component can be disabled
TEST_CASE(__FILE__"/Can_Disable_Component", "[Component]")
{
    // Setup
    Component* comp = WorldObjectManager::NewWorldObject()->AddComponent<TestComponent_>(false);

    // Excersise
    comp->Disable();

    // Verify
    REQUIRE_FALSE(comp->GetIsEnabled());

    // Teardown
    WorldObjectManager::Free();

    return;
}

// Tests that a component can be enabled again
TEST_CASE(__FILE__"/Can_Enable_Component", "[Component]")
{
    // Setup
    Component* comp = WorldObjectManager::NewWorldObject()->AddComponent<TestComponent_>(false);

    // Excersise
    comp->Disable();
    comp->Enable();

    // Verify
    REQUIRE(comp->GetIsEnabled());

    // Teardown
    WorldObjectManager::Free();

    return;
}

// Tests that disabling a component locally will also disable it globally
TEST_CASE(__FILE__"/Disabling_Locally_Also_Disables_Globally", "[Component]")
{
    // Setup
    Component* comp = WorldObjectManager::NewWorldObject()->AddComponent<TestComponent_>(false);

    // Excersise
    comp->Disable();

    // Verify
    REQUIRE_FALSE(comp->GetIsGloballyEnabled());

    // Teardown
    WorldObjectManager::Free();

    return;
}

// Tests that disabling the components world object also disables it globally
TEST_CASE(__FILE__"/Disabling_WorldObject_Also_Disables_Component_Globally", "[Component]")
{
    // Setup
    Component* comp = WorldObjectManager::NewWorldObject()->AddComponent<TestComponent_>(false);

    // Excersise
    comp->worldObject->Disable();

    // Verify
    REQUIRE_FALSE(comp->GetIsGloballyEnabled());

    // Teardown
    WorldObjectManager::Free();

    return;
}

// Tests that no hooks get called when an object is disabled.
TEST_CASE(__FILE__"/No_Hook_When_Disabled", "[Component]")
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
TEST_CASE(__FILE__"/No_Hook_When_Globally_Disabled", "[Component]")
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
