#ifndef THEME_RESOURCES_H__
#define THEME_RESOURCES_H__

#include <stdbool.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "utils/log.h"
#include "./config.h"

#define RES_MAX_REQUESTS 200

typedef enum theme_images
{
    NULL_IMAGE,
    BG_TITLE,
    LOGO,
    BATTERY_0,
    BATTERY_20,
    BATTERY_50,
    BATTERY_80,
    BATTERY_100,
    BATTERY_CHARGING,
    BG_LIST_S,
    BG_LIST_L,
    HORIZONTAL_DIVIDER,
    TOGGLE_ON,
    TOGGLE_OFF,
    BG_FOOTER,
    BUTTON_A,
    BUTTON_B,
    LEFT_ARROW,
    RIGHT_ARROW,
    images_count
} ThemeImages;

typedef enum theme_fonts
{
    NULL_FONT,
    TITLE,
    HINT,
    GRID1x4,
    GRID3x4,
    LIST,
    BATTERY,
    fonts_count
} ThemeFonts;

typedef struct Theme_Resources
{
    Theme_s theme;
    bool _theme_loaded;
    SDL_Surface* surfaces[(int)images_count];
    TTF_Font* fonts[(int)fonts_count];
} Resources_s;

static Resources_s resources = { ._theme_loaded = false };

Theme_s* theme(void)
{
    if (!resources._theme_loaded) {
        resources.theme = theme_load();
        resources._theme_loaded = true;
    }
    return &resources.theme;
}

SDL_Surface* _loadImage(ThemeImages request)
{
    Theme_s *t = theme();
    int real_location, backup_location;

    switch (request) {
        case BG_TITLE: return theme_loadImage(t->path, "bg-title");
        case LOGO: return theme_loadImage(t->path, "miyoo-topbar");
        case BATTERY_0: return theme_loadImage(t->path, "power-0%-icon");
        case BATTERY_20: return theme_loadImage(t->path, "power-20%-icon");
        case BATTERY_50: return theme_loadImage(t->path, "power-50%-icon");
        case BATTERY_80: return theme_loadImage(t->path, "power-80%-icon");
        case BATTERY_100:
            real_location = theme_getImagePath(t->path, "power-full-icon", NULL);
            backup_location = theme_getImagePath(t->path, "power-full-icon_back", NULL);
            return theme_loadImage(t->path, real_location == backup_location ? "power-full-icon_back" : "power-full-icon");
        case BATTERY_CHARGING: return theme_loadImage(t->path, "ic-power-charge-100%");
        case BG_LIST_S: return theme_loadImage(t->path, "bg-list-s");
        case BG_LIST_L: return theme_loadImage(t->path, "bg-list-l");
        case HORIZONTAL_DIVIDER: return theme_loadImage(t->path, "div-line-h");
        case TOGGLE_ON: return theme_loadImage(t->path, "extra/toggle-on");
        case TOGGLE_OFF: return theme_loadImage(t->path, "extra/toggle-off");
        case BG_FOOTER: return theme_loadImage(t->path, "tips-bar-bg");
        case BUTTON_A: return theme_loadImage(t->path, "icon-A-54");
        case BUTTON_B: return theme_loadImage(t->path, "icon-B-54");
        case LEFT_ARROW: return theme_loadImage(t->path, "icon-left-arrow-24");
        case RIGHT_ARROW: return theme_loadImage(t->path, "icon-right-arrow-24");
        default: break;
    }
    return NULL;
}

TTF_Font* _loadFont(ThemeFonts request)
{
    Theme_s *t = theme();
    TTF_Font *font = NULL;

    switch (request) {
        case TITLE: return theme_loadFont(t->path, t->title.font, t->title.size);
        case HINT: return theme_loadFont(t->path, t->hint.font, t->hint.size);
        case GRID1x4: return theme_loadFont(t->path, t->grid.font, t->grid.grid1x4);
        case GRID3x4: return theme_loadFont(t->path, t->grid.font, t->grid.grid3x4);
        case LIST:
            font = theme_loadFont(t->path, t->list.font, t->list.size);
            TTF_SetFontStyle(font, TTF_STYLE_BOLD);
            return font;
        case BATTERY: return theme_loadFont(t->path, t->batteryPercentage.font, t->batteryPercentage.size);
        default: break;
    }

    return NULL;
}

SDL_Surface* resource_getSurface(ThemeImages request)
{
    if (resources.surfaces[request] == NULL)
        resources.surfaces[request] = _loadImage(request);
    return resources.surfaces[request];
}

TTF_Font* resource_getFont(ThemeFonts request)
{
    if (resources.fonts[request] == NULL)
        resources.fonts[request] = _loadFont(request);
    return resources.fonts[request];
}

void resources_free()
{
    for (int i = 0; i < images_count; i++)
        if (resources.surfaces[i] != NULL)
            SDL_FreeSurface(resources.surfaces[i]);
    
    for (int i = 0; i < fonts_count; i++)
        if (resources.fonts[i] != NULL)
            TTF_CloseFont(resources.fonts[i]);
}

#endif // THEME_RESOURCES_H__
