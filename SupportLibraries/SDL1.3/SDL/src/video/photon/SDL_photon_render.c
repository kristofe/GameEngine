/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2009 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org

    QNX Photon GUI SDL driver
    Copyright (C) 2009 Mike Gorchak
    (mike@malva.ua, lestat@i.com.ua)
*/

#include "SDL_config.h"

#include "../SDL_pixels_c.h"
#include "../SDL_yuv_sw_c.h"

#include "SDL_video.h"

#include "SDL_photon_render.h"
#include "SDL_photon.h"

static SDL_Renderer* photon_createrenderer(SDL_Window* window, Uint32 flags);
static int  photon_displaymodechanged(SDL_Renderer* renderer);
static int  photon_activaterenderer(SDL_Renderer* renderer);
static int  photon_createtexture(SDL_Renderer* renderer, SDL_Texture* texture);
static int  photon_querytexturepixels(SDL_Renderer* renderer, SDL_Texture* texture, void** pixels, int* pitch);
static int  photon_settexturepalette(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Color* colors, int firstcolor, int ncolors);
static int  photon_gettexturepalette(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Color* colors, int firstcolor, int ncolors);
static int  photon_settexturecolormod(SDL_Renderer* renderer, SDL_Texture* texture);
static int  photon_settexturealphamod(SDL_Renderer* renderer, SDL_Texture* texture);
static int  photon_settextureblendmode(SDL_Renderer* renderer, SDL_Texture* texture);
static int  photon_settexturescalemode(SDL_Renderer* renderer, SDL_Texture* texture);
static int  photon_updatetexture(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* rect, const void* pixels, int pitch);
static int  photon_locktexture(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* rect, int markDirty, void** pixels, int* pitch);
static void photon_unlocktexture(SDL_Renderer* renderer, SDL_Texture* texture);
static void photon_dirtytexture(SDL_Renderer* renderer, SDL_Texture* texture, int numrects, const SDL_Rect* rects);
static int  photon_renderpoint(SDL_Renderer* renderer, int x, int y);
static int  photon_renderline(SDL_Renderer* renderer, int x1, int y1, int x2, int y2);
static int  photon_renderfill(SDL_Renderer* renderer, const SDL_Rect* rect);
static int  photon_rendercopy(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect);
static void photon_renderpresent(SDL_Renderer* renderer);
static void photon_destroytexture(SDL_Renderer* renderer, SDL_Texture* texture);
static void photon_destroyrenderer(SDL_Renderer* renderer);

SDL_RenderDriver photon_renderdriver=
{
    photon_createrenderer,
    {
       "photon",
       (SDL_RENDERER_SINGLEBUFFER | SDL_RENDERER_PRESENTCOPY |
        SDL_RENDERER_PRESENTFLIP2 | SDL_RENDERER_PRESENTFLIP3 |
        SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_PRESENTDISCARD |
        SDL_RENDERER_ACCELERATED),
       (SDL_TEXTUREMODULATE_NONE | SDL_TEXTUREMODULATE_COLOR |
        SDL_TEXTUREMODULATE_ALPHA),
       (SDL_BLENDMODE_NONE | SDL_BLENDMODE_MASK |
        SDL_BLENDMODE_BLEND | SDL_BLENDMODE_ADD | SDL_BLENDMODE_MOD),
       (SDL_TEXTURESCALEMODE_NONE | SDL_TEXTURESCALEMODE_SLOW),
       13,
       {
          SDL_PIXELFORMAT_INDEX8,
          SDL_PIXELFORMAT_RGB555,
          SDL_PIXELFORMAT_RGB565,
          SDL_PIXELFORMAT_RGB888,
          SDL_PIXELFORMAT_BGR888,
          SDL_PIXELFORMAT_ARGB8888,
          SDL_PIXELFORMAT_RGBA8888,
          SDL_PIXELFORMAT_ABGR8888,
          SDL_PIXELFORMAT_BGRA8888,
          SDL_PIXELFORMAT_YV12,
          SDL_PIXELFORMAT_YUY2,
          SDL_PIXELFORMAT_UYVY,
          SDL_PIXELFORMAT_YVYU
       },
       0,
       0
    }
};

static SDL_Renderer* photon_createrenderer(SDL_Window* window, Uint32 flags)
{
   SDL_VideoDisplay* display = SDL_GetDisplayFromWindow(window);
   SDL_DisplayData* didata = (SDL_DisplayData*)display->driverdata;
   SDL_WindowData* wdata = (SDL_WindowData*)window->driverdata;
   SDL_Renderer* renderer = NULL;
   SDL_RenderData* rdata = NULL;

   /* Allocate new renderer structure */
   renderer=(SDL_Renderer*)SDL_calloc(1, sizeof(SDL_Renderer));
   if (renderer==NULL)
   {
      SDL_OutOfMemory();
      return NULL;
   }

   /* Allocate renderer data */
   rdata=(SDL_RenderData*)SDL_calloc(1, sizeof(SDL_RenderData));
   if (rdata==NULL)
   {
      SDL_free(renderer);
      SDL_OutOfMemory();
      return NULL;
   }

   renderer->DisplayModeChanged=photon_displaymodechanged;
   renderer->ActivateRenderer=photon_activaterenderer;
   renderer->CreateTexture=photon_createtexture;
   renderer->QueryTexturePixels=photon_querytexturepixels;
   renderer->SetTexturePalette=photon_settexturepalette;
   renderer->GetTexturePalette=photon_gettexturepalette;
   renderer->SetTextureAlphaMod=photon_settexturealphamod;
   renderer->SetTextureColorMod=photon_settexturecolormod;
   renderer->SetTextureBlendMode=photon_settextureblendmode;
   renderer->SetTextureScaleMode=photon_settexturescalemode;
   renderer->UpdateTexture=photon_updatetexture;
   renderer->LockTexture=photon_locktexture;
   renderer->UnlockTexture=photon_unlocktexture;
   renderer->DirtyTexture=photon_dirtytexture;
   renderer->RenderPoint=photon_renderpoint;
   renderer->RenderLine=photon_renderline;
   renderer->RenderFill=photon_renderfill;
   renderer->RenderCopy=photon_rendercopy;
   renderer->RenderPresent = photon_renderpresent;
   renderer->DestroyTexture = photon_destroytexture;
   renderer->DestroyRenderer = photon_destroyrenderer;
   renderer->info = photon_renderdriver.info;
   renderer->window = window->id;
   renderer->driverdata = rdata;

   /* Set render acceleration flag in case it is accelerated */
   if ((didata->caps & SDL_PHOTON_ACCELERATED)==SDL_PHOTON_ACCELERATED)
   {
      renderer->info.flags=SDL_RENDERER_ACCELERATED;
   }
   else
   {
      renderer->info.flags&=~(SDL_RENDERER_ACCELERATED);
   }

   rdata->window=window;

   /* Check if upper level requested synchronization on vsync signal */
   if ((flags & SDL_RENDERER_PRESENTVSYNC)==SDL_RENDERER_PRESENTVSYNC)
   {
       rdata->enable_vsync=SDL_TRUE;
   }
   else
   {
      rdata->enable_vsync=SDL_FALSE;
   }

   /* Check what buffer copy/flip scheme is requested */
   rdata->surfaces_count=0;
   if ((flags & SDL_RENDERER_SINGLEBUFFER)==SDL_RENDERER_SINGLEBUFFER)
   {
      if ((flags & SDL_RENDERER_PRESENTDISCARD)==SDL_RENDERER_PRESENTDISCARD)
      {
         renderer->info.flags|=SDL_RENDERER_SINGLEBUFFER | SDL_RENDERER_PRESENTDISCARD;
      }
      else
      {
         renderer->info.flags|=SDL_RENDERER_SINGLEBUFFER | SDL_RENDERER_PRESENTCOPY;
      }
      rdata->surfaces_count=1;
      rdata->surface_visible_idx=0;
      rdata->surface_render_idx=0;
   }
   else
   {
      if ((flags & SDL_RENDERER_PRESENTFLIP2)==SDL_RENDERER_PRESENTFLIP2)
      {
         renderer->info.flags|=SDL_RENDERER_PRESENTFLIP2;
         rdata->surfaces_count=2;
         rdata->surface_visible_idx=0;
         rdata->surface_render_idx=1;
      }
      else
      {
         if ((flags & SDL_RENDERER_PRESENTFLIP3)==SDL_RENDERER_PRESENTFLIP3)
         {
            renderer->info.flags|=SDL_RENDERER_PRESENTFLIP3;
            rdata->surfaces_count=3;
            rdata->surface_visible_idx=0;
            rdata->surface_render_idx=1;
         }
         else
         {
            renderer->info.flags|=SDL_RENDERER_SINGLEBUFFER | SDL_RENDERER_PRESENTCOPY;
            rdata->surfaces_count=1;
            rdata->surface_visible_idx=0;
            rdata->surface_render_idx=0;
         }
      }
   }

   return renderer;
}

void photon_addrenderdriver(_THIS)
{
   uint32_t it;

   for (it=0; it<_this->num_displays; it++)
   {
      SDL_AddRenderDriver(it, &photon_renderdriver);
   }
}

/****************************************************************************/
/* SDL render interface                                                     */
/****************************************************************************/
static int photon_displaymodechanged(SDL_Renderer* renderer)
{
   SDL_RenderData* rdata=(SDL_RenderData*)renderer->driverdata;

   /* Remove all allocated surfaces, they are no more valid */

   /* TODO: Add video mode change detection and new parameters detection */

   return 0;
}

static int photon_activaterenderer(SDL_Renderer* renderer)
{
   SDL_RenderData* rdata=(SDL_RenderData*)renderer->driverdata;
   SDL_VideoDisplay* display = SDL_GetDisplayFromWindow(rdata->window);
   SDL_DisplayData* didata = (SDL_DisplayData*)display->driverdata;

   return 0;
}

static int photon_createtexture(SDL_Renderer* renderer, SDL_Texture* texture)
{
   SDL_RenderData* renderdata=(SDL_RenderData*)renderer->driverdata;
   SDL_Window* window=SDL_GetWindowFromID(renderer->window);
   SDL_VideoDisplay* display=SDL_GetDisplayFromWindow(window);
   SDL_TextureData* tdata=NULL;

   /* Allocate texture driver data */
   tdata=(SDL_TextureData*)SDL_calloc(1, sizeof(SDL_TextureData));
   if (tdata==NULL)
   {
      SDL_OutOfMemory();
      return -1;
   }

   /* Set texture driver data */
   texture->driverdata=tdata;

}

static int photon_querytexturepixels(SDL_Renderer* renderer, SDL_Texture* texture, void** pixels, int* pitch)
{
}

static int photon_settexturepalette(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Color* colors, int firstcolor, int ncolors)
{
}

static int photon_gettexturepalette(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Color* colors, int firstcolor, int ncolors)
{
}

static int photon_settexturecolormod(SDL_Renderer* renderer, SDL_Texture* texture)
{
}

static int photon_settexturealphamod(SDL_Renderer* renderer, SDL_Texture* texture)
{
}

static int photon_settextureblendmode(SDL_Renderer* renderer, SDL_Texture* texture)
{
}

static int photon_settexturescalemode(SDL_Renderer* renderer, SDL_Texture* texture)
{
}

static int photon_updatetexture(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* rect, const void* pixels, int pitch)
{
}

static int photon_locktexture(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* rect, int markDirty, void** pixels, int* pitch)
{
}

static void photon_unlocktexture(SDL_Renderer* renderer, SDL_Texture* texture)
{
}

static void photon_dirtytexture(SDL_Renderer* renderer, SDL_Texture* texture, int numrects, const SDL_Rect* rects)
{
}

static int photon_renderpoint(SDL_Renderer* renderer, int x, int y)
{
}

static int photon_renderline(SDL_Renderer* renderer, int x1, int y1, int x2, int y2)
{
}

static int photon_renderfill(SDL_Renderer* renderer, const SDL_Rect* rect)
{
}

static int photon_rendercopy(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect* srcrect, const SDL_Rect* dstrect)
{
}

static void photon_renderpresent(SDL_Renderer* renderer)
{
}

static void photon_destroytexture(SDL_Renderer* renderer, SDL_Texture* texture)
{
}

static void photon_destroyrenderer(SDL_Renderer* renderer)
{
   SDL_RenderData* rdata=(SDL_RenderData*)renderer->driverdata;
   uint32_t it;

}

/* vi: set ts=4 sw=4 expandtab: */
