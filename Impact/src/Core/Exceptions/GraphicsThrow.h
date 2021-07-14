#pragma once
#include "Core/ImpactWin.h"
#include "HrException.h"

namespace Impact
{
#pragma warning(push)
#pragma warning(disable: 4505)
	static void GFX_EXCEPTION_NOINFO(int line, const char* file, HRESULT hr)
	{
		if(FAILED(hr) )
			throw HrException(line, file, hr);
	}

	static void GFX_THROW_NOINFO(int line, const char* file)
	{
		GFX_EXCEPTION_NOINFO(line, file, GetLastError());
	}

	static void GFX_DEVICE_REMOVED_EXCEPTION(int line, const char* file, HRESULT hr, ID3D11Device* device)
	{
		if(hr == DXGI_ERROR_DEVICE_REMOVED)
			GFX_EXCEPTION_NOINFO(line, file, device->GetDeviceRemovedReason());
		else
			GFX_EXCEPTION_NOINFO(line, file, hr);
	}

#ifdef NDEBUG

#endif // NDEBUG

#pragma warning(pop)
};