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
	m_camera->set(cVector3d(0.7, 0.0, 0.0),    // camera position (eye)
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
	m_cursor = new cShapeSphere(0.008);
	m_world->addChild(m_cursor);

	// insert cursor inside world
	start = new cShapeSphere(0.01);
	endp = new cShapeSphere(0.01);

	m_world->addChild(start);
	m_world->addChild(endp);
	
	//TO CHECK: Coordinates of the sphere x is actually z --> Change protocol threshold check
	start->setLocalPos(0.0, -0.05, 0.0);
	endp->setLocalPos(0.0, 0.04, 0.0);
	//start->setUseTransparency(true,false);
	//start->setTransparencyLevel(0.9,false,false,false);

	cVector3d startPosition;
	//start->setLocalPos(startPosition);
	cVector3d endPosition;
	//endp->setLocalPos(endPosition);

	pointColorEnd.setGrayGainsboro();
	endp->m_material->setColor(pointColorEnd);

	pointColorStart.setGrayGainsboro();
	start->m_material->setColor(pointColorStart);


	// create a font
	font1 = NEW_CFONTCALIBRI20();
	font2 = NEW_CFONTCALIBRI40();
	// create a label to display the haptic and graphic rate of the simulation
	labelRates = new cLabel(font1);
	m_camera->m_frontLayer->addChild(labelRates);
	labelRates->setShowEnabled(true);

	labelHaptics = new cLabel(font2);
	labelHaptics->setShowEnabled(true);
	m_camera->m_frontLayer->addChild(labelHaptics);

	labelTrialInfo = new cLabel(font2);
	labelTrialInfo->m_fontColor = cColorf(1.0, 0.0, 0.0);
	m_camera->m_frontLayer->addChild(labelTrialInfo);

	labelTrialInstructions = new cLabel(font1);
	labelTrialInstructions->m_fontColor = cColorf(0.0, 0.0, 1.0);
	m_camera->m_frontLayer->addChild(labelTrialInstructions);

	// create a level to display velocity data
	levelVelocity = new cLevel();
	m_camera->m_frontLayer->addChild(levelVelocity);
	levelVelocity->setLocalPos(600, 300);
	levelVelocity->setRange(0.0, 0.3);
	levelVelocity->setWidth(80);
	levelVelocity->setNumIncrements(30);
	levelVelocity->setSingleIncrementDisplay(false);
	levelVelocity->setTransparencyLevel(0.5);

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
	//labelTrialInstructions->setLocalPos((int)(0.4 * (a_width - labelTrialInfo->getWidth())), 0.8*a_height - labelTrialInfo->getHeight());
	// update position of label
	labelTrialInfo->setLocalPos((int)(0.5 * (a_width - labelTrialInfo->getWidth())), 0.9*a_height - labelTrialInfo->getHeight());

	// update haptic and graphic rate data
	//labelRates->setText(cStr(freqCounterGraphics.getFrequency(), 0) + " Hz / " + cStr(freqCounterHaptics.getFrequency(), 0) + " Hz");
	// update position of label
	labelRates->setLocalPos((int)(0.5 * (a_width - labelRates->getWidth())), 100);

	// update position of label
	if (trialNumber <= 15) {

	labelHaptics->setText("LEV: " + cStr(lev, 0)+ "          SCORE: "+ cStr(double(score), 0)+"%");
	}
	else {
		labelHaptics->setText("SCORE: " + cStr(double(score), 0) + "%");
	}
	
	// update position of label
	labelHaptics->setLocalPos((int)(0.5 * (a_width - labelHaptics->getWidth())), 0.75 *a_height - labelHaptics->getHeight());

	// update velocity information to level
	levelVelocity->setValue(hapticDeviceVelocity.length());

	if (hapticDeviceVelocity.length() > 0.17) {
		levelVelocity->m_colorActive.setRedLightCoral();
	}
	else {
		levelVelocity->m_colorActive.setBlueAquamarine();
	}
	
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
	m_world->computeGlobalPositions(true);

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
	//m_cursor->setLocalPos(position.z(), position.y(), -position.x());
	m_cursor->setLocalPos(position.z(), position.y(), -position.x());
	m_cursor->setLocalPos(0.71*position.z()+0.71*position.x(), position.y(), 0.71*position.z() -0.71*position.x());
	/////////////////////////////////////////////////////////////////////
	// COMPUTE FORCES
	/////////////////////////////////////////////////////////////////////

	cVector3d force(0, 0, 0);
	cVector3d forceGlob(0, 0, 0);
	cVector3d torque(0, 0, 0);
	cVector3d y(0, 1, 0);
	cVector3d p(0, 0, 0);
	cMatrix3d posRot;
	cMatrix3d forceRot;
	posRot.set(0.0);
	posRot.setCol0(position);
	posRot.rotateAboutGlobalAxisDeg(y, -45);
	p = posRot.getCol0();

	double gripperForce = 0.0;
	double Kp = 0;

	// read linear velocity 
	cVector3d linearVelocity;
	a_hapticDevice->getLinearVelocity(linearVelocity);

	// read angular velocity
	cVector3d angularVelocity;
	a_hapticDevice->getAngularVelocity(angularVelocity);

	/////////////////////////////////////////////////////////////////////
	// APPLY FORCES
	/////////////////////////////////////////////////////////////////////

	if (p.z() < 0.0 && p.z() > -L1) {
		// compute linear force
		Kp = K1* p.z(); // [N/m]
							   //printf("LEV1");
		lev = 1;
	}

	else if (p.z() < -L1) {
		// compute linear force
		Kp = K2*p.z(); // [N/m]
							  //printf("LEV2");
		lev = 2;

	}
	else {

		// compute linear force
		Kp = -0; // [N/m]
		lev = 0;
	}
	//SMOOTH TRANSITION
	/*
	if (position.z() < 0.0 && position.z() > -L1) {
	// compute linear force
	Kp = K1* position.z(); // [N/m]
	//printf("LEV1 \n");
	lev = 1;
	}

	else if (position.z() < -L1) {
	// compute linear force
	Kp = -L1*(K1 + K2) + K2*(position.z() + L1); // [N/m]
	//printf("LEV2 \n");
	lev = 2;

	}
	else{

	// compute linear force
	Kp = -0; // [N/m]
	//printf("LEV0 \n");
	lev = 0;
	}
	*/
	double forceField = -Kp;
	force.z(forceField);

	
	forceRot.set(0.0);
	forceRot.setCol0(force);
	forceRot.rotateAboutGlobalAxisDeg(y, 45);
	forceGlob = forceRot.getCol0();

	cout << " Trial:" << trialNumber <<  "\r";

	// update global variable for graphic display update
	hapticDeviceVelocity = linearVelocity;
	// send computed force, torque, and gripper force to haptic device
	a_hapticDevice->setForceAndTorqueAndGripperForce(forceGlob, torque, gripperForce);
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
	
	
	K1 = K1Cond[trialNumber];
	K2 = K2Cond[trialNumber];
	L1 = L1Cond[trialNumber];
}

void cSetUp::updateProtocol()
{
	/*cVector3d position = m_cursor->getLocalPos();
	cVector3d startPosition = start->getLocalPos();
	cVector3d endPosition = endp->getLocalPos();*/
	m_world->computeGlobalPositions(true);

	cVector3d position = m_cursor->getGlobalPos();
	cVector3d startPosition = start->getGlobalPos();
	cVector3d endPosition = endp->getGlobalPos();

	switch (expState) {
	case 1:{
		//printf("GO TO THE START POSITION\n");
		if (trialNumber == 30){

			labelTrialInfo->setText("End of Training");
			expState = 1;
		}
		else {

			initPilot();
			expState += 1;
		}
		

		
		//endp->m_material->setColor(pointColorEnd);

		start->m_material->setColor(pointColorStart);

		break;
	}
	case 2:{

		double ab_x = position.y() - startPosition.y();
		double ab_y = position.z() - startPosition.z();
		//double ab_x = position.x();
		//double ab_y = position.y();
		double ab_z = position.z();
		//cout << "    abs X:" << setprecision(2) << ab_x << "    abs Y:" << setprecision(2) << ab_y <<"      abs_Z:" << setprecision(2) << ab_z << "\r";

		if (fabs(position.z() - startPosition.z()) < 0.004 && fabs(position.y() - startPosition.y()) < 0.005 && lev == 0)
		{
			//printf("START REACHED\n");
			//score = 0;
			start->m_material->setGreenMediumAquamarine();
			endp->m_material->setColor(pointColorEnd);
			loggingRunning = true;
			expState += 1;
		}
		
		else
		{
			start->m_material->setColor(pointColorStart);
		}

		break;
	}
	case 3:{
		if (fabs(position.z() - endPosition.z()) < 0.005 && fabs(position.y() - endPosition.y()) < 0.005)
		{
			//printf("GOAL REACHED\n");
			loggingRunning = false;
			endp->m_material->setRedCrimson();
			//appendToFile = true;
			expState += 1;
		}
		else {
		
			if (m_cursor->getLocalPos().x() < min) {

				min = m_cursor->getLocalPos().x();
			}
			
		}
		
		break;

	}
	case 4: {
		if (lev == 0)
		{
			
			endp->m_material->setColor(pointColorEnd);
			error = min + (L1)+0.005;
			errop = (error * 100) / double(L1);
			score = 100 + errop;
			if (error > 0 || score < 0) {
				score = 0;
			}

			final_score[trialNumber] = score;
			//appendToFile = true;
			expState += 1;
		}

		break;

	}
	case 5: {
		//printf("END\n");
		loggingRunning = false;
		//appendToFile = false;
		expState = 1;
		trialNumber++;
		break;
	}


	}
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
			<< "cursor_x\t" << "cursor_y\t" << "cursor_z\t"
			<< "K1\t" << "K2\t" << "L1\t" << "Fz\t" << "v_x\t" << "v_y\t" << "v_z\t" << endl;
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
			<< K1 << "\t" << K2 << "\t" << L1 << "\t" << Fz << "\t" << hapticDeviceVelocity.x() << "\t" << hapticDeviceVelocity.y() << "\t" << hapticDeviceVelocity.z() << "\t" << endl;
	}

	trialFile.close();
}

