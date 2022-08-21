#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "theme/config.h"
#include "theme/resources.h"

int main(int argc, char *argv[])
{
    // Boot : Loading screen
    // End_Save : Ending screen with save
    // End : Ending screen without save

    SDL_Init(SDL_INIT_VIDEO);

    char theme_path[STR_MAX];
	theme_getPath(theme_path);

    SDL_Surface* video = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_Surface* screen = SDL_CreateRGBSurface(SDL_HWSURFACE, 640, 480, 32, 0, 0, 0, 0);

    SDL_Surface* background;

    if (argc > 1 && strcmp(argv[1], "End_Save") == 0) {
        background = theme_loadImage(theme_path, "extra/Screen_Off_Save");
    }
    else if (argc > 1 && strcmp(argv[1], "End") == 0) {
        background = theme_loadImage(theme_path, "extra/Screen_Off");
    }
    else {
        background = theme_loadImage(theme_path, "extra/bootScreen");
    }

    SDL_BlitSurface(background, NULL, screen, NULL);

    // Blit twice, to clear the video buffer
    SDL_BlitSurface(screen, NULL, video, NULL);
    SDL_Flip(video);
    SDL_BlitSurface(screen, NULL, video, NULL);
    SDL_Flip(video);

    if (argc > 1 && strcmp(argv[1], "Boot") != 0)
        temp_flag_set(".offOrder", false);

    #ifndef PLATFORM_MIYOOMINI
    sleep(4); // for debugging purposes
    #endif

    SDL_FreeSurface(background);
    SDL_FreeSurface(screen);
    SDL_FreeSurface(video);
    SDL_Quit();


    return EXIT_SUCCESS;
}
