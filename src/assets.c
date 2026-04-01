#include "assets.h"
#include <stdio.h>

/**
 * GLTF/GLB Parsing Note:
 * This implementation relies on Raylib's built-in `LoadModel()`. Raylib internally 
 * uses cgltf to parse `.glb` files. However, the original Rust Toon Dash used 
 * a bespoke parser because Kenney models reference external `colormap.png` files
 * which the Macroquad engine wouldn't automatically resolve. 
 *
 * Raylib usually resolves these fine if the texture file is in the expected path relative 
 * to the model. We can also manually patch models by loading the texture:
 * `Texture2D tex = LoadTexture("assets/textures/colormap_platformer.png");`
 * `model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = tex;`
 *
 * IF you decide you need deeper control to mirror the exact bespoke logic from Rust 
 * (e.g., extracting specific submeshes without external dependencies), you should:
 * 1. Include `cgltf.h` header in this project.
 * 2. Parse the glb bytes: `cgltf_parse_file(&options, path, &data);`
 * 3. `cgltf_load_buffers(&options, data, path);`
 * 4. Iterate `data->meshes` and manual construct Raylib `Mesh` types by mapping 
 *    `data->accessors` (positions, tx coords, colors) to `mesh.vertices`, `mesh.texcoords`, etc.
 * 5. Call `UploadMesh(&mesh, false)` and assign it to a `Model`.
 */

void LoadGameAssets(GameAssets* assets) {
    assets->char_oodi = LoadModel("assets/models/character-oodi.glb");
    assets->char_ooli = LoadModel("assets/models/character-ooli.glb");
    assets->char_oozi = LoadModel("assets/models/character-oozi.glb");
    assets->char_oopi = LoadModel("assets/models/character-oopi.glb");
    assets->char_oobi = LoadModel("assets/models/character-oobi.glb");

    assets->road_straight = LoadModel("assets/models/road-straight.glb");
    assets->fence_low = LoadModel("assets/models/fence-low-straight.glb");
    assets->spike_block = LoadModel("assets/models/spike-block.glb");
    assets->spike_block_wide = LoadModel("assets/models/spike-block-wide.glb");
    assets->trap_spikes_large = LoadModel("assets/models/trap-spikes-large.glb");
    
    assets->coin = LoadModel("assets/models/coin-gold.glb");
    assets->jewel = LoadModel("assets/models/jewel.glb");

    assets->tree = LoadModel("assets/models/decoration/tree.glb");
    assets->tree_pine = LoadModel("assets/models/decoration/tree-pine.glb");
    assets->trunk = LoadModel("assets/models/decoration/tree-pine-small.glb");
    assets->mushroom = LoadModel("assets/models/decoration/mushrooms.glb");
    assets->rocks = LoadModel("assets/models/decoration/rocks.glb");

    // Manually force Kenney textures if they didn't load from GLB definitions natively
    Texture2D atlas = LoadTexture("assets/models/textures/colormap_platformer.png");
    
    // Apply texture to everything
    assets->char_oodi.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = atlas;
    assets->char_ooli.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = atlas;
    assets->char_oozi.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = atlas;
    assets->char_oopi.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = atlas;
    assets->char_oobi.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = atlas;
    assets->road_straight.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = atlas;
    assets->fence_low.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = atlas;
    assets->spike_block.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = atlas;
    assets->spike_block_wide.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = atlas;
    assets->trap_spikes_large.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = atlas;
    assets->coin.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = atlas;
    assets->jewel.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = atlas;
    assets->tree.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = atlas;
    assets->tree_pine.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = atlas;
    assets->trunk.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = atlas;
    assets->mushroom.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = atlas;
    assets->rocks.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = atlas;

    assets->sky_tex = LoadTexture("assets/images/sky.png");
    assets->bg_menu = LoadTexture("assets/images/toon_dash_background.png");
    
    assets->font = LoadFontEx("assets/fonts/LuckiestGuy-Regular.ttf", 64, 0, 0);
    SetTextureFilter(assets->font.texture, TEXTURE_FILTER_BILINEAR);
}

void UnloadGameAssets(GameAssets* assets) {
    UnloadModel(assets->char_oodi);
    UnloadModel(assets->char_ooli);
    UnloadModel(assets->char_oozi);
    UnloadModel(assets->char_oopi);
    UnloadModel(assets->char_oobi);
    
    UnloadModel(assets->road_straight);
    UnloadModel(assets->fence_low);
    UnloadModel(assets->spike_block);
    UnloadModel(assets->spike_block_wide);
    UnloadModel(assets->trap_spikes_large);
    
    UnloadModel(assets->coin);
    UnloadModel(assets->jewel);
    
    UnloadModel(assets->tree);
    UnloadModel(assets->tree_pine);
    UnloadModel(assets->trunk);
    UnloadModel(assets->mushroom);
    UnloadModel(assets->rocks);

    UnloadTexture(assets->sky_tex);
    UnloadTexture(assets->bg_menu);
    UnloadFont(assets->font);
}

Model* GetCharacterModel(GameAssets* assets, CharacterChoice choice) {
    switch (choice) {
        case CHAR_OODI: return &assets->char_oodi;
        case CHAR_OOLI: return &assets->char_ooli;
        case CHAR_OOZI: return &assets->char_oozi;
        case CHAR_OOPI: return &assets->char_oopi;
        case CHAR_OOBI: return &assets->char_oobi;
    }
    return &assets->char_oodi;
}
