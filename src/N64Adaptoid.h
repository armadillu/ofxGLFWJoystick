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

	float getXaxis(){return axis[0];}
	float getYaxis(){return axis[1];}

	//buttons

	bool getStart(){return buttons[8] > 0;}
	bool getA(){return buttons[0] > 0;}
	bool getB(){return buttons[3] > 0;}
	bool getCLeft(){return buttons[4] > 0;}
	bool getCRight(){return buttons[2] > 0;}
	bool getCUp(){return buttons[5] > 0;}
	bool getCDown(){return buttons[1] > 0;}
	bool getLShoulder(){return buttons[6] > 0;}
	bool getRShoulder(){return buttons[7] > 0;}
	bool getZTrigger(){return buttons[9] > 0;}
	bool getDpadLeft(){return buttons[12] > 0;}
	bool getDpadRight(){return buttons[13] > 0;}
	bool getDpadUp(){return buttons[10] > 0;}
	bool getDpadDown(){return buttons[11] > 0;}


private:

		const float* axis;
		const unsigned char * buttons;
		int joyID;


};

#endif
