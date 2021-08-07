#include "SurfaceException.h"
#include <sstream>

namespace Impact
{
	SurfaceException::SurfaceException(int line, const char* file, std::string info) noexcept
		: ImpException(line, file)
		, m_Info(std::move(info))
	{}

	const char* SurfaceException::what() const noexcept
	{
		std::ostringstream oss;
		oss << ImpException::what() << std::endl;
		oss << "[INFO]" << GetInfo();
		m_WhatBuffer = oss.str();

		return m_WhatBuffer.c_str();
	}

	const char* SurfaceException::GetType() const noexcept
	{
		return "Surface Exception";
	}

	const std::string& SurfaceException::GetInfo() const noexcept
	{
		return m_Info;
	}
}