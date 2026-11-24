//
//  ofxGLFWJoystick.h
//  emptyExample
//
//  Created by Oriol Ferrer Mesià on 07/02/15.
//
//

#ifndef __emptyExample__ofxGLFWJoystick__
#define __emptyExample__ofxGLFWJoystick__

#include "ofMain.h"
#ifdef OFX_GLFW_JOYSTICK_USE_SDL
#include "ofxSDLController.h"
#endif

#if (_MSC_VER)
	#include <GLFW/glfw3.h>
#else
	#include "GLFW/glfw3.h"
#endif


class ofxGLFWJoystick{

public:
	enum class Provider { GLFW, SDL, BOTH };
	// Selection is applied on update(); false means SDL support was not built in.
	bool setProvider(Provider provider);
	Provider getProvider() const { return provider; }

	void setup(){};
	void update();

	static ofxGLFWJoystick& one(){
		static ofxGLFWJoystick instance; // Instantiated on first use.
		instance.initialize();
		return instance;
	}


	void printJoystickList();
	map<int,string> getJoystickList();


	int getNumJoysticksAvailable(){ return numJoysticks;};
	bool isJoystickAvailable(int joyID);
	string getJoystickName(int joyID);
	string getJoystickProvider(int joyID);

	void drawDebug(int x, int y);

	//safe access
	float getAxisValue(int axisID, int joyID);
	unsigned char getButtonValue(int axisID, int joyID);

	//direct access, careful with the pointers/indexes or you will crash!
	int getNumButtons(int joyID);
	int getNumAxis(int joyID);
	const unsigned char * getButtonsForJoystick(int joyID);
	const float * getButtonsForAxis(int joyID);

private:

	ofxGLFWJoystick();
	void initialize();
	bool activateProvider(Provider requested);
	void lookForJoysticks();
	void clearJoysticks();
#ifdef OFX_GLFW_JOYSTICK_USE_SDL
	void updateSDL();
	ofxSDLController sdl;
#endif
	Provider provider = Provider::GLFW;
	#ifdef OFX_GLFW_JOYSTICK_USE_SDL
	Provider requestedProvider = Provider::BOTH;
	#else
	Provider requestedProvider = Provider::GLFW;
	#endif

	struct JoyData{
		bool available;
		string name;
		int numButtons;
		const unsigned char * buttonData;
		int numAxis;
		const float * axisData;
		int instanceID = -1;
		Provider source = Provider::GLFW;
		vector<float> sdlAxes;
		vector<unsigned char> sdlButtons;
		JoyData(){
			buttonData = NULL; axisData = NULL;
			numButtons = numAxis = 0;
			available = false;
		}

	};

	int numJoysticks;
	bool initialized = false;
	bool updating = false;
	bool providerReady = false;
	static constexpr int slotsPerProvider = GLFW_JOYSTICK_LAST + 1;
	static constexpr int totalSlots = slotsPerProvider * 2;
	JoyData joyData[totalSlots];
};

#endif /* defined(__emptyExample__ofxGLFWJoystick__) */
