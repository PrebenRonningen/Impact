
#include <Impact.h>
#include <DirectXMath.h>
#include "Core/WinMain.h"

class TestLayer : public Impact::Layer
{
public:
	virtual void OnAttach() noexcept override{};
	virtual void OnDetach() noexcept override{};
	virtual void Update(float dt) noexcept override{dt;};
	virtual void Render() const noexcept override{};
};



class Sandbox : public Impact::Application
{
public:
	Sandbox() : Impact::Application()
	{
		PushLayer(new TestLayer());
	}
	virtual ~Sandbox() override{};
};

Impact::Application* Impact::CreateApplication()
{
	return new Sandbox();
}

