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
	typedef DWORD(WINAPI* XInputPowerOffController)(DWORD i);

	const int DEAD_ZONE = 9000; //X and Y minimum, below this is ignored since all controllers have some stick to them
	const int SCROLL_DEAD_ZONE = 9000; // Right thumbstick should be less sensitive.
	const int TRIGGER_DEAD_ZONE = 0;
	const int FPS = 150;
	const int SLEEP_AMOUNT = 1000/FPS; // number of milliseconds to sleep per iteration
	const int SCROLL_MAX = 50;

	XINPUT_STATE _currentState;

	const float SPEED_LOW = 0.032f;
	const float SPEED_MED = 0.04f;
	const float SPEED_HIGH = 0.072f;
	float speed = SPEED_MED;

	float SCROLL_SPEED;	// Speed at which you scroll page.
	
	float _xRest = 0.0f;
	float _yRest = 0.0f;

	bool _disabled = false; //use for Select sleep mode
	bool _hidden = false; //press Y to hide, check this var
	bool _lTriggerPrevious = false;
	bool _rTriggerPrevious = false;

	//Mouse Clicks
	DWORD CONFIG_MOUSE_LEFT = NULL;
	DWORD CONFIG_MOUSE_RIGHT = NULL;
	DWORD CONFIG_MOUSE_MIDDLE = NULL;

	//Gopher Settings
	DWORD CONFIG_HIDE = NULL;
	DWORD CONFIG_DISABLE = NULL;
	DWORD CONFIG_SPEED_CHANGE = NULL;
	DWORD CONFIG_PWR_OFF = NULL;

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

	HINSTANCE _hXInputDll;
	XInputPowerOffController _powerOffCallback;

	XINPUT_BATTERY_INFORMATION _curBatInfo;

public:

	Gopher(CXBOXController* controller);

	~Gopher();

	void loadConfigFile();

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

	void setupPowerOffCallback();

	void handlePowerOff();

};
