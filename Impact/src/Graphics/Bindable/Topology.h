#pragma once
#include "Graphics/Bindable/Bindable.h"

namespace Impact
{
	class Topology : public Bindable
	{
	public:
		Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type)
			: m_Type{type}
		{
			gfx;
		}
		virtual void Bind(Graphics& gfx) noexcept override
		{
			GetContext(gfx)->IASetPrimitiveTopology(m_Type);
		}
	private:
		D3D11_PRIMITIVE_TOPOLOGY m_Type;
	};
}