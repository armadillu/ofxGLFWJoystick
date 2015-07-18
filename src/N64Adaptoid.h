//
//  N64Adaptoid.h
//  emptyExample
//
//  Created by Oriol Ferrer Mesià on 07/02/15.
//
//

#ifndef __emptyExample__N64Adaptoid__
#define __emptyExample__N64Adaptoid__

#include "ofMain.h"
#include "ofxGLFWJoystick.h"

#if (_MSC_VER)
	#include <GLFW/glfw3.h>
#else
	#include "GLFW/glfw3.h"
#endif

#define CHECK_AND_RETURN(A) if(joyID >=0) A; else return 0;

class N64Adaptoid{

public:

	N64Adaptoid(){
		joyID = -1;
		axis = NULL;
		buttons = NULL;
	}


	void setup(){

		for(int i = 0; i < GLFW_JOYSTICK_LAST; i++){
			if(glfwJoystickPresent(i)){
				if( string(glfwGetJoystickName(i)) == "Adaptoid"){
					joyID = i;
					int nAx = 0;
					axis = glfwGetJoystickAxes(i, &nAx);
					int nBut = 0;
					buttons = glfwGetJoystickButtons(i, &nBut);
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

	float getXaxis(){CHECK_AND_RETURN(axis[0]);}
	float getYaxis(){CHECK_AND_RETURN(axis[1]);}

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


private:

		const float* axis;
		const unsigned char * buttons;
		int joyID;


};

#endif
