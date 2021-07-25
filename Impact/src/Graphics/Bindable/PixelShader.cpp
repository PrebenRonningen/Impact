#include "PixelShader.h"
#include <locale>
#include <codecvt>
#include <d3dcompiler.h>

#pragma comment(lib,"D3DCompiler.lib")
namespace Impact {
	PixelShader::PixelShader(Graphics& gfx, const std::string& path)
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
		GFX_EXCEPTION_NOINFO(__LINE__, __FILE__, GetDevice(gfx)->CreatePixelShader(m_pByteCodeBlob->GetBufferPointer(), m_pByteCodeBlob->GetBufferSize(), nullptr, &m_pPixelShader));
	}
	
	void PixelShader::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
	}
	
	ID3DBlob* PixelShader::GetByteCode() const noexcept
	{
		return m_pByteCodeBlob.Get();
	}
	
	bool PixelShader::isSubDirectory(const std::filesystem::path& absolutePath, const std::filesystem::path& relativePath)
	{
		auto it = std::search(absolutePath.begin(), absolutePath.end(), relativePath.begin(), relativePath.end());
		return it != absolutePath.end();
	}

}