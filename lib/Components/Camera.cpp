#include "Camera.h"
#include "Debug/Logging.h"

using Components::Camera;


Camera::Camera() {
    mainCamera = this;

}

void Camera::SetTransform(Transform *transform) {
    this->transform = transform;
    UpdateVectors();
}

void Camera::UpdateVectors() {
    // calculate the new Front vector
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

glm::mat4 Camera::m4GetView() const {
    return glm::lookAt(transform->position, transform->position + front, up);
}

void Camera::MakeMainCamera() {
    mainCamera = this;
}

Camera *Camera::pGetMainCamera() {
    return mainCamera;
}

REGISTER_DEF_TYPE(Camera);

Base::Result Components::Camera::SetMembers(std::vector<std::string> list) {
    //Make sure camera has a valid transform reference
    //TODO(Note: I plan on writing a proper system for entity management and will create a parent-child system)
    if (!this->transform) {
        MLOG(LOG_ERROR, "Camera object does not contain a reference to a Transform");
        return Base::Result::STATUS_ERROR;
    }

    this->transform->SetMembers(list);


    return Base::Result::STATUS_OK;
}
