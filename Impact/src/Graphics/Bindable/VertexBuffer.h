#pragma once
#include "Graphics/Bindable/Bindable.h"
namespace Impact
{
	class VertexBuffer : public Bindable
	{
	public:
		template <class V>
		VertexBuffer(Graphics& gfx, const std::vector<V>& vertices)
			: m_Stride(sizeof(V))
		{
			D3D11_BUFFER_DESC vertexBufferDesc{};

			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = 0;
			vertexBufferDesc.MiscFlags = 0;
			vertexBufferDesc.StructureByteStride = sizeof(V);
			vertexBufferDesc.ByteWidth = UINT(sizeof(V) * vertices.size());

			D3D11_SUBRESOURCE_DATA vertexSubResourceData = {};
			vertexSubResourceData.pSysMem = vertices.data();
		
			GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, GetDevice(gfx)->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &m_pVertexBuffer));
		}
		virtual void Bind(Graphics& gfx) noexcept override;
	private:
		const uint32_t m_Stride; 	
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
	};
}