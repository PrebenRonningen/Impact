#pragma once
#include <optional>
#include <deque>
#include <utility>

#include "Events/MouseEvent.h"
namespace Impact
{
	class Mouse
	{
		friend class Window;
	public:
		struct RawDelta { int x, y; };
		~Mouse() = default;
	#pragma region delete
		Mouse(const Mouse& other) = delete;
		Mouse(Mouse&& other) = delete;
		Mouse& operator=(const Mouse& other) = delete;
		Mouse& operator=(Mouse&& other) = delete;
	#pragma endregion
		int* GetPos() const noexcept;
		int GetPosX() const noexcept;
		int GetPosY() const noexcept;
		bool IsInWindow() const noexcept;
		bool LeftIsPressed() const noexcept;
		bool RightIsPressed() const noexcept;
		bool MiddleIsPressed() const noexcept;


		//std::optional<MouseEvent> Read() noexcept;
		std::optional<RawDelta> ReadRawDelta() noexcept;

		//bool IsBufferEmpty() const noexcept;
		//void ClearBuffer() noexcept;

		void EnableRaw() noexcept;
		void DisableRaw() noexcept;
		bool IsRawEnabled() const noexcept;

		static Mouse& Get(){return m_Mouse;}

	private:
		Mouse() = default;

		void OnMouseMove(int x, int y) noexcept;
		void OnRawDelta(int dx, int dy) noexcept;
	
		void OnMouseLeave() noexcept;
		void OnMouseEnter() noexcept;
	
		void OnLeftPressed(int x, int y) noexcept;
		void OnLeftReleased(int x, int y) noexcept;
	
		void OnRightPressed(int x, int y) noexcept;
		void OnRightReleased(int x, int y) noexcept;
	
		void OnMiddlePressed(int x, int y) noexcept;
		void OnMiddleReleased(int x, int y) noexcept;
	
		void OnWheelUp(int x, int y) noexcept;
		void OnWheelDown(int x, int y) noexcept;
		void OnWheelLeft(int x, int y) noexcept;
		void OnWheelRight(int x, int y) noexcept;
	
		void OnWheelDelta(int x, int y, int deltaX, int deltaY) noexcept;
	
		void TrimRawInputBuffer() noexcept;


		static Mouse m_Mouse;
	};
}