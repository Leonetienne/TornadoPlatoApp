#include "../_TestingUtilities/Catch2.h"
#include "../Plato/WorldObjectManager.h"

using namespace Plato;

namespace {
    Vector3d ScaleVec(Vector3d a, Vector3d b)
    {
        return Vector3d
        (
            a.x * b.x,
            a.y * b.y,
            a.z * b.z
        );
    }
}

// Will create a new WorldObject and fetch its transform
#define NEW_TRANSFORM WorldObjectManager::NewWorldObject()->transform


// Tests that a transform and its world object are linked
TEST_CASE(__FILE__"/WorldObject_And_Transform_Link", "[Transform]")
{
    // Free any rubbish previously failed tests left behind
    WorldObjectManager::Free();

    WorldObject* wo = WorldObjectManager::NewWorldObject();

    REQUIRE(wo == wo->transform->worldObject);
    REQUIRE(wo->transform == wo->transform->worldObject->transform);
    
    WorldObjectManager::Free();

    return;
}

// Tests that the default parent is nullptr
TEST_CASE(__FILE__"/Default_Parent_Is_Null", "[Transform]")
{
    // Free any rubbish previously failed tests left behind
    WorldObjectManager::Free();

    Transform* tr = NEW_TRANSFORM;

    REQUIRE(tr->GetParent() == nullptr);

    WorldObjectManager::Free();
}

// Tests that the default number of children is 0
TEST_CASE(__FILE__"/No_Children_By_Default", "[Transform]")
{
    Transform* tr = NEW_TRANSFORM;

    REQUIRE(std::size_t(0) == tr->GetNumChildren());

    WorldObjectManager::Free();
}

// Tests that a transform can't have itself as a parent
TEST_CASE(__FILE__"/Cant_Assign_Self_As_Parent", "[Transform]")
{
    // Free any rubbish previously failed tests left behind
    WorldObjectManager::Free();

    Transform* tr = NEW_TRANSFORM;

    tr->SetParent(tr);

    REQUIRE(tr->GetParent() == nullptr);

    WorldObjectManager::Free();
}

// Tests if the parent can be set
TEST_CASE(__FILE__"/Can_Set_Parent", "[Transform]")
{
    // Free any rubbish previously failed tests left behind
    WorldObjectManager::Free();

    Transform* trParent = NEW_TRANSFORM;
    Transform* trChild = NEW_TRANSFORM;

    trChild->SetParent(trParent);

    REQUIRE(trParent == trChild->GetParent());
    REQUIRE(trParent->operator[](0) == trChild);

    WorldObjectManager::Free();
    return;
}

// Tests if a child can be added
TEST_CASE(__FILE__"/Add_Child", "[Transform]")
{
    // Free any rubbish previously failed tests left behind
    WorldObjectManager::Free();

    Transform* trParent = NEW_TRANSFORM;
    Transform* trChild = NEW_TRANSFORM;

    trParent->AddChild(trChild);

    REQUIRE(trParent == trChild->GetParent());

    WorldObjectManager::Free();
    return;
}


// Tests that adding children actually incrases the size of the set of childs
TEST_CASE(__FILE__"/Adding_Child_Increases_SetSize", "[Transform]")
{
    // Free any rubbish previously failed tests left behind
    WorldObjectManager::Free();

    Transform* trParent = NEW_TRANSFORM;
    Transform* trChild = NEW_TRANSFORM;

    REQUIRE(std::size_t(0) == trParent->GetNumChildren());

    trChild->SetParent(trParent);

    REQUIRE(std::size_t(1) == trParent->GetNumChildren());

    WorldObjectManager::Free();
    return;
}

// Tests that assigning a child to another parent removes it from the original parent
TEST_CASE(__FILE__"/Removing_Child_Decreases_SetSize", "[Transform]")
{
    // Free any rubbish previously failed tests left behind
    WorldObjectManager::Free();

    Transform* trParent = NEW_TRANSFORM;
    Transform* trOtherParent = NEW_TRANSFORM;
    Transform* trChild = NEW_TRANSFORM;

    trParent->AddChild(trChild);
    REQUIRE(std::size_t(1) == trParent->GetNumChildren());

    trChild->SetParent(trOtherParent);
    REQUIRE(std::size_t(0) == trParent->GetNumChildren());

    WorldObjectManager::Free();
    return;
}

// Tests that re-assinging children removes the correct children
TEST_CASE(__FILE__"/Removes_Correct_Child", "[Transform]")
{
    // Free any rubbish previously failed tests left behind
    WorldObjectManager::Free();

    Transform* trParent = NEW_TRANSFORM;
    Transform* trChild = NEW_TRANSFORM;
    Transform* trOtherChild = NEW_TRANSFORM;

    trChild->SetParent(trParent);
    trOtherChild->SetParent(trParent);

    trChild->SetParent(nullptr);

    REQUIRE(std::size_t(1) == trParent->GetNumChildren());
    REQUIRE(trOtherChild == trParent->operator[](0));

    WorldObjectManager::Free();
    return;
}

// Tests that a recursive hirachy tree cannot be created
TEST_CASE(__FILE__"/Cant_Create_Alabama_HirachyTree", "[Transform]")
{
    // Free any rubbish previously failed tests left behind
    WorldObjectManager::Free();

    // Setup
    Transform* a = NEW_TRANSFORM;
    Transform* b = NEW_TRANSFORM;
    Transform* c = NEW_TRANSFORM;

    b->SetParent(a);
    c->SetParent(b);

    // Current tree:
    // a
    //  \
    //   b
    //    \
    //     c
    //

    // Now let's try to set c as parent of a
    a->SetParent(c);

    // Check that this did nothing
    REQUIRE(a->GetParent() == nullptr);

    WorldObjectManager::Free();
    return;
}

// Tests that the local transformation matrix works with only translation
TEST_CASE(__FILE__"/Local_Transformation_Matrix_TranslationOnly", "[Transform]")
{
    // Free any rubbish previously failed tests left behind
    WorldObjectManager::Free();

    // Setup
    Vector3d originalPoint(17, 33, 5);
    Transform* a = NEW_TRANSFORM;
    a->SetPosition(Vector3d(255, 12, -23));

    // Exercise
    Vector3d transformedPoint = originalPoint * a->GetLocalTransformationMatrix();

    // Verify
    REQUIRE(transformedPoint.Similar(originalPoint + Vector3d(255, 12, -23)));

    WorldObjectManager::Free();
    return;
}

// Tests that the local transformation matrix works with only rotation
TEST_CASE(__FILE__"/Local_Transformation_Matrix_RotationOnly", "[Transform]")
{
    // Free any rubbish previously failed tests left behind
    WorldObjectManager::Free();

    // Setup
    Vector3d originalPoint(12, 314, -99);
    Transform* a = NEW_TRANSFORM;
    a->SetRotation(Quaternion(Vector3d(0, -90, 90)));

    // Exercise
    Vector3d transformedPoint = originalPoint * a->GetLocalTransformationMatrix();

    // Verify
    Vector3d targetPosition = originalPoint * a->GetRotation().ToRotationMatrix();

    //    Create debug output
    std::wstringstream wss;
    wss << "Original position   : " << originalPoint << std::endl
        << "Transformed position: " << transformedPoint << std::endl
        << "Target position     : " << targetPosition << std::endl;

    //    Assertion
    REQUIRE(transformedPoint.Similar(targetPosition));

    WorldObjectManager::Free();
    return;
}

// Tests that the local transformation matrix works with only scale
TEST_CASE(__FILE__"/Local_Transformation_Matrix_ScalingOnly", "[Transform]")
{
    // Free any rubbish previously failed tests left behind
    WorldObjectManager::Free();

    // Setup
    Vector3d originalPoint(-2, 12, 33);
    Transform* a = NEW_TRANSFORM;
    a->SetScale(Vector3d(-0.22, 22, 1.5));

    // Exercise
    Vector3d transformedPoint = originalPoint * a->GetLocalTransformationMatrix();

    // Verify
    Vector3d targetPosition = Vector3d(originalPoint.x * -0.22, originalPoint.y * 22, originalPoint.z * 1.5);

    // Assertion
    REQUIRE(transformedPoint.Similar(targetPosition));

    WorldObjectManager::Free();
    return;
}

// Tests that recalculating a transforms global transform cache multiple times always yields the same result
TEST_CASE(__FILE__"/Recalculating_GlobalTransformCache_Always_Yields_Same_Result", "[Transform]")
{
    // Free any rubbish previously failed tests left behind
    WorldObjectManager::Free();

    // Setup
    Transform* a = NEW_TRANSFORM;
    a->SetPosition(Vector3d(127, 3, -5));
    a->SetScale(Vector3d(1, 2, -2));
    a->SetRotation(Quaternion(Vector3d(33, 5, 15)));

    // Exercise
    Matrix4x4 goodTrMat = a->GetGlobalTransformationMatrix();

    for (std::size_t i = 0; i < 2000; i++)
        a->RecalculateGlobalTransformCache();

    Matrix4x4 recalculatedMat = a->GetGlobalTransformationMatrix();

    // Verify
    REQUIRE(goodTrMat == recalculatedMat);

    WorldObjectManager::Free();
    return;
}

// Tests that recalculating a transforms global transform cache multiple times always yields the same result
TEST_CASE(__FILE__"/Recalculating_GlobalTransformCache_Always_Yields_Same_Result__Deep_Hierarchy", "[Transform]")
{
    // Free any rubbish previously failed tests left behind
    WorldObjectManager::Free();

    // Setup
    Transform* a = NEW_TRANSFORM;
    a->SetPosition(Vector3d(127, 3, -5));
    a->SetScale(Vector3d(1, 2, -2));
    a->SetRotation(Quaternion(Vector3d(33, 5, 15)));

    Transform* b = NEW_TRANSFORM;
    b->SetParent(a);
    b->SetPosition(Vector3d(127, 3, -5));
    b->SetScale(Vector3d(1, 2, -2));
    b->SetRotation(Quaternion(Vector3d(33, 5, 15)));

    Transform* c = NEW_TRANSFORM;
    c->SetParent(b);
    c->SetPosition(Vector3d(127, 3, -5));
    c->SetScale(Vector3d(1, 2, -2));
    c->SetRotation(Quaternion(Vector3d(33, 5, 15)));

    // Exercise
    Matrix4x4 goodTrMat = c->GetGlobalTransformationMatrix();

    for (std::size_t i = 0; i < 2000; i++)
        c->RecalculateGlobalTransformCache();

    Matrix4x4 recalculatedMat = c->GetGlobalTransformationMatrix();

    // Verify
    REQUIRE(goodTrMat == recalculatedMat);

    WorldObjectManager::Free();
    return;
}

#undef NEW_TRANSFORM

