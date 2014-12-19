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
// Backend Includes
#include "Kinect.h"
// Game Components Includes
#include "Jack.h"
#include "Bowl.h"
#include "Box.h"
#include "Physics.h"

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

Jack* jack;
Bowl* red;
Bowl* blue;
Box* cue;
Box* cue2;
Box* ground;
Box* endWall;
Box* sideWall1;
Box* sideWall2;

KinectInput kinect(sitMode,hand);

//Random Global needs deleting or refactoring
float lastTime = 0.0f;
float testTime = 0.0f;
float maxTime = 10.0f;
int stage = 0;
float lockedZ = 0.0f;
float throwStartTime = 0.0f;
float lockedX = 0.0f;
bool throwTest = false;
float distanceTest = 3.0f;

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

	cue->render(g_prims);
	cue2->render(g_prims);
	jack->render(g_prims);
	red->render(g_prims);
	blue->render(g_prims);
	ground->render(g_prims);
	endWall->render(g_prims);
	sideWall1->render(g_prims);
	sideWall2->render(g_prims);

	g_prims->end();

	// render primitives
	g_prims->render(device, g_view, g_projection);
}

//------------------------------------------------------------------------------------------------------------------------------------
/// This function is called shortly after the application starts. Do any initialization for your game here!
//------------------------------------------------------------------------------------------------------------------------------------
void init()
{
	// use a custom openGL renderer (draw2D and draw3D will no longer be called)
	cgg::setFullCustomDraw(custom_gl_draw);
	
	// TODO: Might need a conditional to check here for errors
	kinect.startTracking();

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

	// start constructing some shapes
	g_prims->begin();

	// generate a red sphere
	maths::Mat43 m = maths::Mat43::kIdentity;
	m.w.x = 0;
	m.w.z = 0;
	m.w.y = -2;

	maths::Vec3 colour;
	colour.x = 1;
	colour.y = 0;
	colour.z = 0;
	red = new Bowl(m, colour,1);
	red->render(g_prims);

	// generate a blue sphere
	m.w.x = 8;
	m.w.z = 0;
	m.w.y = -2;

	colour.x = 0;
	colour.y = 0;
	colour.z = 1;
	blue = new Bowl(m, colour, 1);
	blue->render(g_prims);

	// generate a jack
	m.w.x = -20;
	m.w.z = 0;
	m.w.y = 0;

	colour.x = 1;
	colour.y = 1;
	colour.z = 0;
	jack = new Jack(m, colour, 0.5);
	
	// generate a cue
	m.w.x = -19.5f;
	m.w.z = -5.5f;
	m.w.y = -0.5f;

	colour.x = 1;
	colour.y = 0;
	colour.z = 0;
	cue = new Box(m, colour, 1);
	cue->render(g_prims);

	// generate a cue
	m.w.x = -19.5f;
	m.w.z = 5.5f;
	m.w.y = -0.5f;

	colour.x = 1;
	colour.y = 0;
	colour.z = 0;
	cue2 = new Box(m, colour, 1);
	cue2->render(g_prims);

	// generate ground
	m.w.x = 0;
	m.w.z = 0;
	m.w.y = -4;

	colour.x = 0;
	colour.y = 1;
	colour.z = 0;
	ground = new Box(m, colour, cgg::Vec3(40, 2, 10));
	ground->render(g_prims);

	// generate endWall
	m.w.x = 20.5f;
	m.w.z = 0;
	m.w.y = -3;

	colour.x = 1;
	colour.y = 1;
	colour.z = 1;
	endWall = new Box(m, colour, cgg::Vec3(1, 4, 12));
	endWall->render(g_prims);

	// generate sideWall1
	m.w.x = 0;
	m.w.z = -5.5f;
	m.w.y = -3;

	colour.x = 1;
	colour.y = 1;
	colour.z = 1;
	sideWall1 = new Box(m, colour, cgg::Vec3(40, 4, 1));
	sideWall1->render(g_prims);

	// generate sideWall2
	m.w.x = 0;
	m.w.z = 5.5f;
	m.w.y = -3;

	colour.x = 1;
	colour.y = 1;
	colour.z = 1;
	sideWall2 = new Box(m, colour, cgg::Vec3(40, 4, 1));
	sideWall2->render(g_prims);

	//End the group of primitives
	g_prims->end();
	
	
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
	lastTime += dt;

	if (throwTest)
	{
		testTime += dt;
	}

	kinect.update();
	cgg::Vec3 handPos = kinectPosConversion(kinect.getHandPos());

	switch (stage)
	{
	case 0:
		// Kinect Test (Really Hacky)
		jack->updatePosition(cgg::Vec3(-17, -1, 0) + cgg::Vec3(-5, -1.5f, handPos.z));
		break;
	case 1:
	case 2:
		// Kinect Test (Really Hacky)
		jack->updatePosition(cgg::Vec3(-17, -1, 0) + cgg::Vec3(handPos.x, -1.5, lockedZ));
		break;
	}

	//jack->updatePosition(cgg::Vec3(-17, -1, 0) + handPos);

	//g_camera.track(jack->getPosition().x, jack->getPosition().y);

	if (cgg::isKeyPressed(cgg::kKeyEscape))
	{
		exit(0);
	}

	if (lastTime > maxTime && stage < 4)
	{
		lastTime = 0.0f;
		stage++;
		maths::Mat43 m = maths::Mat43::kIdentity;
		maths::Vec3 colour;
		switch (stage)
		{
		case 1:
			// turn cues yellow
			m.w.x = -19.5f;
			m.w.z = -5.5f;
			m.w.y = -0.5f;

			colour.x = 1;
			colour.y = 1;
			colour.z = 0;
			cue = new Box(m, colour, 1);
			cue->render(g_prims);

			m.w.z = 5.5f;

			cue2 = new Box(m, colour, 1);
			cue2->render(g_prims);

			lockedZ = handPos.z;

			maxTime = 5.0f;
			break;
		case 2:
			// turn cues green
			m.w.x = -19.5f;
			m.w.z = -5.5f;
			m.w.y = -0.5f;

			colour.x = 0;
			colour.y = 1;
			colour.z = 0;
			cue = new Box(m, colour, 1);
			cue->render(g_prims);

			m.w.z = 5.5f;

			cue2 = new Box(m, colour, 1);
			cue2->render(g_prims);

			lockedX = handPos.x;
			throwTest = true;

			maxTime = 10.0f;
			break; 
		case 3:
			jack->updateXVelocity(10.0f);
		}
	}

	if (stage == 2)
	{
		cgg::logi(std::to_string(handPos.x).c_str());
	}
	
	if (stage == 2 && handPos.x > lockedX + distanceTest)
	{
		throwTest = false;
		if ((Physics::kinectInputVelocity(distanceTest, testTime)) < 2.5)
		{
			jack->updateXVelocity((Physics::kinectInputVelocity(distanceTest, testTime))*10.0f);
		}
		else
		{
			jack->updateXVelocity(25.0f);
		}
		stage = 5;
	}
	
	//ball ball collision check test
	if (Physics::collisionCheck(red, blue, dt))
	{
		//new velocities
		Physics::newCollisionVelocities(red, blue);
	}
	/*if (Physics::collisionCheck(red, jack, dt))
	{
		//new velocities
		Physics::newCollisionVelocities(red, jack);
	}*/
	if (Physics::collisionCheck(red, blue, dt))
	{
		//new velocities
		Physics::newCollisionVelocities(red, blue);
	}
	//wall wall collision tests
	if (Physics::collisionCheck(jack, endWall, dt))
	{
		//new velocities
		Physics::newCollisionVelocities(jack);
	}
	if (Physics::collisionCheck(jack, sideWall1, dt))
	{
		//new velocities
		Physics::newCollisionVelocities(jack);
	}
	if (Physics::collisionCheck(jack, sideWall2, dt))
	{
		//new velocities
		Physics::newCollisionVelocities(jack);
	}

	//move balls
	red->changePosition(cgg::Vec3(red->getXVelocity() * dt,	0, red->getZVelocity() * dt));
	blue->changePosition(cgg::Vec3(blue->getXVelocity() * dt, 0, blue->getZVelocity() * dt));
	jack->changePosition(cgg::Vec3(jack->getXVelocity() * dt, 0, jack->getZVelocity() * dt));

	//friction
	Physics::applyFriction(red);
	Physics::applyFriction(blue);
	Physics::applyFriction(jack);

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

cgg::Vec3 kinectPosConversion(cgg::Vec3 pos)
{
	//Scale it up a bit to make movements noticeable
	pos *= 5;

	//If you don't initialize Vec3 similar to this they error (libCGG 'Feature')
	cgg::Vec3 flippedKinectPos = { 0, 0, 0 };
	
	flippedKinectPos.x = -pos.z;
	flippedKinectPos.y = pos.y;
	flippedKinectPos.z = pos.x;
	
	return flippedKinectPos;
}

// main is buried inside this macro!
CGG_APPLICATION("Bowls");
