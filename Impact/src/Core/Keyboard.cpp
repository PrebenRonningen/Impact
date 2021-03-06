#include "Keyboard.h"
#include "Events/KeyEvent.h"
#include <vector>
#include "Events/EventHandler.h"

namespace Impact
{
	struct KeyboardData
	{
		static constexpr uint16_t numKeys = 256u;
		static constexpr uint8_t m_BufferSize = 16u;
		bool m_AutorepeatEnabled = false;
		std::bitset<numKeys> m_KeyStates{};
		std::deque<KeyEvent> m_KeyBuffer{};
		std::deque<wchar_t> m_CharBuffer{};
	};
	static KeyboardData s_KeyboardData{};


	Keyboard Keyboard::m_Keyboard{};

	bool Keyboard::IsKeyDown(uint8_t keyCode) const noexcept
	{
		return s_KeyboardData.m_KeyStates[keyCode];
	}

	bool Keyboard::IsAnyKeyDown(uint8_t* keyCode, uint8_t numberOfKeys) const noexcept
	{
		bool isAnyDown = false;
		for (uint8_t i{}; i < numberOfKeys; i++)
		{
			isAnyDown |= s_KeyboardData.m_KeyStates[*keyCode++];

		}
		return isAnyDown;
	}

	bool Keyboard::IsKeyUp(uint8_t keyCode) const noexcept
	{
		return !s_KeyboardData.m_KeyStates[keyCode];
	}
	bool Keyboard::IsAnyKeyUp(uint8_t* keyCode, uint8_t numberOfKeys) const noexcept
	{
		bool isAnyUp = false;
		for (uint8_t i{}; i < numberOfKeys; i++)
		{
			isAnyUp |= !s_KeyboardData.m_KeyStates[*keyCode++];

		}
		return isAnyUp;
	}
	std::optional<KeyEvent> Keyboard::ReadKey() noexcept
	{
		if ( s_KeyboardData.m_KeyBuffer.empty() )
			return std::nullopt;
	
		KeyEvent keyEvent = std::move(s_KeyboardData.m_KeyBuffer.front());
		s_KeyboardData.m_KeyBuffer.pop_front();
		return keyEvent;
	}

	bool Keyboard::IsKeyBufferEmpty() const noexcept
	{
		return s_KeyboardData.m_KeyBuffer.empty();
	}

	void Keyboard::ClearKeyBuffer() noexcept
	{
		s_KeyboardData.m_KeyBuffer = std::deque<KeyEvent>();
	}

	std::optional<wchar_t> Keyboard::ReadChar() noexcept
	{
		if ( s_KeyboardData.m_CharBuffer.empty() )
			return std::nullopt;
	
		wchar_t charCode = std::move(s_KeyboardData.m_CharBuffer.front());
		s_KeyboardData.m_CharBuffer.pop_front();
		return charCode;
	}

	bool Keyboard::IsCharBufferEmpty() const noexcept
	{
		return s_KeyboardData.m_CharBuffer.empty();
	}

	void Keyboard::ClearCharBuffer() noexcept
	{
		s_KeyboardData.m_CharBuffer = std::deque<wchar_t>();
	}

	void Keyboard::ClearBuffers() noexcept
	{
		ClearKeyBuffer();
		ClearCharBuffer();
	}

	void Keyboard::EnableAutorepeat() noexcept
	{
		s_KeyboardData.m_AutorepeatEnabled = true;
	}

	void Keyboard::DisableAutorepeat() noexcept
	{
		s_KeyboardData.m_AutorepeatEnabled = false;
	}

	bool Keyboard::IsAutorepeatEnabled() const noexcept
	{
		return 	s_KeyboardData.m_AutorepeatEnabled;
	}

	Keyboard& Keyboard::Get()
	{
		return m_Keyboard;
	}

	void Keyboard::OnKeyDown(uint8_t keyCode) noexcept
	{
		s_KeyboardData.m_KeyStates[keyCode] = true;
		EventHandler::AddKeyEvent(std::move(KeyEvent{ Event::EventType::KeyPressed, keyCode }));
		TrimBuffer(s_KeyboardData.m_KeyBuffer);
	}

	void Keyboard::OnKeyUp(uint8_t keyCode) noexcept
	{
		s_KeyboardData.m_KeyStates[keyCode] = false;
		EventHandler::AddKeyEvent(std::move(KeyEvent{ Event::EventType::KeyReleased, keyCode }));
		TrimBuffer(s_KeyboardData.m_KeyBuffer);
	}

	void Keyboard::OnChar(wchar_t character) noexcept
	{
		s_KeyboardData.m_CharBuffer.push_back(std::move(character));
		TrimBuffer(s_KeyboardData.m_CharBuffer);
	}

	void Keyboard::ClearState() noexcept
	{
		s_KeyboardData.m_KeyStates.reset();
	}

	// private 

	template<typename T>
	void Keyboard::TrimBuffer(std::deque<T>& buffer) noexcept
	{
		while ( buffer.size() > s_KeyboardData.m_BufferSize )
		{
			buffer.pop_back();
		}
	}
}