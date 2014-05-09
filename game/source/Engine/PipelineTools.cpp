#include <string>
#include "Globals.h"
#include "Platform.h"
#include "PipelineTools.h"
#include "ImageUtils.h"

void TextureAnimationProcessor::Process(const char* folder, const char* inputName, const char* outputName, int dimPerFile, int dimPerCell, int numSourceImages, bool zeroBasedNumbering)
{
	printf("Loading %s/%s\n",folder,inputName);
	std::string fullpath;
	std::string basefilename = folder;
	basefilename +="/";
	basefilename += inputName;

	GetFullFilePathFromResource(basefilename,fullpath);
	
	SDL_Surface* target = TextureAnimationProcessor::CreateRGBASurface(dimPerFile,dimPerFile);
	int currX = 0;
	int currY = 0;
	int targetCounter = 0;

	int imagesWrittenToTarget = 0;
	bool imagesWaitingToBeWritten = false;
	for(int i = 0; i < numSourceImages; i++)
	{
		char imgPath[512];
		int imageIndex = i;
		imagesWaitingToBeWritten = true;
		if(!zeroBasedNumbering) imageIndex += 1;
		sprintf(imgPath,"%s_%02d.png",fullpath.c_str(),imageIndex);
		printf("Processing %s\n",imgPath);
		SDL_Surface* img = TextureAnimationProcessor::CreateSurfaceFromImage(imgPath);
		SDL_SetSurfaceAlphaMod(img,SDL_ALPHA_TRANSPARENT);
		TextureAnimationProcessor::AddSurfaceToTargetSurface(target,img,currX,currY,dimPerCell);
		currX += img->w;
		if(currX >= dimPerFile)
		{
			currX = 0;
			currY += dimPerCell;//img->h;
			if(currY >= dimPerFile)
			{
				currY = 0;
				TextureAnimationProcessor::SaveSurfaceToPNG(target, targetCounter,outputName,folder);
				targetCounter++;
				imagesWrittenToTarget++;
				SDL_FreeSurface(target);
				target = TextureAnimationProcessor::CreateRGBASurface(dimPerFile,dimPerFile);
				SDL_SetSurfaceAlphaMod(target,SDL_ALPHA_TRANSPARENT);
				imagesWrittenToTarget = 0;
				imagesWaitingToBeWritten = false;
			}

		}
		SDL_FreeSurface(img);
		
	}
	if(imagesWaitingToBeWritten == true)
	{
		TextureAnimationProcessor::SaveSurfaceToPNG(target, targetCounter,outputName,folder);
	}
	SDL_FreeSurface(target);

}



SDL_Surface* TextureAnimationProcessor::CreateSurfaceFromImage(const char* fullpath)
{
	//int width, height;
	//png_byte color_type;
	//png_byte bit_depth;
	//int numChannels = 4;
   /*read_png_file(fullpath,NULL,width,height,color_type,bit_depth);
   if(color_type == PNG_COLOR_TYPE_RGBA)
   {
		numChannels = 4;
   }
   else
   {
		numChannels = 3;
   }*/

   printf("loading %s\n",fullpath);
   SDL_Surface* surface = IMG_Load( fullpath );  
   if( !surface )
    {
        printf("Could not load %s\n", fullpath );
        return NULL;
    }    

	
	// Check that the imageÌs width is a power of 2
	if ( (surface->w & (surface->w - 1)) != 0 ) {
		printf("warning: image.bmpÌs width is not a power of 2\n");
	}

	// Also check if the height is a power of 2
	if ( (surface->h & (surface->h - 1)) != 0 ) {
		printf("warning: image.bmpÌs height is not a power of 2\n");
	}
		
	//contains an alpha channel
	if(surface->format->BytesPerPixel==4)
	{
		
		if(surface->format->Rmask==0x000000ff)
			printf("texture_format=GL_RGBA;");
		else
			printf("texture_format=GL_BGRA;");
	}
	else if(surface->format->BytesPerPixel==3) //no alpha channel
	{
		if(surface->format->Rmask==0x000000ff)
			printf("texture_format=GL_RGB;");
		else
			printf("texture_format=GL_BGR;");
	}
	else
	{
		printf("warning: the image is not truecolor this will break");
	}

	return surface;
}


SDL_Surface* TextureAnimationProcessor::CreateRGBASurface(int width, int height)
{
	unsigned int rmask, gmask, bmask, amask;

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
	//return SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_SRCALPHA, width, height,32, rmask, gmask, bmask, amask);
    return SDL_CreateRGBSurface(SDL_SWSURFACE, width, height,32, rmask, gmask, bmask, amask);
}

void TextureAnimationProcessor::SaveSurfaceToPNG(SDL_Surface* target, int targetCounter,const char* outputName, const char* folder)
{
	std::string fullpath;
	std::string basefilename = folder;
	basefilename +="/";
	basefilename += outputName;
	char imgPath[512];
	sprintf(imgPath,"%s_%02d.png",basefilename.c_str(),targetCounter);
	std::string path = imgPath;
	GetFullFilePathFromResource(path,fullpath);

	SDL_LockSurface(target);
	unsigned int datalength = target->w*target->h*target->format->BytesPerPixel;
	unsigned int rowlength = target->w*target->format->BytesPerPixel;
	unsigned char* pixels = (unsigned char*)calloc(target->w*target->h,target->format->BytesPerPixel);
	memcpy(pixels,(unsigned char*)(target->pixels),datalength);
	SDL_UnlockSurface(target);

	unsigned char** row_pointers = (unsigned char**)calloc(target->h,sizeof(unsigned char*));
	for(int i = 0; i < target->h; i++)
	{
		unsigned char* rowptr = (unsigned char*)malloc(target->format->BytesPerPixel*target->w);
		memcpy(rowptr,pixels,rowlength);
		row_pointers[i] = rowptr;
		pixels += rowlength;
	}
	
	if(target->format->BytesPerPixel == 4)
		write_png_file(fullpath.c_str(),row_pointers,target->w,target->h,PNG_COLOR_TYPE_RGB_ALPHA,8);
	else if( target->format->BytesPerPixel == 3)
		write_png_file(fullpath.c_str(),row_pointers,target->w,target->h,PNG_COLOR_TYPE_RGB,8);
	else
		printf("nothing written... bad image bit depth %d", target->format->BitsPerPixel);


	for (int y=0; y<target->h; y++)
		free(row_pointers[y]);
	free(row_pointers);
	printf("writing %s\n",fullpath.c_str());
	
}

void TextureAnimationProcessor::AddSurfaceToTargetSurface(SDL_Surface* target,SDL_Surface* src,int currX,int currY,int dimPerCell)
{
	SDL_Rect sourceRect;
	sourceRect.x = 0;
	sourceRect.y = 0;
	sourceRect.w = src->w;
	sourceRect.h = dimPerCell;//src->h;

	SDL_Rect targetRect;
	targetRect.x = currX;
	targetRect.y = currY;
	targetRect.w = src->w;
	targetRect.h = dimPerCell;//src->h;

	//SDL_LockSurface(target);
	SDL_BlitSurface(src, &sourceRect, target, &targetRect);
	//SDL_UnlockSurface(target);
	
}