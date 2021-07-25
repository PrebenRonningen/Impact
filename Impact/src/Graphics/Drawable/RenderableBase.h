#pragma once
#include "RenderComponent.h"
#include "Graphics\Bindable\IndexBuffer.h"
namespace Impact
{
	template <typename T>
	class RenderableBase : public RenderComponent
	{
	protected:
		RenderableBase(Entity* pParent)
			: RenderComponent(pParent){};
		bool IsStaticInitialized()
		{
			return !m_StaticBinds.empty();
		}
		void AddStaticBind(std::unique_ptr<Bindable> pBind) noexcept
		{
			m_StaticBinds.push_back(std::move(pBind));
		}
		void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> pIndexBuffer) noexcept
		{
			assert(pIndexBuffer != nullptr);
			m_pIndexBuffer = pIndexBuffer.get();
			m_StaticBinds.push_back(std::move(pIndexBuffer));
		}
		void SetIndexFromStatic() noexcept
		{
			for (const auto& sb : m_StaticBinds)
			{
				if (const IndexBuffer* p = dynamic_cast<IndexBuffer*>(sb.get()))
				{
					m_pIndexBuffer = p;
					return;
				}
			}
		}
	private:
		const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept
		{
			return m_StaticBinds;
		}
		static std::vector<std::unique_ptr<Bindable>> m_StaticBinds;
	};
	template <typename T>
	std::vector<std::unique_ptr<Bindable>> RenderableBase<T>::m_StaticBinds{};
}