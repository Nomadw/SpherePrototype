#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/Camera.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
//#include "cinder/MayaCamUI.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Vector.h"
#include "cinder/CinderResources.h"
#include "cinder/TriMesh.h"
#include <utility>
#include <vector>

using namespace ci;
using namespace ci::app;
using namespace std;

#define BORDERX 30.0f
#define BORDERY 30.0f
#define BORDERZ 30.0f

//Enumeration for states of objects
enum STATE{ NORMAL, DECAY, CREATION_OSCILLATION, CREATION_FAST, DEAD };
