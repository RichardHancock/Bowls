#include "Mesh.h"

Mesh::Mesh(std::string filename)
{
	loadMeshFile(filename);
}

void Mesh::loadMeshFile(std::string filename)
{
	std::fstream ifs;
	ifs.open(filename.c_str(), std::fstream::in);

	// Reads the Mesh File Counts
	ifs >> meshName >> numberOfVertices >> numberOfIndices;

	// read each vertex
	for (uint32_t i = 0; i < numberOfVertices; ++i)
	{
		cgg::VertexNormalUv v;
		ifs >> v.v[0] >> v.v[1] >> v.v[2] >> v.n[0] >> v.n[1] >> v.n[2] >> v.t[0] >> v.t[1];
		vertices.push_back(v);
	}

	// read the indices
	for (uint32_t i = 0; i < numberOfIndices; ++i)
	{
		int currentIndex = 0;
		ifs >> currentIndex;
		indices.push_back(currentIndex);
	}
}

void Mesh::makeBuffers(gl::Device* device)
{
	gl::BufferDesc vertex_desc = { 0, gl::kStaticDraw, gl::kVertexBuffer };
	gl::BufferDesc index_desc = { 0, gl::kStaticDraw, gl::kIndexBuffer };


	vertex_desc.byteWidth = 8 * sizeof(float) * numberOfVertices;
	index_desc.byteWidth = sizeof(int) * numberOfIndices;
	
	// create the buffers (not sure if &vertices[0] will work)
	vertexBuffer = device->createBuffer(vertex_desc);
	vertexBuffer->loadSubBuffer(&vertices[0], vertex_desc.byteWidth, 0);

	indexBuffer = device->createBuffer(index_desc);
	indexBuffer->loadSubBuffer(&indices[0], index_desc.byteWidth, 0);

}

void Mesh::render(
	gl::Device* device,
	gl::Program* program,
	std::vector<uint32_t> uniformIndexs,
	const maths::Mat43 matrices[],
	const maths::Mat43& projection,
	const maths::Mat43& view,
	uint32_t diffuse_tex_channel,
	uint32_t specular_tex_channel) 
{
	//get the uniform locations
	uint32_t mvUniformIndex = uniformIndexs[0];
	uint32_t mvpUniformIndex = uniformIndexs[0];


	// unbind the program we are using
	program->bind();
	// set the colour & texture
	g_program->setUniformMat4(mvUniformIndex, modelview);
	g_program->setUniformMat4(mvpUniformIndex, mvp);
	program->setUniform4f(m_diffuseUniform, m_diffuse);
	program->setUniform4f(m_specularUniform, m_specular);
	program->setUniformSampler(m_textureUniform, diffuse_tex_channel);
	program->setUniformSampler(m_specularTextureUniform, specular_tex_channel);

	// construct vertex + index buffer for each mesh
	for (uint32_t i = 0; i < m_numMeshes; ++i)
	{
		maths::Mat43 m = matrices[m_parents[i]] * view, mvp;
		m.makeGL();
		mvp.multiply4x4(projection, m);

		// bind the program we are using
		program->bind(1, 1, &m_buffers[i], &m_format);

		// set the matrix transform as a uniform to our shader (all geometry will be transformed by this matrix!)
		program->setUniformMat4(m_mvpUniform, mvp);
		program->setUniformMat4(m_mvUniform, m);

		// set the index buffer to use
		device->setIndexBuffer(m_indexBuffers[i], gl::kUInt16, 0);

		// we will be drawing triangles
		device->setPrimitiveTopology(gl::kTriangles);

		// draw 12 triangles (36 indices = 12 triangles)
		device->drawIndexed(m_indexCounts[i], 0, 0);
	}
	// unbind the program we are using
	program->unbind(1, 1, &m_buffers[0], &m_format);
}