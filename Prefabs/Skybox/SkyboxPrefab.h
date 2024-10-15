#pragma once
#include "../../Plato/Prefab.h"
#include "../../Plato/WorldObjectManager.h"
#include "../../Plato/ResourceManager.h"
#include "../../Plato/MeshRenderer.h"

class SkyboxPrefab : public Plato::Prefab
{
public:
    void Instantiate(Plato::Transform* parent = nullptr) override {
        const std::string assetsDir = "../../Prefabs/Skybox/assets";

        // Load resources
        Plato::Texture* texture = Plato::ResourceManager::FindTextureOrLoadFromBmp("skybox", assetsDir+"/example_skybox.bmp");
        Plato::Mesh* mesh       = Plato::ResourceManager::FindMeshOrLoadFromObj("skybox", assetsDir+"/skybox.obj");

        // Create material
        Plato::Material* mat = Plato::ResourceManager::FindMaterial("skybox");
        if (!mat) {
            mat = Plato::ResourceManager::NewMaterial("skybox");
        }
        mat->texture = texture;
        mat->noShading = false;

        // Create world object
        Plato::WorldObject* skybox = Plato::WorldObjectManager::NewWorldObject("skybox", parent);
        skybox->AddComponent<Plato::Components::MeshRenderer>(mesh, mat);
        skybox->transform->Scale(Plato::Vector3d::one * 50);
    }
};

