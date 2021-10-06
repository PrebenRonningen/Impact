#include "LightComponent.h"

namespace Impact {
	LightComponent::Lights LightComponent::m_LightData{};
	LightComponent::LightComponent(Entity* pParent, Graphics& gfx)
	: Component(pParent)
	, m_LightDataCbuf(gfx, Impact::PipelineStage::PixelShader)
	, m_Id{}
	{
		m_Id = m_LightData.size++;
	}

	void LightComponent::SetPointLight()
	{
		m_LightData.lightData[m_Id].lightPos = DirectX::XMFLOAT3(0, 0, 0);
		m_LightData.lightData[m_Id].lightDir = DirectX::XMFLOAT3(0, 0, -1);
		m_LightData.lightData[m_Id].lightColor = DirectX::XMFLOAT3(1, 1, 1);
		m_LightData.lightData[m_Id].lightRange = 1000;
		m_LightData.lightData[m_Id].lightIntensity = 1;
		m_LightData.lightData[m_Id].attConst = 1;

		m_LightData.lightData[m_Id].fallOffType = 2;
		m_LightData.lightData[m_Id].lightType = 0;
	}

	void LightComponent::SetDirectionalLight() 
	{
		m_LightData.lightData[m_Id].lightPos = DirectX::XMFLOAT3(0, 0, 0);
		m_LightData.lightData[m_Id].lightDir = DirectX::XMFLOAT3(0, 0, -1);
		m_LightData.lightData[m_Id].lightColor = DirectX::XMFLOAT3(1, 1, 1);
		m_LightData.lightData[m_Id].lightRange = 1000;
		m_LightData.lightData[m_Id].lightIntensity = 1;
		m_LightData.lightData[m_Id].attConst = 1;

		m_LightData.lightData[m_Id].fallOffType = 2;
		m_LightData.lightData[m_Id].lightType = 1;
	}


	void LightComponent::SetLightColor(const DirectX::XMFLOAT3 color)
	{
		m_LightData.lightData[m_Id].lightColor = color; 
	}

	DirectX::XMFLOAT3& LightComponent::LightPosition()
	{
		return m_LightData.lightData[m_Id].lightPos;
	}

	LightComponent::LightData& LightComponent::UIWindow()
	{


		return m_LightData.lightData[m_Id];
	}

	void LightComponent::Bind(Graphics& gfx) noexcept
	{
		m_LightDataCbuf.Update(gfx, m_LightData);
		m_LightDataCbuf.Bind(gfx);
	}
}