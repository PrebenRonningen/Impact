#pragma once
#include "Bindable.h"
namespace Impact
{
	class SamplerState : public Bindable
	{
	public:
		SamplerState(Graphics& gfx);
		virtual void Bind(Graphics& gfx) noexcept override;
	private:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSamplerState;
	};
}

