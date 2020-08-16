#include <component/Camera.h>

Camera::Camera()
{
    transform = nullptr;
    fov = 45.0f;
    aspect = 1280.0f/720;
    nearClip = 0.01f;
    farClip = 100.0f;
    focusDist = 5.0f;
}


glm::mat4 Camera::GetViewMatrix()
{
    Transform* camTrans = entity->GetComponent<Transform>();
    return glm::lookAtRH(camTrans->position, camTrans->position + camTrans->Front(), camTrans->Up());
}

glm::mat4 Camera::GetProjectionMatrix()
{
    return glm::perspective(glm::radians(fov), aspect, nearClip, farClip);
}