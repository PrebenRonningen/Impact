#include "WindowException.h"
#include <sstream>

namespace Impact
{
	WindowException::WindowException(int line, const char* file, HRESULT hr) noexcept
		: ImpException(line, file)
		, m_hr{hr}
	{
	}
	
	const char* WindowException::what() const noexcept
	{
		std::ostringstream oss{};
		oss << GetType() << std::endl;
		oss << GetOrigin();
		oss << "[ERROR CODE] " << GetErrorCode() << std::endl;
		oss << "[DESCRIPTION] " << GetErrorString();
		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}
	
	const char* WindowException::GetType() const noexcept
	{
		return "Impact Window Exception";
	}
	
	std::string WindowException::TranslateErrorCode(HRESULT hr) noexcept
	{
		//https://stackoverflow.com/questions/455434/how-should-i-use-formatmessage-properly-in-c
		// test this
		// _com_error error(hr);
		// LPCTSTR errorText = error.ErrorMessage();
	
		LPSTR errorMessage = NULL;
		FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			0,						// unused with FORMAT_MESSAGE_FROM_SYSTEM
			hr,
			0,
			(LPSTR)&errorMessage,	// output
			0,						// minimum size for output buffer
			nullptr					// no args
		);
		if ( errorMessage == NULL)
		{
			return "Unknown error code";
		}
		std::string msg = errorMessage;
		LocalFree(errorMessage);
		return msg;
	}
	
	HRESULT WindowException::GetErrorCode() const noexcept
	{
		return m_hr;
	}
	
	std::string WindowException::GetErrorString() const noexcept
	{
		return TranslateErrorCode(m_hr);
	}
}