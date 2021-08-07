#pragma once
#include "Core\ImpactWin.h"
#include "Core\Exceptions\SurfaceException.h"
#include <cstdint>
#include <memory>

namespace Impact
{
	class Surface
	{
	public:
		class ColorARGB
		{
		public:
			constexpr ColorARGB() noexcept
				: color{}
			{};
			constexpr ColorARGB(const ColorARGB& other) noexcept
				: color{ other.color }
			{};
			constexpr ColorARGB(uint32_t col) noexcept
				: color{ col }
			{};
			constexpr ColorARGB(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept
				: color{ uint32_t((a << 24) | (r << 16) | (g << 8) | (b)) }
			{};
			constexpr ColorARGB(uint8_t r, uint8_t g, uint8_t b) noexcept
				: color{ uint32_t((r << 16) | (g << 8) | (b)) }
			{};
			constexpr ColorARGB(ColorARGB col, uint8_t a) noexcept
				: color{ uint32_t((a << 24) | (col.color)) }
			{};
			ColorARGB& operator=(ColorARGB col) noexcept
			{
				color = col.color;
				return *this;
			}
			constexpr uint8_t GetA() const noexcept
			{
				return (color >> 24);
			}
			constexpr uint8_t GetR() const noexcept
			{
				return (color >> 16) & 0xFF;
			}
			constexpr uint8_t GetG() const noexcept
			{
				return (color >> 8) & 0xFF;
			}
			constexpr uint8_t GetB() const noexcept
			{
				return color & 0xFF;
			}
			void SetA(uint8_t a) noexcept
			{
				color = (color & 0xFFFFFFu) | (a << 24u);
			}
			void SetR(uint8_t r) noexcept
			{
				color = (color & 0xFF00FFFFu) | (r << 16u);
			}
			void SetG(uint8_t g) noexcept
			{
				color = (color & 0xFFFF00FFu) | (g << 8u);
			}
			void SetB(uint8_t b) noexcept
			{
				color = (color & 0xFFFFFF00u) | b;
			}
		public:
			uint32_t color;
		};
	public:
		Surface(uint32_t width, uint32_t height) noexcept;
		Surface(const Surface& other) = delete;
		Surface(Surface&& other) noexcept;
		Surface& operator=(const Surface& other) = delete;
		Surface& operator=(Surface&& other) noexcept;
		~Surface();

		uint32_t GetWidth() const noexcept;
		uint32_t GetHeight() const noexcept;
		ColorARGB* GetBufferPtr() const noexcept;
		static Surface CreateFromFile(const std::string& name);
	private:
		Surface(uint32_t width, uint32_t height, std::unique_ptr<ColorARGB[]> pBufferParam) noexcept;
		
		std::unique_ptr<ColorARGB[]> m_pColorBuffer;
		uint32_t m_Width;
		uint32_t m_Height;

	};
}

