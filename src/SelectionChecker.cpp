#include "Globals.h"

#include "MainObject.h"
#include "Sphere.h"
#include "SphereCam.h"
#include "SelectionChecker.h"

SelectionChecker::SelectionChecker(Vec2i MousePos, CameraPersp camera)
{
	mPos.x = camera.getViewDirection().x;
	mPos.y = camera.getViewDirection().y;
	mPos.z = camera.getViewDirection().z - camera.getFov();

	//mPos = camera.getEyePoint();

	mSpeed = camera.getViewDirection();

	size = 0.2f;

	SlowDown = false;
}

void SelectionChecker::Update()
{
	mPos += mSpeed;
	mTransform.setToIdentity();
	mTransform.setTranslate(mPos);
}
