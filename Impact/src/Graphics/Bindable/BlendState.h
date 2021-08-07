#pragma once
#include "Bindable.h"
namespace Impact
{
	class BlendState : public Bindable
	{
	public:
		BlendState(Graphics& gfx);
		virtual void Bind(Graphics& gfx) noexcept override;
	private:
		Microsoft::WRL::ComPtr<ID3D11BlendState> m_pBlendState;
		float m_BlendFactor[4] = {0,0,0,0};
		UINT m_SampleMask = 0xFFFFFFFF;
	};
}

