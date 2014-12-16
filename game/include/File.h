#include <vector>
#include <fstream>

#include "cgg/cgg.h"

namespace File 
{
	std::vector<cgg::VertexNormalUv> loadMeshFile()
	{
		std::fstream ifs;
		ifs.open("assets/", std::fstream::in);


		// Reads the Mesh File Counts
		ifs >> m_name >> m_numVertices >> m_numIndices;

		// Allocates memory based on these counts
		m_vertices.realloc(m_numVertices);
		m_indices.realloc(m_numIndices);

		// read each vertex
		for (uint32_t i = 0; i < m_numVertices; ++i)
		{
			cgg::VertexNormalUv& v = m_vertices[i];
			ifs >> v.v[0] >> v.v[1] >> v.v[2] >> v.n[0] >> v.n[1] >> v.n[2] >> v.t[0] >> v.t[1];
		}

		// read the indices
		for (uint32_t i = 0; i < m_numIndices; ++i)
		{
			ifs >> m_indices[i];
		}
	}
}