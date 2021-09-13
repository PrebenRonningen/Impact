#include "Application.h"
//#include "src/Core/Input.h"
#include "Core/Keyboard.h"
#include "Core/Mouse.h"

#include "Events/EventHandler.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Graphics/Graphics.h"

#include "Graphics/Bindable/TransformCbuf.h"
#include "Core/QuaternionToEuler.h"
#include <sstream>
#include <map>
#include <algorithm>
#include <numeric>
#include <array>
#include "Imgui/Layer/ImguiLayer.h"


namespace Impact
{
	//Input& Application::m_Input = m_Input.Get();
	bool Application::m_Paused = false;
	Keyboard& Application::m_Keyboard = Keyboard::Get();
	Mouse& Application::m_Mouse = Mouse::Get();

	Application::Application()
		: m_Window{ 1280u, 720u, L"Impact 3D" }
		, m_LayerStack{}
		, m_Color{0}
		, m_pImguiLayer{nullptr}
	{
		// make easier and more readable
		EventHandler::RegisterEvent(0, Event::EventType::KeyPressed | Event::EventType::KeyReleased, [this](auto&&... args) -> decltype( auto )
									{
										return this->Application::OnKeyEvent(std::forward<decltype( args )>(args)...);
									});
		
		// this must be fone so the function pointer is the same for the functions registering
		// can be avoided by using bitwise or to add the EventTypes together
		auto onMove = [this](auto&&... args) -> decltype( auto )
		{
			return this->Application::OnMouseEvent(std::forward<decltype( args )>(args)...);
		};
		EventHandler::RegisterEvent(0, Event::EventType::Move, onMove);
		EventHandler::RegisterEvent(0, Event::EventType::LPressed, onMove);
		EventHandler::RegisterEvent(0, Event::EventType::LPressed | Event::EventType::LReleased, onMove);

		PushOverlay(m_pImguiLayer = new ImguiLayer());
		
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
			const float dt = time.count() * m_pImguiLayer->GetSpeedFactor(); // <- testing ImGuiLayer only for Testing.. TODO: Remove when no longer needed
			//if( dt > 30.f/1000.f ) dt = (30.f / 1000.f);
			EventHandler::ProcessEvent();
			Update(dt);
			Render();
		}
	}
	
	void Application::PushLayer(Layer* layer) 
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach(m_Window.GetGraphix());
	}
	
	void Application::PushOverlay(Layer* layer) noexcept
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach(m_Window.GetGraphix());
	}
	
	void Application::ProcessInput()
	{
	}

	void Application::Update(float dt)
	{
		if (!m_Paused)
			for ( Layer* layer : m_LayerStack )
				layer->Update(dt);
	}

	void Application::Render()
	{
		m_Color = sin(std::chrono::duration<float>(std::chrono::steady_clock::now().time_since_epoch()).count()) / 2.f + 0.5f;
		DirectX::XMFLOAT4 col{ m_Color,m_Color,m_Color,m_Color };
		DirectX::XMStoreFloat4(&col, DirectX::XMVectorMultiply(DirectX::XMLoadFloat4(&col), DirectX::Colors::CornflowerBlue));

		m_Window.GetGraphix().ClearBuffer( col );

		for ( Layer* layer : m_LayerStack )
			layer->Render(m_Window.GetGraphix());

		// Imgui Rendering 
		if (m_pImguiLayer->IsEnabled())
			m_pImguiLayer->Begin();

		for (Layer* layer : m_LayerStack)
			layer->ImGuiRender();

		if(m_pImguiLayer->IsEnabled())
			m_pImguiLayer->End();
		
		m_Window.GetGraphix().Present();
	}
	
	bool Application::OnKeyEvent(Event& e)
	{
		auto k = static_cast<KeyEvent&>(e);

		switch ( k.GetType() )
		{
			case Event::EventType::KeyPressed:
				{
					if(m_Keyboard.IsKeyDown('V'))
						m_Window.GetGraphix().VSynchOnOff();

					if(m_Keyboard.IsKeyDown(VK_SPACE))
						m_Paused = !m_Paused;

					if (m_Keyboard.IsKeyDown(VK_TAB))
					{
						(m_pImguiLayer->IsEnabled()) ? m_pImguiLayer->Dissable() : m_pImguiLayer->Enable();
					}

					if (m_Keyboard.IsKeyDown('1'))
					{
						m_Window.GetGraphix().SetState(0);
					}
					if (m_Keyboard.IsKeyDown('2'))
					{
						m_Window.GetGraphix().SetState(1);
					}
					if (m_Keyboard.IsKeyDown('3'))
					{
						m_Window.GetGraphix().SetState(2);
					}
				break;
				}

			default:
				break;
		}

		return false;
	}
	
	bool Application::OnMouseEvent(Event& e)
	{
		MouseEvent m = static_cast<MouseEvent&>( e );
	
		switch ( m.GetType() )
		{
			case Event::EventType::LPressed:
				break;
			case Event::EventType::LReleased:
				break;
			case Event::EventType::RPressed:
				break;
			case Event::EventType::RReleased:
				break;
			case Event::EventType::MPressed:
				break;
			case Event::EventType::MReleased:
				break;
			case Event::EventType::WheelUp:
				break;
			case Event::EventType::WheelDown:
				break;
			case Event::EventType::WheelRight:
				break;
			case Event::EventType::WheelLeft:
				break;
			case Event::EventType::Move:
				break;
			case Event::EventType::RawMove:
				break;
			case Event::EventType::Enter:
				break;
			case Event::EventType::Leave:
				break;
			default:
				break;
		}
	
		return false;
	}
}