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
    
    //! help text
    gl::TextureFontRef	mTextureFont;

    Font				mFont;

	CameraPersp cam;

	SphereCam	mSphereCam;

	Vec2i		mMousePos;

	double mTime;
    
    params::InterfaceGlRef	mParams; //Testing parameter interface
    
    ProgramSettings program;
    
    double framerate;

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
    
    void renderDataToTexture();
    
    void ToggleSpheres() {
        if(program.drawsphere)
            program.drawsphere = false;
        else program.drawsphere = true;
    }
    void ToggleData() {
        if(program.renderdata)
            program.renderdata = false;
        else program.renderdata = true;
    }
    void ForceSpawn()
    {
        for(int i = 0; i < program.SpawnRate; i++) {
            SphereObject *sphere = new SphereObject((rand() % BORDER - (BORDER / 2)) / 10.0f, (rand() % BORDER - (BORDER / 2)) / 10.0f, (rand() % BORDER - (BORDER / 2)) / 10.0f);
            objects.push_back(sphere);
        }
    }
    
    void setupParameter();
    
    void fullreset();
    
    void GoToNearest(int state);
    
    void GoToNearestSpawn() {GoToNearest(CREATION_OSCILLATION);}
    void GoToNearestDecay() {GoToNearest(DECAY);}
};

void SphereAgainApp::setup()
{
	loadResources();
    
    mFont = Font( "PTMono-Bold", 36 );

    mTextureFont = gl::TextureFont::create( mFont );
    
    setupParameter();
    
	cam.lookAt(Vec3f(3.0f, 3.0f, 3.0f), Vec3f::zero());
	cam.setPerspective(60.0f, getWindowAspectRatio(), 1.0f, 1000.0f);
	mSphereCam.setCurrentCam(cam);
    cam.setAspectRatio( getWindowAspectRatio() );
    
    for( int i = 1; i < INITOBJECTS; i++) {
        SphereObject *sphere = new SphereObject((rand() % BORDER - (BORDER / 2)) / 10.0f, (rand() % BORDER - (BORDER / 2)) / 10.0f, (rand() % BORDER - (BORDER / 2)) / 10.0f);
        objects.push_back(sphere);
    }
    renderDataToTexture();
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

	framerate = 1 / elapsed;

	program.camerapos = mSphereCam.GetPosition();

	//Randomly create objects
	//if (objects.size() < 2000) {
	if (floor(getAverageFps()) > program.MINFPS) {
		//if (rand() % 5 == 1){
        for(int i = 0; i < program.SpawnRate; i++) {
			SphereObject *sphere = new SphereObject((rand() % BORDER - (BORDER / 2)) / 10.0f, (rand() % BORDER - (BORDER / 2)) / 10.0f, (rand() % BORDER - (BORDER / 2)) / 10.0f);
			objects.push_back(sphere);
        }
		//}
	}
	//}

    elapsed *= program.ProgramSpeed; //Speeds altered by user editing program speed
    
	//Updates all objects in list
	for (unsigned int i = 0; i < objects.size(); i++) {
		objects.at(i)->Update(elapsed, &objects, program.camerapos, program);
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
    
    program.numberofobjects = objects.size();
    program.framerate = floor(getAverageFps());
}

void SphereAgainApp::draw()
{
    gl::setMatricesWindow( getWindowSize() );
    
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
		(*itr)->Render(&textures, &mShader, program);
	}
	/*Vec3f pickedPoint, pickedNormal;
		for (itr = objects.begin(); itr != objects.end(); ++itr) {
			if (performPicking(&pickedPoint, &pickedNormal, (*itr)))
				(*itr)->Select();
		}*/
	mShader.unbind();
	
	
	gl::popMatrices();

    // render data
    if(program.renderdata && mTextureFont) {
        renderDataToTexture();
    }
    gl::disableAlphaBlending();
    
    // Draw the interface
    mParams->draw();
}

void SphereAgainApp::mouseMove(MouseEvent event)
{
	// keep track of the mouse
	mMousePos = event.getPos();
}

void SphereAgainApp::mouseDown(MouseEvent event)
{
	//Vec3f pickedPoint, pickedNormal;
	//if (event.isLeftDown()) {
	//	for (itr = objects.begin(); itr != objects.end(); ++itr) {
	//		if (performPicking(&pickedPoint, &pickedNormal, (*itr))) {
	//			if ((*itr)->GetState() == NORMAL)
	//				(*itr)->Select();
	//		}
	//	}
	//}
	/*	select.clear();
		SelectionChecker *selecter = new SelectionChecker(mMousePos, cam);
		select.push_back(selecter);
	}
	else
	// let the camera handle the interaction*/
	//else
	mSphereCam.mouseDown(event.getPos());

	
	
}

void SphereAgainApp::mouseDrag(MouseEvent event)
{
	// keep track of the mouse
	mMousePos = event.getPos();

	// let the camera handle the interaction
	mSphereCam.mouseDrag(event.getPos(), event.isRightDown(), event.isControlDown(), event.isLeftDown(), event.isShiftDown());
}

void SphereAgainApp::mouseWheel(MouseEvent event)
{
	mSphereCam.wheelZoom(event.getWheelIncrement());
}

void SphereAgainApp::prepareSettings(Settings *settings)
{
	//settings->setWindowSize(1680, 1050);
	settings->setWindowSize(1920, 1080);
	settings->setFullScreen(true);
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
        SphereObject *sphere = new SphereObject((rand() % BORDER - (BORDER / 2)) / 10.0f, (rand() % BORDER - (BORDER / 2)) / 10.0f, (rand() % BORDER - (BORDER / 2)) / 10.0f);
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
		if(program.renderdata)
            program.renderdata = false;
        else program.renderdata = true;
	}

	if (event.getChar() == 'y')
	{
        GoToNearest(DECAY);
	}
    
    if (event.getChar() == 't')
    {
        GoToNearest(CREATION_OSCILLATION);
    }

	if (event.getChar() == 'a')
	{
        if(program.drawsphere)
            program.drawsphere = false;
        else program.drawsphere = true;
	}

	if (event.getChar() == 'd')
	{

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
        fullreset();
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
        if(program.SpawnRate > 0)
            program.SpawnRate -= 1;
        else program.SpawnRate = 0;
	}
	else if (event.getCode() == 275) //Right arrow key
	{
        if(program.SpawnRate < MAXSPAWNRATE)
            program.SpawnRate += 1;
        else program.SpawnRate = MAXSPAWNRATE;
		
	}

	if (event.getCode() == 273) //Up arrow key
	{
		
	}
	else if (event.getCode() == 274) //Down arrow key
	{
		
	}

	if (event.getCode() == 281) //Page up key
	{
        //Slows program down!
        if(program.ProgramSpeed > 0.1f)
            program.ProgramSpeed -= 0.1f;
        else program.ProgramSpeed = 0.1f;
		
	}
	else if (event.getCode() == 280) //Page down key
	{
        //Speeds program up!
        if(program.ProgramSpeed < MAXPROGRAMSPEED)
            program.ProgramSpeed += 0.1f;
        else program.ProgramSpeed = MAXPROGRAMSPEED;
	}
    
    if(event.getCode() == 127) //Delete key
    {
        //Resets program speed
        program.ProgramSpeed = 1.0f;
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

void SphereAgainApp::renderDataToTexture()
{
    // Draw FPS
    gl::color( Color::white() );
    mTextureFont->drawString("FPS :" + toString( floor(getAverageFps())) , Vec2f( 50, getWindowHeight() - mTextureFont->getDescent() ) );
    
    //Draw Object Number
    mTextureFont->drawString("Objects :" + toString( objects.size()) , Vec2f( 50, getWindowHeight() - (mTextureFont->getDescent() + 50)) );
}

void SphereAgainApp::setupParameter()
{
    // Create the interface and give it a name.
    mParams = params::InterfaceGl::create( getWindow(), "Sphere Prototype", toPixels( Vec2i( 300, 750 ) ) );
    
    mParams->addParam("Particles", &program.numberofobjects).step(0);
    
    mParams->addParam("FPS", &program.framerate).step(0);
    
    mParams->addText("Position:");
    mParams->addParam("X:", &program.camerapos.x).step(0);
    mParams->addParam("Y:", &program.camerapos.y).step(0);
    mParams->addParam("Z:", &program.camerapos.z).step(0);
    
    mParams->addSeparator();
    
    mParams->addText("Peformance options:");
    
    mParams->addParam("Draw Distance", &program.MAXRENDERDISTANCE).min(10).max(2500).step(1);
    
    mParams->addParam("Minimum FPS spawn", &program.MINFPS).min(1).max(60).step(1);
    
    mParams->addButton("Reset all", std::bind(&SphereAgainApp::fullreset, this));
    
    mParams->addSeparator();
    mParams->addText("Application options:");
    mParams->addParam("Program Speed", &program.ProgramSpeed).min( 0.1f ).max( MAXPROGRAMSPEED ).step( 0.1f );
    mParams->addParam("Spawn Rate", &program.SpawnRate).min(0).max(MAXSPAWNRATE).step(1);
    mParams->addButton("Display Spheres/circles", std::bind( &SphereAgainApp::ToggleSpheres, this));
    mParams->addButton("Display data", std::bind(&SphereAgainApp::ToggleData, this));
    
    mParams->addSeparator();
    mParams->addText("Go to nearest: ");
    mParams->addButton("Spawning particle", std::bind(&SphereAgainApp::GoToNearestSpawn, this));
    mParams->addButton("Decaying particle", std::bind(&SphereAgainApp::GoToNearestDecay, this));
    
    mParams->addSeparator();
    mParams->addText("Keyboard shortcuts:");
    mParams->addText(" ");
    mParams->addText("Inc/Dec Speed: PgUp / PgDown");
    mParams->addText("Reset Speed: Del");
    mParams->addText("Clear all particles: C");
    mParams->addText("Inc/Dec Spawn Rate: Left/Right");
    mParams->addText("Toggle Spheres/Circles: A");
    mParams->addText("Force spawn: R");
    mParams->addText("    ");
    mParams->addText("Go to nearest spawn: T");
    mParams->addText("Go to nearest decay: Y");
    mParams->addText("  ");
    mParams->addText("Full Reset: E");
    mParams->addText("   ");
    mParams->addText("Toggle data display: W");
    mParams->addText("Toggle fullscreen: F");
    mParams->addText("Quit application: Esc");
    
    
    mParams->addSeparator();
    mParams->addText("Left click+drag to pan camera");
    mParams->addText("Right click+drag to zoom camera");
    mParams->addText("(Alt: SHIFT+click+drag)");
    mParams->addText("CTRL+click+drag to rotate camera");
}

void SphereAgainApp::fullreset()
{
    program.MINFPS = 25;
    program.MAXRENDERDISTANCE = 500;
    program.drawsphere = true;
    program.ProgramSpeed = 1.0f;
    program.renderdata = false;
    for (int killobj = 0; killobj < objects.size(); killobj++) {
        objects.at(killobj)->Destroy();
    }
    objects.clear();
    for( int i = 1; i < INITOBJECTS; i++) {
        SphereObject *sphere = new SphereObject((rand() % BORDER - (BORDER / 2)) / 10.0f, (rand() % BORDER - (BORDER / 2)) / 10.0f, (rand() % BORDER - (BORDER / 2)) / 10.0f);
        objects.push_back(sphere);
    }
    mSphereCam.setPos(Vec3f(3.0f,3.0f,3.0f));
}

void SphereAgainApp::GoToNearest(int state)
{
    if(!state)
        state = DECAY;
    
    Vec3f distance;
    Vec3f shortestdistance = Vec3f(0.0f, 0.0f, 0.0f);
    
    for(int i = 0; i < objects.size(); i++) {
        if(objects.at(i)->GetState() == state) //Checks if object is in desired state
        {
            distance = program.camerapos - objects.at(i)->GetPos();
            if(shortestdistance == Vec3f(0.0f, 0.0f, 0.0f))
            {
                shortestdistance = objects.at(i)->GetPos();
            }
            else if(distance.length() < shortestdistance.length())
            {
                shortestdistance = objects.at(i)->GetPos();
            }
        }
    }
    if(shortestdistance != Vec3f(0.0f, 0.0f, 0.0f))
    {
        //NewPos.getCamera();
        //NewPos.setCenterOfInterest(0.0f);
        
        //cam = NewPos;
        mSphereCam.setPos(shortestdistance);
    }
    
}

CINDER_APP_NATIVE(SphereAgainApp, RendererGl)
