#pragma once

//globals.h - settings/libraries for all other files to use

//Loading in all relevant Cinder libraries - REMEMBER TO PLACE PROJECT IN SAME DIRECTORY AS CINDER 0.8.6
#include "cinder/app/AppNative.h"
#include "cinder/Camera.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Vector.h"
#include "cinder/CinderResources.h"
#include "cinder/Utilities.h"
#include "cinder/Rand.h"
#include "cinder/params/Params.h"
#include <utility>
#include <vector>
#include <functional>
using namespace ci;
using namespace ci::app;
using namespace std;

//Global program variables - Edit these to change how the base program works

#define BORDERX 1800.0f
#define BORDERY 1800.0f
#define BORDERZ 1800.0f

//Limit speeds of objects in scene
static const float SPHERE_MAX_SPEED = 0.5f;
static const float SPHERE_MIN_SPEED = 0.2f;


static const int BORDER = 4800; //For limiting spawning space of objects (X, Y and Z axis). Increase / Decrease these for a larger / smaller area to work with
static const int INITOBJECTS = 0; //Number of objects to spawn when program opens / reset is made
static const int MAXSPAWNRATE = 20000; //Maximum number of objects that can spawn a moment
static const float MAXPROGRAMSPEED = 5.0f; //Maximum speed for the program - Entire simulation is affected by this!

static const float INIT_PROG_SPEED = 1.0f; //Starting speed for the program - Also when reset
static const int INIT_SPAWN_RATE = 2; //Starting spawn rate - Also when reset

static const bool INIT_RENDER_DATA = false; //Extra, smaller information displayed in corner when this is turned on - Number of objects and FPS
static const bool INIT_DRAW_SPHERE = true; //Start / Reset with either spheres or circles drawing

static const bool INIT_COLLISION_ENABLED = false; //Starts/resets program with collision enabled or not!

static const bool INIT_PAUSE = false; //Pauses entire simulation!

static const int INIT_MAX_RENDER_DISTANCE = 1500; //Starting setting for render distance - any objects beyond this point are not drawn - Also when reset
static const int INIT_MIN_FPS = 24; //Starting setting for min FPS required for objects to automatically spawn (If spawn rate is above 0)

static const int INIT_COLLISION_PERCENT = 50; //When an object is made, chance (from 100) it will be able to collide with other objects (two collidable objects will make collision)
static const int MAX_COLLISION_PERCENT = 100;

static const Vec3f INIT_CAMERA_POS = Vec3f(3.0f, 3.0f, 3.0f); //Starting camera positon

//Program settings - Passed for a lot of individual object functions!
struct ProgramSettings {
    float ProgramSpeed = INIT_PROG_SPEED;
    int SpawnRate = INIT_SPAWN_RATE;
    
    bool renderdata = INIT_RENDER_DATA;
    bool drawsphere = INIT_DRAW_SPHERE;
    int MAXRENDERDISTANCE = INIT_MAX_RENDER_DISTANCE;
    int MINFPS = INIT_MIN_FPS;
    
    int PERCENTCOLLISION = INIT_COLLISION_PERCENT;

	bool collision = INIT_COLLISION_ENABLED;
    
    int numberofobjects;
    int framerate;

	bool pause = INIT_PAUSE;
    
    Vec3f camerapos;
};

//Enumeration for states of objects
enum STATE{ NORMAL, DECAY, CREATION_OSCILLATION, CREATION_FAST, DEAD };

