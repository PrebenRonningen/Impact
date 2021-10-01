#pragma once
#include "Components\Component.h"
#include "Components\Components.h"
#include "Graphics\Bindable\ConstantBuffer.h"
#include <memory>
#include "Graphics\Graphics.h"
#include "Entity\Entity.h"
#include "Graphics\Bindable\Bindable.h"
#include <vector>
namespace Impact 
{

	class LightComponent : public Component
	{
		static const size_t MAX_LIGHTS = 100;
	public:
		LightComponent(Entity* pParent, Graphics& gfx);

		virtual void Bind(Graphics& gfx)noexcept;
		void SetLightColor(DirectX::XMFLOAT3 color);
	private:
		struct LightData
		{
			alignas(16) DirectX::XMFLOAT3 position;
		};

		struct Lights{
			size_t size;
			LightData lightData[MAX_LIGHTS];
		};

	private:
		static Lights m_LightData;
		size_t m_Id;
		ConstantBuffer<Lights> m_LightDataCbuf;
	};
}

