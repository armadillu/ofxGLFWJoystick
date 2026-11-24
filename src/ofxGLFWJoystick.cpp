//
//  ofxGLFWJoystick.cpp
//  emptyExample
//
//  Created by Oriol Ferrer Mesià on 07/02/15.
//
//

#include "ofxGLFWJoystick.h"

namespace {
struct UpdateGuard {
	bool& updating;
	explicit UpdateGuard(bool& flag) : updating(flag) { updating = true; }
	~UpdateGuard() { updating = false; }
};
}

ofxGLFWJoystick::ofxGLFWJoystick(){

	numJoysticks = 0;
	// GLFW discovery runs after construction: it can dispatch a nested draw.
	//this triggers a deadlock on OSX... sad! we'll need to manually update this...
	//ofAddListener(ofEvents().update, &one(), &ofxGLFWJoystick::update, OF_EVENT_ORDER_BEFORE_APP);
}

void ofxGLFWJoystick::initialize(){
	if (initialized) return;
	initialized = true;
}

void ofxGLFWJoystick::clearJoysticks(){
	for (int i = 0; i < totalSlots; ++i) joyData[i] = JoyData();
	numJoysticks = 0;
}

bool ofxGLFWJoystick::setProvider(Provider requested){
	if (updating) return false;
#ifndef OFX_GLFW_JOYSTICK_USE_SDL
	if (requested == Provider::SDL || requested == Provider::BOTH) {
		ofLogError("ofxGLFWJoystick") << "SDL provider not enabled. See the addon README.";
		return false;
	}
#endif
	requestedProvider = requested;
	return true;
}

bool ofxGLFWJoystick::activateProvider(Provider requested){
#ifdef OFX_GLFW_JOYSTICK_USE_SDL
	if (requested != Provider::GLFW) {
		if (!sdl.setup()) return false;
	} else {
		sdl.exit();
	}
#else
	if (requested != Provider::GLFW) return false;
#endif
	clearJoysticks();
	provider = requested;
	providerReady = true;
	return true;
}


void ofxGLFWJoystick::printJoystickList(){

	map<int,string> list = getJoystickList();
	if(list.size() == 0){
		ofLogNotice("ofxGLFWJoystick") << "No Joysticks Found!";
	}
	map<int,string>::iterator it = list.begin();
	while(it != list.end()){
		ofLogNotice("ofxGLFWJoystick") << it->second << " >> ID " << it->first;
		++it;
	}
}


map<int,string> ofxGLFWJoystick::getJoystickList(){

	map<int,string> list;
	for(int i = 0; i < totalSlots; i++)
		if (joyData[i].available) list[i] = joyData[i].name;
	return list;
}


void ofxGLFWJoystick::lookForJoysticks(){

	int n = 0;
	for(int i = 0; i <= GLFW_JOYSTICK_LAST; i++){
		if(glfwJoystickPresent(i)){
			joyData[i].available = true;
			string name = string(glfwGetJoystickName(i));
			if(name != joyData[i].name){
				joyData[i].name = name;
				ofLogNotice("ofxGLFWJoystick") << "Joystick Found at index " << i << ": '" << name << "'";
				joyData[i].axisData = glfwGetJoystickAxes(i, &joyData[i].numAxis);
				joyData[i].buttonData = glfwGetJoystickButtons(i, &joyData[i].numButtons);
			}
			n++;
		}else{
			if (joyData[i].name.size()){
				ofLogNotice("ofxGLFWJoystick") << "Joystick Lost at index " << i << ": '" << joyData[i].name << "'";
			}
			joyData[i].available = false;
			joyData[i].name = "";
		}
	}
	numJoysticks = n;
}


void ofxGLFWJoystick::update(){
	if (updating) return;
	UpdateGuard guard(updating);
	if (!providerReady || requestedProvider != provider) {
		if (!activateProvider(requestedProvider)) {
			ofLogError("ofxGLFWJoystick") << "Could not initialize the requested joystick provider";
			requestedProvider = provider;
			if (!providerReady && !activateProvider(Provider::GLFW)) return;
		}
	}
	if (provider != Provider::SDL) {
		lookForJoysticks();
		for(int j = 0; j <= GLFW_JOYSTICK_LAST; j++){
		joyData[j].available = glfwJoystickPresent(j);
		if(!joyData[j].available){
			joyData[j].axisData = NULL;
			joyData[j].buttonData = NULL;
			joyData[j].numAxis = 0;
			joyData[j].numButtons = 0;
		}else {
			joyData[j].axisData = glfwGetJoystickAxes(j, &joyData[j].numAxis);
			joyData[j].buttonData = glfwGetJoystickButtons(j, &joyData[j].numButtons);
		}
	}
	}
#ifdef OFX_GLFW_JOYSTICK_USE_SDL
	if (provider != Provider::GLFW) updateSDL();
#endif
}

#ifdef OFX_GLFW_JOYSTICK_USE_SDL
void ofxGLFWJoystick::updateSDL(){
	sdl.update();
	const int first = provider == Provider::BOTH ? slotsPerProvider : 0;
	const int end = first + slotsPerProvider;
	// Keep IDs stable across hot-plugging, rather than using SDL's compact list index.
	for (int slot = first; slot < end; ++slot) {
		if (!joyData[slot].available) continue;
		bool found = false;
		for (int i = 0; i < sdl.size(); ++i) if (sdl.instanceID(i) == joyData[slot].instanceID) found = true;
		if (!found) joyData[slot] = JoyData();
	}
	for (int i = 0; i < sdl.size(); ++i) {
		int slot = -1;
		for (int j = first; j < end; ++j)
			if (joyData[j].available && joyData[j].instanceID == sdl.instanceID(i)) { slot = j; break; }
		if (slot < 0) for (int j = first; j < end; ++j)
			if (!joyData[j].available) { slot = j; break; }
		if (slot < 0) break;
		auto& d = joyData[slot];
		d.available = true;
		d.source = Provider::SDL;
		d.instanceID = sdl.instanceID(i);
		d.name = sdl.name(i);
		d.numAxis = sdl.numRawAxes(i);
		const int rawButtons = sdl.numRawButtons(i);
		const int hats = sdl.numRawHats(i);
		const bool mappedDpad = hats == 0 && sdl.isMapped(i);
		d.numButtons = rawButtons + hats * 4 + (mappedDpad ? 4 : 0);
		d.sdlAxes.resize(d.numAxis);
		d.sdlButtons.resize(d.numButtons);
		for (int a = 0; a < d.numAxis; ++a) d.sdlAxes[a] = sdl.rawAxis(i, a);
		for (int b = 0; b < rawButtons; ++b) d.sdlButtons[b] = sdl.rawButton(i, b);
		const Uint8 hatDirections[] = {SDL_HAT_UP, SDL_HAT_RIGHT, SDL_HAT_DOWN, SDL_HAT_LEFT};
		for (int h = 0; h < hats; ++h)
			for (int direction = 0; direction < 4; ++direction)
				d.sdlButtons[rawButtons + h * 4 + direction] = (sdl.rawHat(i, h) & hatDirections[direction]) != 0;
		if (mappedDpad) {
			const SDL_GameControllerButton directions[] = {SDL_CONTROLLER_BUTTON_DPAD_UP, SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
				SDL_CONTROLLER_BUTTON_DPAD_DOWN, SDL_CONTROLLER_BUTTON_DPAD_LEFT};
			for (int direction = 0; direction < 4; ++direction)
				d.sdlButtons[rawButtons + direction] = sdl.button(i, directions[direction]);
		}
		d.axisData = d.sdlAxes.empty() ? nullptr : d.sdlAxes.data();
		d.buttonData = d.sdlButtons.empty() ? nullptr : d.sdlButtons.data();
	}
	numJoysticks = 0;
	for (int i = 0; i < totalSlots; ++i) if (joyData[i].available) ++numJoysticks;
}
#endif


float ofxGLFWJoystick::getAxisValue(int axisID, int joyID){

	if(isJoystickAvailable(joyID)){
		if(axisID < joyData[joyID].numAxis && axisID >= 0){
			return joyData[joyID].axisData[axisID];
		}
	}
	return 0.0f;
}


unsigned char ofxGLFWJoystick::getButtonValue(int buttonID, int joyID){

	if(isJoystickAvailable(joyID)){
		if(buttonID < joyData[joyID].numButtons && buttonID >= 0){
			return joyData[joyID].buttonData[buttonID];
		}
	}
	return 0;
}


void ofxGLFWJoystick::drawDebug(int x, int y){
	if (updating) return;

	ofPushMatrix();
	ofPushStyle();
	ofSetRectMode(OF_RECTMODE_CORNER);
	ofTranslate(x, y);
	float yOffset = 0;

	for(int j = 0; j < totalSlots; j++){


		bool joyOK = joyData[j].available;

		if(joyOK){
			ofTranslate(0 , yOffset);
			string msg = "[" + getJoystickProvider(j) + " ID " + ofToString(j) + "] '" + joyData[j].name + "' numAxis: " + ofToString(joyData[j].numAxis) +
			"  numButtons: " + ofToString(joyData[j].numButtons);

			ofSetColor(255);
			ofDrawBitmapString(msg, 0, 0);

			const float widgetY = 12;
			const float widgetHeight = 18;
			const float gap = 4;
			float xOffset = 0;
			for(int i = 0; i < joyData[j].numAxis; ++i){
				const string label = "A " + ofToString(i);
				// ofDrawBitmapString uses fixed-width 8 pixel characters.
				const float axisWidth = label.size() * 8.0f + 4;
				const float value = joyData[j].axisData[i];
				const float markerY = widgetY + 1 + (1.0f - ofClamp(value, -1.0f, 1.0f)) * 0.5f * (widgetHeight - 2);
				ofSetColor(70);
				ofDrawRectangle(xOffset, widgetY, axisWidth, widgetHeight);
				ofSetColor(130);
				ofDrawRectangle(xOffset, widgetY + widgetHeight * 0.5f - 1, axisWidth, 2);
				ofSetColor(255, 190, 0);
				ofDrawRectangle(xOffset, markerY - 1, axisWidth, 2);
				ofSetColor(255);
				ofDrawBitmapString(label, xOffset + 2, widgetY + 32);
				xOffset += axisWidth + gap;
			}

			const float buttonSize = widgetHeight;
			ofSetRectMode(OF_RECTMODE_CORNER);
			for(int i = 0; i < joyData[j].numButtons; i ++ ){
				bool pressed = joyData[j].buttonData[i] > 0; //mmm not sure why its a char if its only 0/1
				ofSetColor( pressed ? 0:255, pressed ? 255:0, 0 );
				ofDrawRectangle(xOffset, widgetY, buttonSize, buttonSize);
				ofSetColor(255);
				ofDrawBitmapString(ofToString(i), xOffset, widgetY + 32);
				xOffset += buttonSize + gap;
			}
			yOffset = 76;
		}
	}
	ofPopStyle();
	ofPopMatrix();
}


bool ofxGLFWJoystick::isJoystickAvailable(int joyID){
	if (updating) return false;
	if(joyID >= 0 && joyID < totalSlots){
		if(joyData[joyID].available){
			return true;
		}
	}
	return false;
}

string ofxGLFWJoystick::getJoystickProvider(int joyID){
	if (!isJoystickAvailable(joyID)) return "";
	return joyData[joyID].source == Provider::SDL ? "SDL" : "GLFW";
}


string ofxGLFWJoystick::getJoystickName(int joyID){
	if(isJoystickAvailable(joyID)){
		return joyData[joyID].name;
	}
	return "";
}


int ofxGLFWJoystick::getNumButtons(int joyID){
	if(isJoystickAvailable(joyID)){
		return joyData[joyID].numButtons;
	}
	return 0;
}


int ofxGLFWJoystick::getNumAxis(int joyID){
	if(isJoystickAvailable(joyID)){
		return joyData[joyID].numAxis;
	}
	return 0;
}


const unsigned char * ofxGLFWJoystick::getButtonsForJoystick(int joyID){
	if(isJoystickAvailable(joyID)){
		return joyData[joyID].buttonData;
	}
	return NULL;
}


const float * ofxGLFWJoystick::getButtonsForAxis(int joyID){
	if(isJoystickAvailable(joyID)){
		return joyData[joyID].axisData;
	}
	return NULL;
}








