#include "CXBOXController.h"

CXBOXController::CXBOXController(int playerNumber)
{
	_controllerNum = playerNumber - 1; //set number
}

XINPUT_STATE CXBOXController::GetState()
{
	ZeroMemory(&this->_controllerState, sizeof(XINPUT_STATE));
	XInputGetState(_controllerNum, &this->_controllerState);
	return _controllerState;
}

bool CXBOXController::IsConnected()
{
	ZeroMemory(&this->_controllerState, sizeof(XINPUT_STATE));
	DWORD Result = XInputGetState(_controllerNum, &this->_controllerState);

	return (Result == ERROR_SUCCESS);
}

void CXBOXController::Vibrate(int leftVal, int rightVal)
{
	// Create a Vibraton State
	XINPUT_VIBRATION Vibration;

	// Zeroise the Vibration
	ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

	// Set the Vibration Values
	Vibration.wLeftMotorSpeed = leftVal;
	Vibration.wRightMotorSpeed = rightVal;

	// Vibrate the controller
	XInputSetState(_controllerNum, &Vibration);
}

int CXBOXController::getContNum()
{
	return _controllerNum;
}

XINPUT_BATTERY_INFORMATION CXBOXController::GetBatInfo()
{
	ZeroMemory(&this->_batInfo, sizeof(XINPUT_BATTERY_INFORMATION));
	XInputGetBatteryInformation(_controllerNum, BATTERY_DEVTYPE_GAMEPAD, &this->_batInfo);
	return _batInfo;
}
