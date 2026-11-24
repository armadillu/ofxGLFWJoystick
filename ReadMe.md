# ofxGLFWJoystick

## SDL provider

Without SDL support the addon uses GLFW. To enable SDL, add both `ofxGLFWJoystick` and `ofxSDLController` to the project, install SDL2 as described in [ofxSDLController](../ofxSDLController/README.md), and define `OFX_GLFW_JOYSTICK_USE_SDL` for the entire project, including addon source files (for example, `PROJECT_DEFINES += OFX_GLFW_JOYSTICK_USE_SDL` in `config.make`, or add it to the Xcode target's preprocessor definitions). With SDL enabled, both providers start automatically.

Keep calling `update()` each frame and use the existing accessors. `Provider::BOTH` polls both providers, so a physical controller may appear twice. GLFW devices keep their usual IDs 0–15; SDL devices use IDs 16–31. `getJoystickProvider(id)` identifies the source. Use `getJoystickList()` to enumerate IDs: they can have gaps and are not bounded by the device count. Select `Provider::SDL` to use SDL alone (IDs 0–15), or `Provider::GLFW` to switch back. Switching modes resets controller IDs. Without the compile definition and SDL addon, GLFW builds do not depend on SDL and selecting SDL or BOTH returns false. The `exampleSDL` folder is a runnable make-based example.

Discovery and provider changes are applied on the next `update()`, so the device list is empty until then. `setProvider()` reports whether the selection is available in this build; an SDL initialization failure is logged during `update()` and leaves the previous provider active. `getProvider()` reports the active provider.

SDL exposes raw axes and buttons through this API, so control indices and counts may differ from GLFW, even for the same device. Joystick IDs remain stable while a device is connected, but can be reused after disconnecting. Returned raw array pointers remain valid only until the next `update()` or provider change. SDL controller events are consumed by default; see the SDL addon README before sharing its event queue with other SDL code.

For SDL devices, four buttons per raw hat are appended after the raw buttons, in up, right, down, left order. If a mapped SDL controller has no raw hats, its four mapped D-pad directions are appended instead; these can duplicate physical buttons already present in the raw list. GLFW reports hat directions as buttons by default unless the application disables its joystick hat button init hint.

Use any number of joysticks with OpenFrameworks.
This add-on wraps the GLFW functionality, so it will most likely only work if your project uses a ofAppGLFWWindow.

![3 joysticks](https://farm8.staticflickr.com/7300/16278716840_7e6a2ef751_z_d.jpg)

#How to Use

Just update the singleton object every frame:

```
	void ofApp::update(){
		ofxGLFWJoystick::one().update();
	}
```

And then access any values from any class:

```
	int joystickID = 0;
	int axisID = 0;
	float joyX = ofxGLFWJoystick::one().getAxisValue(axisID, joystickID);
```

That's all! To know what features are supported on your joystick, or if it's supported at all, you might want to drawDebug() the joysticks:

```
	ofxGLFWJoystick::one().drawDebug(100,100);
```

#Notes
* Only tested on OSX. 

* It seems that joysticks need to be plugged in before you launch the app for them to be detected.

* I got an USB Xbox 360 controller to work on OSX using [this](http://tattiebogle.net/index.php/ProjectRoot/Xbox360Controller/OsxDriver).

* Access from the main thread only.

* You can also get raw access to the values joystick provided by GLWF without any safety checks, using getButtonsForJoystick() and getButtonsForAxis(). Just be aware that this could lead to crashes if you are not careful accessing the pointers.

#License
MIT
