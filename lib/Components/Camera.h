#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "glass_export.h"
#include "IComponent.h"
#include "Transform.h"

class Camera : IComponent {
public:
     explicit GLASS_EXPORT Camera(Transform *position);

    GLASS_EXPORT void UpdateVectors();

    void MakeMainCamera();

    static Camera* pGetMainCamera();

    glm::mat4 m4GetView();


    Transform *transform;

    glm::vec3 front = glm::vec3(0.0f);
    glm::vec3 right = glm::vec3(0.0f);
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f);
    float yaw = 0;
    float pitch = 0;

private:
    inline static Camera *mainCamera = nullptr;
};