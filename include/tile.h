/**********************************************************************************************
*
**   tile.h is responsible for dealing with external tile and tilemap rendering.
*   
*    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
*    @version 0.1
*
*    @include raylib.h, tmx.h
*    @cite raylib, tmx
*
**********************************************************************************************/
#ifndef TILE_H_
#define TILE_H_

#include "raylib.h"
#include "./external/tmx.h"

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Setups a framebuffer (white canvas) according to a tmx map dimensions.
 *
 * @attention Framebuffer needs to be freed later.
 *
 * @param framebuffer Pointer to the framebuffer that will be initialized
 * @param mapFileName Name of the mapFile in .tmx format
 * 
 * @return Pointer to tmx_map structure generated from mapFileName.tmx
 */
tmx_map* TmxMapFrameBufStartup(RenderTexture2D* framebuffer, char* mapFileName);

/**
 * Renders a map from a .tmx file into a framebuffer (white canvas).
 *
 * @attention Framebuffer needs to be freed later.
 *
 * @param framebuffer Pointer to the framebuffer in which the map will be rendered into
 * @param map         Pointer to the tmx_map that will be rendered
 */
void TmxMapFrameBufRender(RenderTexture2D* framebuffer, tmx_map* map);

/**
 * Renders a tmx_map layer into a RenderTexture2D or to the screen.
 *
 * @note Uses Raylib's DrawTexturePro, so it's necessary to be done inside a Drawing/Texture mode.
 */
void DrawTmxLayer(tmx_map* map, tmx_layer* layer);

/**
 * Renders a tmx_tile into a RenderTexture2D or to the screen in a specific (x, y) coordinate.
 *
 * @note - The (x, y) coordinate is based on the tile, not on the pixels.
 * @note - Uses Raylib's DrawTexturePro, so it's necessary to be done inside a Drawing/Texture mode.
 */
void DrawTmxTile(tmx_tile* tile, int tileX, int tileY);

#endif // TILE_H_