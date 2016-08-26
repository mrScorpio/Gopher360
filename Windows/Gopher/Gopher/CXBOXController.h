#pragma once

#include <Windows.h>
#include <Xinput.h>

class CXBOXController
{
private:
	XINPUT_STATE _controllerState;
	XINPUT_BATTERY_INFORMATION _batInfo;
	int _controllerNum;
public:
	CXBOXController(int playerNumber);
	XINPUT_STATE GetState();
	XINPUT_BATTERY_INFORMATION GetBatInfo();
	bool IsConnected();
	void CXBOXController::Vibrate(int leftVal, int rightVal);
	int getContNum();
};
