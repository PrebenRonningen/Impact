#include "GfxInfoException.h"
#include <sstream>
Impact::GfxInfoException::GfxInfoException(int line, const char* file, std::vector<std::string> infoMessages) noexcept
	:ImpException(line, file)
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

const char* Impact::GfxInfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOrigin();
	m_WhatBuffer = oss.str();
	return m_WhatBuffer.c_str();
}

const char* Impact::GfxInfoException::GetType() const noexcept
{
	return "Impact Graphics Info Exception";;
}

std::string Impact::GfxInfoException::GetErrorInfo() const noexcept
{
	return m_Info;
}
