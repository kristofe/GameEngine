#ifndef _PIPELINE_TOOLS_H
#define _PIPELINE_TOOLS_H

#ifndef TARGET_OS_IPHONE
#include <map>
#include <string>
#include "SDL.h"
#include "SDL_Image.h"
#include "libpng/png.h"


class TextureAnimationProcessor{


public:
	static void Process(const char* folder, const char* inputName, const char* outputName, int dimPerFile, int dimPerCell, int numSourceImages,bool zeroBasedNumbering = false);
	static SDL_Surface* CreateSurfaceFromImage(const char* fullpath);
	static SDL_Surface* CreateRGBASurface(int width, int height);
	static void SaveSurfaceToPNG(SDL_Surface* target, int targetCounter,const char* outputName, const char* folder);
	static void AddSurfaceToTargetSurface(SDL_Surface* target,SDL_Surface* img,int currX,int currY,int dimPerCell);
private:
	//Remove Copy Constructor and assignment operator
	//TextureAnimationProcessor		&operator =  (const RendererObjectFactory &m);
	//TextureAnimationProcessor(const RendererObjectFactory &m);
	

};

#endif//TARGET_OS_IPHONE
#endif //_PIPELINE_TOOLS_H