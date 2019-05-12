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
#include "cSetUp.h"
//---------------------------------------------------------------------------
using namespace chai3d;
using namespace std;
//-----------------------------------------------------.

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
// DECLARED VARIABLES 
//------------------------------------------------------------------------------

// a haptic device handler
cHapticDeviceHandler* handler;

// a pointer to the current haptic device
shared_ptr<cGenericHapticDevice> hapticDevice;


// flag to indicate if the haptic simulation currently running
bool simulationRunning = false;

// flag to indicate if the haptic simulation has terminated
bool simulationFinished = false;

// haptic thread
cThread* hapticsThread;

// logging thread
cThread* loggingThread;

// protocol thread
cThread* protocolThread;

// a handle to window display context
GLFWwindow* window = NULL;

// current width of window
int width = 0;

// current height of window
int height = 0;

// swap interval for the display context (vertical synchronization)
int swapInterval = 1;


//------------------------------------------------------------------------------
// EXPERIMENT SET UP 
//------------------------------------------------------------------------------
cSetUp* m_setUp;

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

// this function contains the main logging loop
void updateLogging(void);

// this function contains the main protocol loop
void updateProtocol(void);

// this function closes the application
void close(void);
//==============================================================================
/*
TEMPLATE:    pilot.cpp

Pilot experiment for surgical cut simulation
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
	cout << "Pilot Experiment" << endl;
	cout << "-----------------------------------" << endl << endl << endl;
	cout << "Keyboard Options:" << endl << endl;
	cout << "[f] - Enable/Disable full screen mode" << endl;
	cout << "[m] - Enable/Disable vertical mirroring" << endl;
	cout << "[Insert] - Increase Stiffness Lev 1 [K1]" << endl;
	cout << "[Delete] - Decrease Stiffness Lev 1 [K1]" << endl;
	cout << "[P_UP] - Increase Stiffness Lev 2 [K2]" << endl;
	cout << "[P_DN] - Decrease Stiffness Lev 2 [K2]" << endl;
	cout << "[Home] - Increase Lev 1 width" << endl;
	cout << "[End] - Decrease Lev 1 width" << endl;
	cout << "[q] - Exit application" << endl;
	cout << "-----------------------------------" << endl;
	cout << endl << endl;

	// parse first arg to try and locate resources
	string resourceRoot = string(argv[0]).substr(0, string(argv[0]).find_last_of("/\\") + 1);

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

	//--------------------------------------------------------------------------
	// EXPERIMENT
	//--------------------------------------------------------------------------

	m_setUp = new cSetUp(resourceRoot, hapticDevice);

	cout << "Please enter Subject's Name:" << endl;
	cin >> m_setUp->subjectName;
	cout << endl << endl;
	cout << "Please enter number of Trial:" << endl;
	cin >> m_setUp->trialNumber;
	m_setUp->trialNumber -= 1;

	cout << endl << endl;


	// set stereo mode
	m_setUp->m_camera->setStereoMode(stereoMode);

	// initialize demo 1
	m_setUp->initPilot();

	//-----------------------------------------------------------------------
	// START SIMULATION
	//-----------------------------------------------------------------------

	// create a thread which starts the main haptics rendering loop
	hapticsThread = new cThread();
	hapticsThread->start(updateHaptics, CTHREAD_PRIORITY_HAPTICS);

	// This is to periodically log data (time, position, force, state) on a buffer
	loggingThread = new cThread(); // This is to periodically log data (time, position, force, state) on a buffer
	loggingThread->start(updateLogging, CTHREAD_PRIORITY_HAPTICS);

	protocolThread = new cThread(); // This is to periodically log data (time, position, force, state) on a buffer
	protocolThread->start(updateProtocol, CTHREAD_PRIORITY_HAPTICS);
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
		m_setUp->freqCounterGraphics.signal(1);
	}

	// close window
	glfwDestroyWindow(window);

	// terminate GLFW library
	glfwTerminate();

	// exit
	return 0;
}

//---------------------------------------------------------------------------



void initPilot()
{
	//m_setUp->init();
}


void windowSizeCallback(GLFWwindow* a_window, int a_width, int a_height)
{
	// update window size
	width = a_width;
	height = a_height;

	// update position of label
	//labelHapticDeviceModel->setLocalPos(20, height - 40, 0);
}

//------------------------------------------------------------------------------

void errorCallback(int a_error, const char* a_description)
{
	cout << "Error: " << a_description << endl;
}

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
		m_setUp->m_camera->setMirrorVertical(mirroredDisplay);
		m_setUp->m_mirroredDisplay = mirroredDisplay;
	}

	if ((a_key == GLFW_KEY_INSERT))
	{
		m_setUp->K1 += m_setUp->deltaK1;
	}
	if ((a_key == GLFW_KEY_DELETE))
	{
		m_setUp->K1 -= m_setUp->deltaK1;
	}
	if ((a_key == GLFW_KEY_PAGE_UP))
	{
		m_setUp->K2 += m_setUp->deltaK2;
	}
	if ((a_key == GLFW_KEY_PAGE_DOWN))
	{
		m_setUp->K2 -= m_setUp->deltaK2;
	}

	if ((a_key == GLFW_KEY_HOME))
	{
		m_setUp->L1 += m_setUp->deltaL1;
	}
	if ((a_key == GLFW_KEY_END))
	{
		m_setUp->L1 -= m_setUp->deltaL1;
	}

}

void close(void)
{
	// stop the simulation
	simulationRunning = false;

	// wait for graphics and haptics loops to terminate
	while (!simulationFinished) { cSleepMs(100); }

	// delete resources
	delete hapticsThread;
	delete protocolThread;
	delete loggingThread;
	delete m_setUp;
	delete handler;
	// close haptic device
	hapticDevice->close();

}

void updateGraphics(void)
{

	// render world
	m_setUp->updateGraphics(width, height);

	// wait until all GL commands are completed
	glFinish();

	// check for any OpenGL errors
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) printf("Error:  %s\n", gluErrorString(err));
}

//-------------------------------------------------------------------------

void updateHaptics()
{
	// simulation in now running
	simulationRunning = true;
	simulationFinished = false;

	// main haptic simulation loop
	while (simulationRunning)
	{
		m_setUp->updateHaptics(hapticDevice);
	}

	// exit haptics thread
	simulationFinished = true;
}

void updateLogging(void)
{
	while (simulationRunning)
		// This is to periodically log data (time, position, force, state) on a buffer
		if (m_setUp->loggingRunning)
		{
			cout << "start logging" << endl;
			m_setUp->updateLogging();
		}
}
void updateProtocol(void)
{
	while (simulationRunning)
		m_setUp->updateProtocol();
}




