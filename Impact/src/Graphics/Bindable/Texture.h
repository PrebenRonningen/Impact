#pragma once
#include "Bindable.h"

namespace Impact
{
	class Surface;
	class Texture : public Bindable
	{
	public:
		Texture(Graphics& gfx, const Surface& surface, const int slot = 0);
		virtual void Bind(Graphics& gfx) noexcept override;
	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTextureView; 
		int m_Slot;
	};
}

