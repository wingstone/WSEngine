# include <component/Transform.h>

Transform::Transform()
{
	this->position = vec3(0.0f);
	this->rotation = vec3(0.0f);
	this->scale = vec3(1.0f, 1.0f, 1.0f);
	transform = this;
}

ComponentType Transform::type() const
{
	return TYPE;
}

mat4 Transform::GetModelMatrix()
{
	mat4 model = mat4(1.0f);
	model = glm::scale(model, scale);
	model = rotate(model, radians(rotation.x), vec3(1, 0, 0));
	model = rotate(model, radians(rotation.y), vec3(0, 1, 0));
	model = rotate(model, radians(rotation.z), vec3(0, 0, 1));
	model = translate(model, position);
	return model;
}

vec3 Transform::Front()
{
	vec4 front = GetModelMatrix()*vec4(0, 0, 1, 0);
	return normalize(vec3(front));
}

vec3 Transform::Up()
{
	vec4 up = GetModelMatrix()*vec4(0, 1, 0, 0);
	return normalize(vec3(up));
}

Transform::~Transform()
{
}