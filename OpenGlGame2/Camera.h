#ifndef CAMERA_H
#define CAMERA_H
#include "Window.h"
#include "lib.h"
#include "Defines.h"
class Camera
{
private:
	void updateVectors();
	mat4 rotation;
	float fov;
	vec3 position;
	vec3 font;
	vec3 up;
	vec3 right;
public:

	Camera(vec3 position, float fov);
	~Camera();

	mat4 getProjection();
	vec3& getFont();
	vec3& getUp();
	vec3& getRight();

	mat4 getView();
	mat4& getRotation();
	vec3& getPosition();
	void rotate(float x, float y, float z);
};

#endif // !CAMERA_H