#include <DirectXMath.h>
#include "Core/WinMain.h"
#include "TestLayer.h"


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

