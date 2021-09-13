#pragma once
#include "Graphics\Drawable\RenderableBase.h"

namespace Impact
{
	class Entity;
	class TexturePlane : public RenderableBase<TexturePlane>
	{
	public:
		TexturePlane(Entity* pParent, Graphics& gfx);
		virtual void Update(float dt) noexcept override{dt;};

	private:

	};
}
