#pragma once
namespace Impact
{
	class Keyboard;
	class Mouse;
	
	class Input
	{
	public:
		static Input& Get(){return m_Input;}
	private:
		Input() = default;
		static Input m_Input;
		static Keyboard& m_Keyboard;
		static Mouse& m_Mouse;
	};
}