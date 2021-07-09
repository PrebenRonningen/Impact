#include "ImpException.h"
#include <sstream>

ImpException::ImpException(int line, const char* file) noexcept
	: m_WhatBuffer{}
	, m_Line{line}
	, m_File{file}
{}

const char* ImpException::what() const noexcept
{
	std::ostringstream oss{};
	oss << GetType() << std::endl;
	oss << GetOrigin();
	m_WhatBuffer = oss.str();

	return m_WhatBuffer.c_str();
}

const char* ImpException::GetType() const noexcept
{
	return "Impact Excemtion";
}

int ImpException::GetLine() const noexcept
{
	return m_Line;
}

const std::string& ImpException::GetFile() const noexcept
{
	return m_File;
}

std::string ImpException::GetOrigin() const noexcept
{
	std::ostringstream oss{};
	oss << "[FILE] " << GetFile() << std::endl;
	oss << "[LINE] " << GetLine() << std::endl;
	return oss.str();
}
