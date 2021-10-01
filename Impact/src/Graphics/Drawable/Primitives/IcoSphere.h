#pragma once
#include "Graphics\Drawable\RenderableBase.h"
#include "IndexedTriangleList.h"
namespace Impact
{
	class Entity;
	class IcoSphere : public RenderableBase<IcoSphere>
	{
		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT3 normal;
			DirectX::XMFLOAT2 texCoord;
		};
	public:
		IcoSphere(Entity* pParent, Graphics& gfx);
		virtual void Update(float dt) noexcept override;
		void UpdateModel(int i);
	private:

		IndexedTriangleList<Vertex> m_Model;
		Graphics* m_Gfx;
	};
}
