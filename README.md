////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//													      //
//						 Sphere Prototype					      //
//													      //
//						Damon Wareham 2015-16					      //
//						   							      //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

1. About

2. Setup for running
i) Windows
ii) Mac

3. Setup for building / creating new applications
i) Windows
ii) Mac
iii) Alternate - Pre-built

4. Features

5. Things to consider

6. Special thanks

-- 1 -- About --------------------------------------------------------------------------------------------------

This project is built to build scientific particle simulations with, created in C++ and making use of Cinder 0.8.6 for graphics, geometry and application building.
This was created for the Entropy project, although anyone is free to use given proper credit.
Rendering is done in OpenGL and uses GLSL version 110 shaders.

The project will work on Windows and Mac, source code will be 100% portable between each platform (Using the correct Cinder libraries / projects are set up!)

If you have any questions or comments, or need help with working on the code, feel free to contact me on damon_wareham@yahoo.com

-- 2 -- Setup for running --------------------------------------------------------------------------------------

i ) Windows: ----------------

-Download .exe here - https://www.dropbox.com/s/dcfzxax1mnp0dvh/SphereAgain.exe?dl=0

-Run .exe

ii ) Mac: -------------------

-Download .zip file here - https://www.dropbox.com/s/ga0eil4o9jhtx69/SphereAgain.app.zip?dl=0

-Extract .app file

-Run .app


-- 3 -- Setup for building -------------------------------------------------------------------------------------

i ) Windows (REQUIRES VISUAL STUDIO 2013 OR 2015) -------------

-Download Cinder 0.8.6 libraries here for Visual C++ 2013 - https://libcinder.org/download (See previous releases)

-Extract .zip folder where you will create your project

-Within the Cinder 0.8.6 folder, run TinderBox (tools\TinderBox-Win\TinderBox)

-Name project as you wish, make sure VC 2013 is selected and have project built in location directly outside Cinder 0.8.6 folder
 (For example: cinder_0.8.6_vc2013/
               ProjectNameHere/ )

-Download all GitHub source code

-Place downloaded source code files into same project folders (all include\ files into include\, etc). Replace resources.h and resources.rc with downloaded files

-Provided source code will have the main application named "SphereAgainApp.cpp". Replace created project file (For example ProjectNameHereApp.cpp) with this

-Open created vc2013\ project file in Visual Studio

-Include src\ files into source files, include\ files into include files, etc

ii ) Mac (REQUIRES XCODE) --------------------

-Download Cinder 0.8.6 libraries here for OS X - https://libcinder.org/download (See previous releases)

-Extract .zip folder where you will create your project

-Within the Cinder 0.8.6 folder, run TinderBox (tools\TinderBox-Mac\TinderBox)

-Name project as you wish, make sure Xcode is selected and have project built in location directly outside Cinder 0.8.6 folder
 (For example: cinder_0.8.6_mac/
               ProjectNameHere/ )

-Download all GitHub source code - vc2013 can be ignored

-Place downloaded source code files into same project folders (all include\ files into include\, etc). Replace resources.h with downloaded files

-Provided source code will have the main application named "SphereAgainApp.cpp". Replace created project file (For example ProjectNameHereApp.cpp) with this

-Open created xcode\ project file in xcode

-Include src\ files into source files, include\ files into include files, etc


iii ) Alternate - Pre-built - (REQUIRES VISUAL STUDIO 2013/2015 FOR WINDOWS, XCODE FOR MAC) -----------

-Download Cinder 0.8.6 libraries here for either Visual C++ 2013 or Mac OSX - https://libcinder.org/download (See previous releases)

-Extract .zip folder

-Download already-made project (For Xcode and VC2013) here - https://www.dropbox.com/s/w7yj2wkw8fscayb/SphereAgaincomment.zip?dl=0

-Extract .zip, place in location directly outside Cinder 0.8.6 folder
 (For example: cinder_0.8.6_vc2013/ - cinder_0.8.6_mac/
               ProjectNameHere/ )

-Open vc2013\ project file in Visual Studio or xcode\ project file in XCode


-- 4 -- Features -------------------------------------------------------------------------------------------------

-Cross-platform Windows/Mac compatibility

-Object-oriented 

-Performance settings - Change draw distance, switch between 2D and 3D objects being rendered, minimum FPS to auto-spawn, etc

-Settings - Alter spawn rate, program update speed, pause, turn collisions on-off, fullscreen toggle

-Rotate, pan, zoom camera - Move camera to nearest object of _______ state

-Spawning objects, decaying objects

-Objects spawning from decayed object use Gaussian randoms (to determine how many to spawn)


-- 5 -- Things to consider -----------------------------------------------------------------------------------------

-Collision detection is VERY intensive. Turned off as default, take caution when turning collisions on with large amounts of objects. Has been known to crash

-Program is most intensive on GPU (Graphics card) through initial profiling - Using Cinder prototypes means individual draw calls. Faster the graphics card the better!

-All source code is commented, for more detail please refer to!

-This is intended to work with 0.8.6 only. Attempts were made to convert to Cinder's latest version (0.9.0 as of writing) but issues found that went outside of timeframe. Get working fully with 0.9.0 possible project?

-- 6 -- Special thanks ----------------------------------------------------------------------------------------------

-Paul Houx, for creating the majority of Cinder's samples which were of great use in learning how to work with Cinder.
