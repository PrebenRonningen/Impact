#pragma once
#include <optional>
#include <deque>
#include <bitset>
#include "src/Events/KeyEvent.h"

class Keyboard
{
	friend class Window;
//	friend class Input;
public:
	~Keyboard() = default;
#pragma region delete
	Keyboard(const Keyboard& other) = delete;
	Keyboard(Keyboard&& other) = delete;
	Keyboard& operator=(const Keyboard& other) = delete;
	Keyboard& operator=(Keyboard&& other) = delete;
#pragma endregion
	
	// key
	bool IsKeyDown(uint8_t keyCode) const noexcept;
	bool IsKeyUp(uint8_t keyCode) const noexcept;
	bool IsKeyRepeat(uint8_t keyCode) const noexcept;
	std::optional<KeyEvent> ReadKey() noexcept;
	bool IsKeyBufferEmpty() const noexcept;
	void ClearKeyBuffer() noexcept;

	// char
	std::optional<wchar_t> ReadChar() noexcept;
	bool IsCharBufferEmpty() const noexcept;
	void ClearCharBuffer() noexcept;
	void ClearBuffers() noexcept;

	// Autorepeat
	void EnableAutorepeat() noexcept;
	void DisableAutorepeat() noexcept;
	bool IsAutorepeatEnabled() const noexcept;

	static Keyboard& Get();
private:
	void OnKeyDown(uint8_t keyCode) noexcept;
	void OnKeyUp(uint8_t keyCode) noexcept;
	void OnKeyRepeat(uint8_t keyCode) noexcept;
	void OnChar(wchar_t character) noexcept;
	void ClearState() noexcept;

	template <typename T>
	static void TrimBuffer(std::deque<T>& buffer) noexcept;
private:
	Keyboard() = default;
	static Keyboard m_Keyboard;

	//// Pimpl idiom
	//struct KeyboardData;
	//static std::unique_ptr<KeyboardData> m_KeyboardData;
};
