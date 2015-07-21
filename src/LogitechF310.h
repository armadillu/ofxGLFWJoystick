//
//  LogitechF310.h
//
//  Created by Oriol Ferrer Mesià on 07/02/15.
//
//

//on OSX, get this driver: https://github.com/d235j/360Controller/releases/tag/v0.14-unofficial
//and run in "X" mode (not "D") - look at the switch on the back

#ifndef __LogitechF310__
#define __LogitechF310__

#include "GenericJoystick.h"

class LogitechF310 : public GenericJoystick{

public:

	LogitechF310(){
		GenericJoystick();
		USB_ID = "Gamepad F310";
	}

	// axis
	float getXAxisL(){CHECK_AND_RETURN(axis[0]);}
	float getYAxisL(){CHECK_AND_RETURN(axis[1]);}
	float getXAxisR(){CHECK_AND_RETURN(axis[2]);}
	float getYAxisR(){CHECK_AND_RETURN(axis[3]);}
	float getLTrigger(){CHECK_AND_RETURN(axis[4]);}
	float getRTrigger(){CHECK_AND_RETURN(axis[5]);}

	//buttons
	bool getDpadUp(){CHECK_AND_RETURN(buttons[0] > 0);}
	bool getDpadDown(){CHECK_AND_RETURN(buttons[1] > 0);}
	bool getDpadLeft(){CHECK_AND_RETURN(buttons[2] > 0);}
	bool getDpadRight(){CHECK_AND_RETURN(buttons[3] > 0);}

	bool getStart(){CHECK_AND_RETURN(buttons[4] > 0);}
	bool getBack(){CHECK_AND_RETURN(buttons[5] > 0);}

	//press left joystik down as a button
	bool getLeftPushableJoystickL(){CHECK_AND_RETURN(buttons[6] > 0);}

	//press right joystik down as a button
	bool getLeftPushableJoystickR(){CHECK_AND_RETURN(buttons[7] > 0);}

	bool getLB(){CHECK_AND_RETURN(buttons[8] > 0);}
	bool getRB(){CHECK_AND_RETURN(buttons[9] > 0);}

	bool getLogitechB(){CHECK_AND_RETURN(buttons[10] > 0);}

	bool getA(){CHECK_AND_RETURN(buttons[11] > 0);}
	bool getB(){CHECK_AND_RETURN(buttons[12] > 0);}
	bool getX(){CHECK_AND_RETURN(buttons[13] > 0);}
	bool getY(){CHECK_AND_RETURN(buttons[14] > 0);}



};

#endif
