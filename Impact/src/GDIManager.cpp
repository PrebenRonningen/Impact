
#ifndef FULL_WIN
	#define FULL_WIN
#endif 
#include "Core\ImpactWin.h"
#include "GDIManager.h"
#include <algorithm>

namespace Gdiplus
{
	using std::min;
	using std::max;
}

#pragma warning( push)
#pragma warning(disable: 4458)
#include <gdiplus.h>
#pragma warning( pop )

//#include <sstream>
#pragma comment(lib, "gdiplus.lib")

namespace Impact
{
	ULONG_PTR GDIManager::m_Token{0};
	int GDIManager::m_RefCount{0};

	GDIManager::GDIManager()
	{
		if (m_RefCount++ == 0)
		{
			Gdiplus::GdiplusStartupInput input;
			input.GdiplusVersion = 1;
			input.DebugEventCallback = nullptr;
			input.SuppressBackgroundThread = false;
			Gdiplus::GdiplusStartup(&m_Token, &input, nullptr);
		}
	}

	GDIManager::~GDIManager() {
		if (--m_RefCount == 0) {
			Gdiplus::GdiplusShutdown(m_Token);
		}
	}

}