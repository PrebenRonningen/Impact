#include "VertexShader.h"
#include <stringapiset.h>
#include <d3dcompiler.h>

#pragma comment(lib,"D3DCompiler.lib")
namespace Impact
{
	VertexShader::VertexShader(Graphics& gfx, const std::string& path)
	{
		m_Path = L"../Impact/Resources/Shaders/";
		std::wstring widePath;
		int conversionResult = MultiByteToWideChar(CP_UTF8, 0, path.c_str(), (int)strlen(path.c_str()), NULL, 0);
		if (conversionResult > 0)
		{
			widePath.resize(conversionResult + 10);
			conversionResult = MultiByteToWideChar(CP_UTF8, 0, path.c_str(), (int)strlen(path.c_str()), &widePath[0], (int)widePath.size());
		}
		m_Path.append(widePath);
	
		GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, D3DReadFileToBlob(m_Path.c_str(), &m_pByteCodeBlob));
		GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, GetDevice(gfx)->CreateVertexShader(m_pByteCodeBlob->GetBufferPointer(), m_pByteCodeBlob->GetBufferSize(), nullptr, &m_pVertexShader));
	}
	
	void VertexShader::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	}
	
	ID3DBlob* VertexShader::GetByteCode() const noexcept
	{
		return m_pByteCodeBlob.Get();
	}
}