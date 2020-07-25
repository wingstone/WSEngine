#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

class Transform
{
public:
	vec3 position;
	vec3 rotation;
	vec3 scale;

public:
	Transform(vec3 position = vec3(0.0f), vec3 rotation = vec3(0.0f), vec3 scale = vec3(1.0f, 1.0f, 1.0f))
	{
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
	}

	mat4 GetModelMatrix()
	{
		mat4 model = mat4(1.0f);
		glm::scale(model, scale);
		rotate(model, radians(rotation.x), vec3(1, 0, 0));
		rotate(model, radians(rotation.y), vec3(0, 1, 0));
		rotate(model, radians(rotation.z), vec3(0, 0, 1));
		translate(model, position);
		return model;
	}

	~Transform()
	{
	}

private:

};


#endif // !SHADER_H
