#pragma once
#include "src/Events/Event.h"
class KeyEvent : public Event
{
public:
	KeyEvent(EventType state, const uint8_t keyCode);
	virtual EventType GetType() const noexcept override;
	bool IsDown() const noexcept;
	bool IsUp() const noexcept;
	uint8_t GetCode() const noexcept;

private:
	EventType m_Type;
	uint8_t m_KeyCode;
};

