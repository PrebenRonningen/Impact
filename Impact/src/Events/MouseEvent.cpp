#include "MouseEvent.h"
#include "Core/Mouse.h"

namespace Impact
{
	MouseEvent::MouseEvent()
		: m_EventType{ EventType::NONE }
		, m_PosX{ 0 }
		, m_PosY{ 0 }
		, m_LeftIsPressed{ 0 }
		, m_RightIsPressed{ 0 }
		, m_MiddleIsPressed{ 0 }
	{
	}
	MouseEvent::MouseEvent(EventType type, const Mouse& parent) noexcept
		: m_EventType{ type }
		, m_PosX{ parent.GetPosX() }
		, m_PosY{ parent.GetPosY() }
		, m_LeftIsPressed{ parent.LeftIsPressed() }
		, m_RightIsPressed{ parent.RightIsPressed() }
		, m_MiddleIsPressed{ parent.MiddleIsPressed() }
	{};

	Event::EventType MouseEvent::GetType() const noexcept
	{
		return m_EventType;
	}

	std::pair<int, int> MouseEvent::GetPos() const noexcept
	{
		return {m_PosX, m_PosY};
	}

	int MouseEvent::GetPosX() const noexcept
	{
		return m_PosX;
	}

	int MouseEvent::GetPosY() const noexcept
	{
		return m_PosY;
	}

	bool MouseEvent::LeftIsPressed() const noexcept
	{
		return m_LeftIsPressed;
	}

	bool MouseEvent::RightIsPressed() const noexcept
	{
		return m_RightIsPressed;
	}
	void MouseEvent::Update(const Mouse& parent)
	{
		m_PosX = parent.GetPosX();
		m_PosY = parent.GetPosY();
		m_LeftIsPressed = parent.LeftIsPressed();
		m_RightIsPressed = parent.RightIsPressed();
		m_MiddleIsPressed = parent.MiddleIsPressed();
	}
}