#include "IcoSphere.h"

#include "Primitive.h"
#include "Graphics\Bindable\Bindables.h"
#include "Entity\Entity.h"
#include <random>
#include "Graphics\Surface.h"

#include "GDIManager.h"
namespace Impact
{

		static int k = 0;
	IcoSphere::IcoSphere(Entity* pParent, Graphics& gfx)
		: RenderableBase(pParent)
		, m_Model{}
	{
		m_Gfx = &gfx;
		GDIManager gdi{};
		if (!IsStaticInitialized())
		{

			AddStaticBind(std::make_unique<BlendState>(gfx));

			std::unique_ptr<VertexShader> pVS = std::make_unique<VertexShader>(gfx, "TextureVS.cso");
			ID3DBlob* pVSbc = pVS->GetByteCode();

			AddStaticBind(std::move(pVS));

			AddStaticBind(std::make_unique<PixelShader>(gfx, "TexturePS.cso"));


			AddStaticBind(std::make_unique<SamplerState>(gfx));

			//struct colorConstBuffer
			//{
			//	struct
			//	{
			//		float r;
			//		float g;
			//		float b;
			//		float a;
			//	}face_colors[8];
			//};

			//const colorConstBuffer constBuffer =
			//{
			//	{
			//		{1.0f, 0.0f, 0.0f, 1.0f},
			//		{0.0f, 1.0f, 0.0f, 1.0f},
			//		{0.0f, 0.0f, 1.0f, 1.0f},
			//		{0.0f, 1.0f, 1.0f, 1.0f},
			//		{1.0f, 0.0f, 1.0f, 1.0f},
			//		{1.0f, 1.0f, 0.0f, 1.0f},
			//		{1.0f, 1.0f, 1.0f, 1.0f},
			//		{0.0f, 0.0f, 0.0f, 1.0f},
			//	}
			//};
			//AddStaticBind(std::make_unique<ConstantBuffer<colorConstBuffer>>(gfx, constBuffer, PipelineStage::PixelShader));

			const std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
			{
				{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"Normal", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TexCoord", 0,  DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};

			AddStaticBind(std::make_unique<InputLayout>(gfx, inputElements, pVSbc));
			AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}
		//	AddBind(std::make_unique<Texture>(gfx, Surface::CreateFromFile("../Impact/Resources/Images/uv_grid_2.png")));
			static int tex = 0;
			if (tex < 1)
			{
				tex++;
				 AddBind(std::make_unique<Texture>(gfx, Surface::CreateFromFile("../Impact/Resources/Images/Earth8k.jpg"), 0, PipelineStage::PixelShader));
				 AddBind(std::make_unique<Texture>(gfx, Surface::CreateFromFile("../Impact/Resources/Images/EarthHeight4k.png"), 1, PipelineStage::PixelShader));
			}
			else
			{
				AddBind(std::make_unique<Texture>(gfx, Surface::CreateFromFile("../Impact/Resources/Images/moon8k.jpg"), 0));
			}

		m_Model = Primitive::IcoSphere::Create<Vertex>();

		AddBind(std::make_unique<VertexBuffer>(gfx, m_Model.m_Vertices));

		AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, m_Model.m_Indices));

		AddBind(std::make_unique<TransformCbuf>(gfx, *pParent->GetComponent<TransformComponent>()));
	}

	void IcoSphere::Update(float dt) noexcept
	{
		dt;
	}

	void IcoSphere::UpdateModel(int recursionLevel)
	{
		RemoveBind<VertexBuffer>();
		RemoveBind<IndexBuffer>();
		//RemoveBind(std::make_unique<IndexBuffer>(*m_Gfx, m_Model.m_Indices));


		m_Model = Primitive::IcoSphere::Create<Vertex>(recursionLevel);
		
		AddBind(std::make_unique<VertexBuffer>(*m_Gfx, m_Model.m_Vertices));

		AddIndexBuffer(std::make_unique<IndexBuffer>(*m_Gfx, m_Model.m_Indices));
	}
}