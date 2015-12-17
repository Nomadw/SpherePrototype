#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/Camera.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
//#include "cinder/MayaCamUI.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Vector.h"
#include "cinder/CinderResources.h"
#include "cinder/TriMesh.h"
//#include "cinder/Text.h"
#include "cinder/Utilities.h"
#include "cinder/Rand.h"
#include "cinder/params/Params.h"
#include <utility>
#include <vector>
#include <functional>

using namespace ci;
using namespace ci::app;
using namespace std;

#define BORDERX 1800.0f
#define BORDERY 1800.0f
#define BORDERZ 1800.0f
static const int BORDER = 4800;
static const int INITOBJECTS = 1000;
static const int MAXSPAWNRATE = 20;
static const float MAXPROGRAMSPEED = 5.0f;


//Enumeration for states of objects
enum STATE{ NORMAL, DECAY, CREATION_OSCILLATION, CREATION_FAST, DEAD };

struct ProgramSettings {
    float ProgramSpeed = 1.0f;
    int SpawnRate = 0;
    
    bool renderdata = false;
    bool drawsphere = true;
    int MAXRENDERDISTANCE = 500;
    int MINFPS = 25;
    
    int numberofobjects;
    int framerate;
    
    Vec3f camerapos;
};



