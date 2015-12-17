#pragma once

#include "Globals.h"

class MainObject : public TriMesh
{
private:
	bool show;
	bool collidable;

protected:
	Matrix44f	mTransform;

	Vec3f mPos;
	Vec3f mRotation;

	int texturenumber;

	float red;
	float green;
	float blue;
	float alpha;

	float shine;

	float size;

	bool selected;

	double age;
	double lifespan;

	int quality;


public:

	MainObject(); //Constructer

	bool CheckShow() { return show; }

	int GetTexNum() { return texturenumber; }

	bool GetCollidable() { return collidable; }
	
	void virtual Render();
	void virtual Render(vector <gl::Texture *> *textures, gl::GlslProg *shader, ProgramSettings settings);

	void virtual Update();
	void virtual Update(double elapsed, ProgramSettings settings);

	void virtual Destroy();

	float GetRed() { return red; }
	float GetBlue() { return blue; }
	float GetGreen() { return green; }
	float GetAlpha() { return alpha; }
	float GetShine() { return shine; }

	int GetQuality() { return quality; }

	float GetSize() { return size; }

	Vec3f GetPos() { return MainObject::mPos; }
	Vec3f GetRot() { return MainObject::mRotation; }

	Matrix44f GetTrans() { return mTransform; }

	bool GetSelected() { return selected; }

	virtual void Select() { MainObject::selected = true; }

	void Deselect() { MainObject::selected = false; }

	float GetX() { return mPos.x; }
	float GetY() { return mPos.y; }
	float GetZ() { return mPos.z; }

	float GetRx() { return mRotation.x; }
	float GetRy() { return mRotation.y; }
	float GetRz() { return mRotation.z; }

	void SetShow(bool show) { MainObject::show = show; }

	void SetTexNum(int texnum) { MainObject::texturenumber = texnum; }

	void SetCollidable(bool collidable) { MainObject::collidable = collidable; }

	void SetX(float x) { MainObject::mPos.x = x; }
	void SetY(float y) { MainObject::mPos.y = y; }
	void SetZ(float z) { MainObject::mPos.z = z; }

	void SetRx(float x) { MainObject::mRotation.x = x; }
	void SetRy(float y) { MainObject::mRotation.y = y; }
	void SetRz(float z) { MainObject::mRotation.z = z; }

	void SetQuality(int quality) { MainObject::quality = quality; }

	void SetRed(float red) { MainObject::red = red; }
	void SetBlue(float blue) { MainObject::blue = blue; }
	void SetGreen(float green) { MainObject::green = green; }
	void SetAlpha(float alpha) { MainObject::alpha = alpha; }
	void SetShine(float shine) { MainObject::shine = shine; }

	void SetSize(float size) { MainObject::size = size; }

	bool CheckCollisions(MainObject *otherObject);
};
