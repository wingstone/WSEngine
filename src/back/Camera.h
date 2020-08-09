#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../core/Component.h"

enum CEMERA_MOVEMENT
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW = 0.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;
const float NEARCLIP = 0.01f;
const float FARCLIP = 100.0f;
const float FOCUSDIST = 5.0f;

class Camera : public Component
{
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float focusDistance;

	float Yaw;
	float Pitch;

	float MovementSpeed;
	float MouseSensitivity;
	float Fov;

	float Aspect;
	float NearCip;
	float FarClip;

	static const ComponentType TYPE = COMPONENT_CAMERA;

	ComponentType type() const
	{
		return TYPE;
	}

	Camera(	float aspect,
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
		float movespeed = SPEED,
		float mouseSensitivity = SENSITIVITY,
		float fov = FOV,
		float near = NEARCLIP,
		float far = FARCLIP,
		float focusDist = FOCUSDIST
	);

	Camera( float aspect,
		float posx, float posy, float posz,
		float upx, float upy, float upz,
		float frontx, float fronty, float frontz,
		float movespeed = SPEED,
		float mouseSensitivity = SENSITIVITY,
		float fov = FOV,
		float near = NEARCLIP,
		float far = FARCLIP,
		float focusDist = FOCUSDIST
	);

	glm::mat4 GetViewMatrix();

	glm::mat4 GetProjectionMatrix();

	void ProcessKeyBoard(CEMERA_MOVEMENT direction, float deltaTime);

	void ProcessMouseMovement(float xOffset, float yOffset, bool focusCenter = true, GLboolean constrainPitch = true);

	void ProcessMouseScroll(float zoomOffset);


private:

	void UpdateCameraVectors();

	void FocusToCenter(glm::vec3 focusCenter );

};


#endif