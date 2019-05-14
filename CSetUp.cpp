//==============================================================================
/*
\author    Roberta Delrio
*/
//==============================================================================

//------------------------------------------------------------------------------

#include "cSetUp.h"
using namespace std;
//---------------------------------------------------------------------------
//------------------------------------------------------------------------------
// DECLARED MACROS
//------------------------------------------------------------------------------
// convert to resource path
#define RESOURCE_PATH(p)    (char*)((a_resourceRoot+string(p)).c_str())
//cStereoMode stereoMode = C_STEREO_DISABLED;

//===========================================================================
/*!
Constructor of cTrial.
*/
//===========================================================================
cSetUp::cSetUp(const string a_resourceRoot, shared_ptr<cGenericHapticDevice> a_hapticDevice)

{
	// load the experiment conditions
	loadTrial();
	// display is not mirrored
	m_mirroredDisplay = false;

	// create world
	m_world = new cWorld();

	// set background color
	m_world->m_backgroundColor.setBlack();


	// create camera
	m_camera = new cCamera(m_world);
	m_world->addChild(m_camera);

	// position and orient the camera
	m_camera->set(cVector3d(0.5, 0.0, 0.0),    // camera position (eye)
		cVector3d(0.0, 0.0, 0.0),    // look at position (target)
		cVector3d(0.0, 0.0, 1.0));   // direction of the (up) vector

	// set the near and far clipping planes of the camera
	m_camera->setClippingPlanes(0.01, 10.0);
	
	// set stereo mode
	//m_camera->setStereoMode(stereoMode);

	// set stereo eye separation and focal length (applies only if stereo is enabled)
	m_camera->setStereoEyeSeparation(0.01);
	m_camera->setStereoFocalLength(0.5);

	// set vertical mirrored display mode
	//m_camera->setMirrorVertical(mirroredDisplay);

	// create a directional light source
	m_light = new cDirectionalLight(m_world);

	// insert light source inside world
	m_world->addChild(m_light);

	// enable light source
	m_light->setEnabled(true);

	// define direction of light beam
	m_light->setDir(-1.0, 0.0, 0.0);

	// create a sphere (cursor) to represent the haptic device
	m_cursor = new cShapeSphere(0.005);
	m_world->addChild(m_cursor);

	// insert cursor inside world
	start = new cShapeSphere(0.02);
	endp = new cShapeSphere(0.02);

	m_world->addChild(start);
	m_world->addChild(endp);
	
	start->setLocalPos(-0.7, -0.05, 0.0);
	endp->setLocalPos(-0.7, 0.3, 0.0);

	cVector3d startPosition;
	//start->setLocalPos(startPosition);
	cVector3d endPosition;
	//endp->setLocalPos(endPosition);

	pointColorEnd.setBlueAquamarine();
	endp->m_material->setColor(pointColorEnd);

	pointColorStart.setRedCrimson();
	start->m_material->setColor(pointColorStart);


	// create a font
	font = NEW_CFONTCALIBRI20();
	// create a label to display the haptic and graphic rate of the simulation
	labelRates = new cLabel(font);
	m_camera->m_frontLayer->addChild(labelRates);
	labelRates->setShowEnabled(false);

	labelHaptics = new cLabel(font);
	labelHaptics->setShowEnabled(false);
	m_camera->m_frontLayer->addChild(labelHaptics);

	labelTrialInfo = new cLabel(font);
	labelTrialInfo->m_fontColor = cColorf(1.0, 0.0, 0.0);
	m_camera->m_frontLayer->addChild(labelTrialInfo);

	labelTrialInstructions = new cLabel(font);
	labelTrialInstructions->m_fontColor = cColorf(0.0, 0.0, 1.0);
	m_camera->m_frontLayer->addChild(labelTrialInstructions);

	//labelHapticDeviceModel = new cLabel(font);
	//m_camera->m_frontLayer->addChild(labelHapticDeviceModel);
}
// update position of label
//===========================================================================
/*!
Update graphics.

*/
//===========================================================================

void cSetUp::updateGraphics(int a_width, int a_height)
{
	// retrieve information about the current haptic device
	//cHapticDeviceInfo info = hapticDevice->getSpecifications();
	// create a label to display the haptic device model
	//labelHapticDeviceModel->setText(info.m_modelName);

	// update haptic and graphic rate data
	labelTrialInstructions->setLocalPos((int)(0.4 * (a_width - labelTrialInfo->getWidth())), 0.8*a_height - labelTrialInfo->getHeight());
	// update position of label
	labelTrialInfo->setLocalPos((int)(0.1 * (a_width - labelTrialInfo->getWidth())), 0.9*a_height - labelTrialInfo->getHeight());

	// update haptic and graphic rate data
	labelRates->setText(cStr(freqCounterGraphics.getFrequency(), 0) + " Hz / " +
		cStr(freqCounterHaptics.getFrequency(), 0) + " Hz");
	// update position of label
	labelRates->setLocalPos((int)(0.5 * (a_width - labelRates->getWidth())), 100);

	// update position of label
	labelHaptics->setText("LEV:" + cStr(lev, 0) + "K1 " +
		cStr(K1, 0) + " K2 " + cStr(K2, 0) + " L1 " + cStr(L1, 2));
	// update position of label
	labelHaptics->setLocalPos((int)(0.1 * (a_width - labelHaptics->getWidth())), 0.9 *a_height - labelHaptics->getHeight());

	// update shadow maps (if any)
	m_world->updateShadowMaps(false, m_mirroredDisplay);

	// render view
	m_camera->renderView(a_width, a_height);

}

void cSetUp::loadTrial()
{
	ifstream K1Condition("K1.txt");
	if (!K1Condition) {
		cout << "Unable to open file";
		exit(1); // terminate with error
	}
	int i = 0;
	while (K1Condition >> K1Cond[i]) {
		cout << K1Cond[i] << " ";
		i += 1;
	}
	cout << endl;
	//delete &visualCondition;
	ifstream K2Condition("K2.txt");
	if (!K2Condition) {
		cout << "Unable to open file";
		exit(1); // terminate with error
	}
	i = 0;
	while (K2Condition >> K2Cond[i]) {
		cout << K2Cond[i] << " ";
		i += 1;
	}
	cout << endl;
	//delete handleCondition;
	ifstream L1Condition("L1.txt");
	if (!L1Condition) {
		cout << "Unable to open file";
		exit(1); // terminate with error
	}
	i = 0;
	while (L1Condition >> L1Cond[i]) {
		cout << setprecision(2) << L1Cond[i] << " ";
		i += 1;
	}
	cout << endl;

	//delete &springCondition;
	cout << "Trial Loaded" << endl << endl;
}

void cSetUp::updateHaptics(shared_ptr<cGenericHapticDevice> a_hapticDevice)
{
	
	// read position 
	cVector3d position;
	//cVector3d positionk = position;
	//position = m_cursor->getGlobalPos();

	a_hapticDevice->getPosition(position);

	// read orientation 
	cMatrix3d rotation;
	a_hapticDevice->getRotation(rotation);
	/*
	// read user-switch status (button 0)
	bool button = false;
	hapticDevice->getUserSwitch(0, button);
	*/

	/////////////////////////////////////////////////////////////////////
	// UPDATE 3D CURSOR MODEL
	/////////////////////////////////////////////////////////////////////

	// update position and orienation of cursor
	m_cursor->setLocalPos(position);
	m_cursor->setLocalRot(rotation);

	//Change pov of the camera
	m_cursor->setLocalPos(position.z(), position.y(), position.x());

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
		printf("LEV1 ");
		lev = 1;
	}

	else if (position.z() < -L1) {
		// compute linear force
		Kp = -L1*(K1 + K2) + K2*(position.z() + L1); // [N/m]
		printf("LEV2 ");
		lev = 2;

	}
	else{

		// compute linear force
		Kp = -0; // [N/m]
		lev = 0;
	}

	double forceField = -Kp;
	force.z(forceField);
	cout << "  K1:" << K1 << "  K2:" << K2 << "  L1:" << L1 << "    X:" << setprecision(2) << position.x() << " Y:" << setprecision(2) << position.y() << " Z:" << setprecision(2) << position.z() << " Force:" << force.z() << "Kp:" << Kp << "\r";


	// send computed force, torque, and gripper force to haptic device
	a_hapticDevice->setForceAndTorqueAndGripperForce(force, torque, gripperForce);
	double time = simClock.getCurrentTimeSeconds();
	double nextSimInterval = cClamp(time, 0.0001, 0.001);

	// reset clock
	simClock.reset();
	simClock.start();

	freqCounterHaptics.signal(1);

}

void cSetUp::initPilot()
{
	// set starting position of cube and its color
	labelTrialInfo->setText(cStr(trialNumber + 1));

	K1 = K1Cond[trialNumber];
	K2 = K2Cond[trialNumber];
	L1 = L1Cond[trialNumber];
}

void cSetUp::updateProtocol()
{
	/*
	//if (expState < 3)
	//	cout << "exp: " << expState << " trial: " << trialState << "\r";
	switch (expState)
	{
	case 1: // Setup Next Trial
		if (!appendToFile)
		{
			labelTrialInstructions->setText("Bring the cursor to the start position");
		}
		initPilot();
		cout << endl << "starting Trial: " << trialNumber + 1 << endl << endl;
		expState += 1;

		break;
	case 2: // wait for subject to reach starting pos
		if (fabs(m_cursor->getLocalPos().x() - startPosition.x()) < 0.01 && fabs(m_cursor->getLocalPos().y() - startPosition.y()) < 0.01)
			expState += 1;
		break;
	case 3: // wait for subject to reach stay at starting pos for a random time between 100-200 milliseconds
	{
		//cout << "------" << endl;
		labelTrialInstructions->setText("Get Ready");

		double waittime = (rand() % 100 + 101) / 100;
		double time0 = logClock.getCurrentTimeSeconds();
		while ((logClock.getCurrentTimeSeconds() - time0) < waittime)
		{
			cout << "now: " << logClock.getCurrentTimeSeconds() << " t0: " << time0 << " wait: " << waittime << "\r";
		}
		if (m_cursor->getLocalPos().x() - startPosition.x() < 0.005 && m_cursor->getLocalPos().y() - startPosition.y() < 0.005)
		{
			cout << "start logging" << endl << endl;
			loggingRunning = true;
			expState += 1; //start trial
			trialState = 1;
		}
		else
			expState -= 1; // go back to case 2 waiting...
		break;
	}
	case 4: // start the trial + trial running
		//cout << "------" << endl;

		labelTrialInstructions->setText("GO");
		if (trialState != 3) // show the button for next trial
		{
			trialState = 3;

		}
		/*else
		if (m_ODEBody1->getLocalPos().z() < boxSize / 2 + 0.0003 && trialState != 3)
		{
		expState = 1;
		//loggingThread->stop();
		//delete &loggingThread;
		loggingRunning = false;
		appendToFile = true;
		cout << "stop logging" << endl;
		labelTrialInstructions->setText("Cube Slipped\n Start over");
		//copy the data file and call it bad at the end;
		}*/
	/*
		switch (trialState)
		{
		case 1: //midlift
			if (m_cursor->getLocalPos().x() > startPosition.x() && m_cursor->getLocalPos().y() > startPosition.y())
				trialState += 1;
			break;
		case 2:
		{
			if (m_cursor->getLocalPos().x() < endPosition.x() && m_cursor->getLocalPos().y() < endPosition.y())
			{
				trialState -= 1;
				//liftingNumber += 1;
				//labelTrialInstructions->setText(cStr(liftingNumber));
				//cout << endl << "lift: " << liftingNumber << endl;
			}
			break;
		}
		/*case 3:
		{
		if (m_ODEBody1->getLocalPos().z() < boxSize / 2 + 0.0003)
		expState += 1;
		labelTrialInstructions->setText("Let go of the Cube");
		break;
		}
		}
		break;*/
		/*case 3:
		{
			trialNumber += 1;
			expState = 1;
			loggingRunning = false;
			appendToFile = false;
			break;
		}
		}
	}
	*/
}

void cSetUp::updateLogging(void)
{
	string filename;
	char trialString[3];
	double time;
	itoa(trialNumber + 1, trialString, 10);
	filename = +"dataLogs\\" + subjectName + "_trial_" + trialString + ".txt";
	/*sprintf(filename, sprintf("%c_trial_%i.txt",subjectName,trialNumber)*/
	std::ofstream trialFile;
	if (appendToFile)
		trialFile.open(filename, std::ios_base::app);
	else
	{
		trialFile.open(filename);
		trialFile << "Time\t"
			<< "finger_proxy_x\t" << "finger_proxy_x\t" << "finger_proxy_x\t"
			<< "finger_goal_x\t" << "finger_goal_y\t" << "finger_goal_z\t"
			<< "thumb_proxy_x\t" << "thumb_proxy_y\t" << "thumb_proxy_z\t"
			<< "thumb_goal_x\t" << "thumb_goal_y\t" << "thumb_goal_z\t"
			<< "K1\t" << "K2\t" << "L1\t" << endl;
		logClock.reset();
		logClock.start();
	}

	time = logClock.getCurrentTimeSeconds();
	double oldtime = 0;
	while (loggingRunning)
	{
		do
		{
			time = logClock.getCurrentTimeSeconds();
		} while ((time - oldtime) < (1 / fs));
		oldtime = time;
		trialFile << std::fixed << time << "\t"
			<< m_cursor->getLocalPos().x() << "\t" << m_cursor->getLocalPos().y() << "\t" << m_cursor->getLocalPos().z() << "\t"
			<< m_cursor->getLocalPos().x() << "\t" << m_cursor->getLocalPos().y() << "\t" << m_cursor->getLocalPos().z() << "\t"
			<< m_cursor->getLocalPos().x() << "\t" << m_cursor->getLocalPos().y() << "\t" << m_cursor->getLocalPos().z() << "\t"
			<< m_cursor->getLocalPos().x() << "\t" << m_cursor->getLocalPos().y() << "\t" << m_cursor->getLocalPos().z() << "\t"
			<< K1 << "\t" << K2 << "\t" << L1 << "\t" << endl;
	}

	trialFile.close();
}

