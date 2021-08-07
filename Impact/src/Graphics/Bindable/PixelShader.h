#pragma once
#include "Graphics/Bindable/Bindable.h"

namespace Impact
{
	class PixelShader : public Bindable
	{
	public:
		PixelShader(Graphics& gfx, const std::string& path); //consider std::string_view
		virtual void Bind(Graphics& gfx) noexcept override;
		ID3DBlob* GetByteCode() const noexcept;
	private:
		std::wstring m_Path;
		Microsoft::WRL::ComPtr<ID3DBlob> m_pByteCodeBlob;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	};
}

