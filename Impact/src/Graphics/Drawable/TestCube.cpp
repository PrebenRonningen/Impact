#include "TestCube.h"
#include "Graphics/Bindable/VertexBuffer.h"
//#include "Graphics/Bindable/IndexBuffer.h"
#include "Graphics/Bindable/VertexShader.h"
#include "Graphics/Bindable/PixelShader.h"
#include "Graphics/Bindable/ConstantBuffer.h"
#include "Graphics/Bindable/InputLayout.h"
#include "Graphics/Bindable/Topology.h"
#include "Graphics/Bindable/TransformCbuf.h"
#include "Entity/Entity.h"
#include "Graphics\Drawable\Primitives\Primitive.h"
namespace Impact
{
	TestCube::TestCube(Entity* pParent, Graphics& gfx)
		: RenderableBase(pParent)
	{
		if (!IsStaticInitialized())
		{
			struct Vertex
			{
				DirectX::XMFLOAT3 pos;
			};

			auto model = Primitive::Plane::CreateSubDivided<Vertex>(5,1);


			AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.m_Vertices));

			std::unique_ptr<VertexShader> pVS = std::make_unique<VertexShader>(gfx, "VertexShader.cso");
			ID3DBlob* pVSbc = pVS->GetByteCode();

			AddStaticBind(std::move(pVS));

			AddStaticBind(std::make_unique<PixelShader>(gfx, "PixelShader.cso"));


			AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.m_Indices));

			struct colorConstBuffer
			{
				struct
				{
					float r;
					float g;
					float b;
					float a;
				}face_colors[6];
			};

			const colorConstBuffer constBuffer =
			{
				{
					{1.0f, 0.0f, 1.0f, 1.0f},
					{1.0f, 0.0f, 0.0f, 1.0f},
					{0.0f, 1.0f, 0.0f, 1.0f},
					{0.0f, 0.0f, 1.0f, 1.0f},
					{1.0f, 1.0f, 0.0f, 1.0f},
					{0.0f, 1.0f, 1.0f, 1.0f},
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
		else
		{
			SetIndexFromStatic();
		}
		AddBind(std::make_unique<TransformCbuf>(gfx, *pParent->GetComponent<TransformComponent>()));
	}

	void TestCube::Update(float dt) noexcept
	{
		dt;
	}
}

