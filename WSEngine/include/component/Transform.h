#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../core/Component.h"
#include "../utils/GlobalLog.h"

using namespace glm;

class Transform : public Component
{
public:
	static const ComponentType TYPE = COMPONENT_TRANSFORM;
	Transform* transform;
	vec3 position;
	vec3 rotation;
	vec3 scale;

	ComponentType type() const;

	Transform();
	~Transform();

	mat4 GetModelMatrix();
	vec3 Front();
	vec3 Up();
	vec3 Right();


private:
};


#endif // !SHADER_H
