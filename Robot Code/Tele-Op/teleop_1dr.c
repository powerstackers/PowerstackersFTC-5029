#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     SMUX,           sensorI2CCustom)
#pragma config(Sensor, S3,     sGyro,          sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     sConvStop,      sensorTouch)
#pragma config(Motor,  motorA,          mFlagRaise1,   tmotorNXT, openLoop)
#pragma config(Motor,  motorB,          mFlagRaise2,   tmotorNXT, openLoop)
#pragma config(Motor,  motorC,          mBlockStop,    tmotorNXT, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_1,     mDriveLeft,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     mDriveRight,   tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     mBsConveyor,   tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     mBsAngle,      tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Servo,  srvo_S1_C3_1,    rConveyorTight,       tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    rBlockDropLeft,       tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    rBlockDropRight,      tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	TELE-OP CODE FOR FTC# 5029
//	2013-2014 BLOCK PARTY
//	UPDATED 8-16-2014
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Include the file to run the motors and servos, and get joystick input
#include "JoystickDriver.c"

// Define constants:
// STICK_TO_MOTOR: Convert the joystick's -128 - +127 value into a -99 - +100 value
// MODE_STRAIGHT/MODE_NORMAL: Replacements for true and false, used for drive modes
#define STICK_TO_MOTOR(x)	((float)x * 0.78125)
#define MODE_STRAIGHT true
#define MODE_NORMAL false

// Define global variables:

// Joysticks:
// stickDriveLeft: The left drive joystick value
// stickDriveRight: The right drive joystick value
// stickAngle: The joystick for the sucker plate
// stickBlockStop: The joystick for the brick gate
short 	stickDriveLeft;
short 	stickDriveRight;
short		stickAngle;
short		stickBlockStop;

// Buttons:
// btnStriaghtDr: The button to activate StraightDrive mode
// btnConveyor: The button to run the conveyor upward
// btnRevers: The button to run the conveyor downward
// btnFlagClock: The button to run the flag raiser clockwise
// btnFlagCounter: The button to run the flag raiser counterclockwise
// btnConveyorTight: The button to activate the conveyor tension servo
bool 		btnStraightDr;
bool		btnConveyor;
bool		btnReverse;
bool		btnFlagClock;
bool		btnFlagCounter;
bool		btnConveyorTight;

bool btnAngleUp;
bool btnAngleDown;

// Boolean flags:
// driveMode: The drive mode (straight or normal)
// brickBlocked: Whether the brick gate is up or not
// blockerMoving: Whether the brick gate is moving or still
bool		driveMode;
//bool		brickBlocked = true;
//bool		blockerMoving = false;

// Constant variables:
// stickThreshold: Joystick values below this number will not be detected
// blockClosedPos: Brick gate closed position
// blockOpenPos: Brick gate open position
// blockOpenThresh: If the brick gate is past this position, it will not close further
// blockClosedThresh: If the brick gate is past this position, it will not open further
// blockSpeed: Motor speed for the brick blocker
const short		stickThreshold = 10;
const int			blockClosedPos = 90;
const int			blockOpenPos = -5;
const int 		blockOpenThresh = -360;
const int 		blockClosedThresh = 360;
const int			blockSpeed = 50;

/////////////////////////////////////////////////////
//
//	Set all motors to the input parameter value
//
/////////////////////////////////////////////////////
void allMotorsTo(int i){
		motor[mDriveLeft] 	= i;
		motor[mDriveRight] 	= i;
		motor[mBsAngle] 		= i;
		motor[mBsConveyor] 	= i;
		motor[mBlockStop] = i;
		motor[mFlagRaise1] = i;
		motor[mFlagRaise2] = i;
}

///////////////////////////////////////////////////////
//
//	Set all drive motors to the input parameter value
//
///////////////////////////////////////////////////////
void driveMotorsTo(int i){
		motor[mDriveLeft] 	= i;
		motor[mDriveRight] 	= i;
}

///////////////////////////////////////////////////
//
//	Set all encoders to the input parameter value
//
///////////////////////////////////////////////////
void allEncodersTo(int i){
	nMotorEncoder[mBlockStop] = i;
	nMotorEncoder[mBsAngle] = i;
	nMotorEncoder[mBsConveyor] = i;
	nMotorEncoder[mDriveLeft] = i;
	nMotorEncoder[mDriveRight] = i;
	writeDebugStreamLine("Encoders initialized");
}

///////////////////////////////////////////////////////////////
//
//	Initialize motor and servo values before the match starts
//
///////////////////////////////////////////////////////////////
void initializeRobot()
{
	allMotorsTo(0);
	allEncodersTo(0);
	servo[rBlockDropLeft] = 128;
	servo[rBlockDropRight] = 128;
	servo[rConveyorTight] = 150;
	PlaySound(soundUpwardTones);
	nxtDisplayCenteredBigTextLine(2, "Tele-Op");
	nxtDisplayCenteredBigTextLine(4, "Ready");
  return;
}

//////////////////////////////////////////////////////////////
//
//	Copy the Joystick values into the custom variables
//
//////////////////////////////////////////////////////////////
void getCustomJoystickSettings(){
	stickDriveLeft 	= joystick.joy1_y1;		// dr1 left joystick
	stickDriveRight = joystick.joy1_y2;		// dr1 right joystick
	stickAngle			= joystick.joy2_y1;		// dr2 left joystick
	stickBlockStop	=	joystick.joy2_y2;		// dr2 right joystick
	btnStraightDr		= (joy1Btn(3) == 1);	// dr1 red button
	//btnFlagClock		= (joy1Btn(5) == 1);	// dr1 left shoulder
	//btnFlagCounter	= (joy1Btn(7) == 1);	// dr1 left trigger
	btnReverse			= (joy1Btn(8) == 1);	// dr2 right trigger
	btnConveyor			=	(joy1Btn(6) == 1);	// dr2 right shoulder
	//btnConveyorTight =(joy2Btn(7) == 1);	// dr2 left trigger
	btnAngleUp = (joy1btn(5) == 1);
	btnAngleDown = (joy1btn(7) == 1);
}

//////////////////////////////////////////////////////////////
//
//	Print custom information to the NXT screen
//
/////////////////////////////////////////////////////////////
void displayButtonValues(){
  	nxtDisplayTextLine(1, "blockStop enc: %d",	nMotorEncoder[mBlockStop]);// Right drive joystick
		nxtDisplayTextLine(3, "stickBlSt:%d", stickBlockStop);						// Brick stopper button
		nxtDisplayTextLine(4, "mtrBlock:%d", motor[mBlockStop]);
  	nxtDisplayTextLine(5, "bsAngle:%d", nMotorEncoder[mBsAngle]);			// Encoder on NXT motor
		nxtDisplayTextLine(6, "stickAngle:%d", stickAngle);								// Brick sucker joystick
}

///////////////////////////////////////////////
//
//	MAIN TASK
//
//////////////////////////////////////////////
task main(){
	// Do some housekeeping to start:
	// Turn off the diagnotstic display on the NXT screen, to make way for our own custom info
	// Clear the debug stream
	// Clear the NXT screen
	// Initialize the robot motors and sensors
	// Clear the timer
	// Set the time since last good signal before an error is thrown
	bDisplayDiagnostics = false;
  clearDebugStream();
	eraseDisplay();
  initializeRobot();
  ClearTimer(T1);
  nNoMessageCounterLimit = 500;

  // Wait for the match to begin
  waitForStart();
  nxtDisplayCenteredBigTextLine(4, "Running");
  PlaySound(soundFastUpwardTones);

  // Loop forever
  while (true){
  	// Get the regular joystick information
  	// Get the custom joystick information
  	getJoystickSettings(joystick);
  	getCustomJoystickSettings();

  	// If the robot loses connection, play and obnoxious sound
  	// And print a big, scary "LOST CONNECTION" to the NXT screen
  	// Until the robot regains connection
  	if(bDisconnected){
  		allMotorsTo(0);
			while(bDisconnected){
				nxtDisplayCenteredBigTextLine(2, "LOST");
				nxtDisplayCenteredBigTextLine(4, "CONNECT");
				nxtDisplayCenteredBigTextLine(6, "-ION");
				PlaySound(soundException);
			}
			ClearSounds();
			nxtDisplayCenteredBigTextLine(2, "Tele-Op");
			nxtDisplayCenteredBigTextLine(4, "Running");
			nxtDisplayCenteredBigTextLine(6, "");
			PlaySound(soundFastUpwardTones);
		}
		// Straight drive mode:
		// If the straight drive button is pressed, set the drive mode to Straight
		// If the straight drive button is not pressed, set the drive mode to Normal
		if(btnStraightDr){
			driveMode = MODE_STRAIGHT;
		}else{
			driveMode = MODE_NORMAL;
		}

		// If the drive mode is set to straight, move both motors at the same speed
		if (driveMode == MODE_STRAIGHT){
			if(abs(stickDriveLeft) > stickThreshold){
					driveMotorsTo(STICK_TO_MOTOR(stickDriveLeft));
			}else{
				driveMotorsTo(0);
			}
		}

		// Normal drive mode:
		else if(driveMode == MODE_NORMAL){

			// If the left stick is above the threshold, move the motor
			// At a speed proportional to the value of the joystick
			if(abs(stickDriveLeft) < stickThreshold){
				motor[mDriveLeft] = 0;
			}
			// If the left stick is below the threshold, stop the motor
			else{
				motor[mDriveLeft] = STICK_TO_MOTOR(stickDriveLeft);
			}

			// If the right stick is above the threshold, move the motor
			// At a speed proportional to the value of the joystick
			if(abs(stickDriveRight) < stickThreshold){
				motor[mDriveRight] = 0;
			}
			// If the right stick is below the threshold, stop the motor
			else{
				motor[mDriveRight] = STICK_TO_MOTOR(stickDriveRight);
			}
		}

		// If the conveyor button is pressed, move the conveyor motors upward
		if(btnConveyor){
			motor[mBsConveyor] = 100;
		}
		// If the conveyor REVERSE button is pressed, move the conveyor downward
		else if(btnReverse){
			motor[mBsConveyor] = -100;
		}
		// If neither of those buttons are pressed, stop the motors
		else{
			motor[mBsConveyor] = 0;
		}

		// If the sucker plate stick is below the threshold, do not move the motors
		if(!btnAngleDown && !btnAngleUp)
			{
			motor[mBsAngle] = 0;
		}

		// If the sucker plate stick is above the threshold,
		// And the stop sensor is not activated, move the motors
		// If the stop sensor is activated, stop the motors
		// This keeps the sucker plate from going down too far and damaging the gears
		else
			{
			if(btnAngleDown && SensorValue[sConvStop] == 1){
				motor[mBsAngle] = 0;
				nMotorEncoder[mBsAngle] = 0;
			}else
				motor[mBsAngle] = ((btnAngleUp)? 100 : ((btnAngleDown)? -100 : 0));
		}

		// If the block gate stick is below the threshold, do not move the motors
		// If it is above the threshold, move it up or down
		// Do not let it go past certain values
		// This keeps it from turning too far and damaging the motors
		if(abs(stickBlockStop) < stickThreshold){
			motor[mBlockStop] = 0;
		}else if(nMotorEncoder[mBlockStop] >= blockOpenThresh && stickBlockStop > 0){
				if(nMotorEncoder[mBlockStop] < blockClosedPos){
					motor[mBlockStop] = blockSpeed;
				}else{
					motor[mBlockStop] = 0;
				}
		}else if(nMotorEncoder[mBlockStop] <= blockClosedThresh && stickBlockStop < 0){
			if(nMotorEncoder[mBlockStop] > blockOpenPos){
				motor[mBlockStop] = blockSpeed * -1;
			}else{
				motor[mBlockStop] = 0;
			}
		}else{
			motor[mBlockStop] = 0;
		}

		// If the flag clockwise button is pressed, move the flag raiser clockwise
		// If the counterclockwise button is pressed, move the flag counterclockwise
		// If neither button is pressed, stop the motor
		if(btnFlagClock){
			if(!btnFlagCounter){
				motor[mFlagRaise1] = 100;
				motor[mFlagRaise2] = 100;
			}
		}else if(btnFlagCounter){
			motor[mFlagRaise1] = -100;
			motor[mFlagRaise2] = -100;
		}else{
			motor[mFlagRaise1] = 0;
			motor[mFlagRaise2] = 0;
		}

		// If the conveyor tension button is pressed, extend the servo
		// If the button is not pressed, retract the servo
		if(btnConveyorTight){
			servo[rConveyorTight] = 170;
		}else{
			servo[rConveyorTight] = 150;
		}

		// If the sucker plate is above a certain height,
		// Move the flipper servos out of the way
		if(nMotorEncoder[mBsAngle] > 3000){
			servo[rBlockDropLeft] = 118;
			servo[rBlockDropRight] = 128;
		}else{
			servo[rBlockDropLeft] = 32;
			servo[rBlockDropRight] = 200;
		}
	}
}