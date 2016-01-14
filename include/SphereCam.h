#pragma once

#include "Globals.h"

//Code taken and edited from cinder/MayaCam - Created for any edits to camera usage

//Wheel zoom functions created, no longer used but left (commented) in

//Controls for camera were altered for Mac usage in mind (Can all be done with one mouse / trackpad button) - Also works fine with Windows!

class SphereCam {
private:
	enum		{ ACTION_NONE, ACTION_ZOOM, ACTION_PAN, ACTION_TUMBLE, ACTION_ZOOM2 };

	Vec2i		mInitialMousePos;
	//float		mInitialMouseWheel;
	CameraPersp	mCurrentCam, mInitialCam; //Uses camera perspectives from Cinder's camera.h
	int			mLastAction;
	//float		mLastWheelAction;

public:
	SphereCam(); 
	SphereCam(const CameraPersp &aInitialCam);

	void mouseDown(const Vec2i &mousePos);
	void mouseDrag(const Vec2i &mousePos, bool rightDown, bool controlDown, bool leftDown, bool shiftDown);
	//void wheelZoom(float wheelIncrement);

	//Returns position
	Vec3f GetPosition() { return mCurrentCam.getEyePoint(); }

	const CameraPersp& getCamera() const { return mCurrentCam; }
	void setCurrentCam(const CameraPersp &aCurrentCam) { mCurrentCam = aCurrentCam; }
    
    void setPos(Vec3f NewPos);
};
