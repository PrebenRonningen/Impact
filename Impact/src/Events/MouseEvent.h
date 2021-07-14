#pragma once
#include "Events/Event.h"
#include <utility>

namespace Impact
{
	class Mouse;
	class MouseEvent : public Event
	{
	public:
		MouseEvent(EventType type, const Mouse& parent) noexcept;
		virtual EventType GetType() const noexcept override;
		std::pair<int, int> GetPos() const noexcept;
		int GetPosX() const noexcept;
		int GetPosY() const noexcept;
		bool LeftIsPressed() const noexcept;
		bool RightIsPressed() const noexcept;
	
	private:
		EventType m_EventType;
		int m_PosX;
		int m_PosY;
		bool m_LeftIsPressed;
		bool m_RightIsPressed;
		bool m_MiddleIsPressed;
	};
}

