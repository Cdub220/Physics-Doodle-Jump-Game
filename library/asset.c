#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>

#include "asset.h"
#include "asset_cache.h"
#include "color.h"
#include "sdl_wrapper.h"

static list_t *ASSET_LIST = NULL;
const size_t INIT_CAPACITY = 5;

typedef struct asset {
  asset_type_t type;
  SDL_Rect bounding_box;
} asset_t;

typedef struct text_asset {
  asset_t base;
  TTF_Font *font;
  const char *text;
  color_t color;
} text_asset_t;

typedef struct image_asset {
  asset_t base;
  SDL_Texture *texture;
  body_t *body;
} image_asset_t;

/**
 * Allocates memory for an asset with the given parameters.
 *
 * @param ty the type of the asset
 * @param bounding_box the bounding box containing the location and dimensions
 * of the asset when it is rendered
 * @return a pointer to the newly allocated asset
 */
static asset_t *asset_init(asset_type_t ty, SDL_Rect bounding_box) {
  // This is a fancy way of malloc'ing space for an `image_asset_t` if `ty` is
  // ASSET_IMAGE, and `text_asset_t` otherwise.
  if (ASSET_LIST == NULL) {
    ASSET_LIST = list_init(INIT_CAPACITY, (free_func_t)asset_destroy);
  }
  asset_t *new =
      malloc(ty == ASSET_IMAGE ? sizeof(image_asset_t) : sizeof(text_asset_t));
  assert(new);
  new->type = ty;
  new->bounding_box = bounding_box;
  return new;
}

void asset_make_image_with_body(const char *filepath, body_t *body) {
  image_asset_t *img = malloc(sizeof(image_asset_t));
  img->base = *asset_init(ASSET_IMAGE, (SDL_Rect){0, 0, 0, 0});
  img->texture =
      (SDL_Texture *)asset_cache_obj_get_or_create(ASSET_IMAGE, filepath);
  img->body = body;
  list_add(ASSET_LIST, img);
}

void asset_make_image(const char *filepath, SDL_Rect bounding_box) {
  image_asset_t *img = malloc(sizeof(image_asset_t));
  img->base = *asset_init(ASSET_IMAGE, bounding_box);
  img->texture =
      (SDL_Texture *)asset_cache_obj_get_or_create(ASSET_IMAGE, filepath);
  img->body = NULL;
  list_add(ASSET_LIST, img);
}

void asset_make_text(const char *filepath, SDL_Rect bounding_box,
                     const char *text, color_t color) {
  text_asset_t *text_asset = malloc(sizeof(text_asset_t));
  text_asset->base = *asset_init(ASSET_TEXT, bounding_box);
  text_asset->font =
      (TTF_Font *)asset_cache_obj_get_or_create(ASSET_TEXT, filepath);
  text_asset->text = text;
  text_asset->color = color;
  list_add(ASSET_LIST, text_asset);
}

void asset_reset_asset_list() {
  if (ASSET_LIST != NULL) {
    list_free(ASSET_LIST);
  }
  ASSET_LIST = list_init(INIT_CAPACITY, (free_func_t)asset_destroy);
}

list_t *asset_get_asset_list() { return ASSET_LIST; }

void asset_remove_body(body_t *body) {
  ssize_t size = list_size(ASSET_LIST);
  for (ssize_t i = 0; i < size; i++) {
    asset_t *asset = list_get(ASSET_LIST, i);
    if (asset->type == ASSET_IMAGE) {
      image_asset_t *image_asset = (image_asset_t *)asset;
      if (image_asset->body == body) {
        list_remove(ASSET_LIST, i);
        asset_destroy(asset);
        size--;
        i--;
      }
    }
  }
}

void asset_render(asset_t *asset) {
  asset_type_t type = asset->type;

  if (type == ASSET_IMAGE) {
    image_asset_t *img = (image_asset_t *)asset;
    if (img->body) {
      SDL_Rect bounding_box = sdl_get_body_bounding_box(img->body);
      sdl_render_image(img->texture, &bounding_box);
    } else {
      sdl_render_image(img->texture, &asset->bounding_box);
    }
  } else {
    text_asset_t *text = (text_asset_t *)asset;
    SDL_Color color = (SDL_Color){
        text->color.red * 255, text->color.green * 255, text->color.blue * 255};
    SDL_Surface *message = TTF_RenderText_Solid(text->font, text->text, color);
    sdl_render_text(message, &asset->bounding_box);
  }
}

void asset_destroy(asset_t *asset) { free(asset); }