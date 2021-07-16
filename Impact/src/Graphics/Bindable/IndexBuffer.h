#pragma once
#include "Graphics/Bindable/Bindable.h"

namespace Impact
{
	class IndexBuffer : public Bindable
	{
	public:
		IndexBuffer(Graphics& gfx, const std::vector<uint16_t>& indices);
		virtual void Bind(Graphics& gfx) noexcept override;
		uint32_t GetCount() const noexcept {return m_Count;};
	private:
		uint32_t m_Count;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
	};
}

