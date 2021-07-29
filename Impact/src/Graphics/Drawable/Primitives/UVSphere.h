#pragma once
#include "Graphics\Drawable\RenderableBase.h"

namespace Impact
{
	class Entity;
	class UVSphere : public RenderableBase<UVSphere>
	{
	public:
		UVSphere(Entity* pParent, Graphics& gfx, bool randomized = false);
		virtual void Update(float dt) noexcept override;

	private:

	};
}
