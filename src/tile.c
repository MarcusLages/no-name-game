/**********************************************************************************************
 *
 **   tile.c is responsible for dealing with tile and tilemap rendering.
 *
 *    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
 *    @version 0.3
 *
 *    @include <stdlib.h>, tile.h, collision.h, spawner.h, texture.h
 *
 **********************************************************************************************/
#include "../include/tile.h"
#include "../include/collision.h"
#include "../include/spawner.h"
#include "../include/texture.h"
#include <stdlib.h>

//* ------------------------------------------
//* FUNCTION PROTOTYPES

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

/**
 * Loads a Texture2D for a .tmx map when needed.
 *
 * @param fileName Name of the tileset image
 * @return Pointer to the loaded texture
 *
 * @note Used by the tmx_load() function when loading textures.
 */
Texture2D* LoadMapTexture(const char* fileName);

/**
 * Unloads a Texture2D for a .tmx map when needed.
 *
 * @param texture Pointer to the texture
 *
 * @note Used by the tmx_load() function when loading textures.
 */
void UnloadMapTexture(Texture2D* texture);

/**
 * Function to return the GID of the tile that needs to displayed at a specific
 * coordinate of a tmx_layer.
 *
 * @param layer Pointer to the tmx_layer
 * @param mapWidth Width of the map which the layer comes from
 * @param x Horizontal (x) coordinate
 * @param y Vertical (y) coordinate
 * @return GID of the tile at that coordinate
 */
static unsigned int GetTileGID(tmx_layer* layer, unsigned int mapWidth, int x, int y);

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

tmx_map* TmxMapFrameBufStartup(RenderTexture2D* framebuffer, char* mapFileName) {
    rooms = NULL;

    // Unload function pointers
    tmx_img_load_func = (void* (*) (const char*) ) LoadMapTexture;
    tmx_img_free_func = (void (*)(void*)) UnloadMapTexture;

    // Load the map into a tmx_map struct
    tmx_map* mapTmx = tmx_load(mapFileName);
    // If the map is not found
    if(mapTmx == NULL) {
        tmx_perror("tmx_load");
        return NULL;
    }

    // Start the framebuffer with the size of the tilemap * size of the tile
    *framebuffer = LoadRenderTexture(
        mapTmx->width * mapTmx->tile_width, mapTmx->height * mapTmx->tile_height);


    TraceLog(LOG_INFO, "TILE.C (TmxMapFrameBufStartup): Tmx map loaded.");

    return mapTmx;
}

void TmxMapFrameBufRender(RenderTexture2D* framebuffer, tmx_map* map) {
    // Checks if the framebuffer is big enough to store the whole map
    if(framebuffer->depth.height < map->height * map->tile_height ||
       framebuffer->depth.width < map->width * map->tile_width) {
        tmx_perror("Framebuffer insuficcient");
        return;
    }

    // Linked list with all the layers in the tilemap
    tmx_layer* layer = NULL;

    // Start drawing at the framebuffer
    BeginTextureMode(*framebuffer);
    ClearBackground(BLACK);

    // Loop through the layer list to draw every layer
    layer = map->ly_head;
    while(layer) {
        if(layer->visible) {
            switch(layer->type) {
                // Checks if layer is visible and it's a tilemap layer
                case L_LAYER:
                    //  Draws the layer
                    DrawTmxLayer(map, layer);
                    break;
                // Ignores all other layers.
                default: tmx_perror("Non tilemap layer or error found."); break;
            }
        }
        layer = layer->next;
    }

    EndTextureMode();

    TraceLog(LOG_INFO, "TILE.C (TmxMapFrameBufRender): Texture rendered from tmx map.");
}

void DrawTmxLayer(tmx_map* map, tmx_layer* layer) {
    // Loops through all the tiles on the map to render them
    for(int row = 0; row < map->height; row++) {
        for(int col = 0; col < map->width; col++) {
            // Get the tile GID through an array formula
            unsigned int tileGID = GetTileGID(layer, map->width, col, row);

            // Checks if tile has to be displayed
            if(tileGID != 0) {
                // Get the specific tile from its GID
                tmx_tile* tile = NULL;
                tile           = map->tiles[tileGID];

                if(tile != NULL) {
                    // Gets the collision property from the tile properties
                    tmx_property* collisionProp =
                        tmx_get_property(tile->properties, "isCollidable");

                    if(collisionProp != NULL) {
                        bool isCollidable = collisionProp->value.boolean;

                        // If the tile is collidable adds it to the collidable tiles collision list
                        if(isCollidable) {
                            if(collidableTiles == NULL)
                                collidableTiles = CreateCollisionList(col, row, 0);
                            else
                                AddCollisionNode(collidableTiles, col, row, 0);
                        }
                    }

                    // Gets the room properties from the tile
                    tmx_property* roomNumberProp =
                        tmx_get_property(tile->properties, "roomNumber");
                    tmx_property* roomSizeProp =
                        tmx_get_property(tile->properties, "roomSize");

                    // If the properties exist on the tile we create/add rooms
                    if(roomNumberProp != NULL && roomSizeProp != NULL) {
                        int roomNumber    = roomNumberProp->value.integer;
                        RoomSize roomSize = roomSizeProp->value.integer;

                        if(rooms == NULL) {
                            rooms = CreateRoomList((Vector2){ col, row }, roomNumber, roomSize);
                        } else {
                            if(!CheckRoomExists(roomNumber)) {
                                AddRoomNode((Vector2){ col, row }, roomNumber, roomSize);
                            } else {
                                // This room already exists so we add a position to it.
                                AddPositionToRoom(roomNumber, (Vector2){ col, row });
                            }
                        }
                    }

                    // Draws the tile on the specific column and row
                    DrawTmxTile(tile, col, row);
                }
            }
        }
    }

    TraceLog(LOG_INFO, "TILE.C (DrawTmxLayer): Collidable tiles list created successfully.");
    TraceLog(
        LOG_INFO, "TILE.C (DrawTmxLayer): Layer (%s) rendered successfully.",
        layer->name);
}

static unsigned int GetTileGID(tmx_layer* layer, unsigned int mapWidth, int x, int y) {
    // Get the full gid + flip bits from the gids array of the layer
    unsigned int GID = (layer->content.gids[y * mapWidth + x]);
    // Bit separation operators for separating gid and flip (unused)
    GID = GID & TMX_FLIP_BITS_REMOVAL;
    return GID;
}

Texture2D* LoadMapTexture(const char* fileName) {
    Texture2D* texture = (Texture2D*) malloc(sizeof(Texture2D));
    if(texture != NULL) {
        *texture = LoadTexture(fileName);
        TraceLog(LOG_INFO, "TILE.C (LoadMapTexture): TMX texture loaded from %s", fileName);
        return texture;
    }
    return NULL;
}

void UnloadMapTexture(Texture2D* texture) {
    if(texture != NULL) {
        UnloadTexture(*texture);
        free(texture);
    }

    TraceLog(
        LOG_INFO, "TILE.C (UnloadMapTexture): TMX texture unloaded. [ID: %d]",
        texture->id);
}

void DrawTmxTile(tmx_tile* tile, int tileX, int tileY) {
    // DrawTexturePro information
    Texture2D* tileTexture = NULL;
    Rectangle sourceRect;
    Rectangle destRect;
    Vector2 origin = Vector2Zero();
    float rotation = 0.0f;

    if(tile->image != NULL)
        // IF tile has its own image, so everything is based of this texture
        tileTexture = (Texture2D*) tile->image->resource_image;
    else
        // Else tile uses the common texture from the tileset/tilemap
        tileTexture = (Texture2D*) tile->tileset->image->resource_image;

    // Creates source and destination rectangles
    sourceRect.x     = tile->ul_x;
    sourceRect.y     = tile->ul_y;
    sourceRect.width = destRect.width = TILE_WIDTH;
    sourceRect.height = destRect.height = TILE_HEIGHT;
    destRect.x                          = tileX * TILE_WIDTH;
    destRect.y                          = tileY * TILE_HEIGHT;

    // Draws on the screen or on the framebuffer
    DrawTexturePro(*tileTexture, sourceRect, destRect, origin, rotation, WHITE);
}