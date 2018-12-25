#pragma once

#include "../maths/maths.h"

class Camera
{
public:
	Camera();
	Camera(float, float, float);
	void initViewParams();
	~Camera();
private:
	vec3 cameraPos, viewDir, up, center;
};

