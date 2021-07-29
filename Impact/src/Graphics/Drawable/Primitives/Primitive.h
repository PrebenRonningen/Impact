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

				const std::vector<uint32_t> indices =
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
			static IndexedTriangleList<V> CreateSubDivided(uint32_t divisionsX, uint32_t divisionsY)
			{
				const float size = 10.f; // width and height
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
				
				std::vector<uint32_t> indices;
					auto res = ((divisionsX * divisionsY)* (divisionsX * divisionsY)) * 6;
					indices.reserve(res);
				{	//idx
				
					const auto vxy2i = [vertexCountX](size_t x, size_t y)
					{
						return uint32_t(y * vertexCountX + x);
					};
					for (size_t y{}; y < divisionsY; ++y)
					{
						for (size_t x{}; x < divisionsX; ++x)
						{
							const std::array<uint32_t, 4> indexArray =
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
			template <typename V>
			static IndexedTriangleList<V> Create()
			{
				return CreateSubDivided<V>(2u,2u);
			}
		};

		class Circle
		{
		public:
			template <typename V>
			static IndexedTriangleList<V> CreateSegmented(int segments)
			{
				const float radius = 10;
								
				DirectX::XMVECTOR nDir{ radius,0,0};

				std::vector<DirectX::XMFLOAT3> vPos;
				std::vector<uint32_t> indices;

				DirectX::XMFLOAT3 moved{};
				DirectX::XMVECTOR rotationDeg{};
				uint32_t j{};
				float twoPi = DirectX::g_XMTwoPi.f[0];
				
				for(float i{}; i <= twoPi; i+= twoPi/segments)
				{
					rotationDeg = DirectX::XMVector3Transform(nDir, DirectX::XMMatrixRotationRollPitchYaw(0, 0, i));
				
					DirectX::XMStoreFloat3(&moved, rotationDeg);
					vPos.push_back({ DirectX::XMFLOAT3(moved.x, moved.y, 0) });
					indices.push_back(j++);
				}; 
				std::vector<V> vertices(vPos.size());
				for (size_t i{}; i < vPos.size(); ++i)
				{
					vertices[i].pos = vPos[i];
				}
				return { std::move(vertices), std::move(indices) };
			};

			template <typename V>
			static IndexedTriangleList<V> Create()
			{
				return CreateSegmented<V>(16);
			};
			
		};
		class UVSphere 
		{
		public:
			template <typename V>
			static IndexedTriangleList<V> CreateSegmented(float radius, uint32_t lateSlice, uint32_t longSlice)
			{
				std::vector<DirectX::XMFLOAT3> vPos;
				const float pi = DirectX::g_XMPi.f[0];

				const DirectX::XMVECTOR base = DirectX::XMVectorSet(0.0f, 0.0f, radius, 0.0f);
				const float phiStep = pi / lateSlice;
				const float thetaStep = 2.0f * pi / longSlice;

				std::vector<V> vertices{};
				vertices.resize(lateSlice * longSlice + 2);
				//vertices[0].pos = { 0.0f, 0.0f, radius };
				DirectX::XMVECTOR lateBase{};
				DirectX::XMFLOAT3 vertexPos{};
				uint32_t idx = 0;
				for (uint32_t i{1}; i < lateSlice; ++i)
				{
					lateBase = DirectX::XMVector3Transform(base, DirectX::XMMatrixRotationX(phiStep * i));
					for (uint32_t j{}; j < longSlice; ++j)
					{
						DirectX::XMStoreFloat3(&vertexPos,
							DirectX::XMVector3Transform(lateBase, DirectX::XMMatrixRotationZ(thetaStep * j)));
						vertices[idx++].pos = vertexPos;
					}
				}

				// add the cap vertices
				const uint32_t iNorthPole = idx;
				vertices[idx++].pos = { 0.0f, 0.0f, radius };
				
				const uint32_t iSouthPole = idx;
				vertices[idx].pos = { 0.0f, 0.0f, -radius };


				// I may have missread the implementation, for I can't get it properly working with lateSlice & longSlice not being the same number.
				// https://www.danielsieger.com/blog/2021/03/27/generating-spheres.html
				// The UV Sphere
				{
				//// add top / bottom triangles
				//for (int i = 0; i < lateSlice; ++i)
				//{
				//	indices.push_back(iNorthPole);
				//	indices.push_back(uint16_t(i));
				//	indices.push_back(uint16_t((i + 1) % lateSlice ));

				//	indices.push_back(iSouthPole);
				//	indices.push_back(uint16_t((i + 1) % lateSlice + lateSlice * (longSlice - 2) ));
				//	indices.push_back(uint16_t(i + lateSlice * (longSlice - 2) ));
				//	
				//}

				//// add quads per stack / slice
				//for (int j = 0; j < longSlice - 2; j++)
				//{
				//	auto j0 = j * lateSlice;
				//	auto j1 = (j + 1) * lateSlice;
				//	for (int i = 0; i < lateSlice; i++)
				//	{
				//		indices.push_back(uint16_t(j0 + (i + 1) % lateSlice));
				//		indices.push_back(uint16_t(j0 + i));
				//		indices.push_back(uint16_t(j1 + (i + 1) % lateSlice));

				//		indices.push_back(uint16_t(j1 + i));
				//		indices.push_back(uint16_t(j1 + (i + 1) % lateSlice));
				//		indices.push_back(uint16_t(j0 + i));

				//	}
				//}
				}
				const auto calcIdx = [lateSlice, longSlice](uint32_t iLat, uint32_t iLong)
				{ return uint32_t(iLat * longSlice + iLong); };

				std::vector<uint32_t> indices;


				for (uint32_t iLat = 0; iLat < lateSlice - 2; iLat++)
				{
					for (uint32_t iLong = 0; iLong < longSlice - 1; iLong++)
					{
						indices.push_back(calcIdx(iLat, iLong));
						indices.push_back(calcIdx(iLat + 1, iLong));
						indices.push_back(calcIdx(iLat, iLong + 1));
						indices.push_back(calcIdx(iLat, iLong + 1));
						indices.push_back(calcIdx(iLat + 1, iLong));
						indices.push_back(calcIdx(iLat + 1, iLong + 1));
					}
					// wrap band
					indices.push_back(calcIdx(iLat, longSlice - 1));
					indices.push_back(calcIdx(iLat + 1, longSlice - 1));
					indices.push_back(calcIdx(iLat, 0));
					indices.push_back(calcIdx(iLat, 0));
					indices.push_back(calcIdx(iLat + 1, longSlice - 1));
					indices.push_back(calcIdx(iLat + 1, 0));
				}

				// cap fans
				for (uint32_t iLong = 0; iLong < longSlice - 1; iLong++)
				{
					// north
					indices.push_back(iNorthPole);
					indices.push_back(calcIdx(0, iLong));
					indices.push_back(calcIdx(0, iLong + 1));
					// south
					indices.push_back(calcIdx(lateSlice - 2, iLong + 1));
					indices.push_back(calcIdx(lateSlice - 2, iLong));
					indices.push_back(iSouthPole);
				}
				// wrap triangles
				// north
				indices.push_back(iNorthPole);
				indices.push_back(calcIdx(0, longSlice - 1));
				indices.push_back(calcIdx(0, 0));
				// south
				indices.push_back(calcIdx(lateSlice - 2, 0));
				indices.push_back(calcIdx(lateSlice - 2, longSlice - 1));
				indices.push_back(iSouthPole);

				return { std::move(vertices), std::move(indices) };
			}

			template <typename V>
			static IndexedTriangleList<V> Create()
			{
				return CreateSegmented<V>(1,16,16);
			}

		};
		class IcoSphere
		{
		public:
			// http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
			template <typename V>
			static IndexedTriangleList<V> CreateRecLeveled( float rad, int recLevels)
			{
				// golden ratio
				const float gr = float((1.0 + sqrt(5.0)) / 2.0) * rad;

				DirectX::XMVECTOR nD{rad, gr,0,0};
				nD = DirectX::XMVector2Normalize(nD);
				DirectX::XMFLOAT2 nR;
				DirectX::XMStoreFloat2(&nR, nD);
				std::vector<DirectX::XMFLOAT3> pos
				{
					{ -nR.x,  nR.y, 0},
					{  nR.x,  nR.y, 0},
					{ -nR.x, -nR.y, 0},
					{  nR.x, -nR.y, 0},

					{  0, -nR.x, nR.y},
					{  0,  nR.x, nR.y},
					{  0, -nR.x,-nR.y},
					{  0,  nR.x,-nR.y},

					{  nR.y, 0, -nR.x},
					{  nR.y, 0,  nR.x},
					{ -nR.y, 0, -nR.x},
					{ -nR.y, 0,  nR.x}
				};

				std::vector<uint32_t> indices =
				{
					0,11,5,
					0,1,7,
					0,5,1,
					0,10,11,
					0,7,10,
					
					1,5,9,
					5,11,4,
					11,10,2,
					10,7,6,
					7,1,8,

					3,9,4,
					3,2,6,
					3,4,2,
					3,8,9,
					3,6,8,

					4,9,5,
					2,4,11,
					6,2,10,
					8,6,7,
					9,8,1
				};


				int recursionLevel = recLevels;
				for (int l{}; l < recursionLevel; ++l) 
				{
					std::vector<uint32_t> newIndices{};
					
					for (uint32_t i{}; i < indices.size(); i += 3)
					{
						uint32_t v1 = indices[i];
						uint32_t v2 = indices[i+1];
						uint32_t v3 = indices[i+2];

						pos.push_back(GetMiddlePoint(pos[v1], pos[v2]));
						pos.push_back(GetMiddlePoint(pos[v2], pos[v3]));
						pos.push_back(GetMiddlePoint(pos[v3], pos[v1]));

						newIndices.push_back(v1);
						newIndices.push_back(uint32_t(pos.size() - 3));
						newIndices.push_back(uint32_t(pos.size() - 1));

						newIndices.push_back(v2);
						newIndices.push_back(uint32_t(pos.size() - 2));
						newIndices.push_back(uint32_t(pos.size() - 3));
						
						newIndices.push_back(v3);
						newIndices.push_back(uint32_t(pos.size() - 1));
						newIndices.push_back(uint32_t(pos.size() - 2));

						newIndices.push_back(uint32_t(pos.size() - 3));
						newIndices.push_back(uint32_t(pos.size() - 2));
						newIndices.push_back(uint32_t(pos.size() - 1));
					}
					indices = newIndices;
				}
				std::vector<V> vertices;
				vertices.resize(pos.size());
				for (int i{}; i < pos.size(); ++i) 
				{ 
					DirectX::XMVECTOR nPos = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&pos[i]));
					DirectX::XMFLOAT3 fPos{};
					DirectX::XMStoreFloat3(&fPos, nPos);
					vertices[i].pos = {fPos.x * rad, fPos.y * rad, fPos.z * rad };
				}

				return { std::move(vertices), std::move(indices) };
			};


			template <typename V>
			static IndexedTriangleList<V> Create()
			{
				return CreateRecLeveled<V>(1, 3);
			}
			private:
				static DirectX::XMFLOAT3 GetMiddlePoint(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2)
				{
					return DirectX::XMFLOAT3((p1.x + p2.x) / 2.0f, (p1.y + p2.y) / 2.0f, (p1.z + p2.z) / 2.0f);
				}
		};
	private:
	};
}