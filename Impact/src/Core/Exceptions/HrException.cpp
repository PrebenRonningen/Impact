#include "HrException.h"
#include "DXError.h"
namespace Impact
{
	HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMessages) noexcept
		: ImpException(line, file)
		, m_Hr(hr)
		, m_Info{}
	{
		// join all info messages with newlines into single string
		for ( const auto& m : infoMessages )
		{
			m_Info += m;
			m_Info.push_back('\n');
		}
		// remove final newline if exists
		if ( !m_Info.empty() )
		{
			m_Info.pop_back();
		}
	}

	const char* HrException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
			<< std::dec << " (" << (unsigned long) GetErrorCode() << ")" << std::endl
			<< "[Error String] " << GetErrorString() << std::endl
			<< "[Description] " << GetErrorDescription() << std::endl;
		if ( !m_Info.empty() )
		{
			oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
		}
		oss << GetOrigin();
		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}

	const char* HrException::GetType() const noexcept
	{
		return "Impact Graphics Exception";
	}

	HRESULT HrException::GetErrorCode() const noexcept
	{
		return m_Hr;
	}

	std::string HrException::GetErrorString() const noexcept
	{
		return DXGetErrorStringA(m_Hr);
	}

	std::string HrException::GetErrorDescription() const noexcept
	{
		char buffer[512];
		DXGetErrorDescriptionA(m_Hr, buffer, sizeof(buffer));
		return buffer;
	}

	std::string HrException::GetErrorInfo() const noexcept
	{
		return m_Info;
	}
}