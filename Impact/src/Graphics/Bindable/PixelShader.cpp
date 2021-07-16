#include "PixelShader.h"
#include <d3dcompiler.h>

#pragma comment(lib,"D3DCompiler.lib")
Impact::PixelShader::PixelShader(Graphics& gfx, const std::string& path)
	: m_Path{path}
{
	GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, D3DReadFileToBlob(L"D:/Git/Impact/Impact/Resources/Shaders/PixelShader.cso", &m_pByteCodeBlob));
	GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, GetDevice(gfx)->CreatePixelShader(m_pByteCodeBlob->GetBufferPointer(), m_pByteCodeBlob->GetBufferSize(), nullptr, &m_pPixelShader));
}

void Impact::PixelShader::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
}

ID3DBlob* Impact::PixelShader::GetByteCode() const noexcept
{
	return m_pByteCodeBlob.Get();
}
