#pragma once
//#include "ImpactWin.h"
#include "Core/ImpactWin.h"
#include <optional>
#include <functional>
#include <memory>

namespace Impact
{
	class Keyboard;
	class Mouse;
	class Event;
	class Graphics;
	class Window
	{
	public:
		Window(uint16_t width, uint16_t height, const wchar_t* name);// noexcept;
		~Window();
	
	#pragma region delete
		Window(const Window& other) = delete;
		Window(Window&& other) = delete;
		Window& operator=(const Window& other) = delete;
		Window& operator=(Window&& other) = delete;
	#pragma endregion
		void SetTitle( const std::wstring& title); // ony for testing
	
		HWND GetWindowHandle() {return m_hWnd;};
		static std::optional<int> ProcessMessages() noexcept;
		//void SetEventCallback(const std::function<void(Event&)>& callback) { m_EventCallBack = callback; }
		Graphics& GetGraphix() const;
	private:
		static void RegisteRawInputDevice();
		static LRESULT CALLBACK HandleMessageStartUp(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT CALLBACK HandleMessageInter(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;
		LRESULT CALLBACK HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;
	private:
		uint16_t m_Width;
		uint16_t m_Height;
		HWND m_hWnd;
		std::unique_ptr<Graphics> pGfx;

		//static UserData m_Data;
		//std::function<void(Event&)> m_EventCallBack;
		static Keyboard& m_Keyboard;
		static Mouse& m_Mouse;
	
		// used for RawInput
		static std::vector<BYTE> m_RawBuffer;
	
	private:	
		// Only Window needs to know about this
		// Consider using singleton template
		class WindowClass
		{
		public:
			static const wchar_t* GetName() noexcept;
			static HINSTANCE GetInstance() noexcept;
		private:
			WindowClass() noexcept;
			~WindowClass();
			#pragma region delete
			WindowClass(const WindowClass& other) = delete;
			WindowClass(WindowClass&& other) = delete;
			WindowClass& operator=(const WindowClass& other) = delete;
			WindowClass& operator=(WindowClass&& other) = delete;
			#pragma endregion
			static constexpr const wchar_t* m_WndClassName = L"Impact Direct3D 11 Engine Window";
			static WindowClass m_WndClass;
			HINSTANCE m_hInstance;
		};
	};
}