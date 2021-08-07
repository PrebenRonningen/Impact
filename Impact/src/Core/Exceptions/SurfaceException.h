#pragma once
#include "Core/ImpactWin.h"
#include "ImpException.h"

namespace Impact
{
	class SurfaceException : public ImpException
	{
	public:
		SurfaceException(int line, const char* file, std::string info) noexcept;
		virtual const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		const std::string& GetInfo() const noexcept;
	private:
		std::string m_Info;
	};
}

