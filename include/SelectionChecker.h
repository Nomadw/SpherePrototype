#pragma once

#include "Globals.h"

class SelectionChecker : public SphereObject
{
public:
	SelectionChecker(Vec2i MousePos, CameraPersp camera);

	void Update();
};
