#include "camera.h"



Camera::Camera()
{
	cameraPos = vec3(0, 0, 0);
	initViewParams();
}

Camera::Camera(float x, float y, float z)
{
	cameraPos = vec3(x, y, z);
	initViewParams();
}

void Camera::initViewParams()
{
	up = vec3(0, 1, 0);
	center = vec3(0, 0, 0);
	viewDir = vec3(0, 0, -1);
}


Camera::~Camera()
{
}
