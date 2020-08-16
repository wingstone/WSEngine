#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../core/Component.h"
#include "../component/Transform.h"
#include "../core/Entity.h"

class Entity;

class Camera : public Component
{
public:
	Transform* transform;
	float fov;
	float aspect;
	float nearClip;
	float farClip;
	float focusDist;

	static const ComponentType TYPE = COMPONENT_CAMERA;

	ComponentType type() const
	{
		return TYPE;
	}

	Camera();

	glm::mat4 GetViewMatrix();

	glm::mat4 GetProjectionMatrix();

};


#endif