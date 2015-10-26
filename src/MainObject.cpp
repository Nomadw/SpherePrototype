#include "Globals.h"

#include "MainObject.h"

MainObject::MainObject()
{
	show = true;
	collidable = true;
	selected = false;

	red = 0.2f;
	blue = 0.2f;
	green = 0.2f;

	alpha = 1.0f;

	shine = 100.0f;

	size = 1.0f;

	age = 0.0;

	texturenumber = rand() % 3;

	quality = 15;

	mPos = Vec3f::zero();
}

void MainObject::Render()
{

}

void MainObject::Update()
{
	mTransform.setToIdentity();
	mTransform.setTranslate(mPos);
}

void MainObject::Update(double elapsed)
{
	age += elapsed;
	mTransform.setToIdentity();
	mTransform.setTranslate(mPos);
}


void MainObject::Destroy()
{
	selected = false;
}

void MainObject::Render(vector <gl::Texture *> *textures, gl::GlslProg *shader)
{
if (CheckShow()) //Does not render if object's show = false
	{
		textures->at(texturenumber)->bind();

		shader->uniform("tex0", 0);
		shader->uniform("alpha", alpha);
		shader->uniform("red", red);
		shader->uniform("blue", blue);
		shader->uniform("green", green);
		shader->uniform("shine", shine);
		
		gl::pushModelView();
		gl::multModelView(mTransform);
		gl::drawSphere(Vec3f::zero(), size, quality);
		gl::popModelView();
		
		textures->at(texturenumber)->unbind();
	}
}

bool MainObject::CheckCollisions(MainObject *otherObject)
{
	Vec3f distance = mPos - otherObject->mPos;

	float length = distance.length();

	// Sum of the radiuses
	float sumradius = size + otherObject->size;

	if (length <= sumradius)
	{
		return true;
	}

	return false;
}