#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Bindable/IndexBuffer.h"
#include "Graphics/Bindable/Bindable.h"
#include <DirectXMath.h>
#include <memory>

namespace Impact
{
	class RenderComponent //: public Component
	{
	public:
		RenderComponent() = default;
		RenderComponent(const RenderComponent& other) = delete;
		virtual ~RenderComponent() = default;

		virtual void Draw(Graphics& gfx) const noexcept;
		virtual void Update(float dt) noexcept{dt;};

		void AddBind(std::unique_ptr<Bindable> pBind) noexcept;	//remove the need for Uniquie pointers
		void AddIndexBuffer(std::unique_ptr<IndexBuffer> pIdxBuffer) noexcept; //remove the need for Uniquie pointers

		// this should be another component
		virtual DirectX::XMFLOAT4X4 GetTransform() const noexcept {return DirectX::XMFLOAT4X4{};};

	private:
		const IndexBuffer* m_pIndexBuffer = nullptr;
		std::vector<std::unique_ptr<Bindable>> m_Binds;
	};
}