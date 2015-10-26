#include "Globals.h"
#include "Resources.h"

#include "MainObject.h"
#include "Sphere.h"
#include "SphereCam.h"

class SphereAgainApp : public AppNative {
public:
	vector <gl::Texture *> textures; //Vector array of texture buffers

	vector <SphereObject *> objects; //Vector of all objects.
	vector <SphereObject *>::iterator itr;
	vector <SphereObject *>::iterator itr2;

	gl::GlslProg		mShader;

	gl::Texture		*mTexture;

	CameraPersp cam;

	SphereCam	mSphereCam;

	Vec2i		mMousePos;

	double mTime;


	void setup();
	void prepareSettings(Settings *settings);
	void loadResources();
	void update();
	void draw();

	void mouseMove(MouseEvent event);
	void mouseDown(MouseEvent event);
	void mouseDrag(MouseEvent event);
	void mouseWheel(MouseEvent event);

	void keyDown(KeyEvent event);

	bool performPicking(Vec3f *pickedPoint, Vec3f *pickedNormal, SphereObject *Object);

	bool CheckCollisions(MainObject *obj1, MainObject *obj2);
};

void SphereAgainApp::setup()
{
	loadResources();

	cam.lookAt(Vec3f(3.0f, 3.0f, 3.0f), Vec3f::zero());
	cam.setPerspective(60.0f, getWindowAspectRatio(), 1.0f, 1000.0f);
	mSphereCam.setCurrentCam(cam);
}

void SphereAgainApp::loadResources() //Load all files here
{
	mShader = gl::GlslProg(loadResource(RES_SHADER_VERT), loadResource(RES_SHADER_FRAG));

	// load the texture
	//  (note: enable mip-mapping by default)
	gl::Texture::Format format;
	format.enableMipmapping(true);

	ImageSourceRef img = loadImage(loadResource(RES_TEXTURE_1));
	if (img) mTexture = new gl::Texture(img, format);
	textures.push_back(mTexture);

	img = loadImage(loadResource(RES_TEXTURE_2));
	if (img) mTexture = new gl::Texture(img, format);
	textures.push_back(mTexture);

	img = loadImage(loadResource(RES_TEXTURE_3));
	if (img) mTexture = new gl::Texture(img, format);
	textures.push_back(mTexture);
}

void SphereAgainApp::update()
{
	// calculate elapsed time
	double elapsed = getElapsedSeconds() - mTime;
	mTime = getElapsedSeconds();

	double frameRate = 1 / elapsed;

	Vec3f camerapos = mSphereCam.GetPosition();

	//Randomly create objects
	//if (objects.size() < 2000) {
	if (frameRate > 25) {
		if (rand() % 10 == 1){
			SphereObject *sphere = new SphereObject((rand() % 1800 - 900) / 10.0f, (rand() % 1800 - 900) / 10.0f, (rand() % 1800 - 900) / 10.0f);
			objects.push_back(sphere);
		}
	}
	//}

	//Updates all objects in list
	for (unsigned int i = 0; i < objects.size(); i++) {
		objects.at(i)->Update(elapsed, &objects, camerapos);
	}

	//Deletes all decayed objects
	for (itr = objects.begin(); itr != objects.end();) {
		if ((*itr)->GetState() == DEAD) {
			(*itr)->Destroy();
			itr = objects.erase(itr);
		}
		else itr++;
	}
	//Orders objects in list based on alpha values - This stops transparencies from bleeding onto solid objects
	sort(objects.begin(), objects.end(), [](MainObject* a, MainObject* b){ return a->GetAlpha() > b->GetAlpha(); });
}

void SphereAgainApp::draw()
{
	if (!mShader)
		return;

	// clear out the window with black
	gl::clear(Color(0, 0, 0));

	// set up the camera 
	gl::pushMatrices();
	gl::setMatrices(mSphereCam.getCamera());

	// enable the depth buffer (after all, we are doing 3D)
	gl::enableDepthRead();
	gl::enableDepthWrite();

	// enable the correct blend mode and set the color (filter)
	gl::enableAlphaBlending();

	mShader.bind();
	
	for (itr = objects.begin(); itr != objects.end(); ++itr) {
		(*itr)->Render(&textures, &mShader);
	}
	/*Vec3f pickedPoint, pickedNormal;
		for (itr = objects.begin(); itr != objects.end(); ++itr) {
			if (performPicking(&pickedPoint, &pickedNormal, (*itr)))
				(*itr)->Select();
		}*/
	mShader.unbind();
	
	gl::disableAlphaBlending();
	gl::popMatrices();

	
}

void SphereAgainApp::mouseMove(MouseEvent event)
{
	// keep track of the mouse
	mMousePos = event.getPos();
}

void SphereAgainApp::mouseDown(MouseEvent event)
{
	Vec3f pickedPoint, pickedNormal;
	if (event.isLeftDown()) {
		for (itr = objects.begin(); itr != objects.end(); ++itr) {
			if (performPicking(&pickedPoint, &pickedNormal, (*itr))) {
				if ((*itr)->GetState() == NORMAL)
					(*itr)->Select();
			}
		}
	}
	/*	select.clear();
		SelectionChecker *selecter = new SelectionChecker(mMousePos, cam);
		select.push_back(selecter);
	}
	else
	// let the camera handle the interaction*/
	else
	mSphereCam.mouseDown(event.getPos());

	
	
}

void SphereAgainApp::mouseDrag(MouseEvent event)
{
	// keep track of the mouse
	mMousePos = event.getPos();

	// let the camera handle the interaction
	mSphereCam.mouseDrag(event.getPos(), event.isRightDown(), event.isMiddleDown());
}

void SphereAgainApp::mouseWheel(MouseEvent event)
{
	mSphereCam.wheelZoom(event.getWheelIncrement());
}

void SphereAgainApp::prepareSettings(Settings *settings)
{
	settings->setWindowSize(1680, 1050);
	//settings->setWindowSize(1920, 1080);
	settings->setFullScreen(false);
	settings->setResizable(true);
	settings->setFrameRate(60.0f);
}

void SphereAgainApp::keyDown(KeyEvent event)
{
	if (event.getChar() == 'f')
		setFullScreen(!isFullScreen());

	if (event.getCode() == 27) //ESC key
		quit();

	if (event.getChar() == 'r') { //Add new object
		SphereObject *sphere = new SphereObject((rand() % 1800 - 900) / 10.0f, (rand() % 1800 - 900) / 10.0f, (rand() % 1800 - 900) / 10.0f);
		objects.push_back(sphere);
	}

	if (event.getChar() == 'c') { //Clear all objects
		for (int killobj = 0; killobj < objects.size(); killobj++) {
			objects.at(killobj)->Destroy();
		}
		objects.clear();
	}

	if (event.getChar() == 'w')
	{
		for (itr = objects.begin(); itr != objects.end(); ++itr) {
			if ((*itr)->GetSelected())
				(*itr)->AddSpeedY(1.0f);
		}
	}

	if (event.getChar() == 's')
	{
		for (itr = objects.begin(); itr != objects.end(); ++itr) {
			if ((*itr)->GetSelected())
				(*itr)->AddSpeedY(-1.0f);
		}
	}

	if (event.getChar() == 'a')
	{
		for (itr = objects.begin(); itr != objects.end(); ++itr) {
			if ((*itr)->GetSelected())
				(*itr)->AddSpeedX(-1.0f);
		}
	}

	if (event.getChar() == 'd')
	{
		for (itr = objects.begin(); itr != objects.end(); ++itr) {
			if ((*itr)->GetSelected())
				(*itr)->AddSpeedX(1.0f);
		}
	}

	if (event.getChar() == 'q')
	{
		for (itr = objects.begin(); itr != objects.end(); ++itr) {
			if ((*itr)->GetSelected())
				(*itr)->AddSpeedZ(-1.0f);
		}
	}

	if (event.getChar() == 'e')
	{
		for (itr = objects.begin(); itr != objects.end(); ++itr) {
			if ((*itr)->GetSelected())
				(*itr)->AddSpeedZ(1.0f);
		}
	}

	if (event.getChar() == 'x')
	{
		for (itr = objects.begin(); itr != objects.end(); ++itr) {
			if ((*itr)->GetSelected())
				(*itr)->SpeedReset();
		}
	}

	if (event.getChar() == 'z')
	{
		for (itr = objects.begin(); itr != objects.end(); ++itr) {
			if ((*itr)->GetSelected())
				(*itr)->Deselect();
		}
	}

	if (event.getChar() == 'h')
	{
		for (itr = objects.begin(); itr != objects.end(); ++itr) {
			if ((*itr)->GetSelected())
				(*itr)->Deselect();
		}
	}


	if (event.getCode() == 276) //Left arrow key
	{
		
	}
	else if (event.getCode() == 275) //Right arrow key
	{
		
	}

	if (event.getCode() == 273) //Up arrow key
	{
		
	}
	else if (event.getCode() == 274) //Down arrow key
	{
		
	}

	if (event.getCode() == 127) //Delete key
	{
		for (itr = objects.begin(); itr != objects.end();) {
			if ((*itr)->GetSelected()) {
				(*itr)->Destroy();
				itr = objects.erase(itr);
			}
			else itr++;
		}
	}
	else if (event.getCode() == 281) //Page down key
	{
		
	}
}

bool SphereAgainApp::performPicking(Vec3f *pickedPoint, Vec3f *pickedNormal, SphereObject *object)
{
	// get our camera 
	CameraPersp cam = mSphereCam.getCamera();

	// generate a ray from the camera into our world
	float u = mMousePos.x / (float)getWindowWidth();
	float v = mMousePos.y / (float)getWindowHeight();
	// because OpenGL and Cinder use a coordinate system
	// where (0, 0) is in the LOWERleft corner, we have to flip the v-coordinate
	Ray ray = cam.generateRay(u, 1.0f - v, cam.getAspectRatio());

	gl::color(Color(1, 1, 0));
	gl::drawVector(ray.getOrigin(), ray.getOrigin() + ray.getDirection());

//	// draw the object space bounding box in yellow
//	gl::color(Color(1, 1, 0));
//	gl::drawStrokedCube(mObjectBounds);

	// the coordinates of the bounding box are in object space, not world space,
	// so if the model was translated, rotated or scaled, the bounding box would not
	// reflect that. 
	//
	// One solution would be to pass the transformation to the calcBoundingBox() function: 
	AxisAlignedBox3f worldBoundsExact = object->calcBoundingBox(object->GetTrans()); // slow

	//if (ray.calcPosition(object->GetZ())

	// draw this transformed box in orange
	//gl::color(Color(1, 0.5, 0));
	//gl::drawStrokedCube(worldBoundsExact);

	// But if you already have an object space bounding box, it's much faster to
	// approximate the world space bounding box like this:
//	AxisAlignedBox3f worldBoundsApprox = mObjectBounds.transformed(mTransform);	// fast

	// draw this transformed box in cyan
//	gl::color(Color(0, 1, 1));
//	gl::drawStrokedCube(worldBoundsApprox);

	// fast detection first - test against the bounding box itself
	if (!worldBoundsExact.intersects(ray))
		return true;
	else return false;

	/*// set initial distance to something far, far away
	float result = 1.0e6f;

	// traverse triangle list and find the picked triangle
	size_t polycount = mMesh.getNumTriangles();
	float distance = 0.0f;
	for (size_t i = 0; i<polycount; ++i)
	{
		Vec3f v0, v1, v2;
		// get a single triangle from the mesh
		mMesh.getTriangleVertices(i, &v0, &v1, &v2);

		// transform triangle to world space
		v0 = mTransform.transformPointAffine(v0);
		v1 = mTransform.transformPointAffine(v1);
		v2 = mTransform.transformPointAffine(v2);

		// test to see if the ray intersects with this triangle
		if (ray.calcTriangleIntersection(v0, v1, v2, &distance)) {
			// set our result to this if its closer than any intersection we've had so far
			if (distance < result) {
				result = distance;
				// assuming this is the closest triangle, we'll set our normal
				// while we've got all the points handy
				*pickedNormal = (v1 - v0).cross(v2 - v0).normalized();
			}
		}
	}

	// did we have a hit?
	if (distance > 0) {
		*pickedPoint = ray.calcPosition(result);
		return true;
	}
	else 
		return false; */
}

bool SphereAgainApp::CheckCollisions(MainObject *obj1, MainObject *obj2)
{
	Vec3f distance = obj1->GetPos() - obj2->GetPos();

	float length = distance.length();

	// Sum of the radiuses
	float sumradius = obj1->GetSize() + obj2->GetSize();

	if (length <= sumradius)
	{
		return true;
	}

	return false;
}

CINDER_APP_NATIVE(SphereAgainApp, RendererGl)
