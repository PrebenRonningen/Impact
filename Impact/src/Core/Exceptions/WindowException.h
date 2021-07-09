#pragma once
#include "src/Core/ImpactWin.h"
#include "ImpException.h"
class WindowException : public ImpException
{
public:
	WindowException(int line, const char* file, HRESULT hr) noexcept;
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept override;
	static std::string TranslateErrorCode(HRESULT hr) noexcept;
	HRESULT GetErrorCode() const noexcept;
	std::string GetErrorString() const noexcept;
private:
HRESULT m_hr;
};

