//
//  GenericJoystick.h
//  emptyExample
//
//  Created by Oriol Ferrer Mesià on 07/02/15.
//
//

#ifndef __GenericJoystick__
#define __GenericJoystick__

#include "ofMain.h"

#if (_MSC_VER)
	#include <GLFW/glfw3.h>
#else
	#include "GLFW/glfw3.h"
#endif

#ifndef CHECK_AND_RETURN
#define CHECK_AND_RETURN(A) if(joyID >=0) return A; else return 0;
#endif

class GenericJoystick{

public:

	GenericJoystick(){
		joyID = -1;
		axis = NULL;
		buttons = NULL;
		nButtons = 0;
		nAxis = 0;
	}


	void setup(){
		for(int i = 0; i < GLFW_JOYSTICK_LAST; i++){
			if(glfwJoystickPresent(i)){
				if( string(glfwGetJoystickName(i)) == USB_ID){
					ofLogNotice("GenericJoystick") << "found Joystick with ID: '" << USB_ID <<
					"' at index " << i << endl;
					joyID = i;
					axis = glfwGetJoystickAxes(i, &nAxis);
					buttons = glfwGetJoystickButtons(i, &nButtons);
				}
			}
		}
	};


	void update(){
		if(joyID >= 0){
			glfwJoystickPresent(joyID);
		}
	}

	// axis

	bool getButtonAtIndex(int index){
		if(index < nButtons && index >= 0){
			return buttons[index] > 0;
		}
		return false;
	}

	//returns [-1..1]
	float getAxisAtIndex(int index){
		if(index < nAxis && index >= 0){
			return axis[index];
		}
		return 0.0f;
	}

protected:

	string USB_ID;
	int joyID;

	const float* axis;
	const unsigned char * buttons;

	int nButtons;
	int nAxis;
};

#endif
