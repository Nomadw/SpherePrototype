#include "Globals.h"

#include "SphereCam.h"

SphereCam::SphereCam()
{
	mInitialCam = mCurrentCam = CameraPersp();
}

SphereCam::SphereCam(const CameraPersp &aInitialCam)
{
	mInitialCam = mCurrentCam = aInitialCam;
}

void SphereCam::mouseDown(const Vec2i &mousePos)
{
	mInitialMousePos = mousePos;
	mInitialCam = mCurrentCam;
	mLastAction = ACTION_NONE;
}

void SphereCam::mouseDrag(const Vec2i &mousePos, bool rightDown, bool controlDown, bool leftDown, bool shiftDown)
{
	int action;
    if ((leftDown || rightDown) && controlDown)
        action = ACTION_TUMBLE;
    else if ((leftDown || rightDown) && shiftDown)
        action = ACTION_ZOOM;
	else if (leftDown && !rightDown)
		action = ACTION_PAN;
	else if (rightDown && !leftDown)
        action = ACTION_ZOOM;
		else return;

	if (action != mLastAction) {
		mInitialCam = mCurrentCam;
		mInitialMousePos = mousePos;
	}

	mLastAction = action;

	if (action == ACTION_ZOOM) { // zooming
		int mouseDelta = (mousePos.x - mInitialMousePos.x) + (mousePos.y - mInitialMousePos.y);

		float newCOI = powf(2.71828183f, -mouseDelta / 500.0f) * mInitialCam.getCenterOfInterest();
		Vec3f oldTarget = mInitialCam.getCenterOfInterestPoint();
		Vec3f newEye = oldTarget - mInitialCam.getViewDirection() * newCOI;
		mCurrentCam.setEyePoint(newEye);
		mCurrentCam.setCenterOfInterest(newCOI);
	}
	else if (action == ACTION_PAN) { // panning
		float deltaX = (mousePos.x - mInitialMousePos.x) / 1000.0f * mInitialCam.getCenterOfInterest();
		float deltaY = (mousePos.y - mInitialMousePos.y) / 1000.0f * mInitialCam.getCenterOfInterest();
		Vec3f mW = mInitialCam.getViewDirection().normalized();
		Vec3f mU = Vec3f::yAxis().cross(mW).normalized();
		Vec3f mV = mW.cross(mU).normalized();
		mCurrentCam.setEyePoint(mInitialCam.getEyePoint() + mU * deltaX + mV * deltaY);
	}
	else { // tumbling
		float deltaY = (mousePos.y - mInitialMousePos.y) / 100.0f;
		float deltaX = (mousePos.x - mInitialMousePos.x) / -100.0f;
		Vec3f mW = mInitialCam.getViewDirection().normalized();
		bool invertMotion = (mInitialCam.getOrientation() * Vec3f::yAxis()).y < 0.0f;
		Vec3f mU = Vec3f::yAxis().cross(mW).normalized();

		if (invertMotion) {
			deltaX = -deltaX;
			deltaY = -deltaY;
		}

		Vec3f rotatedVec = Quatf(mU, deltaY) * (mInitialCam.getEyePoint() - mInitialCam.getCenterOfInterestPoint());
		rotatedVec = Quatf(Vec3f::yAxis(), deltaX) * rotatedVec;

		mCurrentCam.setEyePoint(mInitialCam.getCenterOfInterestPoint() + rotatedVec);
		mCurrentCam.setOrientation(mInitialCam.getOrientation() * Quatf(mU, deltaY) * Quatf(Vec3f::yAxis(), deltaX));
	}
}

void SphereCam::wheelZoom(float wheelIncrement)
{
	//float action;

	//action = wheelIncrement;

	//if (action != mLastWheelAction) {
	//	mInitialCam = mCurrentCam;
	//	mInitialMouseWheel = wheelIncrement;
	//}

	//mLastWheelAction = action;
	//int mouseDelta = (mousePos.x - mInitialMousePos.x) + (mousePos.y - mInitialMousePos.y);
//	float wheelDelta = wheelIncrement - mInitialMouseWheel;

	bool reverse;

	if (wheelIncrement >= 0.0f)
		reverse = false;
	else reverse = true;

	float newCOI;

	//if (reverse)
	//newCOI = powf(2.71828183f, wheelIncrement / 5.0f) * mInitialCam.getCenterOfInterest();
	//else
		newCOI = powf(2.71828183f, -wheelIncrement / 5.0f) * mInitialCam.getCenterOfInterest();
	Vec3f oldTarget = mInitialCam.getCenterOfInterestPoint();
	Vec3f newEye = oldTarget - mInitialCam.getViewDirection() * newCOI;
	mCurrentCam.setEyePoint(newEye);
	mCurrentCam.setCenterOfInterest(newCOI);
}

void SphereCam::setPos(Vec3f NewPos)
{
    float COI = NewPos.length();
    mCurrentCam.setEyePoint(NewPos);
    
    mCurrentCam.setCenterOfInterest(COI);
}
