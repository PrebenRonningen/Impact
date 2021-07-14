// Courtesy of ChiliTomatoNoodle
// https://www.youtube.com/watch?v=UUbXK4G_NCM&list=PLqCJpWy5Fohd3S7ICFXwUomYW0Wv67pDD&index=5 at 2:22
#pragma once
#include "Core/ImpactWin.h"
#include <unordered_map>

namespace Impact
{
	class WindowsMessageMap
	{
	public:
		WindowsMessageMap() noexcept;
		std::string operator()(DWORD msg, LPARAM lParam, WPARAM wParam) const noexcept;
	private:
	std::unordered_map<DWORD, std::string> m_MessageMap;
	};
}