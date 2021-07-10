#pragma once
#include "src/Events/Event.h"
#include <utility>
class Mouse;


class MouseEvent : public Event
{
public:
	MouseEvent(EventType type, const Mouse& parent) noexcept;
	Event::EventType GetType() const noexcept;
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

