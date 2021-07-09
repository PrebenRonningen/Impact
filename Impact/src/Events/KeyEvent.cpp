#include "KeyEvent.h"

KeyEvent::KeyEvent(ButtonState state, const uint8_t keyCode)
	: m_State{ state }
	, m_KeyCode{ keyCode }
{}

bool KeyEvent::IsDown() const noexcept
{
	return m_State == ButtonState::Pressed;
}

bool KeyEvent::IsUp() const noexcept
{
	return m_State == ButtonState::Released;
}

uint8_t KeyEvent::GetCode() const noexcept
{
	return m_KeyCode;
}
