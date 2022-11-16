#pragma once

#include <cstdint>
#include <vector>
#include "Components/IComponent.h"
#include "Components/Transform.h"
#include "Debug/Logging.h"
#include "Mesh.h"
#include "Components/IComponentFactory/DerivedRegister.h"
#include "glass_export.h"

namespace Components {
    class Shader : public Components::IComponent {
    public:
        Base::Result SetMembers(std::vector<std::string> list) override;

        enum UniformType{
            VEC3,
            INT1,
            MAT4
        };


        GLASS_EXPORT void SetUniform(void* value, const std::string& uniform_name, UniformType type) {
            auto location = glGetUniformLocation(this->ID, uniform_name.c_str());
            if(location == -1){
                MLOG(LOG_WARN, "Uniform '"+uniform_name+"' does not exist in shader " + std::to_string(this->ID));
                return;
            }
            switch (type) {
                case VEC3:{
                    auto& cast_value = *((glm::vec3*)value);
                    uStore.StoreVec3.push_back(std::pair(location,cast_value));
                    break;
                }
                case INT1:{
                    auto& cast_value = *((int*)value);
                    uStore.StoreInt.push_back(std::pair(location,cast_value));
                    break;
                }
                case MAT4:{
                    auto& cast_value = *((glm::mat4*)value);
                    uStore.StoreMat4.push_back(std::pair(location,cast_value));
                }
            }
        }

        void ExecUniforms(){
            for(auto& s_mat4 : uStore.StoreMat4){
                auto& location = s_mat4.first;
                auto& cast_value = s_mat4.second;
                glUniformMatrix4fv(location, 1, GL_FALSE, &cast_value[0][0]);
            }
            for(auto& s_vec3 : uStore.StoreVec3){
                auto& location = s_vec3.first;
                auto& cast_value = s_vec3.second;
                glUniform3fv(location, 1, &cast_value[0]);
            }
            for(auto& s_int : uStore.StoreInt){
                auto& location = s_int.first;
                auto& cast_value = s_int.second;
                glUniform1i(location, cast_value);
            }
            //TODO(USE QUEUE INSTEAD)
            uStore.StoreMat4.clear();
            uStore.StoreVec3.clear();
            uStore.StoreInt.clear();
        }


        std::string mountpoint;
        std::string shaderName;

        Mesh *meshToLoad;
        uint32_t ID = 1;
        uint32_t other = 3;

        GLASS_REFLECTABLE();
        REGISTER_DEC_TYPE(Shader);


    private:
        struct UniformStore{
            std::vector<std::pair<int, glm::vec3>> StoreVec3;
            std::vector<std::pair<int, GLint>> StoreInt;
            std::vector<std::pair<int, glm::mat4>> StoreMat4;
        };

        UniformStore uStore;


    };
}
REFL_AUTO(type(Components::Shader, bases < Components::IComponent > ))