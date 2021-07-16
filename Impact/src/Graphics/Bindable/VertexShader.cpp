#include "VertexShader.h"
#include <d3dcompiler.h>

#pragma comment(lib,"D3DCompiler.lib")
Impact::VertexShader::VertexShader(Graphics& gfx, const std::string_view& path)
	: m_Path{path}
{
	GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, D3DReadFileToBlob(L"D:/Git/Impact/Impact/Resources/Shaders/VertexShader.cso", &m_pByteCodeBlob));
	GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, GetDevice(gfx)->CreateVertexShader(m_pByteCodeBlob->GetBufferPointer(), m_pByteCodeBlob->GetBufferSize(), nullptr, &m_pVertexShader));
}

void Impact::VertexShader::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
}

ID3DBlob* Impact::VertexShader::GetByteCode() const noexcept
{
	return m_pByteCodeBlob.Get();
}
