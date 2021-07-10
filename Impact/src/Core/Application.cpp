#include "Application.h"
//#include "src/Core/Input.h"
#include "src/Core/Keyboard.h"
#include "src/Core/Mouse.h"
#include "src/Events/EventHandler.h"
#include "src/Events/KeyEvent.h"
#include "src/Events/MouseEvent.h"
#include <sstream>

#include <map>
#include <algorithm>

//Input& Application::m_Input = m_Input.Get();
Keyboard& Application::m_Keyboard = Keyboard::Get();
Mouse& Application::m_Mouse = Mouse::Get();

Application::Application() noexcept
	: m_Window{ 640, 480, L"Impact 3D" }
{
	// make easier and better to read
	EventHandler::RegisterEvent(0, Event::EventType::KeyPressed, [this](auto&&... args) -> decltype( auto )
								{
									return this->Application::OnEvent(std::forward<decltype( args )>(args)...);
								});

	auto onMove = [this](auto&&... args) -> decltype( auto )
	{
		return this->Application::OnMouseMove(std::forward<decltype( args )>(args)...);
	};
	EventHandler::RegisterEvent(0, Event::EventType::Move, onMove);

	EventHandler::RegisterEvent(0, Event::EventType::LPressed, onMove);

	EventHandler::RegisterEvent(0, Event::EventType((int) Event::EventType::LPressed | (int)Event::EventType::LReleased), onMove);


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

	EventHandler::ProcessEvent();
//	Event e;
//	while(SysyemEventCoontainer.PopEvent(e))
//	{
//			int mouse or keyboard
//		EventHandler::AddEvent(MouseEvent{Event::MouseEventType::WheelUp, *this});
//		
// 
//		UILayer
//		{
//										this <-(that's the layer that'll be an int idk well see)
//			EventHandler::RegisterEvent(this , <Event::MouseEventType::WheelUp, UILayer::OnMouseScroll>, fallthrough = false);
//			EventHandler::RegisterEvent(this , <Event::MouseEventType::WheelDown, UILayer::OnMouseScroll>, fallthrough = true);
// 
//		}
// 
//		{in RegisterEvent(layer*, <EventTypeS, pFunctionCallback>, fallthrough)
//		
//		if(layer* l = m_Layerfunctioncallbacks.find(layer))
//			l.fisrst |= eventTypes;
//			l.second.push_back(pFunctionCallback)
//		}
// 
// 
//		{ in handle event
//		
// 
//		Event event = EventContainer.Pop();
//								consider bitmasking instead of vector
//		map<layer*, vector<pair((eventTypeMask)bitset<16>, Function )>> m_Layerfunctioncallbacks   l
// 
//		for(layer* pLayer : l)
//		{
//			for( pair<(eventTypeMask)bitset<16>, Function> layerEvents : pLayer )
//			{
//				if(layerEvents.first & event.type())
//				{
//					event.handeled |= (layerEvents.second(event) ^ fallThroughEvent);
//					break;
//				}
//			}
//		}
// 
//		}
// 
//		for(auto it = m_LayerStack.rbegin(); it != m_Layerstack.rend(); ++it)
//		{
//			if(e.Handled)
//				break;
//			(*it)->OnEvent(e);
//		}
//	}
//	
//
	//int layer5 = 5;
	//int layer3 = 3;
	//std::map<int, std::vector<std::pair<int, char>>> k;
	//
	//k[layer5].push_back(std::make_pair<int, char>(4, '5'));
	//k[layer3].push_back(std::make_pair<int, char>(40, 'k'));
	//char fins = 'k';
	//int et = 5;
	//auto h = std::find_if(k[layer3].begin(), k[layer3].end(), [&fins](std::pair<int, char>& pair){return pair.second == fins;});
	//
	//if(h == k[layer3].end() )
	//	k[layer3].push_back({et, fins});
	////k.emplace(layer, k[0]);
	//h->first |= et;
	//for ( auto i : k )
	//{
	//	for ( auto me : i.second )
	//	{
	//		me.first;
	//		me.second;
	//	}
	//}






	//while ( const std::optional<KeyEvent> keyEvent = m_Keyboard.ReadKey() )
	//{
	//	//fornow Ignore onRelease
	//	if( !keyEvent->IsDown() )
	//		continue;
	//
	//	bool ctrl = m_Keyboard.IsKeyDown(VK_CONTROL) || m_Keyboard.IsKeyDown(VK_RCONTROL);
	//	bool shift = m_Keyboard.IsKeyDown(VK_SHIFT) || m_Keyboard.IsKeyDown(VK_RSHIFT);
	//
	//	ctrl;
	//	shift;
	//
	//	switch ( keyEvent->GetCode() )
	//	{
	//		case 1:
	//		break;
	//		default:
	//			break;
	//	}
	//}
	//
	//while ( const std::optional<wchar_t> character = m_Keyboard.ReadChar() )
	//{
	//}
	//
	////while ( const std::optional<MouseEvent> mouseEvent = m_Mouse.Read() )
	////{
	////
	////}
	//while ( std::optional<MouseEvent> mouseEvent = m_Mouse.Read() )
	//{
	//	//MouseEvent ev = mouseEvent;
	//	m_Window.m_EventCallBack(*mouseEvent);
	//
	//	static int deltaX = 0;
	//	static int deltaY = 0;
	//
	//	switch( mouseEvent->GetType() )
	//	{
	//		case MouseEvent::EventType::WheelLeft:
	//		{
	//			deltaX--;
	//			std::wostringstream oss;
	//			oss << L"Mouse Position: (" << deltaX << L", " << deltaY << L")";
	//			std::wstring okTitle(oss.str().c_str());
	//			m_Window.SetTitle(okTitle);
	//			break;
	//		}
	//		case MouseEvent::EventType::WheelRight:
	//		{
	//			deltaX++;
	//			std::wostringstream oss;
	//			oss << L"Mouse Position: (" << deltaX << L", " << deltaY << L")";
	//			std::wstring okTitle(oss.str().c_str());
	//			m_Window.SetTitle(okTitle);
	//			break;
	//		}
	//		case MouseEvent::EventType::WheelUp:
	//		{
	//			deltaY++;
	//			std::wostringstream oss;
	//			oss << L"Mouse Position: (" << deltaX << L", " << deltaY << L")";
	//			std::wstring okTitle(oss.str().c_str());
	//			m_Window.SetTitle(okTitle);
	//			break;
	//		}
	//		case MouseEvent::EventType::WheelDown:
	//		{
	//			deltaY--;
	//			std::wostringstream oss;
	//			oss << L"Mouse Position: (" << deltaX << L", " << deltaY << L")";
	//			std::wstring okTitle(oss.str().c_str());
	//			m_Window.SetTitle(okTitle);
	//			break;
	//		}
	//		case MouseEvent::EventType::Move:
	//		{
	//			std::wostringstream oss;
	//			oss << L"Mouse Position: (" << mouseEvent->GetPosX() << L", " << mouseEvent->GetPosY() << L")";
	//			std::wstring okTitle(oss.str().c_str());
	//			m_Window.SetTitle(okTitle);
	//			
	//			break;
	//		}
	//	}
	//}
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
	auto k = static_cast<KeyEvent&>(e).GetType();
	k;


}


void Application::OnMouseMove(Event& e)
{
	static int i = 0;
	auto m = static_cast<MouseEvent&>( e );

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
			{
				#ifdef NDEBUG
				std::wostringstream oss;
				oss << L"Mouse Position: (" << m.GetPosX() << L", " << m.GetPosY() << L")\n";
				std::wstring okTitle(oss.str().c_str());
				OutputDebugStringW(okTitle.c_str());
				#endif // NDEBUG
			break;
			}
		case Event::EventType::RawMove:
			break;
		case Event::EventType::Enter:
			break;
		case Event::EventType::Leave:
			break;
		default:
			break;
	}
}
