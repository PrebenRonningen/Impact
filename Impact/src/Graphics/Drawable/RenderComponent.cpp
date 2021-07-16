#include "RenderComponent.h"

namespace Impact
{
	void RenderComponent::Draw(Graphics& gfx) const noexcept
	{
		for ( auto& b : m_Binds )
		{
			b->Bind(gfx);
		}
		gfx.DrawIndexed(m_pIndexBuffer->GetCount());
	}

	void RenderComponent::AddBind(std::unique_ptr<Bindable> pBind) noexcept
	{
		m_Binds.push_back(std::move(pBind));
	}

	void RenderComponent::AddIndexBuffer(std::unique_ptr<class IndexBuffer> pIdxBuffer) noexcept
	{
		m_pIndexBuffer = pIdxBuffer.get();
		m_Binds.push_back(std::move(pIdxBuffer));
	}
}
