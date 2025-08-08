#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>

#include "asset_cache.h"
#include "list.h"
#include "sdl_wrapper.h"

static list_t *ASSET_CACHE;

const size_t ASSET_CACHE_FONT_SIZE = 18;
const size_t INITIAL_CAPACITY = 5;

typedef struct {
  asset_type_t type;
  const char *filepath;
  void *obj;
} entry_t;

static void asset_cache_free_entry(entry_t *entry) {
  asset_type_t type = entry->type;

  if (type == ASSET_IMAGE) {
    SDL_DestroyTexture((SDL_Texture *)entry->obj);
  } else {
    TTF_CloseFont((TTF_Font *)entry->obj);
  }
  free(entry);
}

void asset_cache_init() {
  ASSET_CACHE =
      list_init(INITIAL_CAPACITY, (free_func_t)asset_cache_free_entry);
}

void asset_cache_destroy() { list_free(ASSET_CACHE); }

entry_t *check_entry(const char *filepath) {
  for (size_t i = 0; i < list_size(ASSET_CACHE); ++i) {
    entry_t *entry = list_get(ASSET_CACHE, i);
    if (strcmp(entry->filepath, filepath) == 0) {
      return entry;
    }
  }
  return NULL;
}

void *asset_cache_obj_get_or_create(asset_type_t ty, const char *filepath) {

  entry_t *entry = check_entry(filepath);
  if (entry) {
    assert(ty == entry->type);
    return entry->obj;
  }

  entry_t *new_entry = malloc(sizeof(entry_t));
  new_entry->type = ty;
  new_entry->filepath = filepath;

  void *obj;
  if (ty == ASSET_IMAGE) {
    obj = sdl_get_image_texture(filepath);
  } else {
    obj = TTF_OpenFont(filepath, ASSET_CACHE_FONT_SIZE);
  }
  new_entry->obj = obj;
  list_add(ASSET_CACHE, new_entry);
  return new_entry->obj;
}
