#pragma once
#include <vector>

namespace Impact
{
	template <typename T>
	struct IndexedTriangleList
	{
		IndexedTriangleList() = default;
		IndexedTriangleList(std::vector<T> verts, std::vector<uint32_t> indices)
			: m_Vertices{verts}
			, m_Indices{indices}{};
	
		std::vector<T> m_Vertices;
		std::vector<uint32_t> m_Indices;
	};
}
