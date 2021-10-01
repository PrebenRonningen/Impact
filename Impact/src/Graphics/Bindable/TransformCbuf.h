#pragma once
#include "Graphics\Bindable\Bindable.h"
#include "Graphics\Bindable\ConstantBuffer.h"
#include "Graphics\Drawable\RenderComponent.h"
#include "Components\TransformComponent.h"
namespace Impact
{
	class TransformCbuf : public Bindable
	{
	private:
		struct Transforms 
		{
			DirectX::XMFLOAT4X4 world;
			DirectX::XMFLOAT4X4 worldViewProjection;
		};
	public:
		TransformCbuf(Graphics& gfx, TransformComponent& parent);
		virtual void Bind(Graphics& gfx) noexcept override;
		static void SetCamera(DirectX::XMFLOAT4X4 c);
	private:
		static std::unique_ptr<ConstantBuffer<Transforms>> m_pVcbuf;
		TransformComponent& m_Parent;
		static DirectX::XMFLOAT4X4 m_Camera;
	};

}