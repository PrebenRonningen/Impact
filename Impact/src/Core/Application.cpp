#include "Application.h"
//#include "src/Core/Input.h"
#include "src/Core/Keyboard.h"
#include "src/Core/Mouse.h"
#include <sstream>



//Input& Application::m_Input = m_Input.Get();
Keyboard& Application::m_Keyboard = Keyboard::Get();
Mouse& Application::m_Mouse = Mouse::Get();


Application::Application() noexcept
	: m_Window{ 640, 480, L"Impact 3D" }
{
	// make easier and better to read
	m_Window.SetEventCallback([this](auto&&... args)->decltype(auto){return this->Application::OnEvent(std::forward<decltype(args)>(args)...);});
}

Application::~Application()
{
}

int Application::Run()
{
	while ( true )
	{
		// process all messages pending, but to not block for new messages
		if ( const auto ecode = Window::ProcessMessages() )
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}

		const auto old = last;
		last = std::chrono::steady_clock::now();
		const std::chrono::duration<float> time = last - old;
		// execute the game logic
		const float dt = time.count();
		//if( dt > 30.f/1000.f ) dt = (30.f / 1000.f);
		ProcessInput();
		Update(dt);
		Render();
	}
}
void Application::ProcessInput()
{
	while ( const std::optional<KeyEvent> keyEvent = m_Keyboard.ReadKey() )
	{
		//fornow Ignore onRelease
		if( !keyEvent->IsDown() )
			continue;

		bool ctrl = m_Keyboard.IsKeyDown(VK_CONTROL) || m_Keyboard.IsKeyDown(VK_RCONTROL);
		bool shift = m_Keyboard.IsKeyDown(VK_SHIFT) || m_Keyboard.IsKeyDown(VK_RSHIFT);

		ctrl;
		shift;

		switch ( keyEvent->GetCode() )
		{
			case 1:
			break;
			default:
				break;
		}
	}

	while ( const std::optional<wchar_t> character = m_Keyboard.ReadChar() )
	{
	}

	//while ( const std::optional<MouseEvent> mouseEvent = m_Mouse.Read() )
	//{
	//
	//}
	while ( std::optional<MouseEvent> mouseEvent = m_Mouse.Read() )
	{
		//MouseEvent ev = mouseEvent;
		m_Window.m_EventCallBack(*mouseEvent);

		static int deltaX = 0;
		static int deltaY = 0;

		switch( mouseEvent->GetEventType() )
		{
			case MouseEvent::MouseEventType::WheelLeft:
			{
				deltaX--;
				std::wostringstream oss;
				oss << L"Mouse Position: (" << deltaX << L", " << deltaY << L")";
				std::wstring okTitle(oss.str().c_str());
				m_Window.SetTitle(okTitle);
				break;
			}
			case MouseEvent::MouseEventType::WheelRight:
			{
				deltaX++;
				std::wostringstream oss;
				oss << L"Mouse Position: (" << deltaX << L", " << deltaY << L")";
				std::wstring okTitle(oss.str().c_str());
				m_Window.SetTitle(okTitle);
				break;
			}
			case MouseEvent::MouseEventType::WheelUp:
			{
				deltaY++;
				std::wostringstream oss;
				oss << L"Mouse Position: (" << deltaX << L", " << deltaY << L")";
				std::wstring okTitle(oss.str().c_str());
				m_Window.SetTitle(okTitle);
				break;
			}
			case MouseEvent::MouseEventType::WheelDown:
			{
				deltaY--;
				std::wostringstream oss;
				oss << L"Mouse Position: (" << deltaX << L", " << deltaY << L")";
				std::wstring okTitle(oss.str().c_str());
				m_Window.SetTitle(okTitle);
				break;
			}
			case MouseEvent::MouseEventType::Move:
			{
				std::wostringstream oss;
				oss << L"Mouse Position: (" << mouseEvent->GetPosX() << L", " << mouseEvent->GetPosY() << L")";
				std::wstring okTitle(oss.str().c_str());
				m_Window.SetTitle(okTitle);
				
				break;
			}
		}
	}
}

void Application::Update(float dt)
{
	static int k = 0;
	static float acc =0;
	k++;
	acc += dt;
	if ( acc >= 1 )
	{
		const auto fps = std::to_wstring(k / 1000.f);
		m_Window.SetTitle(fps);
		k = 0;
		acc-=1;
	}
}
void Application::OnEvent(Event& e)
{
	
	e;


}
