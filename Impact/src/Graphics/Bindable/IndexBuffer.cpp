#include "IndexBuffer.h"

Impact::IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<uint16_t>& indices)
	: m_Count{uint32_t(indices.size())}
{
	D3D11_BUFFER_DESC indexBufferDesc{};

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = sizeof(uint16_t);
	indexBufferDesc.ByteWidth = m_Count * sizeof(uint16_t);

	D3D11_SUBRESOURCE_DATA indexSubResourceData = {};
	indexSubResourceData.pSysMem = indices.data();

	GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, GetDevice(gfx)->CreateBuffer(&indexBufferDesc, &indexSubResourceData, &m_pIndexBuffer));
	
}

void Impact::IndexBuffer::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}
