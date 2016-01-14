#pragma once

#include "Globals.h"

//Main Object class. Sphere object inherits from here, any new objects should inherit from this

//All shine and select options are unused - Previously planned features, left in (commented) for any possible development

class MainObject
{
private:
	bool show;
	bool collidable;

protected:
	Matrix44f	mTransform; //Used for placement in Cinder

	Vec3f mPos; //Three-dimensional world position (X, Y, Z)
	Vec3f mRotation; //Rotation (X, Y, Z)

	int texturenumber; //Number of texture to use in render

	//Shader values - Put into shader
	float red; //RGB values - (Default for all = 0.2)
	float green;
	float blue;
	float alpha; //Transparency

	//float shine;

	float size; //Size of object

	//bool selected;

	double age; //Current time in stage
	double lifespan; //Max age of stage before next turning to next stage

	int quality; //Used for LOD (Level Of Detail) models


public:

	MainObject(); //Constructor

	bool CheckShow() { return show; } //If object is to be rendered or not

	int GetTexNum() { return texturenumber; } //Returns texture number for object

	bool GetCollidable() { return collidable; } //Returns if the object is collidable or not
	
	void virtual Render(); //Base render function
	void virtual Render(vector <gl::Texture *> *textures, gl::GlslProg *shader, ProgramSettings settings); //Render function for all objects

	void virtual Update(); //Base update function
	void virtual Update(double elapsed, ProgramSettings settings); //Update function for objects

	//Base destroy function (currently not used - Implemented in case needed for any future development)
	//void virtual Destroy(); 

	//Returns

	//Returns RGB / shader values
	float GetRed() { return red; }
	float GetBlue() { return blue; }
	float GetGreen() { return green; }
	float GetAlpha() { return alpha; }
	//float GetShine() { return shine; }

	//Returns quality - Useable for LOD features
	int GetQuality() { return quality; }

	//Returns size of object - Used in collision
	float GetSize() { return size; }

	//Returns X, Y and Z vector positions for object (position and rotation)
	Vec3f GetPos() { return MainObject::mPos; }
	Vec3f GetRot() { return MainObject::mRotation; }

	//Returns 4x4 matrix transformation
	Matrix44f GetTrans() { return mTransform; }

	//bool GetSelected() { return selected; }
	//virtual void Select() { MainObject::selected = true; }
	//void Deselect() { MainObject::selected = false; }

	//Returns individual X, Y and Z positions of objects
	float GetX() { return mPos.x; }
	float GetY() { return mPos.y; }
	float GetZ() { return mPos.z; }

	//Returns individual X, Y and Z rotations of objects
	float GetRx() { return mRotation.x; }
	float GetRy() { return mRotation.y; }
	float GetRz() { return mRotation.z; }


	//Settings

	//Set if object is to be rendered or not
	void SetShow(bool show) { MainObject::show = show; }

	//Set texture of object
	void SetTexNum(int texnum) { MainObject::texturenumber = texnum; }

	//Set if object is to be collidable or not
	void SetCollidable(bool collidable) { MainObject::collidable = collidable; }

	//Set individual X, Y and Z positions of objects
	void SetX(float x) { MainObject::mPos.x = x; }
	void SetY(float y) { MainObject::mPos.y = y; }
	void SetZ(float z) { MainObject::mPos.z = z; }

	//Set individual X, Y and Z rotations of objects
	void SetRx(float x) { MainObject::mRotation.x = x; }
	void SetRy(float y) { MainObject::mRotation.y = y; }
	void SetRz(float z) { MainObject::mRotation.z = z; }

	//Set quality of objects- Used in LODs
	void SetQuality(int quality) { MainObject::quality = quality; }

	//Set R / G / B + transparency values - Used in shader
	void SetRed(float red) { MainObject::red = red; }
	void SetBlue(float blue) { MainObject::blue = blue; }
	void SetGreen(float green) { MainObject::green = green; }
	void SetAlpha(float alpha) { MainObject::alpha = alpha; }
	//void SetShine(float shine) { MainObject::shine = shine; }

	//Set size of object
	void SetSize(float size) { MainObject::size = size; }

	//Checking collisions - Unused, check main code for collision detection
	bool CheckCollisions(MainObject *otherObject);
};
