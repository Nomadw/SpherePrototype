#pragma once

#include "Globals.h"

//Code taken from cinder/MayaCam - Created for any edits to camera usage

class SphereCam {
private:
	enum		{ ACTION_NONE, ACTION_ZOOM, ACTION_PAN, ACTION_TUMBLE, ACTION_ZOOM2 };

	Vec2i		mInitialMousePos;
	float		mInitialMouseWheel;
	CameraPersp	mCurrentCam, mInitialCam;
	int			mLastAction;
	float		mLastWheelAction;

public:
	SphereCam(); 
	SphereCam(const CameraPersp &aInitialCam);

	void mouseDown(const Vec2i &mousePos);
	void mouseDrag(const Vec2i &mousePos, bool rightDown, bool middleDown);
	void wheelZoom(float wheelIncrement);

	Vec3f GetPosition() { return mCurrentCam.getEyePoint(); }

	const CameraPersp& getCamera() const { return mCurrentCam; }
	void setCurrentCam(const CameraPersp &aCurrentCam) { mCurrentCam = aCurrentCam; }

};
