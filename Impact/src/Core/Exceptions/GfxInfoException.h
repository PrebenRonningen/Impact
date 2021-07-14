#pragma once
#include "Core/ImpactWin.h"
#include "ImpException.h"
#include <vector>
namespace Impact
{
	class GfxInfoException : public ImpException
	{
	public:
		GfxInfoException(int line, const char* file, std::vector<std::string> infoMessages) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string m_Info;
	};
}

