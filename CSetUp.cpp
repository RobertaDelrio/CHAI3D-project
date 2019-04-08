//==============================================================================
/*
\author    Roberta Delrio
*/
//==============================================================================

//------------------------------------------------------------------------------
/ -------------------------------------------------------------------------- -
#include "cSetUp.h"
using namespace std;
//---------------------------------------------------------------------------
//------------------------------------------------------------------------------
// DECLARED MACROS
//------------------------------------------------------------------------------
// convert to resource path
#define RESOURCE_PATH(p)    (char*)((a_resourceRoot+string(p)).c_str())


//===========================================================================
/*!
Constructor of cTrial.
*/
//===========================================================================
cSetUp::cSetUp(const std::string a_resourceRoot, std::shared_ptr<cGenericHapticDevice> a_hapticDevice);

{
	// load the experiment conditions
	loadTrial();
	// display is not mirrored
	m_mirroredDisplay = false;

	// initialize tool radius
	//m_cursorRadius = 0.0025;

	// create world
	m_world = new cWorld();;

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

	// create a directional light source
	m_light = new cDirectionalLight(world);

	// insert light source inside world
	world->addChild(light);

	// enable light source
	m_light->setEnabled(true);

	// define direction of light beam
	m_light->setDir(-1.0, 0.0, 0.0);

	// create a sphere (cursor) to represent the haptic device
	m_cursor = new cShapeSphere(0.005);

	// insert cursor inside world
	start = new cShapeSphere(0.02);
	endp = new cShapeSphere(0.02);

	m_world->addChild(start);
	m_world->addChild(endp);

	start->setLocalPos(-0.7, -0.05, 0.0);
	endp->setLocalPos(-0.7, 0.3, 0.0);

	pointColorEnd.setBlueAquamarine();
	endp->m_material->setColor(pointColorEnd);

	pointColorStart.setRedCrimson();
	start->m_material->setColor(pointColorStart);

	m_world->addChild(m_cursor);
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

	labelHapticDeviceModel = new cLabel(font);
	camera->m_frontLayer->addChild(labelHapticDeviceModel);

	// update position of label
	//===========================================================================
	/*!
	Update graphics.
	
	*/
	//===========================================================================

	void cTrial::updateGraphics(int a_width, int a_height)
	{

		// create a label to display the haptic device model
		labelHapticDeviceModel->setText(info.m_modelName);

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
		labelHaptics->setText("LEV:" + cStr(lev, 0)  + "K1 " +
			cStr(K1, 0) + " K2 " + cStr(K2, 0) + " L1 " + cStr(L1, 2));
		// update position of label
		labelHaptics->setLocalPos((int)(0.65 * (a_width - labelRates->getWidth())), 200);
		
	}

	void cTrial::loadTrial()
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
			cout << L1Cond[i] << " ";
			i += 1;
		}
		cout << endl;
		
		//delete &springCondition;
		cout << "Trial Loaded" << endl << endl;
	}

	void cSetUp::updateHaptics()
	{
		// read position 
		cVector3d position;
		//cVector3d positionk = position;
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

}

	void cTrial::initTrial()
	{
		// set starting position of cube and its color
		labelTrialInfo->setText(cStr(trialNumber + 1));
		
		K1 = K1Cond[trialNumber];
		K2 = K2Cond[trialNumber];
		L1 = L1Cond[trialNumber];
	}

	void cTrial::updateProtocol()
	{
		//if (expState < 3)
		//	cout << "exp: " << expState << " trial: " << trialState << "\r";
		switch (expState)
		{
		case 1: // Setup Next Trial
			if (!appendToFile)
			{
				labelTrialInstructions->setText("Bring the cursor to the start position");
			}
			initTrial();
			cout << endl << "starting Trial: " << trialNumber + 1 << endl << endl;
			expState += 1;

			break;
		case 2: // wait for subject to reach starting pos
			if (m_cursor->getLocalPos().x() - downTarget) < 0.01)
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
			if (fabs(m_ODEBody1->getLocalPos().z() - downTarget) < 0.005)
			{
				cout << "start logging" << endl << endl;
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
			if (liftingNumber > 4 && trialState != 3) // show the button for next trial
			{
				cout << 100 << endl << endl;
				if (kVisual == 3)
				{
					box->setShowEnabled(true);
					box2->setShowEnabled(true);
				}
				//expState += 1;
				trialState = 3;
				box->m_material->setBlueNavy();
				m_ODEBody1->m_material->setBlueNavy();
				box->m_material->setBlueNavy();
				m_ODEBody1->m_material->setBlueNavy();
			}
			else
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
				}
				else if (gap < boxSize / 5 && trialState != 3)
				{
					expState = 1;
					//loggingThread->stop();
					//delete &loggingThread;
					loggingRunning = false;
					appendToFile = true;
					cout << "stop logging" << endl;
					labelTrialInstructions->setText("Cube Broke\n Start over");
					//copy the data file and call it bad at the end;
				}
			switch (trialState)
			{
			case 1: //midlift
				if (m_ODEBody1->getLocalPos().z() > upTarget)
					trialState += 1;
				break;
			case 2:
			{
				if (m_ODEBody1->getLocalPos().z() < downTarget)
				{
					trialState -= 1;
					liftingNumber += 1;
					labelTrialInstructions->setText(cStr(liftingNumber));
					cout << endl << "lift: " << liftingNumber << endl;
				}
				break;
			}
			case 3:
			{
				if (m_ODEBody1->getLocalPos().z() < boxSize / 2 + 0.0003)
					expState += 1;
				labelTrialInstructions->setText("Let go of the Cube");
				break;
			}
			}
			break;
		case 5:
			trialNumber += 1;
			expState = 1;
			loggingRunning = false;
			appendToFile = false;
			break;
		}
	}

	void cTrial::updateLogging(void)
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
				<< "cube_x\t" << "cube_y\t" << "cube_z\t"
				<< "force_x\t" << "force_y\t" << "force_z\t" << "force_grip\t"
				<< "lift_number\t" << "kSpring\t" << "kHandle\t" << "kBoundary\t" << "kVisual" << endl;
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
				<< m_tool0->m_hapticPointFinger->m_sphereProxy->getLocalPos().x() << "\t" << m_tool0->m_hapticPointFinger->m_sphereProxy->getLocalPos().y() << "\t" << m_tool0->m_hapticPointFinger->m_sphereProxy->getLocalPos().z() << "\t"
				<< m_tool0->m_hapticPointFinger->m_sphereGoal->getLocalPos().x() << "\t" << m_tool0->m_hapticPointFinger->m_sphereGoal->getLocalPos().y() << "\t" << m_tool0->m_hapticPointFinger->m_sphereGoal->getLocalPos().z() << "\t"
				<< m_tool0->m_hapticPointThumb->m_sphereProxy->getLocalPos().x() << "\t" << m_tool0->m_hapticPointThumb->m_sphereProxy->getLocalPos().y() << "\t" << m_tool0->m_hapticPointThumb->m_sphereProxy->getLocalPos().z() << "\t"
				<< m_tool0->m_hapticPointThumb->m_sphereGoal->getLocalPos().x() << "\t" << m_tool0->m_hapticPointThumb->m_sphereGoal->getLocalPos().y() << "\t" << m_tool0->m_hapticPointThumb->m_sphereGoal->getLocalPos().z() << "\t"
				<< m_ODEBody1->getLocalPos().x() << "\t" << m_ODEBody1->getLocalPos().y() << "\t" << m_ODEBody1->getLocalPos().z() << "\t"
				<< m_tool0->getDeviceGlobalForce().x() << "\t" << m_tool0->getDeviceGlobalForce().y() << "\t" << m_tool0->getDeviceGlobalForce().z() << "\t" << m_tool0->getGripperForce() << "\t"
				<< liftingNumber << "\t" << kSpring << "\t" << kHandle << "\t" << kBoundary << "\t" << kVisual << endl;
		}
		trialFile.close();
	}

