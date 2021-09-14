#pragma once
#include <deque>
#include <array>
#include <map>
#include <vector>
#include <functional>
#include "Events/Event.h"

namespace Impact
{
	class KeyEvent;
	class MouseEvent;
	class Layer;
	class EventHandler
	{
	public:
		static void AddKeyEvent(KeyEvent&& event) noexcept;
		static void AddMouseEvent( MouseEvent&& event) noexcept;

		static void RegisterEvent(void* ovner, int layer, const Event::EventType eventType, const std::function<bool(Event&)>& callback) noexcept;
		static void ProcessEvent() noexcept;
	private:
		static std::map<int, std::vector<std::pair<uint16_t, std::function<bool(Event&)>>>> m_LayerCallback;
		static const uint8_t m_BufferSize = 16u;
		static std::deque<KeyEvent> m_KeyBuffer;
		static std::array<MouseEvent, m_BufferSize> m_MouseBuffer;

		static uint16_t m_RegisteredEventTypes;
		static void HandleEvent(Event* event) noexcept;
		static std::vector<uint64_t> m_funcSources;

		template<typename T>
		static void TrimBuffer(std::deque<T>& buffer) noexcept;
	};
}