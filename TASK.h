----------------------------------------
#ifndef TASK_H
#define TASK_H

#include <string>
#include <sstream>
#include "chai3d.h"

using namespace chai3d;

class Task
{
public:
	Task() : m_isInitialized(false) { }
	virtual ~Task() { }

	virtual std::string getName() const = 0;

	virtual void initialize(chai3d::cWorld* world, cCamera* camera) = 0;
	virtual void updateGraphics() = 0;
	virtual void updateHaptics(cGenericHapticDevice* hapticDevice, double timeStep, double totalTime) = 0;

	volatile inline bool isInitialized() const { return m_isInitialized; }
	void setInitialized(bool value) { m_isInitialized = value; }


	cVector3d hapticDevicePosition;

private:
	volatile bool m_isInitialized;
};

#endif // TASK_H