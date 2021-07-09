#include "src/Core/Keyboard.h"
#include "src/Core/Mouse.h"
#include "Input.h"

Input Input::m_Input{};
Keyboard& Input::m_Keyboard = Keyboard::Get();
//Mouse& Input::m_Mouse = Mouse::Get();