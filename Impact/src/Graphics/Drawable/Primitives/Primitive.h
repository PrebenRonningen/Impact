#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include <array>
namespace Impact
{
	class Primitive
	{
	public:
		class Cube
		{
		public:
			template <typename V>
			static IndexedTriangleList<V> Create()
			{
				const float size = 0.5f; // half of width, height, depth
				const std::vector<DirectX::XMFLOAT3> vPos =
				{
					{DirectX::XMFLOAT3(-size, -size, -size)},
					{DirectX::XMFLOAT3( size, -size, -size)},
					{DirectX::XMFLOAT3(-size,  size, -size)},
					{DirectX::XMFLOAT3( size,  size, -size)},

					{DirectX::XMFLOAT3(-size, -size,  size)},
					{DirectX::XMFLOAT3( size, -size,  size)},
					{DirectX::XMFLOAT3(-size,  size,  size)},
					{DirectX::XMFLOAT3( size,  size,  size)},
				};

				const std::vector<uint16_t> indices =
				{
					0,2,1,	2,3,1,
					1,3,5,	3,7,5,
					2,6,3,	3,6,7,
					4,5,7,	4,7,6,
					0,4,2,	2,4,6,
					0,1,4,	1,5,4,
				};

				std::vector<V> vertices(vPos.size());
				for (size_t i{}; i < vPos.size(); ++i)
				{
					vertices[i].pos = vPos[i];
				}
				return { std::move(vertices), std::move(indices)};
			}
		};

		class Plane 
		{
		public:
			template <typename V>
			static IndexedTriangleList<V> CreateSubDivided(uint16_t divisionsX, uint16_t divisionsY)
			{
				const float size = 1.f; // width and height
				const uint32_t vertexCountX = divisionsX + 1;
				const uint32_t vertexCountY = divisionsY + 1;
				std::vector<V> vertices(vertexCountX * vertexCountY);
				

				const float width = size / 2.f;
				const float height = size / 2.f;
				const float divSizeX = size / float(divisionsX);
				const float divSizeY = size / float(divisionsY);
				const DirectX::XMVECTOR bottomLeft = DirectX::XMVectorSet(-width, -height, 0.0f, 0.0f);
				
				{	// verts
					for (uint32_t y{}, i{}; y < vertexCountY; ++y) {
						const float yPos = float(y) * divSizeY;
						for (uint32_t x{}; x < vertexCountX; ++x, ++i)
						{
							const DirectX::XMVECTOR v = DirectX::XMVectorAdd(bottomLeft, DirectX::XMVectorSet(float(x) * divSizeX, yPos,0,0));
							DirectX::XMStoreFloat3(&vertices[i].pos, v);
						}
					}
				}
				
				std::vector<uint16_t> indices;
					auto res = ((divisionsX * divisionsY)* (divisionsX * divisionsY)) * 6;
					indices.reserve(res);
				{	//idx
				
					const auto vxy2i = [vertexCountX](size_t x, size_t y)
					{
						return uint16_t(y * vertexCountX + x);
					};
					for (size_t y{}; y < divisionsY; ++y)
					{
						for (size_t x{}; x < divisionsX; ++x)
						{
							const std::array<unsigned short, 4> indexArray =
							{ 
								vxy2i(x,y),
								vxy2i(x + 1,y),
								vxy2i(x,y + 1),
								vxy2i(x + 1,y + 1) 
							};
				
							indices.push_back(indexArray[0]);
							indices.push_back(indexArray[2]);
							indices.push_back(indexArray[1]);
							indices.push_back(indexArray[1]);
							indices.push_back(indexArray[2]);
							indices.push_back(indexArray[3]);
						}
					}
				}

				return { std::move(vertices), std::move(indices) };
			}

		};
	private:
	};
}