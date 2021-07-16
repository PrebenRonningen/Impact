#pragma once
#include "Graphics/Bindable/Bindable.h"
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

	template<typename C>
	class ConstantBuffer : public Bindable
	{
	public:

		ConstantBuffer(Graphics& gfx, const C& consts, PipelineStage stage)
			: m_Stage{stage}
		{
			D3D11_BUFFER_DESC constantBufferDesc{};

			constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			constantBufferDesc.MiscFlags = 0;
			constantBufferDesc.StructureByteStride = 0;
			constantBufferDesc.ByteWidth = sizeof(consts);

			D3D11_SUBRESOURCE_DATA constBufferSubresourceData = {};
			constBufferSubresourceData.pSysMem = &consts;

			GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, GetDevice(gfx)->CreateBuffer(&constantBufferDesc, &constBufferSubresourceData, &m_pConstantBuffer));
		}
		ConstantBuffer(Graphics& gfx, PipelineStage stage)
			: m_Stage{stage}
		{

			D3D11_BUFFER_DESC constantBufferDesc{};
			constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			constantBufferDesc.MiscFlags = 0;
			constantBufferDesc.ByteWidth = sizeof(C);
			constantBufferDesc.StructureByteStride = 0;
			GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, GetDevice(gfx)->CreateBuffer(&constantBufferDesc, nullptr, &m_pConstantBuffer));
		}

		void Update(Graphics& gfx, const C& consts)
		{

			D3D11_MAPPED_SUBRESOURCE mappedSubresource;
			GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, 
				GetContext(gfx)->Map(
					m_pConstantBuffer.Get(), 0,
					D3D11_MAP_WRITE_DISCARD, 0,
					&mappedSubresource
			));
			memcpy(mappedSubresource.pData, &consts, sizeof(consts));
			GetContext(gfx)->Unmap(m_pConstantBuffer.Get(), 0);
		}

		virtual void Bind(Graphics& gfx) noexcept override
		{
			switch ( m_Stage )
			{
				case PipelineStage::VertexShader:
					GetContext(gfx)->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
					break;
				case PipelineStage::PixelShader:
					GetContext(gfx)->PSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
					break;
				default:
					break;
			}
		}

	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
		const PipelineStage m_Stage;
	};
}

