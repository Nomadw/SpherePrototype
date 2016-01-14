#include "Globals.h"

#include "MainObject.h" //Class variables and functions declared in MainObject.h

//All MainObject functions defined here

//Constructor - All other contructors based off this one
MainObject::MainObject()
{
	//Sets all base values

	show = true;
	collidable = true;
	//selected = false;

	red = 0.2f;
	blue = 0.2f;
	green = 0.2f;

	alpha = 1.0f;

	//shine = 100.0f;

	size = 1.0f;

	age = 0.0;

	texturenumber = rand() % 3;

	quality = 15;

	mPos = Vec3f::zero();
}

//Base render function - Nothing to render
void MainObject::Render()
{

}


//Base update function
void MainObject::Update()
{
	mTransform.setToIdentity();
	mTransform.setTranslate(mPos);
}

//Update function, using delta time
void MainObject::Update(double elapsed, ProgramSettings settings)
{
	age += elapsed;
	mTransform.setToIdentity();
	mTransform.setTranslate(mPos);
}

//Base destroy function - Currently unused, left in (commented) for future development
//void MainObject::Destroy()
//{
	//selected = false;
//}

//Render function - Used in all current objects (No overriding Render function in Sphere
void MainObject::Render(vector <gl::Texture *> *textures, gl::GlslProg *shader, ProgramSettings settings)
{
if (CheckShow()) //Does not render if object's show = false
	{
		//Binds texture based on texture number of object
		textures->at(texturenumber)->bind();
        
		//Passing all object information to shader for render
		shader->uniform("tex0", 0);
		shader->uniform("alpha", alpha);
		shader->uniform("red", red);
		shader->uniform("blue", blue);
		shader->uniform("green", green);
		//shader->uniform("shine", shine);
		
		gl::pushModelView();
		gl::multModelView(mTransform); //Places object in scene based on 4x4 matrix used in update function
        
        //If Spheres on, draw textured sphere. If not, set circle size and draw
        if(settings.drawsphere)
            gl::drawSphere(Vec3f::zero(), size, quality);
        else if(size < 1.0f) {
            gl::drawSolidCircle(Vec2f::zero(), size * 2);
        }
        else {
            gl::drawSolidCircle(Vec2f::zero(), size * 1.25);
        }
		gl::popModelView();
		
		textures->at(texturenumber)->unbind(); //Unbinds texture for next object render
	}
}

//Object collision check
bool MainObject::CheckCollisions(MainObject *otherObject)
{
	Vec3f distance = mPos - otherObject->mPos; //Gets distance by subtracting vector of one positon from the other

	float length = distance.length(); //Calculates single value from vector

	// Sum of the radiuses
	float sumradius = size + otherObject->size; //Gets size of both radii

	if (length <= sumradius) //Objects must be touching (colliding) if distance is less than both radii
	{
		return true;
	}

	return false;
}
