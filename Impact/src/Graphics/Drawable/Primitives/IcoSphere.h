#pragma once
#include "Graphics\Drawable\RenderableBase.h"

namespace Impact
{
	class Entity;
	class IcoSphere : public RenderableBase<IcoSphere>
	{
	public:
		IcoSphere(Entity* pParent, Graphics& gfx);
		virtual void Update(float dt) noexcept override;

	private:

	};
}
