#pragma once
#include "Graphics\Drawable\RenderableBase.h"

namespace Impact
{
	class Entity;
	class Plane : public RenderableBase<Plane>
	{
	public:
		Plane(Entity* pParent, Graphics& gfx, bool randomized = false);
		virtual void Update(float dt) noexcept override;

	private:

	};
}
