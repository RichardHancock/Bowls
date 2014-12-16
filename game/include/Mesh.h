#include <vector>
#include <fstream>
#include <string>

#include "cgg/cgg.h"
#include "gl/Device.h"
#include "gl/Program.h"
#include "gl/Shader.h"
#include "gl/Buffer.h"
#include "gl/VertexFormat.h"
#include "gl/Primitives.h"
#include "gl/DepthStencilState.h"
#include "gl/RasterState.h"

class Mesh 
{
public:
	Mesh(std::string filename);

	std::vector<cgg::VertexNormalUv> getVertices();
	std::vector<int> getIndices();

private:

	std::string meshName;
	int numberOfVertices;
	int numberOfIndices;
	std::vector<cgg::VertexNormalUv> vertices;
	std::vector<int> indices;
	gl::Buffer* vertexBuffer;
	gl::Buffer* indexBuffer;

	void loadMeshFile(std::string filename);
	void makeBuffers(gl::Device* device);
	void Mesh::render(
		gl::Device* device,
		gl::Program* program,
		const maths::Mat43 matrices[],
		const maths::Mat43& projection,
		const maths::Mat43& view,
		uint32_t diffuse_tex_channel,
		uint32_t specular_tex_channel);
};

