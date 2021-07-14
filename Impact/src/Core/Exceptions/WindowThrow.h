#pragma once
#include "Core/ImpactWin.h"
#include "WindowException.h"

namespace Impact
{
#pragma warning(push)
#pragma warning(disable: 4505)
	static void WND_EXCEPTION(int line, const char* file, HRESULT hr)
	{
		throw WindowException(line, file, hr);
	}

	static WindowException WND_LAST_EXCEPTION(int line, const char* file)
	{
		throw WindowException(line, file, GetLastError());
	}
#pragma warning(pop)
};