#pragma once
#include "src/Core/Window.h"
#include "src/Core/Input.h"
#include <chrono>
class KeyEvent;
class MouseEvent;
class Application
{
public:
	Application() noexcept;
	~Application();
#pragma region delete
	Application(const Application& other) = delete;
	Application(Application&& other) = delete;
	Application& operator=(const Application& other) = delete;
	Application& operator=(Application&& other) = delete;
#pragma endregion

	int Run();
	void ProcessInput();
	void Update(float dt);
	void Render(){};
	void OnEvent(Event& e);
	void OnMouseMove(Event& e);
private:
	Window m_Window;	
	static Keyboard& m_Keyboard;
	static Mouse& m_Mouse;




	std::chrono::steady_clock::time_point last = std::chrono::high_resolution_clock::now();
};

