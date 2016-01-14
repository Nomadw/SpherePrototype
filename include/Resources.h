#pragma once
#include "cinder/CinderResources.h"

//Write any resources - shader files, textures, etc, here.
//If developing for Macintosh, add files into resources folder in xcode
//If developing for Windows, write the defined name (e.g. RES_TEXTURE_1) in Resources.rc (found in vc2013 folder) - Use a text editor such as notepad

//#define RES_MY_RES			CINDER_RESOURCE( ../resources/, image_name.png, 128, IMAGE )

//Shader files
#define RES_SHADER_FRAG		CINDER_RESOURCE( ../resources/, phong_frag.glsl, 128, GLSL )
#define RES_SHADER_VERT		CINDER_RESOURCE( ../resources/, phong_vert.glsl, 129, GLSL )

//Textures
#define RES_TEXTURE_1		CINDER_RESOURCE( ../resources/, fire.jpg, 130, IMAGE )
#define RES_TEXTURE_2		CINDER_RESOURCE(../resources/, greenfire.jpg, 131, IMAGE )
#define RES_TEXTURE_3		CINDER_RESOURCE(../resources/, particle.jpg, 132, IMAGE )

