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

	//rot mat use quaternion, euler is not friendly
	quat rotquat = quat(radians(rotation));
	mat4 rot = glm::toMat4(rotquat);
	model = rot*model;
	model = translate(model, position);
	return model;
}

vec3 Transform::Front()
{
	vec3 front = GetModelMatrix()*vec4(0, 0, 1, 0);
	return normalize(front);
}

vec3 Transform::Up()
{
	vec3 up = GetModelMatrix()*vec4(0, 1, 0, 0);
	return normalize(vec3(up));
}

vec3 Transform::Right()
{
	vec3 right = GetModelMatrix()*vec4(1, 0, 0, 0);
	return normalize(vec3(right));
}

Transform::~Transform()
{
}