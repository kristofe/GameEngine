#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <SDL2/SDL.h>
#ifdef TARGETIPHONE
    //#include <SDL2/SDL_opengles.h>
#else
    #include "PipelineTools.h"
#endif

#include "Platform.h"
#include "Scene.h"
#include "Game.h"
#include "RendererGL15.h"

/* screen width, height, and bit depth */
#define WINDOW_WIDTH  320
#define WINDOW_HEIGHT 480
#define WINDOW_BPP     32

/* Array for all available keys */
SDL_bool pressedkeys[SDL_NUM_SCANCODES];

Renderer*			_renderer;
Game*				_gameEngine;

/* function to release/destroy our resources and restoring the old desktop */
void Quit(int returnCode)
{
	
	
    SDL_Quit();
	
	
	delete _gameEngine;
	delete _renderer;
	
    /* and exit appropriately */
    exit(returnCode);
}



/* function to handle key press events */
void handleKeyPress(SDL_Keysym* keysym, SDL_bool pressed)
{
    if (pressed==SDL_TRUE)
    {
		pressedkeys[keysym->scancode]=SDL_TRUE;
    }
    else
    {
		pressedkeys[keysym->scancode]=SDL_FALSE;
    }
	
    /* Handle key presses only for these keys */
    if ((pressedkeys[SDL_SCANCODE_ESCAPE]) && (pressed))
    {
        Quit(0);
    }
}

void handleRepeatedKeys()
{
}

/* Here goes our drawing code */
void update()
{
    _gameEngine->Update();
}

/* Here goes our drawing code */
void render()
{
	_renderer->Render();
}

/* Here goes our drawing code */
void lateUpdate()
{
	_gameEngine->LateUpdate();
}

int main(int argc, char** argv)
{
    int status;
#if _WIN32 && _DEBUG
	RedirectIOToConsole();
#endif
    /* main loop variable */
    SDL_bool done=SDL_FALSE;
    /* used to collect events */
    SDL_Event event;
    /* whether or not the window is active */
    SDL_bool isActive=SDL_TRUE;
	
    status=SDL_Init(SDL_INIT_VIDEO);
    if (status<0)
    {
        fprintf(stderr, "Can't init default SDL video driver: %s\n", SDL_GetError());
        exit(-1);
    }
	
    // Select first display
    //status=SDL_SelectVideoDisplay(0);
    //if (status<0)
    //{
    //    fprintf(stderr, "Can't attach to first display: %s\n", SDL_GetError());
    //    exit(-1);
    //}
	
	/*for(int i = 1; i <= 8; i++)
	{
		char srcFile[64];
		char targetFile[64];
		sprintf(srcFile,"gameClip_%02d",i);
		sprintf(targetFile,"gameClip00%02d",i);
		TextureAnimationProcessor::Process("revised_hanged_assets/new_inorder_clips",srcFile, targetFile,1024,256,48,false);
	}
	TextureAnimationProcessor::Process("revised_hanged_assets/tidal_wave_sized","tidalwave_256", "tidalwave256",1024,256,24,false);
	return 0;*/
	_renderer = new RendererGL15(WINDOW_WIDTH, WINDOW_HEIGHT);
    
	_gameEngine = &(Game::GetInstance());
	_gameEngine->SetRenderer(_renderer);
	_renderer->SetGameEngine(_gameEngine);
	_gameEngine->LoadXMLData("Config.xml");
	_gameEngine->Start(0);
	
    /* wait for events */
    while (!done)
    {
        /* handle the events in the queue */
        while (SDL_PollEvent(&event))
        {
			float u,v,u2,v2;
			int button;
			bool pressed = false;
			u = v = u2 = v2 = 0.0f;
			button = 0;
			
            switch (event.type)
            {
                case SDL_WINDOWEVENT:
					switch (event.window.event)
					{
						case SDL_WINDOWEVENT_CLOSE:
							done=SDL_TRUE;
							break;
						case SDL_WINDOWEVENT_HIDDEN:
							isActive=SDL_FALSE;
							break;
						case SDL_WINDOWEVENT_SHOWN:
							isActive=SDL_TRUE;
							break;
					}
					break;
				case SDL_MOUSEBUTTONUP:
					button = event.button.which;
					u = event.button.x/((float)WINDOW_WIDTH);
					v = event.button.y/((float)WINDOW_HEIGHT);
					_gameEngine->MouseUp(u, 1.0f-v, button);
					break;
				case SDL_MOUSEBUTTONDOWN:
					button = event.button.which;
					u = event.button.x/((float)WINDOW_WIDTH);
					v = event.button.y/((float)WINDOW_HEIGHT);
					_gameEngine->MouseDown(u,1.0f-v, button);
					break;
				case SDL_MOUSEMOTION:
					pressed = event.motion.state == SDL_PRESSED;
					if(pressed)
					{
						button = event.motion.which;
						u = event.motion.x/((float)WINDOW_WIDTH);
						v = event.motion.y/((float)WINDOW_HEIGHT);	
						u2 = (event.motion.x + event.motion.xrel)/((float)WINDOW_WIDTH);
						v2 = (event.motion.y + event.motion.yrel)/((float)WINDOW_HEIGHT);	
						//printf("Mouse Dragged pos(%d,%d) rel(%d,%d)\n",event.motion.x,event.motion.y,event.motion.xrel,event.motion.yrel);
						_gameEngine->MouseDragged(u2, 1.0f-v2,u,1.0f-v, button);
						
					}
					break;
					
                case SDL_KEYDOWN:
					/* handle key presses */
					handleKeyPress(&event.key.keysym, SDL_TRUE);
					break;
                case SDL_KEYUP:
					/* handle key presses */
					handleKeyPress(&event.key.keysym, SDL_FALSE);
					break;
                case SDL_QUIT:
					/* handle quit requests */
					done=(SDL_bool)true;
					break;
                default:
					break;
            }
        }
		
        /* draw the scene */
        if (isActive)
        {
			handleRepeatedKeys();
			update();
			render();
			lateUpdate();
        }
    }
	
    /* clean ourselves up and exit */
    Quit(0);
	
    /* Should never get here */
    return 0;
}
