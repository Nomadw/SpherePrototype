# SpherePrototype

Vector-based multiple objects
  Independant colour values
  Transparency values
  Position vector
  Velocity (Movement) vector
  Lifespan of object (Before it begins to decay)
  Stages of life within object

Object life stages
   Creation
    Oscillation
    Fast object creation - For objects being created from decay
  “Normal”
    Objects become selectable (Click, currently selects all selectable objects):
    Press W, A, S, D, Q and E to move all selected objects along the X, Y and Z axis respectively
    Press X to cancel all current movement
    Press Z to deselect all selected objects
    Press DELETE to remove all selected objects
  Decay
    Fast object creation - Smaller objects will spawn from the decaying object
    Current order of object->spawned objects is orange->Green->blue
    Blue objects do not spawn any objects when decaying for so performance isn’t strained (Too many objects)

Press C to clear ALL objects
Press F to toggle fullscreen on/off
Press R to force a new object to spawn (random location, will oscillate)
Press ESC to exit the application

Camera
  Right mouse button + drag to zoom in/out
  Middle mouse button (Scroll wheel) + drag to pan camera
  Hold right + middle buttons and drag to tumble camera
  Scroll wheel zooms in/out (Not fully working yet)
  Objects will increase and decrease in quality based on distance from camera


Features to add?


Features to fix / remove?
  Object selection - Currently selects all objects
