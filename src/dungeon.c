/**********************************************************************************************
 *
 **   dungeon.c is responsible for dealing and assemblying all functionalities related to the
 **   dungeon game screen.
 *
 *    @authors Marcus Vinicius Santos Lages, Samarjit Bhogal
 *    @version 0.1
 *
 *    @include screen.h, player.h
 *
 **********************************************************************************************/

#include "../include/enemy.h"
#include "../include/external/tmx.h"
#include "../include/player.h"
#include "../include/screen.h"

//* ------------------------------------------
//* GLOBAL VARIABLES

/** 2D array of type Tile for the world level (texture.h) */
Tile** world;

/** Framebuffer (white canvas) for displaying the map. */
RenderTexture2D worldCanvas;

/** Linked list with indexes to all the possible collidable tiles. */
CollisionNode* collidableTiles;

/** A reference to the game's tilemap (texture.h) */
Texture2D* textures;

//* ------------------------------------------
//* FUNCTION PROTOTYPES

/**
 * Starts the 2D camera by initializing it.
 */
static void StartCamera();

/**
 * Loads all of the textures required for the dungeon by populating the array of textures.
 */
static void LoadTextures();

/**
 * Allocates all the tiles into the world array and the collidableTiles list.
 */
static void InitializeTiles();

// TODO: Move tmx functions to a header
/**
 * Setups a framebuffer (white canvas) according to a tmx map dimensions.
 *
 * @attention Framebuffer needs to be freed later.
 *
 * @param framebuffer Pointer to the framebuffer that will be initialized
 * @param mapFileName Name of the mapFile in .tmx format
 */
void TmxMapFrameBufStartup(RenderTexture2D* framebuffer, char* mapFileName);

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

unsigned int GetTileGID(tmx_layer* layer, unsigned int mapWidth, int x, int y);

void DrawTmxTile(tmx_tile* tile, int tileX, int tileY);

Texture2D *LoadMapTexture(const char *fileName);

void UnloadMapTexture(Texture2D *tex);

//* ------------------------------------------
//* FUNCTION IMPLEMENTATIONS

void DungeonStartup() {
    // Sets the current screen
    currentScreen = DUNGEON;

    // Allocate memory for the game's textures as Texture2D
    textures = (Texture2D*) malloc(MAX_TEXTURES * sizeof(Texture2D));

    // Allocate memory for the word as a 2D array of Tile
    world = (Tile**) malloc(WORLD_WIDTH * sizeof(Tile*));

    // Assign initial null value value for the collidableTiles list
    collidableTiles = NULL;

    // Populating textures array with the texture images
    LoadTextures();

    // Allocating tiles of type Tile into 2D array
    InitializeTiles();

    StartCamera();
    EnemyStartup();
    PlayerStartup();
}

void DungeonUpdate() {
    PlayerMovement();
    EnemyMovement();
    EnemyAttack();
    PlayerAttack();

    // Update camera to follow the player
    camera.target = (Vector2){ (int) player.pos.x + 8, (int) player.pos.y + 16 };
}

void DungeonRender() {
    // Tile tile;

    // // Rendering each tile by calling DrawTile
    // for(int j = 0; j < WORLD_HEIGHT; j++) {
    //     for(int i = 0; i < WORLD_WIDTH; i++) {
    //         tile = world[j][i];
    //         if(j != 3)
    //             DrawTile(tile.x * TILE_WIDTH, tile.y * TILE_HEIGHT, 7, 2, TILE_MAP);
    //         else
    //             DrawTile(tile.x * TILE_WIDTH, tile.y * TILE_HEIGHT, 4, 2, TILE_MAP);
    //     }
    // }

    DrawTextureRec(worldCanvas.texture,
        (Rectangle) {0, 0, worldCanvas.texture.width, -worldCanvas.texture.height},
        (Vector2) {0.0f, 0.0f},
        WHITE);

    // Draw player on the screen
    EnemyRender();
    PlayerRender();
}

void DungeonUnload() {
    // Unloads the player sprites and animations.
    PlayerUnload();

    // Unloads the enemy sprites and animations.
    EnemyUnload();

    // Unloads all tiles
    for(int j = 0; j < WORLD_HEIGHT; j++) {
        free(world[j]);
        world[j] = NULL;
    }
    free(world);
    world = NULL;

    // Unloads collidableTiles list
    FreeCollisionList(collidableTiles);
    collidableTiles = NULL;

    // Unloads texture array
    for(int i = 0; i < MAX_TEXTURES; i++) {
        UnloadTexture(textures[i]);
    }
    free(textures);
    textures = NULL;

    UnloadRenderTexture(worldCanvas);
}

static void StartCamera() {
    camera.target   = (Vector2){ player.pos.x + 8, player.pos.y + 16 };
    camera.offset   = (Vector2){ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
    camera.rotation = 0.0f;
    camera.zoom     = 4.0f;
}

static void LoadTextures() {
    Image img          = LoadImage("resources/tilemap.png");
    textures[TILE_MAP] = LoadTextureFromImage(img);
    UnloadImage(img);

    img                        = LoadImage("resources/player-idle.png");
    textures[TILE_PLAYER_IDLE] = LoadTextureFromImage(img);
    UnloadImage(img);

    img                        = LoadImage("resources/player-movement.png");
    textures[TILE_PLAYER_MOVE] = LoadTextureFromImage(img);
    UnloadImage(img);

    img                          = LoadImage("resources/player-attack.png");
    textures[TILE_PLAYER_ATTACK] = LoadTextureFromImage(img);
    UnloadImage(img);

    img                       = LoadImage("resources/enemy-1-idle.png");
    textures[TILE_ENEMY_IDLE] = LoadTextureFromImage(img);
    UnloadImage(img);

    img                       = LoadImage("resources/enemy-1-movement.png");
    textures[TILE_ENEMY_MOVE] = LoadTextureFromImage(img);
    UnloadImage(img);

    //! NOTE: player attack is used here temporailly. New asset must be found.
    img                         = LoadImage("resources/player-attack.png");
    textures[TILE_ENEMY_ATTACK] = LoadTextureFromImage(img);
    UnloadImage(img);
}

static void InitializeTiles() {
    // TODO: Add .tmx map
    //  OBJ: Function that renders tmx map to framebuffer
    //  OBS: WORLD tile won't be used anymore, but collisionTiles algorithm needs
    //       to be included in tmx display
    //  OBS: Frame buffer is used so once the canvas is created once, u don't need
    //       to create it again
    //  1. Create RenderTexture2D framebuffer
    TmxMapFrameBufStartup(&worldCanvas, "resources/map/map.tmx");
    // TODO: Add TmxMapFrameBufRender to here instead

    // 2. Load the tmx map using the function: tmx_map *tmx_load(".tmx")
    // 2.5. Check for errors
    // 3. Start a framebuffer with the mapTmx width and height
    // 4. Begin/EndTextureMode(framebuffer)
    // 5. while(mapTmx.layer) check if layer is visible
    // 6. Check for the layer type (L_LAYER, etc.) and render it accordingly
    // 7. DrawTmxLayer
    // 8. tmx_map__free(mapTmx)
    for(int j = 0; j < WORLD_HEIGHT; j++) {
        Tile* tiles = (Tile*) malloc(WORLD_HEIGHT * sizeof(Tile));
        for(int i = 0; i < WORLD_WIDTH; i++) {
            Tile tile = { .x = i, .y = j, .isCollidable = (j != 3) ? false : true };
            tiles[i] = tile;

            // if(tile.isCollidable == true) {
            //     if(collidableTiles == NULL)
            //         collidableTiles = CreateCollisionList(tile.x, tile.y, 0);
            //     else
            //         AddCollisionNode(collidableTiles, tile.x, tile.y, 0);
            // }
        }
        world[j] = tiles;
    }
}

void TmxMapFrameBufStartup(RenderTexture2D* framebuffer, char* mapFileName) {
    // TODO: Check if function pointers for the tmx maps libs are necessary
    // Unload function pointers
    tmx_img_load_func = (void *(*)(const char *))LoadMapTexture;
    tmx_img_free_func = (void (*)(void *))UnloadMapTexture;

    // Load the map into a tmx_map struct
    tmx_map* mapTmx = tmx_load(mapFileName);
    // If the map is not found
    if(mapTmx == NULL) {
        tmx_perror("tmx_load");
        return;
    }

    // Start the framebuffer with the size of the tilemap * size of the tile
    *framebuffer = LoadRenderTexture(
        mapTmx->width * mapTmx->tile_width, mapTmx->height * mapTmx->tile_height);

    TmxMapFrameBufRender(framebuffer, mapTmx);
}

void TmxMapFrameBufRender(RenderTexture2D* framebuffer, tmx_map* map) {
    if(framebuffer->depth.height < map->height * map->tile_height ||
       framebuffer->depth.width < map->width * map->tile_width) {
        tmx_perror("Framebuffer insuficcient");
        return;
    }

    // Array with all the layers in the tilemap
    tmx_layer* layer = NULL;

    BeginTextureMode(*framebuffer);
        // TODO: Use tmx_map background color
        ClearBackground(BLACK);
        
        layer = map->ly_head;
        while(layer) {
            if(layer->visible) {
                switch(layer->type) {
                // Checks if layer is visible and it's a tilemap layer
                case L_LAYER:
                    //TODO: Use tracelog with layer->name
                    DrawTmxLayer(map, layer);
                    break;
                // Ignores all other layers.
                default:
                    tmx_perror("Non tilemap layer found.");
                    break;
                }
            }
            layer = layer->next;
        }

    EndTextureMode();

    tmx_map_free(map);
}

void DrawTmxLayer(tmx_map* map, tmx_layer* layer) {
    for(int row = 0; row < map->height; row++) {
        for(int col = 0; col < map->width; col++) {
            // Get the tile GID through an array formula
            unsigned int tileGID = GetTileGID(layer, map->width, col, row);

            if(tileGID != 0) {
                // Get the specific tile from its GID
                tmx_tile* tile = NULL;
                tile = map->tiles[tileGID];

                if(tile != NULL) {
                    tmx_property* collisionProp = tmx_get_property(tile->properties, "isCollidable");
                    bool isCollidable = collisionProp->value.boolean;

                    if(isCollidable) {
                        if(collidableTiles == NULL)
                            collidableTiles = CreateCollisionList(col, row, 0);
                        else
                            AddCollisionNode(collidableTiles, col, row, 0);
                    }

                    DrawTmxTile(tile, col, row);
                }
            }
        }
    }
}

unsigned int GetTileGID(tmx_layer* layer, unsigned int mapWidth, int x, int y) {
    // Bit separation operators for separating gid and flip (unused)
    unsigned int GID = (layer->content.gids[y * mapWidth + x]);
    GID = GID & TMX_FLIP_BITS_REMOVAL;
    return GID;
}

Texture2D *LoadMapTexture(const char *fileName)
{
    Texture2D *texture = (Texture2D *) malloc(sizeof(Texture2D));
    if (texture != NULL) 
    {
        *texture = LoadTexture(fileName);
        TraceLog(LOG_INFO, "TMX texture loaded from %s", fileName);
        return texture;
    }
    return NULL;
}

void UnloadMapTexture(Texture2D *texture)
{
    if (texture != NULL) 
    {
        UnloadTexture(*texture);
        free(texture);
    }
}

void DrawTmxTile(tmx_tile* tile, int tileX, int tileY) {
    // DrawTexturePro information
    Texture2D* tileTexture = NULL;
    Rectangle sourceRect;
    Rectangle destRect;
    Vector2 origin = Vector2Zero();
    float rotation = 0.0f;

    int tile_width = 0;
    int tile_height = 0;

    if(tile->image != NULL) {
        // Tile has its own image, so everything is based of this texture
        tileTexture = (Texture2D *) tile->image->resource_image;
        tile_width = tile->image->width;
        tile_height = tile->image->height;
        // sourceRect.x = 0.0f;
        // sourceRect.y = 0.0f;

    } else {
        tileTexture = (Texture2D *) tile->tileset->image->resource_image;
        tile_width = tile->tileset->tile_width;
        tile_height = tile->tileset->tile_height;
    }

    sourceRect.x = tile->ul_x;
    sourceRect.y = tile->ul_y;
    sourceRect.width = destRect.width = tile_width;
    sourceRect.height = destRect.height = tile_height;
    destRect.x = tileX * tile_width;
    destRect.y = tileY * tile_height;

    DrawTexturePro(*tileTexture, sourceRect, destRect, origin, rotation, WHITE);
}

void DrawTile(int xPos, int yPos, int textureTileX, int textureTileY, TextureFile tileTexture) {
    Rectangle source = { (float) (textureTileX * TILE_WIDTH),
                         (float) (textureTileY * TILE_HEIGHT), (float) TILE_WIDTH,
                         (float) (tileTexture > TILE_MAP ? ENTITY_TILE_HEIGHT : TILE_HEIGHT) };
    Rectangle dest   = { (float) xPos, (float) yPos, (float) TILE_WIDTH,
                         (float) (tileTexture > TILE_MAP ? ENTITY_TILE_HEIGHT : TILE_HEIGHT) };
    Vector2 origin   = { 0, 0 };
    DrawTexturePro(textures[tileTexture], source, dest, origin, 0.0f, WHITE);
}