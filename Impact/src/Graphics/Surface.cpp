#ifndef FULL_WIN
	#define FULL_WIN
#endif 
#include "Surface.h"
#include <algorithm>
namespace Gdiplus
{
	using std::min;
	using std::max;
}

#pragma warning( push)
#pragma warning(disable: 4458)
#include <gdiplus.h>
#pragma warning( pop )

#include <sstream>
#pragma comment(lib, "gdiplus.lib")

namespace Impact
{
	Surface::Surface(uint32_t width, uint32_t height) noexcept
		: m_pColorBuffer{std::make_unique<ColorARGB[]>(width * height)}
		, m_Width{width}
		, m_Height{height}
		{}

	Surface::Surface(Surface&& other) noexcept
		: m_pColorBuffer(std::move(other.m_pColorBuffer))
		, m_Width(other.m_Width)
		, m_Height(other.m_Height)
	{}

	Surface& Surface::operator=(Surface&& other) noexcept
	{
		m_Width = other.m_Width;
		m_Height = other.m_Height;
		m_pColorBuffer = std::move(other.m_pColorBuffer);
		other.m_pColorBuffer = nullptr;
		return *this;
	}

	Surface::~Surface()
	{
	}

	uint32_t Surface::GetWidth() const noexcept
	{
		return m_Width;
	}
	uint32_t Surface::GetHeight() const noexcept
	{
		return m_Height;
	}

	Surface Surface::CreateFromFile(const std::string& name)
	{
		uint32_t width, height = 0;
		std::unique_ptr<ColorARGB[]> pColorBuffer{};

		wchar_t wName[512];
		mbstowcs_s(nullptr, wName, name.c_str(), _TRUNCATE);

		Gdiplus::Bitmap bitmap(wName);
		if (bitmap.GetLastStatus() != Gdiplus::Status::Ok)
		{
			std::stringstream ss;
			ss << "Loading image [" << name << "]: failed to load.";
			throw SurfaceException(__LINE__, __FILE__, ss.str());
		}

		width = bitmap.GetWidth();
		height = bitmap.GetHeight();
		pColorBuffer = std::make_unique<ColorARGB[]>(width * height);

		for (uint32_t y = 0; y < height; ++y)
		{
			for (uint32_t x = 0; x < width; ++x)
			{
				Gdiplus::Color c;
				bitmap.GetPixel(x, y, &c);
				pColorBuffer[y * width + x] = c.GetValue();
			}
		}

		return Surface(width, height, std::move(pColorBuffer));
	}

	Surface::ColorARGB* Surface::GetBufferPtr() const noexcept
	{
		return m_pColorBuffer.get();
	}

	Surface::Surface(uint32_t width, uint32_t height, std::unique_ptr<ColorARGB[]> pColorBuffer) noexcept
		: m_Width(width)
		, m_Height(height)
		, m_pColorBuffer(std::move(pColorBuffer))
	{}
}