#include "Globals.h"

#include "MainObject.h"
#include "Sphere.h"

SphereObject::SphereObject()
{
	MainObject::MainObject();

	SlowDown = true;

	mSpeed = Vec3f::zero();

	state = CREATION_FAST;

	age = 0.0;
	lifespan = 100.0;
    decayed = false;
}

SphereObject::SphereObject(float sred, float sgreen, float sblue, float salpha, float ssize, int stexturenumber)
{
	SphereObject::SphereObject();

	red = sred;
	blue = sblue;
	green = sgreen;

	alpha = salpha;

	size = ssize;

	texturenumber = stexturenumber;
    decayed = false;
}

SphereObject::SphereObject(float x, float y, float z)
{
	SphereObject::SphereObject();

	red = 1.0f;
	blue = 0.2f;
	green = 0.2f;

	alpha = 0.0f;

	size = 1.00f;

	mPos.x = x;
	mPos.y = y;
	mPos.z = z;

	texturenumber = 0;
    
    decayed = false;
    
	state = CREATION_OSCILLATION;
	lifespan = 50.0;
}

SphereObject::SphereObject(float x, float y, float z, int mstate)
{
	SphereObject::SphereObject();

	state = mstate;

	red = rand() % 10 / 10.0f;
	blue = rand() % 10 / 10.0f;
	green = rand() % 10 / 10.0f;

	alpha = rand() % 10 / 10.0f;

	size = rand() % 100 / 100.0f;

	mPos.x = x;
	mPos.y = y;
	mPos.z = z;
    decayed = false;
}

SphereObject::SphereObject(float x, float y, float z, float msize, float speedx, float speedy, float speedz)
{
	SphereObject::SphereObject();

	red = 1.0f;
	green = 0.2f;
	blue = 0.2f;

	mPos.x = x;
	mPos.y = y;
	mPos.z = z;

	size = msize / 2;

	if (size > 0.25f)
		texturenumber = 1;
	else
		texturenumber = 2;

	mSpeed.x = speedx;
	mSpeed.y = speedy;
	mSpeed.z = speedz;
    decayed = false;

	state = CREATION_FAST;
	lifespan = 50.0;
}

SphereObject::SphereObject(float sred, float sgreen, float sblue, float salpha, float ssize, int stexturenumber, float x, float y, float z)
{
	SphereObject::SphereObject();

	SlowDown = true;

	red = sred;
	blue = sblue;
	green = sgreen;

	alpha = salpha;

	size = ssize;

	texturenumber = stexturenumber;

	mPos.x = x;
	mPos.y = y;
	mPos.z = z;
    
    decayed = false;
    
	mSpeed = Vec3f::zero();
}

void SphereObject::Update(double elapsed, vector <SphereObject *> *objects, Vec3f camerapoint, ProgramSettings settings)
{
	Vec3f distance = mPos - camerapoint;

	int dist = (int)distance.length();
    
    if(dist > settings.MAXRENDERDISTANCE)
    {
        SetShow(false); //do not render if too far away
    } else {
        SetShow(true); //RENDER IF NOT CALCULATED OR IN RANGE
    }

	quality = 60 - (dist / 2);

	if (quality <= 6)
		quality = 6;

    
	if (GetSelected()) {
		SetShine(0.0f);
	}
		else {
			SetShine(100.0f);
			RandMovement();
			age += elapsed;
		}

		mPos += mSpeed * (float)elapsed;
		SpeedUpdate(elapsed);
		mTransform.setToIdentity();
		mTransform.setTranslate(mPos);
    green = 0.2f;
    if(!settings.drawsphere) {                      //IF DRAWING CIRCLES INSTEAD OF SPHERES
            mTransform.rotate(Vec3f::yAxis(), 45.0f);
    }
		//mTransform.createRotation(mRotation);

	/*	if ((mPos.x < -BORDERX) || (mPos.x > BORDERX) || (mPos.y < -BORDERY)
			|| (mPos.y > BORDERY) || (mPos.z < -BORDERZ) || (mPos.z > BORDERZ))
		{
			mSpeed = -mSpeed;
		}*/

	if (state == CREATION_FAST) {
		
		if (alpha < 1.0f) {
			alpha += 0.2f *(float)elapsed;
		}
		else
			SetState(NORMAL);
	}

	else if (state == DECAY) {
		if (blue < 1.0f) {
			blue += 0.05f *(float)elapsed;
			//red -= 0.05f *(float)elapsed;
			//green -= 0.05f *(float)elapsed;
		}
		if (alpha > 0.0f) {
			alpha -= 0.075f *(float)elapsed;
			
				if (rand() % 350 == 1) {
					
				}
			
		} else SetState(DEAD);
        if(alpha <= 0.2f)
        {
            if(!decayed) {
                decayed = true;
            if (size > 0.25f) {
            int spawn = 2 + randGaussian();
            for (int i = 0; i < spawn; i++) {
                SphereObject *sphere = new SphereObject(mPos.x, mPos.y, mPos.z, size, (rand() % 20 - 10) / 10.0f, (rand() % 20 - 10) / 10.0f, (rand() % 20 - 10) / 10.0f);
                objects->push_back(sphere);
            }
            }
            }
        }
        
		
		
	}

	else if (state == CREATION_OSCILLATION) {
		if (alpha < 1.0f) {
			alpha = (cosf((float)getElapsedSeconds() * 2.0f) * 0.4f) + (age / 15.0f);
			if (alpha < 0.1f) { alpha = 0.1f; }
		}
		else
			SetState(NORMAL);
	}

	else { // state must be NORMAL
		if (red > 0.2f) {
			red -= 0.3f *(float)elapsed;
		}
        if(settings.drawsphere) {
            mTransform.rotate(Vec3f::yAxis(), ((float)getElapsedSeconds() * 0.3f) * settings.ProgramSpeed);
        } else { green = 1.0f; }
       
		if (age >= lifespan)
		{
			SetState(DECAY);
		}
	}
    
}

void SphereObject::SpeedUpdate(double elapsed)
{
	if (mSpeed.x > 0.5f) { mSpeed.x = 0.5f; }
	if (mSpeed.y > 0.5f) { mSpeed.y = 0.5f; }
	if (mSpeed.z > 0.5f) { mSpeed.z = 0.5f; }
	if (mSpeed.x < -0.5f) { mSpeed.x = -0.5f; }
	if (mSpeed.y < -0.5f) { mSpeed.y = -0.5f; }
	if (mSpeed.z < -0.5f) { mSpeed.z = -0.5f; }

	if (CheckSlowDown()) {
		if (mSpeed.x > 0.2f)
			mSpeed.x -= 0.1f * (float) elapsed;
		else if (mSpeed.x < 0.2f)
			mSpeed.x += 0.1f * (float) elapsed;

		if (mSpeed.y > 0.2f)
			mSpeed.y -= 0.1f * (float) elapsed;
		else if (mSpeed.y < 0.2f)
			mSpeed.y += 0.1f * (float) elapsed;

		if (mSpeed.z > 0.2f)
			mSpeed.z -= 0.1f * (float) elapsed;
		else if (mSpeed.z < 0.2f)
			mSpeed.z += 0.1f * (float) elapsed;

	}
}

void SphereObject::RandMovement()
{
	if (mSpeed.x <= 0.01f && mSpeed.y <= 0.01f && mSpeed.z <= 0.01f )
	{
		mSpeed.x += (rand() % 20 - 10) / 10.0f;
		mSpeed.y += (rand() % 20 - 10) / 10.0f;
		mSpeed.z += (rand() % 20 - 10) / 10.0f;
	}
}
