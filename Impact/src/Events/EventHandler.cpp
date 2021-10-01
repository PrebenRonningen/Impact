#include "EventHandler.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"


namespace Impact
{
	std::deque<KeyEvent> EventHandler::m_KeyBuffer{};
	std::array<MouseEvent, EventHandler::m_BufferSize> EventHandler::m_MouseBuffer{};
	std::vector<uint64_t> EventHandler::m_funcSources{};
	
	uint8_t m_MouseIndex{0};
	std::map<int, std::vector<std::pair<uint16_t, std::function<bool(Event&)>>>> EventHandler::m_LayerCallback{};
	uint16_t EventHandler::m_RegisteredEventTypes{0};
	static uint16_t eT{};
	void EventHandler::AddKeyEvent(KeyEvent&& event) noexcept
	{
		m_KeyBuffer.push_back(std::move(event));
		TrimBuffer(m_KeyBuffer);
	}

	void EventHandler::AddMouseEvent( MouseEvent&& event) noexcept
	{
		m_MouseBuffer[(m_MouseIndex < m_BufferSize) ? m_MouseIndex++ : m_BufferSize-1] = std::move(event);
	}

	void EventHandler::RegisterEvent(void* ovner, int layer, const Event::EventType eventType, const std::function<bool(Event&)>& callback) noexcept
	{
		bool exist = false;
		auto source = std::find(m_funcSources.begin(), m_funcSources.end(), (uint64_t)ovner);
		if(source != m_funcSources.end())
			exist = true;

	//	m_MouseBuffer.resize(m_BufferSize);
		auto it = m_LayerCallback.find(layer);
		if ( it != m_LayerCallback.end() )
		{
			auto eventPair = std::find_if(it->second.begin(), it->second.end(), 
							[&callback](std::pair<uint16_t, std::function<bool(Event&)>>& pair)
							{
								bool k = (uint64_t&) callback ^ (uint64_t&) pair.second;
								return !k;
							});

			if ( eventPair != it->second.end() )
			{
				eventPair->first |= static_cast<uint16_t>( eventType );
				m_RegisteredEventTypes |= static_cast<uint16_t>(eventType);
				if(exist)
					return;
			}

		//m_LayerCallback[layer].push_back(std::make_pair(eventType, callback));
		//if ( Layer* l = m_LayerCallback .find(layer) )
		}

		m_LayerCallback[layer].push_back(std::make_pair(static_cast<uint16_t>(eventType), callback));
		m_RegisteredEventTypes |= static_cast<uint16_t>( eventType );

		if(!exist)
			m_funcSources.push_back((uint64_t)ovner);

	}

	void EventHandler::ProcessEvent() noexcept
	{
		static KeyEvent* kEvent = nullptr;
		while ( !m_KeyBuffer.empty())
		{
			kEvent = std::move(&m_KeyBuffer.front());
			m_KeyBuffer.pop_front();

			HandleEvent(kEvent);
		}

		for (uint8_t i{ 0 }; i < m_MouseIndex; ++i)
		{
			HandleEvent(&m_MouseBuffer[i]);
		}
		m_MouseIndex = 0;
	}

	void EventHandler::HandleEvent(Event* event) noexcept
	{
		eT = static_cast<uint16_t>( event->GetType() );
		if ( !( eT & m_RegisteredEventTypes ) )
			return;

		for ( auto it = m_LayerCallback.begin(); it != m_LayerCallback.end(); ++it )
		{
			for ( const auto& eventPair : it->second )
			{
				if ( eT & eventPair.first )
				{
					event->m_Handled |= eventPair.second(*event);
					if( event->m_Handled ) break;
				}
			}
			if (event->m_Handled) break;
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