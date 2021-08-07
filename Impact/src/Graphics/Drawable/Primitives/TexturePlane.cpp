#include "TexturePlane.h"
#include "Primitive.h"
#include "Graphics\Bindable\Bindables.h"
#include "Entity\Entity.h"
#include "Graphics\Surface.h"

#include "GDIManager.h"

namespace Impact {
	

	TexturePlane::TexturePlane(Entity* pParent, Graphics& gfx, bool)
		: RenderableBase(pParent)
	{
		GDIManager gdi{};
		if (!IsStaticInitialized())
		{
			struct Vertex {
				DirectX::XMFLOAT3 pos;
				DirectX::XMFLOAT2 texCoord;
			};

			auto model = Primitive::Plane::CreateSubDivided<Vertex>(1,1);
			model.m_Vertices[0].texCoord = { 0.0f, 0.0f };
			model.m_Vertices[1].texCoord = { 1.0f, 0.0f };
			model.m_Vertices[2].texCoord = { 0.0f, 1.0f };
			model.m_Vertices[3].texCoord = { 1.0f, 1.0f };

			AddStaticBind(std::make_unique<Texture>(gfx, Surface::CreateFromFile("../Impact/Resources/Images/planet5.png")));

			AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.m_Vertices));

			AddStaticBind(std::make_unique<SamplerState>(gfx));

			std::unique_ptr<VertexShader> pVS = std::make_unique<VertexShader>(gfx, "TextureVS.cso");
			ID3DBlob* pVSbc = pVS->GetByteCode();
			AddStaticBind(std::move(pVS));

			AddStaticBind(std::make_unique<PixelShader>(gfx, "TexturePS.cso"));

			AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.m_Indices));

			//AddStaticBind(std::make_unique<BlendState>(gfx));

			const std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
			{
				{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};

			AddStaticBind(std::make_unique<InputLayout>(gfx, inputElements, pVSbc));
			AddStaticBind(std::make_unique<Topology>(gfx, D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}
		else
		{
			SetIndexFromStatic();
		}

		AddBind(std::make_unique<TransformCbuf>(gfx, *pParent->GetComponent<TransformComponent>()));
	}
}
