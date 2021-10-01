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
	void LightComponent::SetLightColor(const DirectX::XMFLOAT3 color)
	{
		m_LightData.lightData[m_Id].position = color; 
	}
	void LightComponent::Bind(Graphics& gfx) noexcept
	{
		m_LightDataCbuf.Update(gfx, m_LightData);
		m_LightDataCbuf.Bind(gfx);
	}
}