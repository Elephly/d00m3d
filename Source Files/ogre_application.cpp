#include "ogre_application.h"
#include "../bin/Source Files/path_config.h"

namespace ogre_application
{
	/* Some configuration constants */
	/* They are written here as global variables, but ideally they should be loaded from a configuration file */

	/* Initialization */
	const Ogre::String config_filename_g = "";
	const Ogre::String plugins_filename_g = "";
	const Ogre::String log_filename_g = "Ogre.log";

	/* Main window settings */
	const Ogre::String window_title_g = PROJECT_NAME;
	const Ogre::String custom_window_capacities_g = "";
	const unsigned int window_width_g = 1200;
	const unsigned int window_height_g = (int)((float)window_width_g / (16.0f / 9.0f));
	const bool window_full_screen_g = false;

	Ogre::Viewport *viewport;
	/* Viewport and camera settings */
	float viewport_width_g = 1.0f;
	float viewport_height_g = 1.0f;
	float viewport_left_g = (1.0f - viewport_width_g) * 0.5f;
	float viewport_top_g = (1.0f - viewport_height_g) * 0.5f;
	unsigned short viewport_z_order_g = 100;
	//const Ogre::ColourValue viewport_background_color_g((100.0f / 255.0f), (149.0f / 255.0f), (237.0f / 255.0f));
	const Ogre::ColourValue viewport_background_color_g(0.0f, 0.0f, 0.0f);
	float camera_near_clip_distance_g = 0.01f;
	float camera_far_clip_distance_g = 10000.0f;
	Ogre::Vector3 camera_position_g(0.0, 1.0, 2.0);
	Ogre::Vector3 camera_look_at_g(0.0, 0.0, 0.0);
	Ogre::Vector3 camera_up_g(0.0, 1.0, 0.0);

	/* Materials */
	const Ogre::String resource_directory_g = "./Resources";


	OgreApplication::OgreApplication()
	{
		/* Don't do work in the constructor, leave it for the Init() function */
	}

	OgreApplication::~OgreApplication()
	{
	}

	void OgreApplication::Initialize()
	{
		/* Set default values for the variables */
		ogre_input_manager_ = NULL;
		mouse_ = NULL;
		keyboard_ = NULL;

		/* Run all initialization steps */
		InitRootNode();
		InitPlugins();
		InitRenderSystem();
		InitWindow();
		InitViewport();
		InitEvents();
		InitOIS();
		LoadResources();
		RegisterInputCallbacks();

		MeshFactory::Initialize(&ogre_root_);

		Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
		Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

		//Ogre::OverlayManager* ovrly_ptr;
		//Ogre::OverlayManager& overlay_manager = Ogre::OverlayManager::getSingleton();

		////setting up the world
		//
		//ovrly_ptr = &overlay_manager;
	//	world.initWorld(scene_manager, &camera, &inputManager, ovrly_ptr); 
		world.initWorld(scene_manager, &camera, &inputManager,viewport); 

	}

	void OgreApplication::InitRootNode()
	{
		try
		{
			/* We need to have an Ogre root to be able to access all Ogre functions */
			ogre_root_ = std::auto_ptr<Ogre::Root>(new Ogre::Root(config_filename_g, plugins_filename_g, log_filename_g));
			//ogre_root_->showConfigDialog();
		}
		catch (Ogre::Exception &e)
		{
			throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
		}
		catch(std::exception &e)
		{
			throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
		}
	}
	
	void OgreApplication::InitPlugins()
	{
		try
		{
			/* Load plugin responsible for OpenGL render system */
			Strings plugin_names;
			plugin_names.push_back("RenderSystem_GL");
		
			Strings::iterator iter = plugin_names.begin();
			Strings::iterator iter_end = plugin_names.end();
			for (; iter != iter_end; iter++)
			{
				Ogre::String& plugin_name = (*iter);
				if (OGRE_DEBUG_MODE)
				{
					plugin_name.append("_d");
				}
				ogre_root_->loadPlugin(plugin_name);
			}
		}
		catch (Ogre::Exception &e)
		{
			throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
		}
		catch(std::exception &e)
		{
			throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
		}
	}

	void OgreApplication::InitRenderSystem()
	{
		try
		{
			const Ogre::RenderSystemList& render_system_list = ogre_root_->getAvailableRenderers();
			if (render_system_list.size() == 0)
			{
				throw(OgreAppException(std::string("OgreApp::Exception: Sorry, no rendersystem was found.")));
			}

			Ogre::RenderSystem *render_system = render_system_list.at(0);
			ogre_root_->setRenderSystem(render_system);
		}
		catch (Ogre::Exception &e)
		{
			throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
		}
		catch(std::exception &e)
		{
			throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
		}
	}

	void OgreApplication::InitWindow()
	{
		try
		{
			/* Create main window for the application */
			bool create_window_automatically = false;
			ogre_root_->initialise(create_window_automatically, window_title_g, custom_window_capacities_g);

			Ogre::NameValuePairList params;
			params["FSAA"] = "0";
			params["vsync"] = "true";
			ogre_window_ = ogre_root_->createRenderWindow(window_title_g, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), window_full_screen_g, &params);

			ogre_window_->setActive(true);
			ogre_window_->setAutoUpdated(false);
			isFocus = true;
		}
		catch (Ogre::Exception &e)
		{
			throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
		}
		catch(std::exception &e)
		{
			throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
		}
		
	}

	void OgreApplication::InitViewport()
	{
		try
		{
			/* Retrieve scene manager and root scene node */
			Ogre::SceneManager* scene_manager = ogre_root_->createSceneManager(Ogre::ST_GENERIC, "MySceneManager");
			Ogre::SceneNode* root_scene_node = scene_manager->getRootSceneNode();

			/* Create camera object */
			Ogre::Camera* ogreCamera = scene_manager->createCamera("MyCamera");
			Ogre::SceneNode* camera_scene_node = root_scene_node->createChildSceneNode("MyCameraNode");
			

			ogreCamera->setNearClipDistance(camera_near_clip_distance_g);
			ogreCamera->setFarClipDistance(camera_far_clip_distance_g); 

			ogreCamera->setPosition(camera_position_g);
			ogreCamera->lookAt(camera_look_at_g);
			ogreCamera->setFixedYawAxis(true, camera_up_g);		


			/* Create viewport */
			viewport = ogre_window_->addViewport(ogreCamera, viewport_z_order_g, viewport_left_g, viewport_top_g, viewport_width_g, viewport_height_g);

			viewport->setAutoUpdated(true);
			viewport->setBackgroundColour(viewport_background_color_g);
			

			/* Set aspect ratio */
			float ratio = float(viewport->getActualWidth()) / float(viewport->getActualHeight());
			ogreCamera->setAspectRatio(ratio);

			camera.initCamera(ogreCamera, camera_scene_node);
		}
		catch (Ogre::Exception &e)
		{
			throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
		}
		catch(std::exception &e)
		{
			throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
		}
	}

	void OgreApplication::InitEvents()
	{
		try
		{
			/* Add this object as a FrameListener for render events (see frameRenderingQueued event) */
			ogre_root_->addFrameListener(this);

			/* Add this object as a WindowEventListener to handle the window resize event */
			Ogre::WindowEventUtilities::addWindowEventListener(ogre_window_, this);
		}
		catch (Ogre::Exception &e)
		{
			throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
		}
		catch(std::exception &e)
		{
			throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
		}
	}

	void OgreApplication::InitOIS()
	{
		/* Initialize the Object Oriented Input System (OIS) */
		try
		{
			/* Initialize input manager */
			OIS::ParamList pl; // Parameter list passed to the input manager initialization
			size_t windowHnd = 0;
			std::ostringstream windowHndStr;
			ogre_window_->getCustomAttribute("WINDOW", &windowHnd);
			windowHndStr << windowHnd;
			pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
			//pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
			//pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
			ogre_input_manager_ = OIS::InputManager::createInputSystem(pl);

			/*size_t hWnd = 0;
			ogre_window_->getCustomAttribute("WINDOW", &hWnd);
			ogre_input_manager_ = OIS::InputManager::createInputSystem(hWnd);*/

			/* Initialize keyboard and mouse */
			keyboard_ = static_cast<OIS::Keyboard*>(ogre_input_manager_->createInputObject(OIS::OISKeyboard, false));

			mouse_ = static_cast<OIS::Mouse*>(ogre_input_manager_->createInputObject(OIS::OISMouse, false));
			unsigned int width, height, depth;
			int top, left;
			ogre_window_->getMetrics(width, height, depth, left, top);
			const OIS::MouseState &ms = mouse_->getMouseState();
			ms.width = width;
			ms.height = height;

			inputManager.Initialize(keyboard_, mouse_);
		}
		catch(std::exception &e)
		{
			throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
		}
	}

	void OgreApplication::LoadResources()
	{
		try
		{
			
			/* Load materials that can then be assigned to objects in the scene */
			Ogre::String resource_group_name = "MyGame";
			Ogre::ResourceGroupManager& resource_group_manager = Ogre::ResourceGroupManager::getSingleton();
			resource_group_manager.createResourceGroup(resource_group_name);
			bool is_recursive = true;
			resource_group_manager.addResourceLocation(resource_directory_g, "FileSystem", resource_group_name, is_recursive);
			resource_group_manager.initialiseResourceGroup(resource_group_name);
			resource_group_manager.loadResourceGroup(resource_group_name);

			//load font 
			//Ogre::ResourceGroupManager& resource_group_manager = Ogre::ResourceGroupManager::getSingleton();
		    resource_group_manager.addResourceLocation("C:\\Windows\\Fonts", "FileSystem");
            
		}
		catch (Ogre::Exception &e)
		{
			throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
		}
		catch(std::exception &e)
		{
			throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
		}
	}
	
	void OgreApplication::RegisterInputCallbacks()
	{
		inputManager.RegisterCallback(this, ExitOgreApplication, INPUT_SOURCE_KEYBOARD, INPUT_EVENT_RELEASE, MOUSE_MOTION_STATE_EITHER, (int)OIS::KC_ESCAPE);
		inputManager.RegisterCallback(this, CameraZoom, INPUT_SOURCE_NONE, INPUT_EVENT_NONE, MOUSE_MOTION_STATE_MOVING, 0);
	}

	void OgreApplication::MainLoop()
	{
		try
		{
			/* Main loop to keep the application going */
			ogre_root_->clearEventTimes();

			while(!ogre_window_->isClosed())
			{
				ogre_window_->update(false);

				ogre_window_->swapBuffers();

				ogre_root_->renderOneFrame();

				Ogre::WindowEventUtilities::messagePump();

				if (isFocus)
				{
					unsigned int width, height, depth;
					int top, left;
					ogre_window_->getMetrics(width, height, depth, left, top);

					LPPOINT point = new tagPOINT();
					GetCursorPos(point);
					SetCursorPos(left + (width / 2), top + (height / 2));
					inputManager.CompensateManualMouseSetPosition((left + (width / 2)) - point->x,
						(top + (height / 2)) - point->y);
					delete point;
				}
			}
		}
		catch (Ogre::Exception &e)
		{
			throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
		}
		catch(std::exception &e)
		{
			throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
		}
	}

	bool OgreApplication::frameRenderingQueued(const Ogre::FrameEvent& fe)
	{
		if (isFocus)
		{
			/* Capture input */
			inputManager.Update(fe);

			world.updateWorld(fe);
		}

		return true;
	}

	void OgreApplication::windowFocusChange(Ogre::RenderWindow* rw)
	{
		isFocus = !isFocus;
	}

	void OgreApplication::windowResized(Ogre::RenderWindow* rw)
	{
		/* Update the window and aspect ratio when the window is resized */
		int width = rw->getWidth(); 
		int height = rw->getHeight();
      
		Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
		Ogre::Camera* camera = scene_manager->getCamera("MyCamera");

		if (camera != NULL){
			//std::cout << "1 " << (double)width/height << std::endl;
			camera->setAspectRatio((float)width/height);
		}

		const OIS::MouseState &ms = mouse_->getMouseState();
		ms.width = width;
		ms.height = height;

		ogre_window_->resize(width, height);
		ogre_window_->windowMovedOrResized();
		ogre_window_->update();
	}

	void OgreApplication::ExitOgreApplication(void *context, const Ogre::FrameEvent& fe, int x1, int y1, int z1, int x2, int y2, int z2)
	{
		if (context)
		{
			OgreApplication *app = static_cast<OgreApplication *>(context);
			app->ogre_root_->shutdown();
			app->ogre_window_->destroy();
			app->world.JudgementDay();
			app->inputManager.Interrupt();
		}
	}

	void OgreApplication::CameraZoom(void *context, const Ogre::FrameEvent& fe, int x1, int y1, int z1, int x2, int y2, int z2)
	{
		if (context && z1 != z2)
		{
			OgreApplication *app = static_cast<OgreApplication *>(context);
			Ogre::Vector3 dist = app->camera.getPosition();
			Ogre::Vector3 dir = dist.normalisedCopy();
			Ogre::Vector3 minDist = Ogre::Vector3(dir.x * 0.1f, dir.y * 0.1f, dir.z * 0.1f);
			Ogre::Vector3 maxDist = Ogre::Vector3(dir.x * 4.0f, dir.y * 4.0f, dir.z * 4.0f);
			Ogre::Vector3 newDist = dist - dir * ((float)(z2 - z1) / 240.0f);
			Ogre::Vector3 finalDist = Ogre::Vector3(std::min(maxDist.x, std::max(minDist.x, newDist.x)),
				std::min(maxDist.y, std::max(minDist.y, newDist.y)), std::min(maxDist.z, std::max(minDist.z, newDist.z)));
			app->camera.setDistance(finalDist);
		}
	}
}