#pragma once
#include "Graphics/Graphics.h"
#include "Core/Exceptions/GraphicsThrow.h"

namespace Impact
{
	class Bindable
	{
	public:
		virtual void Bind(Graphics& gfx) noexcept = 0;
		virtual ~Bindable() = default;
	protected:
		static ID3D11DeviceContext* GetContext( Graphics& gfx) noexcept
		{
			return gfx.m_pDeviceContext; 
		}; 
		static ID3D11Device* GetDevice(Graphics& gfx) noexcept
		{
			return gfx.m_pDevice;
		};
	};
}