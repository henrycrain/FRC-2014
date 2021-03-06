#include "WPILib.h"
#include "BigBlueBallShooter.h"

BigBlueBallShooter::BigBlueBallShooter(void):
	winderMotor(WINDER_MOTOR_PORT),
	winderLimit(WINDER_LIMIT_PORT),
	lifterMotor(LIFTER_MOTOR_PORT),
	upperLimit(FORK_UPPER_LIMIT_PORT),
	lowerLimit(FORK_LOWER_LIMIT_PORT)
{
	this->pSolenoid1 = new Solenoid(CYLINDER_PORT1); // connect solenoid to proper channel on pneumatics card
//	this->pSolenoid2 = new Solenoid(CYLINDER_PORT2); // connect solenoid to proper channel on pneumatics card
	this->Release();
	this->winderMotor.Set(0);
	this->setMode(FORK_STOPPED);
}
void BigBlueBallShooter::Release()
{
	this->pSolenoid1->Set(false);
//	this->pSolenoid2->Set(true);
}

void BigBlueBallShooter::Engage()
{
	this->pSolenoid1->Set(true);
//	this->pSolenoid2->Set(false);
}
void BigBlueBallShooter::Wind(){
	if (not winderLimit.Get()){
		this->Engage();
		printf("winding\n");
		winderMotor.Set(1.0);
	}
	else {
		printf("hit switch, stopping");
		winderMotor.Set(0);
	}
}
void BigBlueBallShooter::Kick(){
	winderMotor.Set(-1.0);
	Wait(.05);
	this->stopWind();
}
void BigBlueBallShooter::stopWind(){
	winderMotor.Set(0);
}
void BigBlueBallShooter::Shoot()
{
	// Engage winder gear
	printf("shoot sequence\n");
	this->Engage();
	// Wait until the limit switch is pressed
	this->Wind();
	Wait(2.5);
	this->lowerFork();
	Wait(.3);
	this->stopFork();
	while (winderLimit.Get() == 0)
	{
		// wind
		Wait(.01);
	}
//	this->lowerFork();  // figure out what this is for, it seems like its old cold that is useless now 
	this->stopWind();
//	Wait(.25);
//	this->stopFork();
	this->Fire();
}
void BigBlueBallShooter::Fire(){
		// disengage winder gear to shoot
		this->Release();
		// Reverse motor briefly to jar the cylinder loose and allow spring to retract
		this->Kick();
}
void BigBlueBallShooter::raiseFork()
{
	if (this->upperLimit.Get() == 0) {
		this->lifterMotor.Set(-.5);
		this->setMode(FORK_GOING_UP);		
	}
	else {
		this->stopFork();
		this->lifterMotor.Set(0);
		this->setMode(FORK_STOPPED);
	}
}
void BigBlueBallShooter::kickDown(){
	this->lifterMotor.Set(1);
}

void BigBlueBallShooter::lowerFork()
{
	if (this->lowerLimit.Get() == 0) {
		this->lifterMotor.Set(.5);
		this->setMode(FORK_GOING_DN);
	}
	else{
		this->lifterMotor.Set(0);
		this->setMode(FORK_STOPPED);
	}
}
void BigBlueBallShooter::stopFork(){
	this->lifterMotor.Set(0);
	this->setMode(FORK_STOPPED);
}
bool BigBlueBallShooter::returnUpperLimit(){
	return this->upperLimit.Get();
}
bool BigBlueBallShooter::returnLowerLimit(){
	return this->lowerLimit.Get();
}
bool BigBlueBallShooter::returnWinderLimit(){
	return this->winderLimit.Get();
}
void BigBlueBallShooter::setMode(int mode) {
	this->raiseMode = mode;
}

int BigBlueBallShooter::getMode() {
	return this->raiseMode;
}
