#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S3,     IRS_L,          sensorI2CCustom)
#pragma config(Sensor, S4,     armStop,        sensorTouch)
#pragma config(Motor,  motorA,          mFlagRaise1,   tmotorNXT, openLoop)
#pragma config(Motor,  motorB,          mFlagRaise2,   tmotorNXT, openLoop)
#pragma config(Motor,  motorC,          mBlockStop,    tmotorNXT, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_1,     mDriveLeft,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     mDriveRight,   tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     mBsConveyor,   tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     mBsAngle,      tmotorTetrix, openLoop, reversed, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

int motorUsing = 0;
string motorActive = "";
task main()
{
	ClearTimer(T1);
	nxtDisplayCenteredTextLine(0, "After Match");
	nxtDisplayTextLine(2, "   DOWN     UP");
	nxtDisplayBigTextLine(3, "  <   >");
	while(true){
			if(motorUsing == 0)
				motorActive = "Sucker";
			else if(motorUsing == 1)
				motorActive = "Arm";
			else
				motorActive = "Flag";
			nxtDisplayTextLine(1, "%s", motorActive);
			nxtDisplayTextLine(5, "Sucker:%d", motor[mBsConveyor]);
			nxtDisplayTextLine(6, "Lift:%d", motor[mBsAngle]);
			nxtDisplayTextLine(7, "Flag:1(%d)2(%d)", motor[mFlagRaise1], motor[mFlagRaise2]);
			if(nNxtButtonPressed == 2){
				if(motorUsing == 0)
					motor[mBsConveyor] = -100;
				else if(motorUsing == 1 && SensorValue[armStop] != 1)
					motor[mBsAngle] = -100;
				else{
					motor[mFlagRaise1] = -100;
					motor[mFlagRaise2] = -100;
				}
			}else if(nNxtButtonPressed == 1){
				if(motorUsing == 0)
					motor[mBsConveyor] = 100;
				else if(motorUsing == 1)
					motor[mBsAngle] = 100;
				else{
					motor[mFlagRaise1] = 100;
					motor[mFlagRaise2] = 100;
				}
			}else{
				motor[mBsAngle] = 0;
				motor[mBsConveyor] = 0;
				motor[mFlagRaise1] = 0;
				motor[mFlagRaise2] = 0;
			}

			if(nNxtButtonPressed == 3){
				if(motorUsing == 0)
					motorUsing = 1;
				else if(motorUsing == 1)
					motorUsing = 2;
				else
					motorUsing = 0;
				wait10Msec(50);
				PlaySound(soundBlip);
			}
	}


}
