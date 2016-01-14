#pragma once

#include "Globals.h"

//Sphere object - All particles. Inherits from MainObject

class SphereObject : public MainObject
{
protected:

	Vec3f mSpeed; //Vector of current speed - X Y and Z values
	bool SlowDown; //Set if object will naturally slow down

	int state; //State of object (See enumeratuion in Globals.h)
    
    bool decayed; //Forces decay spawning to only happen once - See DECAY state in update function
	double statetime;

public:
	SphereObject(); //Base Constructer
	
	SphereObject(ProgramSettings settings); //New object spawning
	SphereObject(float x, float y, float z, float size, ProgramSettings settings); //Decayed object spawning

	//Returns

	//Returns slowdown boolean
	bool CheckSlowDown() { return SlowDown; }

	//Returns individual X, Y and Z speed properties of object
	float GetSpeedX() { return mSpeed.x; }
	float GetSpeedY() { return mSpeed.y; }
	float GetSpeedZ() { return mSpeed.z; }

	int GetState() { return state; }
    
    bool GetDecayed() {return decayed;}

	void RandMovement();

	//Settings

	void SetSlowDown(bool SlowDown) { SphereObject::SlowDown = SlowDown; }

	//Add to current X, Y and Z speed of object
	void AddSpeedX(float xspeed) { SphereObject::mSpeed.x += xspeed; }
	void AddSpeedY(float yspeed) { SphereObject::mSpeed.y += yspeed; }
	void AddSpeedZ(float zspeed) { SphereObject::mSpeed.z += zspeed; }

	void SpeedReset() { SphereObject::mSpeed = Vec3f::zero(); }

	//void Select() { SphereObject::selected = true; SpeedReset(); }
    
    void SetDecayed(bool decayed) {SphereObject::decayed = decayed;}

	//Setting state of object will also reset age
	void SetState(int state) { SphereObject::state = state; SphereObject::age = 0.0; }

	//Update and collision functions
	void Update(double elapsed, vector <SphereObject *> *objects, Vec3f camerapoint, ProgramSettings settings);
	void SpeedUpdate(double elapsed);
    
	//Code to carry out when two objects have collided
    void Collided(SphereObject *otherobject);

	//Render and collison checking functions directly inherited from MainObject
};
