#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "glass_export.h"
#include "IComponent.h"
#include "Components/IComponentFactory/DerivedRegister.h"
#include "Transform.h"

namespace Components {

    class Camera : public IComponent {
    public:
        explicit GLASS_EXPORT Camera();

        Base::Result SetMembers(std::vector<std::string> list) override;

        GLASS_EXPORT void SetTransform(Transform *transform);

        GLASS_EXPORT void UpdateVectors();


        void MakeMainCamera();

        static Camera *pGetMainCamera();

        [[nodiscard]] glm::mat4 m4GetView() const;


        Transform *transform = new Transform();

        glm::vec3 front = glm::vec3(0.0f);
        glm::vec3 right = glm::vec3(0.0f);
        glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 up = glm::vec3(0.0f);
        float yaw = 0;
        float pitch = 0;
        GLASS_REFLECTABLE();
        REGISTER_DEC_TYPE(Camera);

    private:
        inline static Camera *mainCamera = nullptr;


    };

}
REFL_AUTO(type(Components::Camera, bases<Components::IComponent>))