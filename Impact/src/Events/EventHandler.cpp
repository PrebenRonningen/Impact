#include "EventHandler.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"


namespace Impact
{
	static constexpr uint8_t m_BufferSize = 16u;
	std::deque<KeyEvent> EventHandler::m_KeyBuffer{};
	std::deque<MouseEvent> EventHandler::m_MouseBuffer{};
	std::map<int, std::vector<std::pair<uint16_t, std::function<bool(Event&)>>>> EventHandler::m_LayerCallback{};
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

	void EventHandler::RegisterEvent(int layer, const Event::EventType eventType, const std::function<bool(Event&)>& callback) noexcept
	{
		auto it = m_LayerCallback.find(layer);
		if ( it != m_LayerCallback.end() )
		{
			auto eventPair = std::find_if(it->second.begin(), it->second.end(), 
							[&callback](std::pair<uint16_t, std::function<bool(Event&)>>& pair)
							{
								bool k = (uint32_t&) callback ^ (uint32_t&) pair.second;
								return !k;
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

	void EventHandler::HandleEvent(Event* event) noexcept
	{
		eT = static_cast<uint16_t>( event->GetType() );
		if ( !( eT & m_RegisteredEventTypes ) )
			return;

		for ( auto it = m_LayerCallback.rbegin(); it != m_LayerCallback.rend(); ++it )
		{
			for ( const auto& eventPair : it->second )
			{
				if ( eT & eventPair.first )
				{
					event->m_Handled |= eventPair.second(*event);
					if( event->m_Handled ) break;
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
}