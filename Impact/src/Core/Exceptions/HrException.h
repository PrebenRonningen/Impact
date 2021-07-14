#pragma once
#include "Core/ImpactWin.h"
#include "ImpException.h"
#include <vector>
#include <sstream>
namespace Impact
{
	class HrException : public ImpException
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMessages = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;
	private:
		HRESULT m_Hr;
		std::string m_Info;
	};
}