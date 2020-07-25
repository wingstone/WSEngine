#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

class Camera
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

public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
		float movespeed = SPEED,
		float mouseSensitivity = SENSITIVITY,
		float fov = FOV,
		float aspect = 1.3333f,
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

	Camera(float posx, float posy, float posz,
		float upx, float upy, float upz,
		float frontx, float fronty, float frontz,
		float movespeed = SPEED,
		float mouseSensitivity = SENSITIVITY,
		float fov = FOV,
		float aspect = 1.3333f,
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

	glm::mat4 GetViewMatrix()
	{
		return glm::lookAtRH(Position, Position + Front, Up);
	}

	glm::mat4 GetProjectionMatrix()
	{
		return glm::perspective(glm::radians(Fov), Aspect, NearCip, FarClip);
	}

	void ProcessKeyBoard(CEMERA_MOVEMENT direction, float deltaTime)
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

	void ProcessMouseMovement(float xOffset, float yOffset, bool focusCenter = true, GLboolean constrainPitch = true)
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

	void ProcessMouseScroll(float zoomOffset)
	{
		//不用改变fov的方法试试
		//Fov -= zoomOffset;
		//if (Fov < 1.0f)
		//{
		//	Fov = 1.0f;
		//}
		//if (Fov > 60.0f)
		//{
		//	Fov = 60.0f;
		//}

		//移动相机距离的方法
		if ( focusDistance < 1.1f)
		{
			return;
		}

		Position += glm::clamp(zoomOffset, 0.0f, 1.0f) * Front;
		focusDistance -= zoomOffset;
	}


private:

	void UpdateCameraVectors() 
	{
		glm::vec3 front;
		front.x = sin(glm::radians(Yaw))*cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = -cos(glm::radians(Yaw))*cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}

	//根据相机朝向更新相机坐标
	void FocusToCenter(glm::vec3 focusCenter )
	{
		Position = focusCenter - Front * focusDistance;
	}

};


#endif