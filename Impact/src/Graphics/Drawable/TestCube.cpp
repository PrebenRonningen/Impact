#include "TestCube.h"
#include "Graphics/Bindable/VertexBuffer.h"
//#include "Graphics/Bindable/IndexBuffer.h"
#include "Graphics/Bindable/VertexShader.h"
#include "Graphics/Bindable/PixelShader.h"
#include "Graphics/Bindable/ConstantBuffer.h"
#include "Graphics/Bindable/InputLayout.h"
#include "Graphics/Bindable/Topology.h"
#include "Graphics/Bindable/TransformCbuf.h"

namespace Impact
{
	TestCube::TestCube(Graphics& gfx, std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist)
		: r{ rdist(rng) }
		, droll{ ddist(rng) }
		, dpitch{ ddist(rng) }
		, dyaw{ ddist(rng) }
		, dphi{ odist(rng) }
		, dtheta{ odist(rng) }
		, dchi{ odist(rng) }
		, chi{ adist(rng) }
		, theta{ adist(rng) }
		, phi{ adist(rng) }
	{
		struct Vertex
		{
			float x,y,z;
		};

		const std::vector<Vertex> verices =
		{
			{-1.0f, -1.0f, -1.0f},
			{ 1.0f, -1.0f, -1.0f},
			{-1.0f,  1.0f, -1.0f},
			{ 1.0f,  1.0f, -1.0f},

			{-1.0f, -1.0f,  1.0f},
			{ 1.0f, -1.0f,  1.0f},
			{-1.0f,  1.0f,  1.0f},
			{ 1.0f,  1.0f,  1.0f},
		};

		AddBind(std::make_unique<VertexBuffer>(gfx, verices));

		std::unique_ptr<VertexShader> pVS = std::make_unique<VertexShader>(gfx, "VertexShader.cso");
		ID3DBlob* pVSbc = pVS->GetByteCode();

		AddBind(std::move(pVS));

		AddBind(std::make_unique<PixelShader>(gfx, "PixelShader.cso"));

		const std::vector<uint16_t> indices =
		{
			0,2,1,	2,3,1,
			1,3,5,	3,7,5,
			2,6,3,	3,6,7,
			4,5,7,	4,7,6,
			0,4,2,	2,4,6,
			0,1,4,	1,5,4,
		};
		AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

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
				{1.0f, 0.0f, 1.0f},
				{1.0f, 0.0f, 0.0f},
				{0.0f, 1.0f, 0.0f},
				{0.0f, 0.0f, 1.0f},
				{1.0f, 1.0f, 0.0f},
				{0.0f, 1.0f, 1.0f},
			}
		};
		AddBind(std::make_unique<ConstantBuffer<colorConstBuffer>>(gfx, PipelineStage::PixelShader));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements=
		{
			{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0}
		};

		AddBind(std::make_unique<InputLayout>(gfx, inputElements, pVSbc));
		AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		AddBind(std::make_unique<TransformCbuf>(gfx, *this));
	}

	TestCube::TestCube(Graphics& gfx)
	{
		gfx;
	}

	void TestCube::Update(float dt) noexcept
	{
		roll	= droll		* dt;
		pitch	= dpitch	* dt;
		yaw		= dyaw		* dt;
		theta	= dtheta	* dt;;
		phi		= dphi		* dt;
		chi		= dchi		* dt;
	}

	DirectX::XMFLOAT4X4 TestCube::GetTransform() const noexcept
	{
		DirectX::XMFLOAT4X4 transform;
		DirectX::XMMATRIX temp = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll)*
			DirectX::XMMatrixTranslation(r, 0.0f, 0.0f)*
			DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi)*
			DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
		DirectX::XMStoreFloat4x4(&transform, temp);
		return transform;
	}
}

