#include "IcoSphere.h"

#include "Primitive.h"
#include "Graphics\Bindable\Bindables.h"
#include "Entity\Entity.h"
#include <random>
namespace Impact
{
	IcoSphere::IcoSphere(Entity* pParent, Graphics& gfx, bool randomized)
		: RenderableBase(pParent)
	{
		if (!IsStaticInitialized())
		{
			std::unique_ptr<VertexShader> pVS = std::make_unique<VertexShader>(gfx, "VertexShader.cso");
			ID3DBlob* pVSbc = pVS->GetByteCode();

			AddStaticBind(std::move(pVS));

			AddStaticBind(std::make_unique<PixelShader>(gfx, "PixelShader.cso"));

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
		if (randomized)
		{
			std::mt19937 rng(std::random_device{}());
			std::uniform_real_distribution<float> radius(1, 6);
			std::uniform_int_distribution<int> recursionLevels(1, 4);
			model = Primitive::IcoSphere::CreateRecLeveled<Vertex>(radius(rng), recursionLevels(rng));
		}
		else
		{
			model = Primitive::IcoSphere::Create<Vertex>();
		}
		AddBind(std::make_unique<VertexBuffer>(gfx, model.m_Vertices));

		AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.m_Indices));

		AddBind(std::make_unique<TransformCbuf>(gfx, *pParent->GetComponent<TransformComponent>()));
	}

	void IcoSphere::Update(float dt) noexcept
	{
		dt;
	}
}