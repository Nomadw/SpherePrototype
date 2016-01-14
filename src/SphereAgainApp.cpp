#include "Globals.h"
#include "Resources.h"

#include "MainObject.h"
#include "Sphere.h"
#include "SphereCam.h"


//Application is created as a class - Inherits from Cinder's AppNative application class and contains public functions / variables
class SphereAgainApp : public AppNative {
public:
	vector <gl::Texture *> textures; //Vector array of texture buffers

	vector <SphereObject *> objects; //Vector of all objects.
	vector <SphereObject *>::iterator itr; //Iterator for vector of objects

	gl::GlslProg		mShader; //Shader file to load in

	gl::Texture		*mTexture; //Texture file(s) to load in
    
    //! help text
    gl::TextureFontRef	mTextureFont;

    Font				mFont; //Font file to load in

	CameraPersp cam; //Camera (Cinder) to use

	SphereCam	mSphereCam; //Overlay for camera (Controls, etc)

	Vec2i		mMousePos; //X and Y position vector for mouse(pointer) on screen

	double mTime; //Time variable - Used in delta calculations, framerate etc
    
    params::InterfaceGlRef	mParams; //Creates parameter interface (Cinder library)
    
    ProgramSettings program; //Loading in settings structure created in globals.h
    
    double framerate; //Framerate variable - Used to display framerate-based caluclations

	//Base functions
	void setup();
	void prepareSettings(Settings *settings);
	void loadResources();
	void update();
	void draw();

	//Mouse usage
	void mouseMove(MouseEvent event);
	void mouseDown(MouseEvent event);
	void mouseDrag(MouseEvent event);
	//void mouseWheel(MouseEvent event);

	//Keyboard usage
	void keyDown(KeyEvent event);

	//Collisions function - Unused (commented) - See MainObject for used collisions
	//bool CheckCollisions(MainObject *obj1, MainObject *obj2);
    
	//Data render function - Draws 
    void renderDataToTexture();
    
	//Toggle functions - Used for interface buttons
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

	void ToggleCollisions() {
		if (program.collision)
			program.collision = false;
		else program.collision = true;
	}

	void TogglePause() {
		if (program.pause)
			program.pause = false;
		else program.pause = true;
	}

    void ForceSpawn()
    {
        for(int i = 0; i < program.SpawnRate; i++) { //Will create an amount of objects based on current spawn rate
            SphereObject *sphere = new SphereObject(program);
            objects.push_back(sphere);
        }
    }
    
	//Creates interface parameters
    void setupParameter();
    
    void fullreset();

    //Moves to nearest object of supplied state (e.g. GoToNearest(CREATION_FAST) )
    void GoToNearest(int state);
    
	//Pre-made function for interface buttons
    void GoToNearestSpawn() {GoToNearest(CREATION_OSCILLATION);}
    void GoToNearestDecay() {GoToNearest(DECAY);}
};


//All main functions for application

//Set up application
void SphereAgainApp::setup()
{
	loadResources(); //Load in files (textures / shaders)
    
    mFont = Font( "PTMono-Bold", 36 ); //Font used - Can replace with any font / size
    mTextureFont = gl::TextureFont::create( mFont ); //Creates textured font from selected font
    
    setupParameter(); //Initially sets up interface
    
	//Setting up camera position, parameters and setting aspect ratio to window size
	cam.lookAt(INIT_CAMERA_POS, Vec3f::zero());
	cam.setPerspective(45.0f, getWindowAspectRatio(), 1.0f, 2500.0f);
	mSphereCam.setCurrentCam(cam);
    cam.setAspectRatio( getWindowAspectRatio() );
    
	//Automatically creates starting number of objects - If any set in globals.h
    for( int i = 1; i < INITOBJECTS; i++) {
        SphereObject *sphere = new SphereObject(program);
        objects.push_back(sphere);
    }

	//Initially draws info to texture (data panel)
    renderDataToTexture();
}

void SphereAgainApp::loadResources() //Load all files here
{
	//Load the shader
	mShader = gl::GlslProg(loadResource(RES_SHADER_VERT), loadResource(RES_SHADER_FRAG));

	//Load the textures
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

	//Calculates framerate
	framerate = 1 / elapsed;

	program.camerapos = mSphereCam.GetPosition();

	elapsed *= program.ProgramSpeed; //Speeds altered by user editing program speed

	//Updates all objects in list
	for (unsigned int i = 0; i < objects.size(); i++) {
		objects.at(i)->Update(elapsed, &objects, program.camerapos, program);
	}

	if (!program.pause) { //If program is paused, skip all of this!

		//Spawns new objects within the field, if spawn rate is above zero and current FPS is more than minimum FPS set to spawn
		if (floor(getAverageFps()) > program.MINFPS) {
			for (int i = 0; i < program.SpawnRate; i++) {
				SphereObject *sphere = new SphereObject(program);
				objects.push_back(sphere);
			}

		}

		//Collision detection - WILL SLOW DOWN APPLICATION. Turn off for increased performance
		if (program.collision) { //Will only perform if collision is turned on
			for (unsigned int i = 0; i < objects.size(); i++)
			{
				for (unsigned int ii = 0; ii < objects.size(); ii++)
				{
					if (!objects.at(i)->GetCollidable()) continue; //If either object is not collidable, skip this
					if (!objects.at(ii)->GetCollidable()) continue;
					if (objects.at(i)->CheckCollisions(objects.at(ii))) {
						objects.at(i)->Collided(objects.at(ii)); //Calls both object's collision code
						objects.at(ii)->Collided(objects.at(i));
					}
				}
			}
		}

		//Deletes all decayed objects
		for (itr = objects.begin(); itr != objects.end();) {
			if ((*itr)->GetState() == DEAD) {
				//(*itr)->Destroy(); //Unused - See MainObject
				itr = objects.erase(itr); //Delete from memory + vector
			}
			else itr++; //Check next object
		}

	}
	//Orders objects in list based on alpha values - This stops transparencies from bleeding onto solid objects
	sort(objects.begin(), objects.end(), [](MainObject* a, MainObject* b){ return a->GetAlpha() > b->GetAlpha(); });
    
	//Updates program's display values (Number of objects - framerate)
    program.numberofobjects = objects.size();
    program.framerate = floor(getAverageFps());
}

void SphereAgainApp::draw()
{
    gl::setMatricesWindow( getWindowSize() );
    
	if (!mShader)
		return; //Does not draw if shader not loaded!

	// Keep background black
	gl::clear(Color(0, 0, 0));

	// set up the camera 
	gl::pushMatrices();
	gl::setMatrices(mSphereCam.getCamera());

	// enable the depth buffer (after all, we are doing 3D)
	gl::enableDepthRead();
	gl::enableDepthWrite();

	// enable the correct blend mode and set the color (filter)
	gl::enableAlphaBlending();

	//Binding shader for all rendering
	mShader.bind();
	
	//Renders all objects in scene (Using their own draw code) - This is expensive, could be improved via instancing
	for (itr = objects.begin(); itr != objects.end(); ++itr) {
		(*itr)->Render(&textures, &mShader, program);
	}
	
	//Ubnbinding shader after all rendering
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
	//Gives information to the SphereCam class for camera movement
	mSphereCam.mouseDown(event.getPos());
}

void SphereAgainApp::mouseDrag(MouseEvent event)
{
	// keep track of the mouse
	mMousePos = event.getPos();

	// let the camera handle the interaction
	mSphereCam.mouseDrag(event.getPos(), event.isRightDown(), event.isControlDown(), event.isLeftDown(), event.isShiftDown());
}

/*void SphereAgainApp::mouseWheel(MouseEvent event)
{
	mSphereCam.wheelZoom(event.getWheelIncrement());
}*/

void SphereAgainApp::prepareSettings(Settings *settings)
{
    DisplayRef display = Display::getMainDisplay();
    Vec2i displaySize = display->getSize();
	settings->setWindowSize(displaySize);
	settings->setFullScreen(true);
	settings->setResizable(true);
	settings->setFrameRate(60.0f);
}

//Keyboard input
void SphereAgainApp::keyDown(KeyEvent event)
{
	if (event.getChar() == 'f') //Toggle fullscreen
		setFullScreen(!isFullScreen());

	if (event.getCode() == 27) //ESC key
		quit();

	if (event.getChar() == 'r') { //Add new object
        ForceSpawn();
	}

	if (event.getChar() == 'c') { //Clear all objects
		//for (int killobj = 0; killobj < objects.size(); killobj++) {
		//	objects.at(killobj)->Destroy(); //Unused - See MainObject
		//}
		objects.clear(); // Clears vector + memory
	}

	if (event.getChar() == 'w') //Turns data display on/off
	{
		ToggleData();
	}

	if (event.getChar() == 'y') //Move to nearest decaying object - Will need to zoom out
	{
        GoToNearest(DECAY);
	}
    
    if (event.getChar() == 't') //Move to nearest spawning object - Will need to to zoom out
    {
        GoToNearest(CREATION_OSCILLATION);
    }

	if (event.getChar() == 'a') //Toggles between drawing spheres and circles - Circles do not billboard!
	{
		ToggleSpheres();
	}

	if (event.getChar() == 'd')
	{

	}

	if (event.getChar() == 'p')
	{
		TogglePause();
	}

	if (event.getChar() == 'q')
	{
        
	}

	if (event.getChar() == 'e') //Resets all - To settings as seen in globals.h
	{
        fullreset();
	}

	if (event.getChar() == 'x')
	{
        
	}

	if (event.getChar() == 'z')
	{
        
	}

	if (event.getChar() == 'h')
	{
        
	}


	if (event.getCode() == 276) //Left arrow key
	{
        //Decreases spawn rate
        if(program.SpawnRate > 0)
            program.SpawnRate -= 1;
        else program.SpawnRate = 0;
	}
	else if (event.getCode() == 275) //Right arrow key
	{
        //Increases spawn rate
        if(program.SpawnRate < MAXSPAWNRATE)
            program.SpawnRate += 1;
        else program.SpawnRate = MAXSPAWNRATE;
		
	}

	if (event.getCode() == 273) //Up arrow key
	{
        //Speeds program up!
        if(program.ProgramSpeed < MAXPROGRAMSPEED)
        program.ProgramSpeed += 0.1f;
        else program.ProgramSpeed = MAXPROGRAMSPEED;
	}
	else if (event.getCode() == 274) //Down arrow key
	{
        //Slows program down!
        if(program.ProgramSpeed > 0.1f)
        program.ProgramSpeed -= 0.1f;
        else program.ProgramSpeed = 0.1f;
	}

	if (event.getCode() == 281) //Page up key
	{
       
		
	}
	else if (event.getCode() == 280) //Page down key
	{
        
	}
    
    if(event.getCode() == 127) //Delete key
    {
        //Resets program speed
        program.ProgramSpeed = 1.0f;
    }

	//For all other keycodes see Cinder documentation (KeyEvent)
}

/*bool SphereAgainApp::CheckCollisions(MainObject *obj1, MainObject *obj2)
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
}*/

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
    // Create the interface and give it a name
    mParams = params::InterfaceGl::create( getWindow(), "Sphere Prototype", toPixels( Vec2i( 300, 750 ) ) );
    
    mParams->addParam("Particles", &program.numberofobjects).step(0); //Shows number of particles on screen (Non-changeable)
    
    mParams->addParam("FPS", &program.framerate).step(0); //Shows FPS (Non changeable)
    
    mParams->addText("Position:"); //Displays current X, Y and Z position of camera
    mParams->addParam("X:", &program.camerapos.x).step(0);
    mParams->addParam("Y:", &program.camerapos.y).step(0);
    mParams->addParam("Z:", &program.camerapos.z).step(0);
    
    mParams->addSeparator(); //Creates a seperating bar on the interface
    
    mParams->addText("Peformance options:");
    
    mParams->addParam("Draw Distance", &program.MAXRENDERDISTANCE).min(10).max(2500).step(1); //Objects after set distance will NOT render - Ideal for lesser-powered machines
    
    mParams->addParam("Minimum FPS spawn", &program.MINFPS).min(1).max(60).step(1); //Objects will not spawn if FPS is below this amount

	mParams->addButton("Toggle collisisons", std::bind(&SphereAgainApp::ToggleCollisions, this)); //Turns collisions on/off

    mParams->addButton("Reset all", std::bind(&SphereAgainApp::fullreset, this)); //Resets all parameters to default
    
    mParams->addSeparator();
    mParams->addText("Application options:");
    mParams->addParam("Program Speed", &program.ProgramSpeed).min( 0.0f ).max( MAXPROGRAMSPEED ).step( 0.1f );
    mParams->addParam("Spawn Rate", &program.SpawnRate).min(0).max(MAXSPAWNRATE).step(1);
	mParams->addParam("% chance of collision", &program.PERCENTCOLLISION).min(0).max(MAX_COLLISION_PERCENT).step(1);
    mParams->addButton("Display Spheres/circles", std::bind( &SphereAgainApp::ToggleSpheres, this));
    mParams->addButton("Display data", std::bind(&SphereAgainApp::ToggleData, this));
    
    mParams->addSeparator();
    mParams->addText("Go to nearest: ");
    mParams->addButton("Spawning particle", std::bind(&SphereAgainApp::GoToNearestSpawn, this));
    mParams->addButton("Decaying particle", std::bind(&SphereAgainApp::GoToNearestDecay, this));
    
	//Display keyboard shortcuts/controls in text. One command per line
    mParams->addSeparator();
    mParams->addText("Keyboard shortcuts:");
    mParams->addText(" "); //Makes a space!
    mParams->addText("Inc/Dec Speed: Up / Down");
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
    mParams->addText("Pause: P");
    mParams->addText("Toggle data display: W");
    mParams->addText("Toggle fullscreen: F");
    mParams->addText("Quit application: Esc");
    
    
    mParams->addSeparator();
    mParams->addText("Left click+drag to pan camera");
    mParams->addText("Right click+drag to zoom camera");
    mParams->addText("(Alt: SHIFT+click+drag)");
    mParams->addText("CTRL+click+drag to rotate camera");
}

void SphereAgainApp::fullreset() //Resets all parameters to those set in globals.h
{
    program.MINFPS = INIT_MIN_FPS;
    program.MAXRENDERDISTANCE = INIT_MAX_RENDER_DISTANCE;
    program.drawsphere = INIT_DRAW_SPHERE;
    program.ProgramSpeed = INIT_PROG_SPEED;
    program.renderdata = INIT_RENDER_DATA;
    program.SpawnRate = INIT_SPAWN_RATE;
    program.PERCENTCOLLISION = INIT_COLLISION_PERCENT;
	program.collision = INIT_COLLISION_ENABLED;
    //for (int killobj = 0; killobj < objects.size(); killobj++) { //Clears all current objects - Unused - See Mainobject
    //    objects.at(killobj)->Destroy();
    //}
    objects.clear(); //Clears vector and memory
    mSphereCam.setPos(INIT_CAMERA_POS); //Returns camera
}

void SphereAgainApp::GoToNearest(int state)
{
    if(!state) //If no state is given, check for DECAY state
        state = DECAY;
    
    Vec3f distance; //Current object's distance
    Vec3f shortestdistance = Vec3f(0.0f, 0.0f, 0.0f); //Shortest distance of current object
    
    for(int i = 0; i < objects.size(); i++) {
        if(objects.at(i)->GetState() == state) //Checks if object is in desired state
        {
            distance = program.camerapos - objects.at(i)->GetPos(); //Checks distance from camera
            if(shortestdistance == Vec3f(0.0f, 0.0f, 0.0f))
            {
                shortestdistance = objects.at(i)->GetPos(); //If shortest distance is nill, make current object closest
            }
            else if(distance.length() < shortestdistance.length()) 
            {
                shortestdistance = objects.at(i)->GetPos(); //If current object is closest, replace previous object
            }
        }
    }
    if(shortestdistance != Vec3f(0.0f, 0.0f, 0.0f)) //Do not move camera to 0 if no objects of state were found!
    {
        mSphereCam.setPos(shortestdistance); //Moves camera to the object that was found closest
    }
    
}

//Open the application - Uses program class and OpenGL
CINDER_APP_NATIVE(SphereAgainApp, RendererGl)
