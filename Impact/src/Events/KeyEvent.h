#pragma once
#include "src/Events/Event.h"
class KeyEvent : public Event
{
public:
	KeyEvent(ButtonState state, const uint8_t keyCode);

	bool IsDown() const noexcept;
	bool IsUp() const noexcept;
	uint8_t GetCode() const noexcept;

private:
	ButtonState m_State;
	uint8_t m_KeyCode;
};

