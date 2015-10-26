#pragma once

#include "Globals.h"

class SphereObject : public MainObject
{
protected:
	Vec3f mSpeed;
	bool SlowDown;

	int state;
	double statetime;

public:
	SphereObject(); //Constructer
	SphereObject(float sred, float sgreen, float sblue, float salpha, float ssize, int stexturenumber);
	SphereObject(float x, float y, float z);
	SphereObject(float x, float y, float z, int mstate);
	SphereObject(float x, float y, float z, float size, float speedx, float speedy, float speedz);
	SphereObject(float sred, float sgreen, float sblue, float salpha, float ssize, int stexturenumber, float x, float y, float z);

	bool CheckSlowDown() { return SlowDown; }

	float GetSpeedX() { return mSpeed.x; }
	float GetSpeedY() { return mSpeed.y; }
	float GetSpeedZ() { return mSpeed.z; }

	int GetState() { return state; }

	void RandMovement();

	bool SetSlowDown(bool SlowDown) { SphereObject::SlowDown = SlowDown; }

	void AddSpeedX(float xspeed) { SphereObject::mSpeed.x += xspeed; }
	void AddSpeedY(float yspeed) { SphereObject::mSpeed.y += yspeed; }
	void AddSpeedZ(float zspeed) { SphereObject::mSpeed.z += zspeed; }

	void SpeedReset() { SphereObject::mSpeed = Vec3f::zero(); }

	void Select() { SphereObject::selected = true; SpeedReset(); }

	void SetState(int state) { SphereObject::state = state; SphereObject::age = 0.0; }

	void Update(double elapsed, vector <SphereObject *> *objects, Vec3f camerapoint);
	void SpeedUpdate(double elapsed);
};
