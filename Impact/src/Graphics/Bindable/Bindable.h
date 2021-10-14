#pragma once
#include "Graphics/Graphics.h"
#include "Core/Exceptions/GraphicsThrow.h"

namespace Impact
{
		enum class PipelineStage : uint8_t
		{
			VertexShader = 1,		// supported
			//HullShader = 2,
			//DomainShader = 3,
			//GeometryShader = 4,
			PixelShader = 5,		// supported
			//ComputeShader = 6
		};
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