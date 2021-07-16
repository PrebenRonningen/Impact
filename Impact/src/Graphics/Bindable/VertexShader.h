#pragma once
#include "Graphics/Bindable/Bindable.h"

namespace Impact
{
	class VertexShader : public Bindable
	{
	public:
		VertexShader(Graphics& gfx, const std::string_view& path); //consider std::string_view
		virtual void Bind(Graphics& gfx) noexcept override;
		ID3DBlob* GetByteCode() const noexcept;
	private:
		std::string m_Path;
		Microsoft::WRL::ComPtr<ID3DBlob> m_pByteCodeBlob;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	};
}

