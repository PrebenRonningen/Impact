#include "EventHandler.h"
#include "src/Events/KeyEvent.h"
#include "src/Events/MouseEvent.h"

static constexpr uint8_t m_BufferSize = 16u;
std::deque<KeyEvent> EventHandler::m_KeyBuffer{};
std::deque<MouseEvent> EventHandler::m_MouseBuffer{};
std::map<int, std::vector<std::pair<uint16_t, std::function<void(Event&)>>>>  EventHandler::m_LayerCallback{};
uint16_t EventHandler::m_RegisteredEventTypes{0};
static uint16_t eT{};
void EventHandler::AddKeyEvent(KeyEvent&& event) noexcept
{
	m_KeyBuffer.push_back(std::move(event));
	TrimBuffer(m_KeyBuffer);
}

void EventHandler::AddMouseEvent(MouseEvent&& event) noexcept
{
	m_MouseBuffer.push_back(std::move(event));
	TrimBuffer(m_MouseBuffer);
}

void EventHandler::RegisterEvent(int layer, const Event::EventType eventType, const std::function<void(Event&)>& callback) noexcept
{
	auto it = m_LayerCallback.find(layer);
	if ( it != m_LayerCallback.end() )
	{
		auto eventPair = std::find_if(it->second.begin(), it->second.end(), 
						[&callback](std::pair<uint16_t, std::function<void(Event&)>>& pair)
						{
							auto a = callback.target_type().name();
							auto b = pair.second.target_type().name();

							auto k = a == b;
							return k;
						});

		if ( eventPair != it->second.end() )
		{
			eventPair->first |= static_cast<uint16_t>( eventType );
			m_RegisteredEventTypes |= static_cast<uint16_t>(eventType);
			return;
		}

	//m_LayerCallback[layer].push_back(std::make_pair(eventType, callback));
	//if ( Layer* l = m_LayerCallback .find(layer) )
	}

	m_LayerCallback[layer].push_back(std::make_pair(static_cast<uint16_t>(eventType), callback));
	m_RegisteredEventTypes |= static_cast<uint16_t>( eventType );

}

void EventHandler::ProcessEvent() noexcept
{
	while ( !m_KeyBuffer.empty())
	{
		static KeyEvent* kEvent = std::move(&m_KeyBuffer.front());
		m_KeyBuffer.pop_front();

		HandleEvent(kEvent);
	}
	while ( !m_MouseBuffer.empty() )
	{
		static MouseEvent* mEvent = std::move(&m_MouseBuffer.front());
		m_MouseBuffer.pop_front();
	
		HandleEvent(mEvent);
	}
}

template <typename T>
void EventHandler::HandleEvent(const T event) noexcept
{
	eT = static_cast<uint16_t>( event->GetType() );
	if ( !( eT & m_RegisteredEventTypes ) )
		return;

	for ( auto it = m_LayerCallback.begin(); it != m_LayerCallback.end(); ++it )
	{
		for ( auto eventPair : it->second )
		{
			if ( eT & eventPair.first )
			{
				eventPair.second(*event);
				return;
			}
		}

		//for( pair<(eventTypeMask)bitset<16>, Function> layerEvents : pLayer )
		//{
		//	if(layerEvents.first & event.type())
		//	{
		//		event.handeled |= (layerEvents.second(event) ^ fallThroughEvent);
		//		break;
		//	}
		//}
	}
}

template<typename T>
void EventHandler::TrimBuffer(std::deque<T>& buffer) noexcept
{
	while ( buffer.size() > m_BufferSize )
	{
		buffer.pop_back();
	}
}