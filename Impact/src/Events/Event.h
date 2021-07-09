#pragma once
#include <cstdint>
class Event
{
public:
	// TODO: look into repeat
	enum class ButtonState : uint8_t
	{
		None = 0x00,
		Pressed = 0x01,
		Released = 0x02,
	};

	enum class MouseEventType : uint16_t
	{
		NONE = 0x0000,

		LPressed = 0x0001,
		LReleased = 0x0002,

		RPressed = 0x0004,
		RReleased = 0x0008,

		MPressed = 0x0010,
		MReleased = 0x0020,

		WheelUp = 0x0040,
		WheelDown = 0x0080,

		WheelRight = 0x0100,
		WheelLeft = 0x0200,

		Move = 0x0400,
		RawMove = 0x800,

		Enter = 0x1000,
		Leave = 0x2000
	};
};

