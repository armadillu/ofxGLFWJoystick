//
//  N64Adaptoid.h
//
//  Created by Oriol Ferrer Mesià on 07/02/15.
//
//

#ifndef __N64Adaptoid__
#define __N64Adaptoid__

#include "GenericJoystick.h"

class N64Adaptoid : public GenericJoystick{

public:

	N64Adaptoid(){
		GenericJoystick();
		USB_ID = "Gamepad F310";
	}

	// axis
	float getXAxis(){CHECK_AND_RETURN(axis[0]);}
	float getYAxis(){CHECK_AND_RETURN(axis[1]);}

	//buttons
	bool getStart(){CHECK_AND_RETURN(buttons[8] > 0);}
	bool getA(){CHECK_AND_RETURN(buttons[0] > 0);}
	bool getB(){CHECK_AND_RETURN(buttons[3] > 0);}
	bool getCLeft(){CHECK_AND_RETURN(buttons[4] > 0);}
	bool getCRight(){CHECK_AND_RETURN(buttons[2] > 0);}
	bool getCUp(){CHECK_AND_RETURN(buttons[5] > 0);}
	bool getCDown(){CHECK_AND_RETURN(buttons[1] > 0);}
	bool getLShoulder(){CHECK_AND_RETURN(buttons[6] > 0);}
	bool getRShoulder(){CHECK_AND_RETURN(buttons[7] > 0);}
	bool getZTrigger(){CHECK_AND_RETURN(buttons[9] > 0);}
	bool getDpadLeft(){CHECK_AND_RETURN(buttons[12] > 0);}
	bool getDpadRight(){CHECK_AND_RETURN(buttons[13] > 0);}
	bool getDpadUp(){CHECK_AND_RETURN(buttons[10] > 0);}
	bool getDpadDown(){CHECK_AND_RETURN(buttons[11] > 0);}

};

#endif
