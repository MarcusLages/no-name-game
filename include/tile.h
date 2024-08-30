/**********************************************************************************************
*
**   tile.h is responsible for dealing with external tile and tilemap rendering.
*   
*    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
*    @version 0.3
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

#endif // TILE_H_