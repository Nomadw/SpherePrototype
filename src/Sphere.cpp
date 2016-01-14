#include "Globals.h"

#include "MainObject.h" //Inherits from MainObject
#include "Sphere.h" //Variable and function declarations for object in header

//Definitions for SphereObject functions

//Base constructor for Sphere object
SphereObject::SphereObject()
{
	MainObject::MainObject(); //Inherits from base MainObject constructor

	SlowDown = true;

	mSpeed = Vec3f::zero();

	state = CREATION_FAST;

	age = 0.0;
	lifespan = 100.0;
	decayed = false;

	SetCollidable(true);
}

//Main constructor class - Creates all starting objects in current application. Only given position
SphereObject::SphereObject(ProgramSettings settings)
{
	SphereObject::SphereObject(); //Inherits from base SphereObject constructor

	red = 1.0f;
	blue = 0.2f;
	green = 0.2f;

	alpha = 0.0f;

	size = 1.00f;

	//Random positon- Within borders
	mPos.x = (rand() % BORDER - (BORDER / 2)) / 10.0f;
	mPos.y = (rand() % BORDER - (BORDER / 2)) / 10.0f;
	mPos.z = (rand() % BORDER - (BORDER / 2)) / 10.0f;

	texturenumber = 0;

	decayed = false;

	//If random chance of 100 is above percentage, set collidable to true (Percentage of collision being set globally by user)
	if (rand() % 100 > settings.PERCENTCOLLISION) {
		SetCollidable(true);
	}
	else SetCollidable(false);

	//Start in oscillation stage
	state = CREATION_OSCILLATION;
	lifespan = 50.0;
}

//Used for all decayed object spawning. Takes previous object's position + size
SphereObject::SphereObject(float x, float y, float z, float msize, ProgramSettings settings)
{
	SphereObject::SphereObject(settings); //Inherits from base SphereObject constructor
	 
	red = 1.0f;
	green = 0.2f;
	blue = 0.2f;

	//Set positon to position of decayed object
	mPos.x = x;
	mPos.y = y;
	mPos.z = z;

	size = msize / 2; //Always half size of decayed object

	if (size > 0.25f) //Texture used based on size of object
		texturenumber = 1;
	else
		texturenumber = 2;

	//If random chance of 100 is above percentage, set collidable to true (Percentage of collision being set globally by user)
	if (rand() % 100 > settings.PERCENTCOLLISION) {
		SetCollidable(true);
	}
	else SetCollidable(false);

	//Random speed
	mSpeed.x = (rand() % 20 - 10) / 10.0f;
	mSpeed.y = (rand() % 20 - 10) / 10.0f;
	mSpeed.z = (rand() % 20 - 10) / 10.0f;
	decayed = false;

	state = CREATION_FAST;
	lifespan = 50.0;
}

void SphereObject::Update(double elapsed, vector <SphereObject *> *objects, Vec3f camerapoint, ProgramSettings settings)
{
	Vec3f distance = mPos - camerapoint; //Object's distance from camera calculated

	int dist = (int)distance.length();

	//Updates that are done in any state

	if (dist > settings.MAXRENDERDISTANCE)
	{
		SetShow(false); //do not render if too far away (From set draw distance)
	}
	else {
		SetShow(true); //RENDER IF NOT CALCULATED OR IN RANGE
	}

	quality = 60 - (dist / 2); //Sets quality 

	if (quality <= 6) //Stops object from using too little segments - Becoming less than a 'sphere'!
		quality = 6;

	if (!settings.pause) { //Skip all of this if paused!

		//if (GetSelected()) {
		//	SetShine(0.0f);
		//}
		//else {
		//	SetShine(100.0f);
		RandMovement(); //Updates movement if object not moving
		age += elapsed; //Updates age of object - Keeps in time with frame delta / program speed
		//}

		mPos += mSpeed * (float)elapsed; //Updates position - Adds speed vector to position vector (kept in time with delta / program speed manually set)
		SpeedUpdate(elapsed); //Updates speed vector
		mTransform.setToIdentity(); //Sets 4x4 matrix as translation identity 
		mTransform.setTranslate(mPos); //Plug XYZ position vector into 4x4 translation matrix (Cinder uses 4x4 matrix for world position, XYZ vector is easier to use!)
		green = 0.2f; //Currently green value is always 0.2, make sure
		if (!settings.drawsphere) {     //IF DRAWING CIRCLES INSTEAD OF SPHERES
			//Billboard - Possible update would be to fix to camera position
			mTransform.rotate(Vec3f::yAxis(), 45.0f);
		}
		//mTransform.createRotation(mRotation);

		/*	if ((mPos.x < -BORDERX) || (mPos.x > BORDERX) || (mPos.y < -BORDERY)
		|| (mPos.y > BORDERY) || (mPos.z < -BORDERZ) || (mPos.z > BORDERZ))
		{
		mSpeed = -mSpeed;
		}*/

		//Updates that are done in specific states - See enumeration of states in globals.h

		if (state == CREATION_FAST) {

			if (alpha < 1.0f) {
				alpha += 0.2f *(float)elapsed;
			}
			else
				SetState(NORMAL);
		}

		else if (state == DECAY) {
			if (blue < 1.0f) { //Increase blue value up to max
				blue += 0.05f *(float)elapsed;
			}
			if (alpha > 0.0f) { //Decrease alpha to fade out object
				alpha -= 0.075f *(float)elapsed;

			}
			else SetState(DEAD); //Set object to dead (To be deleted from memory) when object has fully faded out

			if (alpha <= 0.2f)
			{
				if (!decayed) {
					decayed = true; //Sets decayed to true - Stops spawning happening more than one per decayed object!
					if (size > 0.25f) {
						int spawn = 2 + randGaussian(); // Selects a number based on Gaussian distribution
						for (int i = 0; i < spawn; i++) {
							SphereObject *sphere = new SphereObject(mPos.x, mPos.y, mPos.z, size, settings);
							objects->push_back(sphere);
						}
					}
				}
			}
		}

		else if (state == CREATION_OSCILLATION) {
			if (alpha < 1.0f) {
				alpha = (cosf((float)getElapsedSeconds() * 2.0f) * 0.4f) + (age / 15.0f); //"Pulses" alpha for object, giving illusion of phasing in (Using cosine)
				if (alpha < 0.1f) { alpha = 0.1f; } //Stops object from going fully invisible during spawn!
			}
			else
				SetState(NORMAL); //Brings object into normal state once alpha is above 1.0
		}

		else {
			// state must be NORMAL - Not spawning or decaying, standard lifespan

			if (red > 0.2f) {
				red -= 0.3f *(float)elapsed; //Brings red value back to normal
			}
			if (settings.drawsphere) { //Manually rotates across Y axis
				mTransform.rotate(Vec3f::yAxis(), ((float)getElapsedSeconds() * 0.3f) * settings.ProgramSpeed);
			}
			else { green = 1.0f; } //Makes object appear as green if in 2D mode

			if (age >= lifespan)
			{
				SetState(DECAY);
			}
		}
	}
}

void SphereObject::Collided(SphereObject *otherobject)
{
	if (size < otherobject->GetSize()) { //If object is smaller than other object, set to decay (unless already decaying)
		if (GetState() != DECAY)
			SetState(DECAY);
	}
	else
		mSpeed = -mSpeed; //If object is same size, or smaller than other object, reverse direction
}

void SphereObject::SpeedUpdate(double elapsed)
{   //Stops object going too fast in any direction! Comment / remove these lines if you want to remove speed limit
	if (mSpeed.x > SPHERE_MAX_SPEED) { mSpeed.x = SPHERE_MAX_SPEED; }
	if (mSpeed.y > SPHERE_MAX_SPEED) { mSpeed.y = SPHERE_MAX_SPEED; }
	if (mSpeed.z > SPHERE_MAX_SPEED) { mSpeed.z = SPHERE_MAX_SPEED; }
	if (mSpeed.x < -SPHERE_MAX_SPEED) { mSpeed.x = -SPHERE_MAX_SPEED; }
	if (mSpeed.y < -SPHERE_MAX_SPEED) { mSpeed.y = -SPHERE_MAX_SPEED; }
	if (mSpeed.z < -SPHERE_MAX_SPEED) { mSpeed.z = -SPHERE_MAX_SPEED; }

	//Gradually slows down objects, unless already at minimum speed
	if (CheckSlowDown()) {
		if (mSpeed.x > SPHERE_MIN_SPEED)
			mSpeed.x -= 0.1f * (float)elapsed;
		else if (mSpeed.x < SPHERE_MIN_SPEED)
			mSpeed.x += 0.1f * (float)elapsed;

		if (mSpeed.y > SPHERE_MIN_SPEED)
			mSpeed.y -= 0.1f * (float)elapsed;
		else if (mSpeed.y < SPHERE_MIN_SPEED)
			mSpeed.y += 0.1f * (float)elapsed;

		if (mSpeed.z > SPHERE_MIN_SPEED)
			mSpeed.z -= 0.1f * (float)elapsed;
		else if (mSpeed.z < SPHERE_MIN_SPEED)
			mSpeed.z += 0.1f * (float)elapsed;

	}
}

void SphereObject::RandMovement() //Creates random movement for object if object is moving too slowly
{
	if (mSpeed.x <= 0.01f && mSpeed.y <= 0.01f && mSpeed.z <= 0.01f)
	{
		mSpeed.x += (rand() % 20 - 10) / 10.0f;
		mSpeed.y += (rand() % 20 - 10) / 10.0f;
		mSpeed.z += (rand() % 20 - 10) / 10.0f;
	}
}
