#include "Camera.h"
#include "Globals.h"
Camera::Camera(Transform* transform) {
    mainCamera = this;
	this->transform = transform;
    updateVectors();
}

void Camera::updateVectors() {
    // calculate the new Front vector
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

glm::mat4 Camera::GetView() {
    return glm::lookAt(transform->position, transform->position + front, up);
}

void Camera::MakeMainCamera() {
    mainCamera = this;
}

Camera* Camera::GetMainCamera(){
    return mainCamera;
}