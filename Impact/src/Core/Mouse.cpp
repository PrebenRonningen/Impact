#include "ImpactWin.h"
#include "Mouse.h"
#include "src/Events/EventHandler.h"

struct MouseData
{
	static constexpr uint8_t m_WheelDelta = 120u;
	static constexpr uint8_t m_BufferSize = 16u;

	int m_PosX;
	int m_PosY;
	
	int m_WheelDeltaXCarry = 0;
	int m_WheelDeltaYCarry = 0;
	
	bool m_IsLeftPressed = false;
	bool m_IsRightPressed = false;
	bool m_IsMiddlePressed = false;
	
	bool m_RawEnabled = false;
	
	bool m_IsInWindow = false;

	std::deque<MouseEvent> m_EventBuffer{};
	std::deque<Mouse::RawDelta> m_RawDeltaBuffer{};
};
MouseData g_MouseData{};

Mouse Mouse::m_Mouse{};

std::pair<int, int> Mouse::GetPos() const noexcept
{
	return { g_MouseData.m_PosX, g_MouseData.m_PosY};
}

int Mouse::GetPosX() const noexcept
{
	return g_MouseData.m_PosX;
}

int Mouse::GetPosY() const noexcept
{
	return g_MouseData.m_PosY;
}

bool Mouse::IsInWindow() const noexcept
{
	return g_MouseData.m_IsInWindow;
}

bool Mouse::LeftIsPressed() const noexcept
{
	return g_MouseData.m_IsLeftPressed;
}
bool Mouse::RightIsPressed() const noexcept
{
	return g_MouseData.m_IsRightPressed;
}

bool Mouse::MiddleIsPressed() const noexcept
{
	return g_MouseData.m_IsMiddlePressed;
}

std::optional<MouseEvent> Mouse::Read() noexcept
{
	if ( g_MouseData.m_EventBuffer.empty() )
		return std::nullopt;

	MouseEvent et = std::move(g_MouseData.m_EventBuffer.front());
	g_MouseData.m_EventBuffer.pop_front();
	return et;
}

std::optional<Mouse::RawDelta> Mouse::ReadRawDelta() noexcept
{
	if(g_MouseData.m_RawDeltaBuffer.empty() )
		return std::nullopt;

	const RawDelta delta = std::move(g_MouseData.m_RawDeltaBuffer.front());
	g_MouseData.m_RawDeltaBuffer.pop_front();
	return delta;
}

bool Mouse::IsBufferEmpty() const noexcept
{
	return g_MouseData.m_EventBuffer.empty();
}

void Mouse::ClearBuffer() noexcept
{
	g_MouseData.m_EventBuffer = std::deque<MouseEvent>();
}

void Mouse::EnableRaw() noexcept
{
	g_MouseData.m_RawEnabled = true;
}

void Mouse::DisableRaw() noexcept
{
	g_MouseData.m_RawEnabled = false;
}

bool Mouse::IsRawEnabled() const noexcept
{
	return g_MouseData.m_RawEnabled;
}

void Mouse::OnMouseMove(int x, int y) noexcept
{
	g_MouseData.m_PosX = x;
	g_MouseData.m_PosY = y;

	EventHandler::AddMouseEvent(std::move(MouseEvent{Event::EventType::Move, *this}));
	
}

void Mouse::OnRawDelta(int dx, int dy) noexcept
{
	g_MouseData.m_RawDeltaBuffer.push_back(std::move(RawDelta{dx, dy}));
	TrimRawInputBuffer();
}

void Mouse::OnMouseLeave() noexcept
{
	g_MouseData.m_IsInWindow = false;
	EventHandler::AddMouseEvent(std::move(MouseEvent{Event::EventType::Leave, *this}));
}

void Mouse::OnMouseEnter() noexcept
{
	g_MouseData.m_IsInWindow = true;
	EventHandler::AddMouseEvent(std::move(MouseEvent{Event::EventType::Enter, *this}));
}

void Mouse::OnLeftPressed(int /*x*/ , int /*y*/) noexcept
{
	g_MouseData.m_IsLeftPressed = true;
	EventHandler::AddMouseEvent(std::move(MouseEvent{Event::EventType::LPressed, *this}));
}

void Mouse::OnLeftReleased(int /*x*/, int /*y*/) noexcept
{
	g_MouseData.m_IsLeftPressed = false;
	EventHandler::AddMouseEvent(std::move(MouseEvent{Event::EventType::LReleased, *this}));
}

void Mouse::OnRightPressed(int /*x*/, int /*y*/) noexcept
{
	g_MouseData.m_IsRightPressed = true;
	EventHandler::AddMouseEvent(std::move(MouseEvent{Event::EventType::RPressed, *this}));
}

void Mouse::OnRightReleased(int /*x*/, int /*y*/) noexcept
{
	g_MouseData.m_IsRightPressed = false;
	EventHandler::AddMouseEvent(std::move(MouseEvent{Event::EventType::RReleased, *this}));
}

void Mouse::OnMiddlePressed(int /*x*/, int /*y*/) noexcept
{
	g_MouseData.m_IsMiddlePressed = true;
	EventHandler::AddMouseEvent(std::move(MouseEvent{Event::EventType::MPressed, *this}));
}

void Mouse::OnMiddleReleased(int /*x*/, int /*y*/) noexcept
{
	g_MouseData.m_IsMiddlePressed = false;
	EventHandler::AddMouseEvent(std::move(MouseEvent{Event::EventType::MReleased, *this}));
}

void Mouse::OnWheelUp(int /*x*/, int /*y*/) noexcept
{
	EventHandler::AddMouseEvent(std::move(MouseEvent{Event::EventType::WheelUp, *this}));
}

void Mouse::OnWheelDown(int /*x*/, int /*y*/) noexcept
{
	EventHandler::AddMouseEvent(std::move(MouseEvent{Event::EventType::WheelDown, *this}));
}

void Mouse::OnWheelLeft(int /*x*/, int /*y*/) noexcept
{
	EventHandler::AddMouseEvent(std::move(MouseEvent{Event::EventType::WheelLeft, *this}));
}

void Mouse::OnWheelRight(int /*x*/, int /*y*/) noexcept
{
	EventHandler::AddMouseEvent(std::move(MouseEvent{Event::EventType::WheelRight, *this}));
}

void Mouse::OnWheelDelta(int x, int y, int deltaX, int deltaY) noexcept
{
	g_MouseData.m_WheelDeltaXCarry += deltaX;
	g_MouseData.m_WheelDeltaYCarry += deltaY;

	// generate events for every 120
	// horizontal scroll
	while ( g_MouseData.m_WheelDeltaXCarry >= WHEEL_DELTA )
	{
		g_MouseData.m_WheelDeltaXCarry -= WHEEL_DELTA;
		OnWheelRight(x, y);
	}
	while ( g_MouseData.m_WheelDeltaXCarry <= -WHEEL_DELTA )
	{
		g_MouseData.m_WheelDeltaXCarry += WHEEL_DELTA;
		OnWheelLeft(x, y);
	}
	// vertical scroll
	while ( g_MouseData.m_WheelDeltaYCarry >= WHEEL_DELTA )
	{
		g_MouseData.m_WheelDeltaYCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while ( g_MouseData.m_WheelDeltaYCarry <= -WHEEL_DELTA )
	{
		g_MouseData.m_WheelDeltaYCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}

void Mouse::TrimRawInputBuffer() noexcept
{
	while ( g_MouseData.m_RawDeltaBuffer.size() > g_MouseData.m_BufferSize )
	{
		g_MouseData.m_RawDeltaBuffer.pop_back();
	}
}


