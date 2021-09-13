#pragma once
#include "Graphics\Drawable\RenderableBase.h"

namespace Impact
{
	class Entity;
	class UVSphere : public RenderableBase<UVSphere>
	{
	public:
		UVSphere(Entity* pParent, Graphics& gfx);
		virtual void Update(float dt) noexcept override;

	private:

	};
}
