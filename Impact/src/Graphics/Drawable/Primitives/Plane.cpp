#include "Plane.h"

#include "Primitive.h"
#include "Graphics\Bindable\Bindables.h"
#include "Entity\Entity.h"
#include <random>
namespace Impact
{
	Plane::Plane(Entity* pParent, Graphics& gfx)
		: RenderableBase(pParent)
	{
		if (!IsStaticInitialized())
		{
			std::unique_ptr<VertexShader> pVS = std::make_unique<VertexShader>(gfx, "ColorIndexVS.cso");
			ID3DBlob* pVSbc = pVS->GetByteCode();

			AddStaticBind(std::move(pVS));

			AddStaticBind(std::make_unique<PixelShader>(gfx, "ColorIndexPS.cso"));

			struct colorConstBuffer
			{
				struct
				{
					float r;
					float g;
					float b;
					float a;
				}face_colors[8];
			};

			const colorConstBuffer constBuffer =
			{
				{
					{1.0f, 0.0f, 0.0f, 1.0f},
					{0.0f, 1.0f, 0.0f, 1.0f},
					{0.0f, 0.0f, 1.0f, 1.0f},
					{0.0f, 1.0f, 1.0f, 1.0f},
					{1.0f, 0.0f, 1.0f, 1.0f},
					{1.0f, 1.0f, 0.0f, 1.0f},
					{1.0f, 1.0f, 1.0f, 1.0f},
					{0.0f, 0.0f, 0.0f, 1.0f},
				}
			};
			AddStaticBind(std::make_unique<ConstantBuffer<colorConstBuffer>>(gfx, constBuffer, PipelineStage::PixelShader));

			const std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
			{
				{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};

			AddStaticBind(std::make_unique<InputLayout>(gfx, inputElements, pVSbc));
			AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}

		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
		};

		IndexedTriangleList<Vertex> model;

		model = Primitive::Plane::Create<Vertex>();

		AddBind(std::make_unique<VertexBuffer>(gfx, model.m_Vertices));
		AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.m_Indices));

		AddBind(std::make_unique<TransformCbuf>(gfx, *pParent->GetComponent<TransformComponent>()));
	}

	void Plane::Update(float dt) noexcept
	{
		dt;
	}
}