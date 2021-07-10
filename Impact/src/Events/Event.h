#pragma once
#include <cstdint>
class Event
{
public:
	// TODO: look into repeat

	enum class EventType : uint16_t
	{
		NONE = 0x0000,

		KeyPressed = 0x01,
		KeyReleased = 0x02,

		LPressed = 0x0004,
		LReleased = 0x0008,

		RPressed = 0x0010,
		RReleased = 0x0020,

		MPressed = 0x0040,
		MReleased = 0x0080,

		WheelUp = 0x0100,
		WheelDown = 0x0200,

		WheelRight = 0x0400,
		WheelLeft = 0x0800,

		Move = 0x1000,
		RawMove = 0x2000,

		Enter = 0x4000,
		Leave = 0x8000
	};
};

