#pragma once
#include "Core/Window.h"
#include "Core/Input.h"
#include "Core/LayerStack.h"
#include <chrono>
class KeyEvent;
class MouseEvent;

namespace Impact
{
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
	
		void PushLayer(Layer* layer) noexcept;
		void PushOverlay(Layer* layer) noexcept;
	
	private:
		// TODO: not necasserily needed.. mainly used for testing
		bool OnKeyEvent(Event& e);
		bool OnMouseMove(Event& e);
		
		
		void ProcessInput();
		void Update(float dt);
		void Render();
	private:
		Window m_Window;
		LayerStack m_LayerStack;
	
		float m_Color;
		// TODO: move to imputmanager
		static Keyboard& m_Keyboard;
		static Mouse& m_Mouse;
	
	
		std::vector<std::unique_ptr<class TestCube>> boxes;
		// TODO: move to time class
		std::chrono::steady_clock::time_point last = std::chrono::high_resolution_clock::now();
	};

	Application* CreateApplication();
}
