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
		struct LightData
		{
			DirectX::XMFLOAT3 lightPos;
			float pad;
			DirectX::XMFLOAT3 lightDir;
			float pad2;
			DirectX::XMFLOAT3 lightColor;
			float pad3;

			float lightRange;
			float lightIntensity;
			float attConst;
			int fallOffType;
			alignas(16) int lightType;
		};

		static const size_t MAX_LIGHTS = 100;
		struct Lights {
			size_t size;
			LightData lightData[MAX_LIGHTS];
		};
	public:
		LightComponent(Entity* pParent, Graphics& gfx);

		virtual void Bind(Graphics& gfx)noexcept;
		void SetLightColor(DirectX::XMFLOAT3 color);
		void SetPointLight();
		void SetDirectionalLight();
		DirectX::XMFLOAT3& LightPosition();
		LightData& UIWindow();

		static void ToggleLights();

	private:
		static Lights m_LightData;
		static size_t m_CurrentActiveLight;
		size_t m_Id;
		ConstantBuffer<Lights> m_LightDataCbuf;
	};
}

