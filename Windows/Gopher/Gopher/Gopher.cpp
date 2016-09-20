#include "Gopher.h"
#include "ConfigFile.h"

void inputKeyboard(WORD cmd, DWORD flag)
{
	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = 0;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	input.ki.wVk = cmd;
	input.ki.dwFlags = flag;
	SendInput(1, &input, sizeof(INPUT));
}

void inputKeyboardDown(WORD cmd)
{
	inputKeyboard(cmd, 0);
}

void inputKeyboardUp(WORD cmd)
{
	inputKeyboard(cmd, KEYEVENTF_KEYUP);
}

void mouseEvent(DWORD dwFlags, DWORD mouseData = 0)
{
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.mouseData = 0;
	input.mi.dwFlags = dwFlags;
	input.mi.time = 0;
	SendInput(1, &input, sizeof(INPUT));
}

Gopher::Gopher(CXBOXController * controller)
	: _controller(controller)
{
	setupPowerOffCallback();
	SCROLL_SPEED = 20;
}

Gopher::~Gopher()
{
	if (SUCCEEDED(_hXInputDll))
	{
		FreeLibrary(_hXInputDll);
	}
}

void Gopher::loadConfigFile()
{
	ConfigFile cfg("config.ini");
	
	//Configuration bindings
	CONFIG_MOUSE_LEFT = strtol(cfg.getValueOfKey<std::string>("CONFIG_MOUSE_LEFT").c_str(), 0, 0);
	CONFIG_MOUSE_RIGHT = strtol(cfg.getValueOfKey<std::string>("CONFIG_MOUSE_RIGHT").c_str(), 0, 0);
	CONFIG_MOUSE_MIDDLE = strtol(cfg.getValueOfKey<std::string>("CONFIG_MOUSE_MIDDLE").c_str(), 0, 0);
	CONFIG_HIDE = strtol(cfg.getValueOfKey<std::string>("CONFIG_HIDE").c_str(), 0, 0);
	CONFIG_DISABLE = strtol(cfg.getValueOfKey<std::string>("CONFIG_DISABLE").c_str(), 0, 0);
	CONFIG_SPEED_CHANGE = strtol(cfg.getValueOfKey<std::string>("CONFIG_SPEED_CHANGE").c_str(), 0, 0);
	CONFIG_PWR_OFF = strtol(cfg.getValueOfKey<std::string>("CONFIG_PWR_OFF").c_str(), 0, 0);
	CONFIG_NXT_WND = strtol(cfg.getValueOfKey<std::string>("CONFIG_NXT_WND").c_str(), 0, 0);
	CONFIG_PRV_WND = strtol(cfg.getValueOfKey<std::string>("CONFIG_PRV_WND").c_str(), 0, 0);

	//Controller bindings
	GAMEPAD_DPAD_UP = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_DPAD_UP").c_str(), 0, 0);
	GAMEPAD_DPAD_DOWN = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_DPAD_DOWN").c_str(), 0, 0);
	GAMEPAD_DPAD_LEFT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_DPAD_LEFT").c_str(), 0, 0);
	GAMEPAD_DPAD_RIGHT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_DPAD_RIGHT").c_str(), 0, 0);
	GAMEPAD_START = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_START").c_str(), 0, 0);
	GAMEPAD_BACK = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_BACK").c_str(), 0, 0);
	GAMEPAD_LEFT_THUMB = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_LEFT_THUMB").c_str(), 0, 0);
	GAMEPAD_RIGHT_THUMB = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_RIGHT_THUMB").c_str(), 0, 0);
	GAMEPAD_LEFT_SHOULDER = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_LEFT_SHOULDER").c_str(), 0, 0);
	GAMEPAD_RIGHT_SHOULDER = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_RIGHT_SHOULDER").c_str(), 0, 0);
	GAMEPAD_A = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_A").c_str(), 0, 0);
	GAMEPAD_B = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_B").c_str(), 0, 0);
	GAMEPAD_X = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_X").c_str(), 0, 0);
	GAMEPAD_Y = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_Y").c_str(), 0, 0);
	GAMEPAD_TRIGGER_LEFT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_TRIGGER_LEFT").c_str(), 0, 0);
	GAMEPAD_TRIGGER_RIGHT = strtol(cfg.getValueOfKey<std::string>("GAMEPAD_TRIGGER_RIGHT").c_str(), 0, 0);

	//LOOP over all the other keys
}

void Gopher::loop() {
	Sleep(SLEEP_AMOUNT);

	_currentState = _controller->GetState();

	// the controller is turned off
	if (_currentState.dwPacketNumber == NULL)
	{
		return;
	}

	handleDisableButton();
	if (_disabled)
	{
		return;
	}

	handlePowerOff();
	handleMouseMovement();
	handleScrolling();
	handleChngNxtWindow();
	handleChngPrvWindow();
	
	//Mouse functions
	if (CONFIG_MOUSE_LEFT)
		mapMouseClick(CONFIG_MOUSE_LEFT, MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_LEFTUP);
	if (CONFIG_MOUSE_RIGHT)
		mapMouseClick(CONFIG_MOUSE_RIGHT, MOUSEEVENTF_RIGHTDOWN, MOUSEEVENTF_RIGHTUP);
	if (CONFIG_MOUSE_MIDDLE)
		mapMouseClick(CONFIG_MOUSE_MIDDLE, MOUSEEVENTF_MIDDLEDOWN, MOUSEEVENTF_MIDDLEUP);

	//Hides the console
	if (CONFIG_HIDE)
	{
		setXboxClickState(CONFIG_HIDE);
		if (_xboxClickIsDown[CONFIG_HIDE])
		{
			toggleWindowVisibility();
		}
	}

	//Will change between the current speed values
	setXboxClickState(CONFIG_SPEED_CHANGE);
	if (_xboxClickIsDown[CONFIG_SPEED_CHANGE]) {

		if (speed == SPEED_LOW)
		{
			speed = SPEED_MED;
			_controller->Vibrate(0, 30000);
			Sleep(400);
			_controller->Vibrate(0, 0);
		}
		else if (speed == SPEED_MED)
		{
			speed = SPEED_HIGH;
			_controller->Vibrate(0, 65000);
			Sleep(400);
			_controller->Vibrate(0, 0);
		}
		else if (speed == SPEED_HIGH)
		{
			speed = SPEED_LOW;
			_controller->Vibrate(0, 10000);
			Sleep(400);
			_controller->Vibrate(0, 0);
		}
	}

	//Set all controller keys.
	handleTriggers(GAMEPAD_TRIGGER_LEFT, GAMEPAD_TRIGGER_RIGHT);
	if (GAMEPAD_DPAD_UP)
		mapKeyboard(XINPUT_GAMEPAD_DPAD_UP, GAMEPAD_DPAD_UP);
	if (GAMEPAD_DPAD_DOWN)
		mapKeyboard(XINPUT_GAMEPAD_DPAD_DOWN, GAMEPAD_DPAD_DOWN);
	if (GAMEPAD_DPAD_LEFT)
		mapKeyboard(XINPUT_GAMEPAD_DPAD_LEFT, GAMEPAD_DPAD_LEFT);
	if (GAMEPAD_DPAD_RIGHT)
		mapKeyboard(XINPUT_GAMEPAD_DPAD_RIGHT, GAMEPAD_DPAD_RIGHT);
	if (GAMEPAD_START)
		mapKeyboard(XINPUT_GAMEPAD_START, GAMEPAD_START);
	if (GAMEPAD_BACK)
		mapKeyboard(XINPUT_GAMEPAD_BACK, GAMEPAD_BACK);
	if (GAMEPAD_LEFT_THUMB)
		mapKeyboard(XINPUT_GAMEPAD_LEFT_THUMB, GAMEPAD_LEFT_THUMB);
	if (GAMEPAD_RIGHT_THUMB)
		mapKeyboard(XINPUT_GAMEPAD_RIGHT_THUMB, GAMEPAD_RIGHT_THUMB);
	if (GAMEPAD_LEFT_SHOULDER)
		mapKeyboard(XINPUT_GAMEPAD_LEFT_SHOULDER, GAMEPAD_LEFT_SHOULDER);
	if (GAMEPAD_RIGHT_SHOULDER)
		mapKeyboard(XINPUT_GAMEPAD_RIGHT_SHOULDER, GAMEPAD_RIGHT_SHOULDER);
	if (GAMEPAD_A)
		mapKeyboard(XINPUT_GAMEPAD_A, GAMEPAD_A);
	if (GAMEPAD_B)
		mapKeyboard(XINPUT_GAMEPAD_B, GAMEPAD_B);
	if (GAMEPAD_X)
		mapKeyboard(XINPUT_GAMEPAD_X, GAMEPAD_X);
	if (GAMEPAD_Y)
		mapKeyboard(XINPUT_GAMEPAD_Y, GAMEPAD_Y);
}

void Gopher::handleDisableButton()
{
	//Select + Start will disable.
	setXboxClickState(CONFIG_DISABLE);
	if (_xboxClickIsDown[CONFIG_DISABLE])
	{
		_disabled = !_disabled;

		if (_disabled) {
			_controller->Vibrate(10000, 10000);
			Sleep(400);
			_controller->Vibrate(0, 0);
		}
		else {
			_controller->Vibrate(65000, 65000);
			Sleep(400);
			_controller->Vibrate(0, 0);
		}
	}
}

void Gopher::toggleWindowVisibility()
{
	_hidden = !_hidden;

	if (_hidden)
	{
		HWND hWnd = GetConsoleWindow();
		ShowWindow(hWnd, SW_HIDE);
		printf("Window hidden\n");
	}
	else
	{
		HWND hWnd = GetConsoleWindow();
		ShowWindow(hWnd, SW_SHOW);
		printf("Window unhidden\n");
		printf("Controller number: %u \n", _controller->getContNum() + 1);
		if (_batInfoCallback!=NULL)
		{
			ZeroMemory(&this->_curBatInfo, sizeof(XINPUT_BATTERY_INFORMATION));
			_batInfoCallback(_controller->getContNum(), BATTERY_DEVTYPE_GAMEPAD, &this->_curBatInfo);
			printf("Battery type: %u, Charge level(0-3): %u \n\n", _curBatInfo.BatteryType, _curBatInfo.BatteryLevel);
		}
		else
			printf("Can't get battery info. XInput1_3.dll is needed!");
	}
}

void Gopher::handleChngNxtWindow()
{
	setXboxClickState(CONFIG_NXT_WND);
	if (_xboxClickIsDown[CONFIG_NXT_WND])
	{
		inputKeyboardDown(VK_MENU);
		inputKeyboardDown(VK_ESCAPE);
	}
	if (_xboxClickIsUp[CONFIG_NXT_WND])
	{
		inputKeyboardUp(VK_ESCAPE);
		inputKeyboardUp(VK_MENU);
	}
}

void Gopher::handleChngPrvWindow()
{
	setXboxClickState(CONFIG_PRV_WND);
	if (_xboxClickIsDown[CONFIG_PRV_WND])
	{
		inputKeyboardDown(VK_MENU);
		inputKeyboardDown(VK_SHIFT);
		inputKeyboardDown(VK_ESCAPE);
	}
	if (_xboxClickIsUp[CONFIG_PRV_WND])
	{
		inputKeyboardUp(VK_ESCAPE);
		inputKeyboardUp(VK_SHIFT);
		inputKeyboardUp(VK_MENU);
	}
}

template <typename T>
int sgn(T val)
{
	return (T(0) < val) - (val < T(0));
}

float Gopher::getDelta(short t)
{
	//filter non-32768 and 32767, wireless ones can glitch sometimes and send it to the edge of the screen, it'll toss out some HUGE integer even when it's centered
	if (t > 32767) t = 0;
	if (t < -32768) t = 0;

	float delta = 0.0;

	if (abs(t) > DEAD_ZONE)
	{
		t = sgn(t) * (abs(t) - DEAD_ZONE);
		delta = speed * t / FPS;
	}

	return delta;
}

void Gopher::handleMouseMovement()
{
	POINT cursor;
	GetCursorPos(&cursor);

	short tx = _currentState.Gamepad.sThumbLX;
	short ty = _currentState.Gamepad.sThumbLY;

	float x = cursor.x + _xRest;
	float y = cursor.y + _yRest;

	float dx = getDelta(tx);
	float dy = getDelta(ty);

	x += dx;
	_xRest = x - (float)((int)x);

	y -= dy;
	_yRest = y - (float)((int)y);

	SetCursorPos((int)x, (int)y); //after all click input processing
}

void Gopher::handleScrolling()
{
	if (abs(_currentState.Gamepad.sThumbRY) > SCROLL_DEAD_ZONE)
	{
		SCROLL_SPEED = (abs(_currentState.Gamepad.sThumbRY) - SCROLL_DEAD_ZONE) / (32767.0 - SCROLL_DEAD_ZONE) * SCROLL_MAX;
		INPUT input;
		input.type = INPUT_MOUSE;
		input.mi.mouseData = (int)(SCROLL_SPEED*sgn(_currentState.Gamepad.sThumbRY));
		input.mi.dwFlags = MOUSEEVENTF_WHEEL;
		input.mi.time = 0;
		SendInput(1, &input, sizeof(INPUT));
	}

	if (abs(_currentState.Gamepad.sThumbRX) > SCROLL_DEAD_ZONE)
	{
		SCROLL_SPEED = (abs(_currentState.Gamepad.sThumbRX) - SCROLL_DEAD_ZONE) / (32767.0 - SCROLL_DEAD_ZONE) * SCROLL_MAX;
		INPUT input;
		input.type = INPUT_MOUSE;
		input.mi.mouseData = (int)(SCROLL_SPEED*sgn(_currentState.Gamepad.sThumbRX));
		input.mi.dwFlags = MOUSEEVENTF_HWHEEL;
		input.mi.time = 0;
		SendInput(1, &input, sizeof(INPUT));
	}
}

void Gopher::handleTriggers(WORD lKey, WORD rKey)
{
	bool lTriggerIsDown = _currentState.Gamepad.bLeftTrigger > TRIGGER_DEAD_ZONE;
	bool rTriggerIsDown = _currentState.Gamepad.bRightTrigger > TRIGGER_DEAD_ZONE;


	if (lTriggerIsDown != _lTriggerPrevious)
	{
		_lTriggerPrevious = lTriggerIsDown;
		if (lTriggerIsDown)
		{
			inputKeyboardDown(lKey);
		}
		else
		{
			inputKeyboardUp(lKey);
		}
	}

	if (rTriggerIsDown != _rTriggerPrevious)
	{
		_rTriggerPrevious = rTriggerIsDown;
		if (rTriggerIsDown)
		{
			inputKeyboardDown(rKey);
		}
		else
		{
			inputKeyboardUp(rKey);
		}
	}
}

void Gopher::setXboxClickState(DWORD STATE)
{
	_xboxClickIsDown[STATE] = false;
	_xboxClickIsUp[STATE] = false;

	if (!this->xboxClickStateExists(STATE))
	{
		_xboxClickStateLastIteration[STATE] = false;
	}

	bool isDown = _currentState.Gamepad.wButtons == STATE;

	if (isDown && !_xboxClickStateLastIteration[STATE])
	{
		_xboxClickStateLastIteration[STATE] = true;
		_xboxClickIsDown[STATE] = true;
	}

	if (!isDown && _xboxClickStateLastIteration[STATE])
	{
		_xboxClickStateLastIteration[STATE] = false;
		_xboxClickIsUp[STATE] = true;
	}

	_xboxClickStateLastIteration[STATE] = isDown;
}

bool Gopher::xboxClickStateExists(DWORD xinput)
{
	auto it = _xboxClickStateLastIteration.find(xinput);
	if (it == _xboxClickStateLastIteration.end())
	{
		return false;
	}

	return true;
}

void Gopher::mapKeyboard(DWORD STATE, WORD key)
{
	setXboxClickState(STATE);
	if (_xboxClickIsDown[STATE])
	{
		inputKeyboardDown(key);
	}

	if (_xboxClickIsUp[STATE])
	{
		inputKeyboardUp(key);
	}
}

void Gopher::mapMouseClick(DWORD STATE, DWORD keyDown, DWORD keyUp)
{
	setXboxClickState(STATE);
	if (_xboxClickIsDown[STATE])
	{
		mouseEvent(keyDown);
	}

	if (_xboxClickIsUp[STATE])
	{
		mouseEvent(keyUp);
	}
}

void Gopher::setupPowerOffCallback()
{
	if (SUCCEEDED(_hXInputDll = LoadLibraryA("XInput1_3.dll")))
	{
		_powerOffCallback = (XInputPowerOffController)GetProcAddress(_hXInputDll, (LPCSTR)103);
		_batInfoCallback = (XInputGetBatInfo)GetProcAddress(_hXInputDll, (LPCSTR)"XInputGetBatteryInformation");
	}
	else
	{
		printf("\nWarning: Could not set up power off functionality.\n");
	}
}

void Gopher::handlePowerOff()
{
	setXboxClickState(CONFIG_PWR_OFF);
	if (SUCCEEDED(_hXInputDll) && _xboxClickIsDown[CONFIG_PWR_OFF])
	{
		if (SUCCEEDED(_powerOffCallback(_controller->getContNum())))
		{
			printf("Controller number %u is powered off \n\n", _controller->getContNum() + 1);
		}
	}
}
