#include "Camera.h"

Camera::Camera(	float aspect,
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
    float movespeed = SPEED,
    float mouseSensitivity = SENSITIVITY,
    float fov = FOV,
    float near = NEARCLIP,
    float far = FARCLIP,
    float focusDist = FOCUSDIST
    ) : 
    Yaw(YAW),
    Pitch(PITCH)
{
    Position = position;
    WorldUp = up;
    Front = front;
    MovementSpeed = movespeed;
    MouseSensitivity = mouseSensitivity;
    Fov = fov;
    Aspect = aspect;
    NearCip = near;
    FarClip = far;
    focusDistance = focusDist;

    UpdateCameraVectors();
}

Camera::Camera( float aspect,
    float posx, float posy, float posz,
    float upx, float upy, float upz,
    float frontx, float fronty, float frontz,
    float movespeed = SPEED,
    float mouseSensitivity = SENSITIVITY,
    float fov = FOV,
    float near = NEARCLIP,
    float far = FARCLIP,
    float focusDist = FOCUSDIST
) :
    Yaw(YAW),
    Pitch(PITCH)
{
    Position = glm::vec3(posx, posy, posz);
    WorldUp = glm::vec3(upx, upy, upz);
    Front = glm::vec3(frontx, fronty, frontz);
    MovementSpeed = movespeed;
    MouseSensitivity = mouseSensitivity;
    Fov = fov;
    Aspect = aspect;
    NearCip = near;
    FarClip = far;
    focusDistance = focusDist;

    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAtRH(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix()
{
    return glm::perspective(glm::radians(Fov), Aspect, NearCip, FarClip);
}

void Camera::ProcessKeyBoard(CEMERA_MOVEMENT direction, float deltaTime)
{
    float offset = MovementSpeed * deltaTime;
    switch (direction)
    {
    case FORWARD:
        Position += Front * offset;
        break;
    case BACKWARD:
        Position -= Front * offset;
        break;
    case LEFT:
        Position -= Right * offset;
        break;
    case RIGHT:
        Position += Right * offset;
        break;
    default:
        break;
    }
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool focusCenter = true, GLboolean constrainPitch = true)
{
    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;

    Yaw += xOffset;
    Pitch += yOffset;

    if (constrainPitch)
    {
        if (Pitch > 89.0f)
        {
            Pitch = 89.0f;
        }
        if (Pitch < -89.0f)
        {
            Pitch = -89.0f;
        }
    }

    glm::vec3 focusPos = Position + Front * focusDistance;

    UpdateCameraVectors();

    if (focusCenter)
    {
        FocusToCenter(focusPos);
    }
}

void Camera::ProcessMouseScroll(float zoomOffset)
{
    //??????fov?????????
    //Fov -= zoomOffset;
    //if (Fov < 1.0f)
    //{
    //	Fov = 1.0f;
    //}
    //if (Fov > 60.0f)
    //{
    //	Fov = 60.0f;
    //}

    //??????????????
    if ( focusDistance < zoomOffset && zoomOffset > 0)
    {
        return;
    }

    Position += zoomOffset * Front;
    focusDistance -= zoomOffset;
}

void Camera::UpdateCameraVectors() 
{
    glm::vec3 front;
    front.x = sin(glm::radians(Yaw))*cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = -cos(glm::radians(Yaw))*cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}

//?????????????????????
void Camera::FocusToCenter(glm::vec3 focusCenter )
{
    Position = focusCenter - Front * focusDistance;
}