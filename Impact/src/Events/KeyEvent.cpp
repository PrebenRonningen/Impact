#include "KeyEvent.h"

KeyEvent::KeyEvent(EventType type, const uint8_t keyCode)
	: m_Type{ type }
	, m_KeyCode{ keyCode }
{}

Event::EventType KeyEvent::GetType() const noexcept
{
	return m_Type;
}

bool KeyEvent::IsDown() const noexcept
{
	return m_Type == EventType::KeyPressed;
}

bool KeyEvent::IsUp() const noexcept
{
	return m_Type == EventType::KeyReleased;
}

uint8_t KeyEvent::GetCode() const noexcept
{
	return m_KeyCode;
}
