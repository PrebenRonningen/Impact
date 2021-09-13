#pragma once
#include "Core/Window.h"
#include "Core/Input.h"
#include "Core/LayerStack.h"
#include <chrono>

class KeyEvent;
class MouseEvent;

namespace Impact
{
	class ImguiLayer;

	class Application
	{
	public:
		Application();
		virtual ~Application();
	#pragma region delete
		Application(const Application& other) = delete;
		Application(Application&& other) = delete;
		Application& operator=(const Application& other) = delete;
		Application& operator=(Application&& other) = delete;
	#pragma endregion
	
		int Run();
	
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer) noexcept;
	
	private:
		// TODO: not necasserily needed.. mainly used for testing
		bool OnKeyEvent(Event& e);
		bool OnMouseEvent(Event& e);
		
		
		void ProcessInput();
		void Update(float dt);
		void Render();
	private:
		Window m_Window;
		LayerStack m_LayerStack;
		
		ImguiLayer* m_pImguiLayer;
		//std::vector<Entity*> m_Cameras;

		float m_Color;
		// TODO: move to imputmanager
		static Keyboard& m_Keyboard;
		static Mouse& m_Mouse;
	
		static bool m_Paused; 
		// TODO: move to time class
		std::chrono::steady_clock::time_point last = std::chrono::high_resolution_clock::now();
	};

	Application* CreateApplication();
}
