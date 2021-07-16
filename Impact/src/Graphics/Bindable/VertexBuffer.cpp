#include "VertexBuffer.h"

void Impact::VertexBuffer::Bind(Graphics& gfx) noexcept
{
	const UINT offset = 0;
	GetContext(gfx)->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &m_Stride, &offset);

}
