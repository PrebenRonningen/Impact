#include "Core/Keyboard.h"
#include "Core/Mouse.h"
#include "Input.h"
namespace Impact
{
	Input Input::m_Input{};
	Keyboard& Input::m_Keyboard = Keyboard::Get();
	//Mouse& Input::m_Mouse = Mouse::Get();
}