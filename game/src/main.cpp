// Engine Includes
#include "main.h"
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
#include "lib/bass.h"
// Backend Includes
#include "Kinect.h"
#include "Timer.h"
#include "Audio.h"
// Game Components Includes
#include "Game.h"

// Maybe Include a way to set these (Menu, Command line argument or Hidden Key Combo)
const TrackingPoint hand = RightHand; // Which hand to track
const bool sitMode = false; // if the Kinect should look for sitting/standing skeletons (Cannot be changed once initialized)

cgg::MayaCamera g_camera;
cgg::Mat43 g_model;
cgg::Mat43 g_view;
cgg::Mat43 g_projection;
cgg::Mat43 g_mvp;
gl::Primitives* g_prims = 0;
gl::DepthStencilState* g_depthStencilState = 0;
gl::RasterState* g_rasterState = 0;

Game * game = new Game(sitMode,hand);
bool initish = BASS_Init(-1, 44100, BASS_DEVICE_DEFAULT, 0, NULL);
//----------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------------------------
/// this method will handle all drawing (draw2D and draw3D will have no effect)
//------------------------------------------------------------------------------------------------------------------------------------
void custom_gl_draw(gl::Device* device)
{
	// set viewport
	gl::Viewport viewport;
	viewport.left = 0;
	viewport.right = cgg::getClientWidth();
	viewport.top = 0;
	viewport.bottom = cgg::getClientHeight();
	device->setViewports(1, &viewport);

	// clear screen
	device->clear();

	// set viewport
	device->setViewports(1, &viewport);
	
	// set the raster state (currently set to the defaults)
	device->setRasterState(g_rasterState);

	// set the depth testing mode (currently set to back face culling)
	device->setDepthStencilState(g_depthStencilState);

	// compute aspect ratio
	float aspectRatio = cgg::getClientHeight() ? float(cgg::getClientWidth()) / cgg::getClientHeight() : 1.0f;

	// compute perspective projection
	g_projection.perspectiveRH(45.0f * cgg::PI / 180.0f, aspectRatio, 0.5f, 250.0f);

	// grab camera matrix from the maya camera controller
	g_camera.computeInverseCameraMatrix(g_view);
	g_view.makeGL();
	
	g_prims->begin();

	game->render(g_prims);

	g_prims->end();

	// render primitives
	g_prims->render(device, g_view, g_projection);
}

//------------------------------------------------------------------------------------------------------------------------------------
/// This function is called shortly after the application starts. Do any initialization for your game here!
//------------------------------------------------------------------------------------------------------------------------------------
void init()
{
	if (HIWORD(BASS_GetVersion()) != BASSVERSION)
	{
		cgg::loge("Wrong Version");
	}
	cgg::fullScreen(true);
	// use a custom openGL renderer (draw2D and draw3D will no longer be called)
	cgg::setFullCustomDraw(custom_gl_draw);

	g_camera.setCentreOfInterest(cgg::Vec3(-20, 0, 0));
	g_camera.rotate(cgg::HALF_PI /2, -0.25);
}

/// Loads the assets for the game
void loadAssets()
{
	// allocate new primitives
	g_prims = new gl::Primitives;

	// init openGL data
	g_prims->initGL(cgg::getGlDevice());	

	//load the world
	game->loadWorld();
	
	//Need to investigate what these do(ATM keep because stuff breaks if not kept)
	// enable depth testing
	gl::DepthStencilStateDesc depth_stencil_desc;
	depth_stencil_desc.init();
	depth_stencil_desc.depthEnable = true;
	g_depthStencilState = cgg::getGlDevice()->createDepthStencilState(depth_stencil_desc);

	// enable back face culling
	gl::RasterStateDesc raster_desc;
	raster_desc.init();
	raster_desc.cullMode = gl::kCullBack;
	g_rasterState = cgg::getGlDevice()->createRasterState(raster_desc);

	game->startGame();
}

//------------------------------------------------------------------------------------------------------------------------------------
/// When an app closes on a mobile phone, it doesn't actually close. Instead, it asks the application to free any assets by calling
/// the freeAssets method here. 
//------------------------------------------------------------------------------------------------------------------------------------
void freeAssets()
{
	delete g_prims;
	g_prims = 0;
}

//------------------------------------------------------------------------------------------------------------------------------------
/// This function is called just before an application terminates.
//------------------------------------------------------------------------------------------------------------------------------------
void kill()
{
	delete g_prims;
	g_prims = 0;
	exit(0);
}

//------------------------------------------------------------------------------------------------------------------------------------
/// When the application is updated, this function will be called. The 'dt' parameter is the time (in seconds) since the last time
/// the game was updated. Use 'dt' when animating things in your game.
//------------------------------------------------------------------------------------------------------------------------------------
void update(float dt)
{
	Timer::update(dt);
	game->update(dt, g_camera);

}

//------------------------------------------------------------------------------------------------------------------------------------
/// Called before draw2D(). Draw your 3D objects here
//------------------------------------------------------------------------------------------------------------------------------------
void draw3D()
{
	// this will not be called, but must remain as an empty function
}

//------------------------------------------------------------------------------------------------------------------------------------
/// Called after draw3D. Allows you to draw a 2D overlay.
//------------------------------------------------------------------------------------------------------------------------------------
void draw2D()
{
	// this will not be called, but must remain as an empty function
}

int g_lastX = 0;
int g_lastY = 0;

//------------------------------------------------------------------------------------------------------------------------------------
/// called when a mouse button is pressed
/// button: 0 == left mouse button, 1 == middle mouse button, 2 == right mouse button.
/// x and y tell you the position of the mouse cursor. [0, 0] is the top left of the window. 
//------------------------------------------------------------------------------------------------------------------------------------
void mousePress(int32_t /*button*/, int32_t x, int32_t y)
{
	g_lastX = x;
	g_lastY = y;
}

//------------------------------------------------------------------------------------------------------------------------------------
/// called when a mouse button is released
/// button: 0 == left mouse button, 1 == middle mouse button, 2 == right mouse button.
/// x and y tell you the position of the mouse cursor. [0, 0] is the top left of the window. 
//------------------------------------------------------------------------------------------------------------------------------------
void mouseRelease(int32_t /*button*/, int32_t x, int32_t y)
{
	g_lastX = x;
	g_lastY = y;
}

//------------------------------------------------------------------------------------------------------------------------------------
/// called when the mouse is moved (and a button is held)
/// x and y tell you the position of the mouse cursor. [0, 0] is the top left of the window. 
//------------------------------------------------------------------------------------------------------------------------------------
void mouseMove(int32_t x, int32_t y)
{
	if (cgg::isAltPressed())
	{
		int32_t dx = x - g_lastX;
		int32_t dy = y - g_lastY;

		// if left & right buttons held
		if (cgg::isButtonHeld(0) && cgg::isButtonHeld(2))
		{
			g_camera.track(dx * 0.01f, dy * 0.01f);
		}
		else
			// if left & middle buttons held, or the right button is held
			if ((cgg::isButtonHeld(0) && cgg::isButtonHeld(1)) || cgg::isButtonHeld(2))
			{
			g_camera.zoom(1.0f - dx * 0.003f);
			}
			else
				// if middle button held
				if (cgg::isButtonHeld(1))
				{
			g_camera.track(dx * 0.01f, dy * 0.01f);
				}
				else
					// if left button held
					if (cgg::isButtonHeld(0))
					{
			g_camera.rotate(dx * 0.003f, -dy * 0.003f);
					}
	}

	g_lastX = x;
	g_lastY = y;
}

// main is buried inside this macro!
CGG_APPLICATION("Bowls");
