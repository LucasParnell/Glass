#include <cassert>
#include "EntityManager.h"
#include "Components/Mesh.h"
#include "ComponentManager.h"
#include "Components/IComponentFactory/IComponentFactory.h"
#include "Components/Camera.h"
#include "Components/Shader.h"

using namespace Managers;
Entity EntityManager::CreateEntity() {
    UUIDv4::UUID uuid = uuidGenerator.getUUID();
    std::string s = uuid.str();
    entities.push_back(s);
    return s;
}

void EntityManager::ConstructGSD(std::string mountPoint, std::string filename, ComponentManager* compMan, Rendering::RenderSystem &renderSystem) {
    std::string json_str = Filesystem::VFS::ReadToString(mountPoint, filename);
    auto json = nlohmann::json::parse(json_str);

    //Setup camera
    auto cam = json["scene_camera"];
    auto cam_transform = cam["Transform"];

    auto cam_inst = (Components::Camera*) IComponentFactory::createInstance("Camera");
    cam_inst->SetMembers(cam_transform);

    auto cam_entity = CreateEntity();
    auto cam_comp_id = compMan->RegisterComponent("Camera");
    compMan->AddComponent(cam_entity, cam_comp_id);
    compMan->SetComponent(cam_comp_id, cam_inst);

    cam_inst->MakeMainCamera();



    //Iterate through scene entities

    for (auto &ent: json["scene_entities"].get<nlohmann::json::object_t>()) {
        auto entity = CreateEntity();
        MLOG(LOG_INFO, "Creating Entity '" + ent.first + "'" + " with UUID: " + entity);

        //Iterate through and build each component
        std::vector<Components::Mesh*> meshes;
        std::vector<Components::Shader*> shaders;
        for (auto &comp: ent.second.get<nlohmann::json::object_t>()) {

            auto comp_id = compMan->RegisterComponent(comp.first);
            compMan->AddComponent(entity, comp_id);

            auto comp_inst = IComponentFactory::createInstance(comp.first);
            std::vector<std::string> members = comp.second;

            comp_inst->SetMembers(members);

            if(comp.first == "Mesh")
                meshes.push_back((Components::Mesh*)comp_inst);
            if(comp.first == "Shader")
                shaders.push_back((Components::Shader*)comp_inst);

            compMan->SetComponent(comp_id, comp_inst);

        }

        if(meshes.size() == shaders.size()){
            for(int i=0; i<shaders.size(); i++){
                Loaders::ShaderLoader::LoadObject(*shaders[i], *meshes[i], renderSystem);
            }
        }

    }
}
