#include "pch.h"
#include "Message.h"


int Tools_Tool::WindowsSystem::ShellMessage::GetErrorCode()
{
	return this->ErrorCode;
}

bool Tools_Tool::WindowsSystem::ShellMessage::IsSucceed()
{
	return this->Status;
}