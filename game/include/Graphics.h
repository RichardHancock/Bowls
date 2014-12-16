#include "cgg/cgg.h"
#include "gl/Device.h"
#include "gl/Program.h"
#include "gl/Shader.h"
#include "gl/Buffer.h"
#include "gl/VertexFormat.h"
#include "gl/Primitives.h"
#include "gl/DepthStencilState.h"
#include "gl/RasterState.h"
#include "maths/MayaCamera.h"

// These functions were pulled from the libCGG tutorial, as they are core to the library

namespace GraphicsGL
{
	gl::TextureSampler* makeSampler()
	{
		// set up the texture sampler
		gl::TextureSamplerDesc sampler_desc =
		{
			gl::kLinear, // minification filter
			gl::kLinear, // magnification filter
			gl::kTextureAddressWrap, // wrap mode U dir
			gl::kTextureAddressWrap, // wrap mode V dir 
			gl::kTextureAddressWrap, // wrap mode W dir
			
		};

		// create the sampler
		return cgg::getGlDevice()->createTextureSampler(sampler_desc);
	}

	//----------------------------------------------------------------------------------------------------------------------
	gl::DepthStencilState* makeDepthStencilState()
	{
		// build object to wrap up all settings for the depth & stencil buffers
		gl::DepthStencilStateDesc depth_stencil_desc;
		depth_stencil_desc.init();
		depth_stencil_desc.depthEnable = true;
		return cgg::getGlDevice()->createDepthStencilState(depth_stencil_desc);
	}

	//----------------------------------------------------------------------------------------------------------------------
	gl::RasterState* makeRasterState()
	{
		// build up that holds all settings for the rasterizer
		gl::RasterStateDesc raster_desc;
		raster_desc.init();
		raster_desc.cullMode = gl::kCullBack;
		return cgg::getGlDevice()->createRasterState(raster_desc);
	}

	//----------------------------------------------------------------------------------------------------------------------
	gl::Buffer* makeIndexBuffer(float indices[])
	{
		// construct a description of the vertex buffer I'm about to create
		gl::BufferDesc index_desc;
		index_desc.bindFlags = gl::kIndexBuffer;    //< We will be creating a index buffer this time!
		index_desc.byteWidth = sizeof(indices);   //< the size for the new buffer
		index_desc.usage = gl::kStaticDraw;         //< we will be drawing from this buffer (and it's contents will not change)

		// create a buffer using those settings.
		gl::Buffer* buffer = cgg::getGlDevice()->createBuffer(index_desc);

		// load the buffer contents
		// The first param is a pointer to the data
		// The second param is the number of bytes to load
		buffer->loadBuffer(indices, sizeof(indices));

		// return the new buffer
		return buffer;
	}

	/**
	@brief Creates a Vertex buffer
	@todo Possibly pass by ref or pointer
	@param
	@return
	*/
	gl::Buffer* makeVertexBuffer(float points[])
	{
		// construct a description of the vertex buffer I'm about to create
		gl::BufferDesc vertex_desc;
		vertex_desc.bindFlags = gl::kVertexBuffer;
		vertex_desc.byteWidth = sizeof(points);
		vertex_desc.usage = gl::kStaticDraw;

		// create a buffer using those settings.
		gl::Buffer* buffer = cgg::getGlDevice()->createBuffer(vertex_desc);

		// load the buffer contents
		buffer->loadBuffer(points, sizeof(points));

		// return the new buffer
		return buffer;
	}

	//----------------------------------------------------------------------------------------------------------------------
	gl::Buffer* makeMaterialBuffer()
	{
		// the uniform data for the programs uniform attributes
		const float material_data[] =
		{
			0.2f, 0.2f, 0.2f, 1.0f, // ps_ambient
			0.7f, 0.7f, 0.7f, 1.0f, // ps_diffuse
			1.0f, 1.0f, 1.0f, 1.0f, // ps_specular
			15.0f                   // ps_cosPower
		};

		// Initialise the Material uniform buffer with data from the uniform_data array
		const gl::BufferDesc uniform_desc =
		{
			sizeof(material_data),
			gl::kStaticDraw,
			gl::kConstantBuffer
		};

		// create the buffer
		gl::Buffer* buffer = cgg::getGlDevice()->createBuffer(uniform_desc);

		// load the contents
		buffer->loadBuffer(material_data, sizeof(material_data));

		// return new buffer
		return buffer;
	}

	//----------------------------------------------------------------------------------------------------------------------
	gl::Buffer* makeMatrixBuffer()
	{
		// Initialise the Matrix uniform buffer (created empty - it will be filled when rendering)
		const gl::BufferDesc matrix_desc =
		{
			sizeof(cgg::Mat43) * 2,
			gl::kStreamDraw,
			gl::kConstantBuffer
		};

		// create the buffer
		return cgg::getGlDevice()->createBuffer(matrix_desc);
	}

	//----------------------------------------------------------------------------------------------------------------------
	gl::VertexFormat* makeVertexFormat()
	{
		// A description of each vertex attribute in the mesh
		gl::InputElementDesc vertex_format_desc[3] =
		{
			{
				"vs_position",     // name of the attribute (unused unless you are using GLES2)
				0,                 // the attribute ID (must match the layout specified in the shader)
				3,                 // number of elements (3 for x,y,z)
				gl::kFloat,        // type of each element
				0,                 // the vertex buffer slot (0 because we're only using 1 buffer here)
				0,                 // offset (in bytes) from start of buffer where the first element exists
				sizeof(float) * 8, // stride (in bytes) between each element
				gl::kPerVertex,    // per-vertex or per-instance?
				0                  // step rate (must be 0 unless using HW instancing)
			},
			{
				"vs_normal",       // name of the attribute (unused unless you are using GLES2)
				1,                 // the attribute ID (must match the layout specified in the shader)
				3,                 // number of elements (3 for x,y,z)
				gl::kFloat,        // type of each element
				0,                 // the vertex buffer slot (0 because we're only using 1 buffer here)
				sizeof(float) * 5, // offset (in bytes) from start of buffer where the first element exists
				sizeof(float) * 8, // stride (in bytes) between each element
				gl::kPerVertex,    // per-vertex or per-instance?
				0                  // step rate (must be 0 unless using HW instancing)
			},
			{
				"vs_uv",           // name of the attribute (unused unless you are using GLES2)
				2,                 // the attribute ID (must match the layout specified in the shader)
				2,                 // number of elements (3 for x,y,z)
				gl::kFloat,        // type of each element
				0,                 // the vertex buffer slot (0 because we're only using 1 buffer here)
				sizeof(float) * 3, // offset (in bytes) from start of buffer where the first element exists
				sizeof(float) * 8, // stride (in bytes) between each element
				gl::kPerVertex,    // per-vertex or per-instance?
				0                  // step rate (must be 0 unless using HW instancing)
			}
		};

		// construct a vertex format
		return cgg::getGlDevice()->createVertexFormat(3, vertex_format_desc);
	}

	//----------------------------------------------------------------------------------------------------------------------
	gl::Program* makeProgram(const char* vtx_shader, const char* pixelShader, gl::Shader* output[2])
	{
		// load the shader assets
		cgg::Asset* vtx_shader_asset = cgg::Asset::load(vtx_shader);
		cgg::Asset* pix_shader_asset = cgg::Asset::load(pixelShader);
		if (vtx_shader_asset && pix_shader_asset)
		{
			// create shaders
			output[0] = cgg::getGlDevice()->createVertexShader((const char*)vtx_shader_asset->begin());
			output[1] = cgg::getGlDevice()->createPixelShader((const char*)pix_shader_asset->begin());

			// make sure they compiled ok
			if (!output[0]->valid() || !output[1]->valid())
			{
				cgt::loge("Shaders failed to compile");
			}

			// create program
			gl::Program* program = cgg::getGlDevice()->createProgram();

			// link the shaders into the program
			if (program->init(output[0], output[1]))
			{
				// return new program
				return program;
			}


			// if failed, log error, and delete program and shaders
			cgt::loge("Shaders failed to link into a program :(");
			delete program;
			delete output[0];
			delete output[1];
		}
		else
		{
			// one or more not found, delete anything that was allocated, log an error, and return 
			delete vtx_shader_asset;
			delete pix_shader_asset;
			cgt::loge("Could not load shaders");
		}
		return 0;
	}
}