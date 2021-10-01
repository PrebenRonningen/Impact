#include "TransformCbuf.h"

namespace Impact {
	std::unique_ptr<ConstantBuffer<TransformCbuf::Transforms>> TransformCbuf::m_pVcbuf;
	DirectX::XMFLOAT4X4 TransformCbuf::m_Camera{};

	TransformCbuf::TransformCbuf(Graphics& gfx, TransformComponent& parent)
		: m_Parent{ parent }
	{
		if (!m_pVcbuf)
		{
			m_pVcbuf = std::make_unique<ConstantBuffer<Transforms>>(gfx, PipelineStage::VertexShader);
		}
	}
	void TransformCbuf::Bind(Graphics& gfx) noexcept
	{
		DirectX::XMFLOAT4X4 parentTrans = m_Parent.GetTransform();
		DirectX::XMFLOAT4X4 proj = gfx.GetProjection();
		Transforms transforms;
		DirectX::XMStoreFloat4x4(&transforms.world, DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&parentTrans)));

		parentTrans = m_Parent.GetTransform();
		DirectX::XMMATRIX temp = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&parentTrans) * DirectX::XMLoadFloat4x4(&m_Camera) * DirectX::XMLoadFloat4x4(&proj));
		DirectX::XMStoreFloat4x4(&transforms.worldViewProjection, temp);


		m_pVcbuf->Update(gfx, transforms);
		m_pVcbuf->Bind(gfx);
	}
	void TransformCbuf::SetCamera(DirectX::XMFLOAT4X4 c)
	{
		m_Camera = c;
	}
}