#include "TransformCbuf.h"

namespace Impact {
	std::unique_ptr<ConstantBuffer<DirectX::XMFLOAT4X4>> TransformCbuf::m_pVcbuf;
	DirectX::XMFLOAT4X4 TransformCbuf::m_Camera{};

	void TransformCbuf::SetCamera(DirectX::XMFLOAT4X4 c)
	{
		m_Camera = c;
	}
}