#include "Camera.h"

Camera::Camera(vec3 position, float fov) : position(position),
fov(fov), rotation(1)
{
	updateVectors();
}

mat4 Camera::getProjection()
{
	float aspect = (float)Window::getWidth() / (float)Window::getHeight();
	return glm::perspective(fov, aspect, 0.1f, 100.f);
}

vec3& Camera::getFont()
{
	return font;
}

vec3& Camera::getUp()
{
	return up;
}

vec3& Camera::getRight()
{
	return right;
}

mat4 Camera::getView()
{
	return glm::lookAt(position, position + font, up);
}

mat4& Camera::getRotation()
{
	return rotation;
}

vec3& Camera::getPosition()
{
	return position;
}

void Camera::rotate(float x, float y, float z)
{
	rotation = glm::rotate(rotation, z, vec3(0, 0, 1));
	rotation = glm::rotate(rotation, y, vec3(0, 1, 0));
	rotation = glm::rotate(rotation, x, vec3(1, 0, 0));
	updateVectors();
}

void Camera::updateVectors()
{
	font =  vec3(rotation * vec4(0, 0,-1, 1));
	right = vec3(rotation * vec4(1, 0, 0, 1));
	up =    vec3(rotation * vec4(0, 1, 0, 1));
}
