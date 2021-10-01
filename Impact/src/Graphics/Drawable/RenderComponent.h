#pragma once
#include "Graphics/Graphics.h"
#include "Components/Component.h"
#include <DirectXMath.h>
#include <memory>

namespace Impact
{
	class IndexBuffer;
	class Bindable;
	class RenderComponent : public Component
	{
		template <typename T>
		friend class RenderableBase;
	public:
		RenderComponent(Entity* pParent)
			:Component(pParent){}
		RenderComponent(const RenderComponent& other) = delete;
		virtual ~RenderComponent() = default;

		virtual void Draw(Graphics& gfx) const noexcept;
		virtual void Update(float dt) noexcept{dt;};

	protected:
		void AddBind(std::unique_ptr<Bindable> pBind) noexcept;	//remove the need for Uniquie pointers

		template <typename T>
		void RemoveBind() noexcept
		{
			const type_info& ti = typeid(T);

			for (size_t i{}; i < m_Binds.size(); i++) 
			{
				if (m_Binds[i] && typeid(*m_Binds[i]) == ti)
					m_Binds.erase(std::find(m_Binds.begin(), m_Binds.end(), m_Binds[i]));
			}
		};

		void AddIndexBuffer(std::unique_ptr<IndexBuffer> pIdxBuffer) noexcept; //remove the need for Uniquie pointers

	private:
		virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept = 0;

		const IndexBuffer* m_pIndexBuffer = nullptr;
		std::vector<std::unique_ptr<Bindable>> m_Binds;
	};
}