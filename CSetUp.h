//---------------------------------------------------------------------------
#ifndef cSetUpH
#define cSetUpH
//---------------------------------------------------------------------------
#include "chai3d.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
//#include "TeleLiftingExperiment.cpp"

using namespace chai3d;

class cTrial
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
	virtual void initTrial();
	//! Update haptics
	virtual void updateHaptics();
	//! update protocol
	virtual void updateProtocol();
	//! update logging
	void updateLogging(void);
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
	cDirectionalLight *light;

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
	// a label to display the haptic device model
	cLabel* labelHapticDeviceModel;

	// haptic related stuff
	
	bool loggingRunning = false;

	/*--------------------------------------------------------------------*/
	/* EXPERIMENT RELATED STUFF */

	double K1 = 100;
	double K2 = 50;
	double L1 = 0.05;
	double L2 = 0;
	double lev = 0;
	double deltaK1 = 1;
	double deltaK2 = 1;
	double deltaL1 = 0.01;s

	// arrays for experiment properties from files
	// loaded in loadTrial()
	int K1Cond[500];
	int K2Cond[500];
	int L1Cond[500];
	

	// properties of current trial
	// setup of trial in updateProtocol()
	
	double kBoundary;
	double upTarget = 0.12;
	double downTarget = 0.06;
	cMesh* m_downTarget;
	cMesh* m_upTarget;
	cMesh* m_boundary;

	// data logging properties
	cThread* loggingThread;
	double fs = 2000; // sampling frequency [Hz]
	std::string subjectName = "";
	bool appendToFile;

	// experiment statemachine
	int expState = 1;
	int trialState = 1;
	int trialNumber = 115;
	int liftingNumber;
	/*--------------------------------------------------------------------*/
};

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
