//==============================================================================
/*
\author    Roberta Delrio
*/
//==============================================================================

//------------------------------------------------------------------------------
#include "chai3d.h"
#include <iostream>
#include <iomanip>  
//------------------------------------------------------------------------------
#include <GLFW/glfw3.h>
//------------------------------------------------------------------------------
using namespace chai3d;
using namespace std;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// GENERAL SETTINGS
//------------------------------------------------------------------------------

// stereo Mode
/*
C_STEREO_DISABLED:            Stereo is disabled
C_STEREO_ACTIVE:              Active stereo for OpenGL NVDIA QUADRO cards
C_STEREO_PASSIVE_LEFT_RIGHT:  Passive stereo where L/R images are rendered next to each other
C_STEREO_PASSIVE_TOP_BOTTOM:  Passive stereo where L/R images are rendered above each other
*/
cStereoMode stereoMode = C_STEREO_DISABLED;

// fullscreen mode
bool fullscreen = false;

// mirrored display
bool mirroredDisplay = false;


//------------------------------------------------------------------------------
// DECLARED VARIABLES add in the class
//------------------------------------------------------------------------------

// a world that contains all objects of the virtual environment
cWorld* world;

// a camera to render the world in the window display
cCamera* camera;

// a light source to illuminate the objects in the world
cDirectionalLight *light;

// a haptic device handler
cHapticDeviceHandler* handler;

// a pointer to the current haptic device
cGenericHapticDevicePtr hapticDevice;

// a label to display the haptic device model
cLabel* labelHapticDeviceModel;

// a label to display the position [m] of the haptic device
cLabel* labelHapticDevicePosition;

cLabel* labelStart;
cLabel* labelEnd;

// a global variable to store the position [m] of the haptic device
cVector3d hapticDevicePosition;

// a global variable to store the velocity [m/s] of the haptic device
cVector3d hapticDeviceVelocity;

// a font for rendering text
cFontPtr font;

// a label to display the rates [Hz] at which the simulation is running
cLabel* labelRates;

cShapeLine* cursorLine;

//---------------------------------------------------------------------
//GLOBAL VARIABLE

double K1 = 100;
double K2 = 50;
double L1 = 0.05;
double L2 = 0;
double lev = 0;
double deltaK1 = 1;
double deltaK2 = 1;
double deltaL1 = 0.01;

//---------------------------------------------------------------------
// a small sphere representing the starting and the final point
cShapeSphere* start;
cShapeSphere* endp;
//-------------------------------------------------------------------------------
// a small sphere (cursor) representing the haptic device 
cShapeSphere* cursor;

// a virtual tool representing the haptic device in the scene
//cToolCursor* tool;

// selected paint color
cColorb pointColorEnd;
cColorb pointColorStart;
//-------------------------------------------------------------------------------

// a mesh object to model a piece of tissue
cMesh* canvas;

// copy of blank canvas texture
cImagePtr canvasOriginal;

// flag to indicate if the haptic simulation currently running
bool simulationRunning = false;

// flag to indicate if the haptic simulation has terminated
bool simulationFinished = false;

// a frequency counter to measure the simulation graphic rate
cFrequencyCounter freqCounterGraphics;

// a frequency counter to measure the simulation haptic rate
cFrequencyCounter freqCounterHaptics;

// haptic thread
cThread* hapticsThread;

// a handle to window display context
GLFWwindow* window = NULL;

// current width of window
int width = 0;

// current height of window
int height = 0;

// swap interval for the display context (vertical synchronization)
int swapInterval = 1;

//------------------------------------------------------------------------------
// DECLARED MACROS
//------------------------------------------------------------------------------

// convert to resource path
#define RESOURCE_PATH(p)    (char*)((resourceRoot+string(p)).c_str())


//------------------------------------------------------------------------------
// DECLARED FUNCTIONS
//------------------------------------------------------------------------------

// callback when the window display is resized
void windowSizeCallback(GLFWwindow* a_window, int a_width, int a_height);

// callback when an error GLFW occurs
void errorCallback(int error, const char* a_description);

// callback when a key is pressed
void keyCallback(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods);

// this function renders the scene
void updateGraphics(void);

// this function contains the main haptics simulation loop
void updateHaptics(void);

// this function closes the application
void close(void);


//==============================================================================
/*
TEMPLATE:    application.cpp

Description of your application.
*/
//==============================================================================

int main(int argc, char* argv[])
{
	//--------------------------------------------------------------------------
	// INITIALIZATION
	//--------------------------------------------------------------------------

	cout << endl;
	cout << "-----------------------------------" << endl;
	cout << "CHAI3D" << endl;
	cout << "-----------------------------------" << endl << endl << endl;
	cout << "Keyboard Options:" << endl << endl;
	cout << "[f] - Enable/Disable full screen mode" << endl;
	cout << "[m] - Enable/Disable vertical mirroring" << endl;
	cout << "[c] - Clear tissue " << endl;
	cout << "[q] - Exit application" << endl;
	cout << endl << endl;


	//--------------------------------------------------------------------------
	// OPENGL - WINDOW DISPLAY
	//--------------------------------------------------------------------------

	// initialize GLFW library
	if (!glfwInit())
	{
		cout << "failed initialization" << endl;
		cSleepMs(1000);
		return 1;
	}

	// set error callback
	glfwSetErrorCallback(errorCallback);

	// compute desired size of window
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int w = 0.8 * mode->height;
	int h = 0.5 * mode->height;
	int x = 0.5 * (mode->width - w);
	int y = 0.5 * (mode->height - h);

	// set OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	// set active stereo mode
	if (stereoMode == C_STEREO_ACTIVE)
	{
		glfwWindowHint(GLFW_STEREO, GL_TRUE);
	}
	else
	{
		glfwWindowHint(GLFW_STEREO, GL_FALSE);
	}

	// create display context
	window = glfwCreateWindow(w, h, "CHAI3D", NULL, NULL);
	if (!window)
	{
		cout << "failed to create window" << endl;
		cSleepMs(1000);
		glfwTerminate();
		return 1;
	}

	// get width and height of window
	glfwGetWindowSize(window, &width, &height);

	// set position of window
	glfwSetWindowPos(window, x, y);

	// set key callback
	glfwSetKeyCallback(window, keyCallback);

	// set resize callback
	glfwSetWindowSizeCallback(window, windowSizeCallback);

	// set current display context
	glfwMakeContextCurrent(window);

	// sets the swap interval for the current display context
	glfwSwapInterval(swapInterval);

#ifdef GLEW_VERSION
	// initialize GLEW library
	if (glewInit() != GLEW_OK)
	{
		cout << "failed to initialize GLEW library" << endl;
		glfwTerminate();
		return 1;
	}
#endif


	//--------------------------------------------------------------------------
	// WORLD - CAMERA - LIGHTING add class
	//--------------------------------------------------------------------------

	// create a new world.
	world = new cWorld();

	// set the background color of the environment
	world->m_backgroundColor.setBlack();

	// create a camera and insert it into the virtual world
	camera = new cCamera(world);
	world->addChild(camera);

	// position and orient the camera
	camera->set(cVector3d(0.5, 0.0, 0.0),    // camera position (eye)
		cVector3d(0.0, 0.0, 0.0),    // look at position (target)
		cVector3d(0.0, 0.0, 1.0));   // direction of the (up) vector

	// set the near and far clipping planes of the camera
	camera->setClippingPlanes(0.01, 10.0);

	// set stereo mode
	camera->setStereoMode(stereoMode);

	// set stereo eye separation and focal length (applies only if stereo is enabled)
	camera->setStereoEyeSeparation(0.01);
	camera->setStereoFocalLength(0.5);

	// set vertical mirrored display mode
	camera->setMirrorVertical(mirroredDisplay);

	// create a directional light source
	light = new cDirectionalLight(world);

	// insert light source inside world
	world->addChild(light);

	// enable light source
	light->setEnabled(true);

	// define direction of light beam
	light->setDir(-1.0, 0.0, 0.0);

	// create a sphere (cursor) to represent the haptic device
	cursor = new cShapeSphere(0.005);

	// insert cursor inside world
	world->addChild(cursor);
	
	cursorLine = new cShapeLine(cVector3d(0, 0, 0), cVector3d(0, 0, 0));
	world->addChild(cursorLine);
	//--------------------------------------------------------------------------
	// HAPTIC DEVICE
	//--------------------------------------------------------------------------

	// create a haptic device handler
	handler = new cHapticDeviceHandler();

	// get a handle to the first haptic device
	handler->getDevice(hapticDevice, 0);

	// open a connection to haptic device
	hapticDevice->open();

	// calibrate device (if necessary)
	hapticDevice->calibrate();

	// retrieve information about the current haptic device
	cHapticDeviceInfo info = hapticDevice->getSpecifications();

	// display a reference frame if haptic device supports orientations
	if (info.m_sensedRotation == true)
	{
		// display reference frame
		cursor->setShowFrame(true);

		// set the size of the reference frame
		cursor->setFrameSize(0.05);
	}
	

	// create a tool (cursor) and insert into the world
	//tool = new cToolCursor(world);
	//world->addChild(tool);

	// connect the haptic device to the tool
	//tool->setHapticDevice(hapticDevice);

	// define a radius for the tool
	//double toolRadius = 0.01;

	// set tool radius
	//tool->setRadius(toolRadius);

	// map the physical workspace of the haptic device to a larger virtual workspace.
	//tool->setWorkspaceRadius(1.0);
	// start the haptic tool
	//tool->start();
	// if the device has a gripper, enable the gripper to simulate a user switch
	//hapticDevice->setEnableGripperUserSwitch(true);

	//--------------------------------------------------------------------------
	// CREATE OBJECTS
	//--------------------------------------------------------------------------

	/////////////////////////////////////////////////////////////////////////
	// REFERENCE POINT:
	////////////////////////////////////////////////////////////////////////
	start = new cShapeSphere(0.02);
	endp = new cShapeSphere(0.02);

	world->addChild(start);
	world->addChild(endp);

	start->setLocalPos(-0.7,-0.05, 0.0);
	endp->setLocalPos(-0.7, 0.3, 0.0);

	pointColorEnd.setBlueAquamarine();
	endp->m_material->setColor(pointColorEnd);

	pointColorStart.setRedCrimson();
	start->m_material->setColor(pointColorStart);
	/////////////////////////////////////////////////////////////////////////
	// CANVAS: add
	////////////////////////////////////////////////////////////////////////

	// initialize a default color for tool brush
	//paintColor.setBlack();
	//tool->m_hapticPoint->m_sphereProxy->m_material->setColor(paintColor);
	// create a mesh
	canvas = new cMesh();

	// create a plane
	cCreatePlane(canvas, 1.5, 1.5);

	// create collision detector
	canvas->createBruteForceCollisionDetector();

	// add object to world
	world->addChild(canvas);

	// set the position of the object
	canvas->setLocalPos(-2.5, 0.3, 0.0);
	canvas->rotateAboutGlobalAxisRad(cVector3d(0, 1, 0), cDegToRad(90));
	canvas->rotateAboutGlobalAxisRad(cVector3d(1, 0, 0), cDegToRad(90));

	// set graphic properties
	canvas->m_texture = cTexture2d::create();
	bool fileload = canvas->m_texture->loadFromFile("bin/resources/images/canvas.jpg");
	if (!fileload)
	{
#if defined(_MSVC)
		fileload = canvas->m_texture->loadFromFile("bin/resources/images/canvas.jpg");
#endif
	}
	if (!fileload)
	{
		cout << "Error - Texture image failed to load correctly." << endl;
		close();
		return (-1);
	}

	// create a copy of canvas so that we can clear page when requested
	canvasOriginal = canvas->m_texture->m_image->copy();

	// we disable lighting properties for canvas
	canvas->setUseMaterial(false);

	// enable texture mapping
	canvas->setUseTexture(true);

	// create normal map from texture data
	cNormalMapPtr normalMap = cNormalMap::create();
	normalMap->createMap(canvas->m_texture);
	canvas->m_normalMap = normalMap;

	// set haptic properties
	canvas->m_material->setStiffness(0.5);
	canvas->m_material->setStaticFriction(0.30);
	canvas->m_material->setDynamicFriction(0.20);
	canvas->m_material->setHapticTriangleSides(true, false);
	canvas->m_material->setTextureLevel(1.5);


	//--------------------------------------------------------------------------
	// WIDGETS add
	//--------------------------------------------------------------------------

	// create a font
	cFontPtr font = NEW_CFONTCALIBRI20();

	// create a label to display the haptic device model
	labelHapticDeviceModel = new cLabel(font);
	camera->m_frontLayer->addChild(labelHapticDeviceModel);
	labelHapticDeviceModel->setText(info.m_modelName);

	// create a label to display the haptic and graphic rates of the simulation
	labelRates = new cLabel(font);
	labelRates->m_fontColor.setWhite();
	camera->m_frontLayer->addChild(labelRates);

	labelEnd = new cLabel(font);
	labelEnd->m_fontColor.setBlueMediumTurquoise();
	camera->m_frontLayer->addChild(labelEnd);

	labelStart = new cLabel(font);
	labelStart->m_fontColor.setRedCrimson();
	camera->m_frontLayer->addChild(labelStart);


	//--------------------------------------------------------------------------
	// START SIMULATION
	//--------------------------------------------------------------------------

	// create a thread which starts the main haptics rendering loop
	hapticsThread = new cThread();
	hapticsThread->start(updateHaptics, CTHREAD_PRIORITY_HAPTICS);

	// setup callback when application exits
	atexit(close);


	//--------------------------------------------------------------------------
	// MAIN GRAPHIC LOOP
	//--------------------------------------------------------------------------

	// call window size callback at initialization
	windowSizeCallback(window, width, height);

	// main graphic loop
	while (!glfwWindowShouldClose(window))
	{
		// get width and height of window
		glfwGetWindowSize(window, &width, &height);

		// render graphics
		updateGraphics();

		// swap buffers
		glfwSwapBuffers(window);

		// process events
		glfwPollEvents();

		// signal frequency counter
		freqCounterGraphics.signal(1);
	}

	// close window
	glfwDestroyWindow(window);

	// terminate GLFW library
	glfwTerminate();

	// exit
	return 0;
}

//------------------------------------------------------------------------------

void windowSizeCallback(GLFWwindow* a_window, int a_width, int a_height)
{
	// update window size
	width = a_width;
	height = a_height;

	// update position of label
	labelHapticDeviceModel->setLocalPos(20, height - 40, 0);
}

//------------------------------------------------------------------------------

void errorCallback(int a_error, const char* a_description)
{
	cout << "Error: " << a_description << endl;
}

//------------------------------------------------------------------------------

void keyCallback(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods)
{
	// filter calls that only include a key press
	if (a_action != GLFW_PRESS)
	{
		return;
	}

	// option - exit
	if ((a_key == GLFW_KEY_ESCAPE) || (a_key == GLFW_KEY_Q))
	{
		glfwSetWindowShouldClose(a_window, GLFW_TRUE);
	}

	// option - toggle fullscreen
	if (a_key == GLFW_KEY_F)
	{
		// toggle state variable
		fullscreen = !fullscreen;

		// get handle to monitor
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();

		// get information about monitor
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		// set fullscreen or window mode
		if (fullscreen)
		{
			glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			glfwSwapInterval(swapInterval);
		}
		else
		{
			int w = 0.8 * mode->height;
			int h = 0.5 * mode->height;
			int x = 0.5 * (mode->width - w);
			int y = 0.5 * (mode->height - h);
			glfwSetWindowMonitor(window, NULL, x, y, w, h, mode->refreshRate);
			glfwSwapInterval(swapInterval);
		}
	}

	// option - toggle vertical mirroring
	if (a_key == GLFW_KEY_M)
	{
		mirroredDisplay = !mirroredDisplay;
		camera->setMirrorVertical(mirroredDisplay);
	}

	if ((a_key == GLFW_KEY_INSERT))
	{
		K1 += deltaK1;
	}
	if ((a_key == GLFW_KEY_DELETE))
	{
		K1 -= deltaK1;
	}
	if ((a_key == GLFW_KEY_PAGE_UP))
	{
		K2 += deltaK2;
	}
	if ((a_key == GLFW_KEY_PAGE_DOWN))
	{
		K2 -= deltaK2;
	}

	if ((a_key == GLFW_KEY_HOME))
	{
		L1 += deltaL1;
	}
	if ((a_key == GLFW_KEY_END))
	{
		L1 -= deltaL1;
	}

	// option c - clear canvas
	if (a_key == GLFW_KEY_C)
	{
		// copy original image of canvas to texture
		canvasOriginal->copyTo(canvas->m_texture->m_image);

		// update texture
		canvas->m_texture->markForUpdate();

		// update console message
		cout << "> Canvas has been erased.            \r";
	}
}

//------------------------------------------------------------------------------

void close(void)
{
	// stop the simulation
	simulationRunning = false;

	// wait for graphics and haptics loops to terminate
	while (!simulationFinished) { cSleepMs(100); }

	// close haptic device
	hapticDevice->close();

	// delete resources
	delete hapticsThread;
	delete world;
	delete handler;
}

//------------------------------------------------------------------------------

void updateGraphics(void)
{
	/////////////////////////////////////////////////////////////////////
	// UPDATE WIDGETS
	/////////////////////////////////////////////////////////////////////

	// update haptic and graphic rate data
	labelRates->setText("LEV:" + cStr(lev, 0) +cStr(freqCounterGraphics.getFrequency(), 0) + " Hz / " +
		cStr(freqCounterHaptics.getFrequency(), 0) + " Hz / K1 " +
		cStr(K1, 0) + " K2 " + cStr(K2, 0) + " L1 " + cStr(L1, 2));
	labelEnd->setText( "GOAL");
	labelStart->setText("START");
	// update position of label
	labelRates->setLocalPos((int)(0.5 * (width - labelRates->getWidth())), 15);
	
	labelEnd->setLocalPos(2 * 640, 2 * 290, 2 * 400);
	labelStart->setLocalPos(2 * 430, 2 * 290, 2 * 400);
	/////////////////////////////////////////////////////////////////////
	// RENDER SCENE
	/////////////////////////////////////////////////////////////////////

	// update shadow maps (if any)
	world->updateShadowMaps(false, mirroredDisplay);

	// render world
	camera->renderView(width, height);

	// wait until all GL commands are completed
	glFinish();

	// check for any OpenGL errors
	GLenum err;
	err = glGetError();
	if (err != GL_NO_ERROR) cout << "Error:  %s\n" << gluErrorString(err);
}

//------------------------------------------------------------------------------

void updateHaptics(void)
{
	// simulation in now running
	simulationRunning = true;
	simulationFinished = false;

	// main haptic simulation loop
	while (simulationRunning)
	{
		/////////////////////////////////////////////////////////////////////
		// READ HAPTIC DEVICE
		/////////////////////////////////////////////////////////////////////

		// read position 
		cVector3d position;
		cVector3d positionk = position;
		hapticDevice->getPosition(position);

		// read orientation 
		cMatrix3d rotation;
		hapticDevice->getRotation(rotation);

		// read user-switch status (button 0)
		bool button = false;
		hapticDevice->getUserSwitch(0, button);


		/////////////////////////////////////////////////////////////////////
		// UPDATE 3D CURSOR MODEL
		/////////////////////////////////////////////////////////////////////

		// update position and orienation of cursor
		cursor->setLocalPos(position);
		cursor->setLocalRot(rotation);

		//Change pov of the camera
		cursor->setLocalPos(position.z(),position.y(),position.x());
		cursorLine->m_pointA = positionk;
		cursorLine->m_pointB = cAdd(positionk, position);
		/////////////////////////////////////////////////////////////////////
		// COMPUTE FORCES
		/////////////////////////////////////////////////////////////////////

		cVector3d force(0, 0, 0);
		cVector3d torque(0, 0, 0);
		double gripperForce = 0.0;
		double Kp = 0;

		/////////////////////////////////////////////////////////////////////
		// APPLY FORCES
		/////////////////////////////////////////////////////////////////////
		/*
		if (position.z() < 0.0 && position.z() > -L1) {
			// compute linear force
			Kp = K1* position.z(); // [N/m]
			printf("LEV1");
			lev = 1;
		}

		else if (position.z() < -L1) {
			// compute linear force
			Kp = K2*position.z(); // [N/m]
			printf("LEV2");
			lev = 2;

		}
		else{

			// compute linear force
			Kp = -0; // [N/m]
			lev = 0;
		}*/
		//SMOOTH TRANSITION

		if (position.z() < 0.0 && position.z() > -L1) {
		// compute linear force
		Kp = K1* position.z(); // [N/m]
		printf("LEV1");
		lev = 1;
		}

		else if (position.z() < -L1) {
		// compute linear force
		Kp = -L1*(K1 + K2) + K2*(position.z() + L1); // [N/m]
		printf("LEV2");
		lev = 2;

		}
		else{

		// compute linear force
		Kp = -0; // [N/m]
		lev = 0;
		}

		
		double forceField = -Kp;
		force.z(forceField);
		cout << "X:" << setprecision(2) << position.x() << " Y:" << setprecision(2) << position.y() << " Z:" << setprecision(2) << position.z() << "Force:" << force.z() << "Kp:" << Kp << "\r";
		
		// send computed force, torque, and gripper force to haptic device
		hapticDevice->setForceAndTorqueAndGripperForce(force, torque, gripperForce);

		// signal frequency counter
		freqCounterHaptics.signal(1);
	}

	// exit haptics thread
	simulationFinished = true;
}

//