//---------------------------------------------------------------------------
#ifndef cSetUpH
#define cSetUpH
//---------------------------------------------------------------------------
#include "chai3d.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>  
//#include "TeleLiftingExperiment.cpp"

using namespace chai3d;

class cSetUp
{
	//-----------------------------------------------------------------------
	// CONSTRUCTOR & DESTRUCTOR:
	//-----------------------------------------------------------------------

public:

	//! Constructor of cTrial.
	cSetUp(const std::string a_resourceRoot, std::shared_ptr<cGenericHapticDevice> a_hapticDevice);

	//! Destructor of cTrial.
	virtual ~cSetUp() {};


	//-----------------------------------------------------------------------
	// PUBLIC METHODS:
	//-----------------------------------------------------------------------
public:

	//! Initialize to demo
	virtual void initPilot();
	//! Update haptics
	virtual void updateHaptics(std::shared_ptr<cGenericHapticDevice> a_hapticDevice);
	//! update protocol
	virtual void updateProtocol();
	//! update logging
	virtual void updateLogging(void);
	//! Update graphics
	virtual void updateGraphics(int a_width, int a_height);
	//!
	virtual void loadTrial();
	//-----------------------------------------------------------------------
	// PUBLIC MEMBERS:
	//-----------------------------------------------------------------------

public:

	//! virtual world
	cWorld* m_world;

	//! camera
	cCamera* m_camera;

	// a light source to illuminate the objects in the world
	cDirectionalLight* m_light;

	// a font for rendering text
	cFontPtr font;

	// a sphere for the cursor
	cShapeSphere* m_cursor;

	// a small sphere representing the starting and the final point
	cShapeSphere* start;
	cShapeSphere* endp;
	// selected paint color
	cColorb pointColorEnd;
	cColorb pointColorStart;

	//! simulation clock
	cPrecisionClock simClock;

	//! logging clock 
	cPrecisionClock logClock;

	//! mirroed display
	bool m_mirroredDisplay;

	// a frequency counter to measure the simulation graphic rate
	cFrequencyCounter freqCounterGraphics;

	// a frequency counter to measure the simulation haptic rate
	cFrequencyCounter freqCounterHaptics;

	// a label to display the rate [Hz] at which the simulation is running
	cLabel* labelRates;
	cLabel* labelHaptics;
	cLabel* labelTrialInfo;
	cLabel* labelTrialInstructions;

	// a haptic device handler
	//cHapticDeviceHandler* handler;
	// haptic related stuff
	// a pointer to the current haptic device
	//cGenericHapticDevicePtr hapticDevice;
	// a label to display the haptic device model
	//cLabel* labelHapticDeviceModel;


	bool loggingRunning = false;
	// a pointer to the current haptic device

	/*--------------------------------------------------------------------*/
	/* EXPERIMENT RELATED STUFF */

	double K1 = 100;
	double K2 = 50;
	double L1 = 0.05;
	double L2 = 0;
	double lev = 0;
	double deltaK1 = 1;
	double deltaK2 = 1;
	double deltaL1 = 0.01;

	cVector3d startPosition;
	cVector3d endPosition;

	// arrays for experiment properties from files
	// loaded in loadTrial()
	int K1Cond[500];
	int K2Cond[500];
	int L1Cond[500];
	
	// properties of current trial
	// setup of trial in updateProtocol()

	// data logging properties
	cThread* loggingThread;
	double fs = 2000; // sampling frequency [Hz]
	std::string subjectName = "";
	bool appendToFile;

	// experiment statemachine
	int expState = 1;
	int trialState = 1;
	int trialNumber = 115;

	/*--------------------------------------------------------------------*/
};

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
