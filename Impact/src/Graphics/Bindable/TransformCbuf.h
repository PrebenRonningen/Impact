#pragma once
#include "Graphics\Bindable\Bindable.h"
#include "Graphics\Bindable\ConstantBuffer.h"
#include "Graphics\Drawable\RenderComponent.h"
#include "Components\TransformComponent.h"
namespace Impact
{
	class TransformCbuf : public Bindable
	{
	public:
		TransformCbuf(Graphics& gfx, TransformComponent& parent)
			: m_Parent{parent}
		{
			if ( !m_pVcbuf )
			{
				m_pVcbuf = std::make_unique<ConstantBuffer<DirectX::XMFLOAT4X4>>(gfx,PipelineStage::VertexShader);
			}
		}
		virtual void Bind(Graphics& gfx) noexcept override
		{
			DirectX::XMFLOAT4X4 trns;

			DirectX::XMFLOAT4X4 parentTrans = m_Parent.GetTransform();
			DirectX::XMFLOAT4X4 proj = gfx.GetProjection();


			DirectX::XMMATRIX temp = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&parentTrans) * DirectX::XMLoadFloat4x4(&m_Camera) * DirectX::XMLoadFloat4x4(&proj));
			DirectX::XMStoreFloat4x4(&trns, temp);

			m_pVcbuf->Update(gfx, trns);
			m_pVcbuf->Bind(gfx);
		}
		static void SetCamera(DirectX::XMFLOAT4X4 c);
	private:
		static std::unique_ptr<ConstantBuffer<DirectX::XMFLOAT4X4>> m_pVcbuf;
		TransformComponent& m_Parent;
		static DirectX::XMFLOAT4X4 m_Camera;
	};

}