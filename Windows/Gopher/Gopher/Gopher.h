#include <Windows.h> //for Beep()
#include <iostream>
#include <Xinput.h> //controller
#include <stdio.h> //for printf
#include <cmath> //for abs()
#include <mmdeviceapi.h> //vol
#include <endpointvolume.h> //vol

#include <map>

#include "CXBOXController.h"

#pragma once
class Gopher
{
private:
	int DEAD_ZONE = 0;
	int SCROLL_DEAD_ZONE = 0;
	int TRIGGER_DEAD_ZONE = 0;
	int SCROLL_SPEED = 0; // Speed at which you scroll page.
	int FPS = 0;
	int SLEEP_VALUE = 0;

	XINPUT_STATE _currentState;

	float SPEED_LOW = 0;
	float SPEED_MED = 0;
	float SPEED_HIGH = 0;
	float speed = SPEED_MED;

	bool _disabled = false; //use for Select sleep mode
	bool _hidden = false; //press Y to hide, check this var

	bool _lTriggerPrevious = false;
	bool _rTriggerPrevious = false;

	float _xRest = 0;
	float _yRest = 0;

	//Mouse Clicks
	DWORD CONFIG_MOUSE_LEFT = NULL;
	DWORD CONFIG_MOUSE_RIGHT = NULL;
	DWORD CONFIG_MOUSE_MIDDLE = NULL;

	//Gopher Settings
	DWORD CONFIG_HIDE = NULL;
	DWORD CONFIG_DISABLE = NULL;
	DWORD CONFIG_SPEED_CHANGE = NULL;

	//Gamepad bindings
	DWORD GAMEPAD_DPAD_UP = NULL;
	DWORD GAMEPAD_DPAD_DOWN = NULL;
	DWORD GAMEPAD_DPAD_LEFT = NULL;
	DWORD GAMEPAD_DPAD_RIGHT = NULL;
	DWORD GAMEPAD_START = NULL;
	DWORD GAMEPAD_BACK = NULL;
	DWORD GAMEPAD_LEFT_THUMB = NULL;
	DWORD GAMEPAD_RIGHT_THUMB = NULL;
	DWORD GAMEPAD_LEFT_SHOULDER = NULL;
	DWORD GAMEPAD_RIGHT_SHOULDER = NULL;
	DWORD GAMEPAD_A = NULL;
	DWORD GAMEPAD_B = NULL;
	DWORD GAMEPAD_X = NULL;
	DWORD GAMEPAD_Y = NULL;
	DWORD GAMEPAD_TRIGGER_LEFT = NULL;
	DWORD GAMEPAD_TRIGGER_RIGHT = NULL;


	std::map<DWORD, bool> _xboxClickStateLastIteration;
	std::map<DWORD, bool> _xboxClickIsDown;
	std::map<DWORD, bool> _xboxClickIsUp;

	CXBOXController* _controller;

public:

	Gopher(CXBOXController* controller);

	void loadConfigFile();

	void loadKeybindingFile();

	void loop();

	void toggleWindowVisibility();

	float getDelta(short tx);

	void handleMouseMovement();

	void handleDisableButton();

	void handleScrolling();

	void handleTriggers(WORD lKey, WORD rKey);

	bool xboxClickStateExists(DWORD xinput);

	void mapKeyboard(DWORD STATE, WORD key);

	void mapMouseClick(DWORD STATE, DWORD keyDown, DWORD keyUp);

	void setXboxClickState(DWORD state);
};
