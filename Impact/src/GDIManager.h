#pragma once
#include "Core\ImpactWin.h"
namespace Impact
{
	class GDIManager
	{
	public:
		GDIManager();
		~GDIManager();
	private:
		static ULONG_PTR m_Token;
		static int m_RefCount;
	};
}

