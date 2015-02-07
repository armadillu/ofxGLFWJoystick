# ofxGLFWJoystick

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