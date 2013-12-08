
#include <iostream>
#include <cstring>
#include <SDL.h>
#include <SDL_image.h>

int main(int argc, char *argv[])
{
    /* Get arguments */
    if(argc != 4) {
        std::cout << "Usage : " << argv[0] << " texture letters output" << std::endl;
        return 1;
    }

    /* Init SDL */
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Couldn't load SDL : " << SDL_GetError() << std::endl;
        return 1;
    }

    /* Save letters */
    SDL_RWops* save = SDL_RWFromFile(argv[3], "wb");
    if(save == NULL) {
        std::cout << "Couldn't open " << argv[3] << std::endl;
        return 1;
    }
    std::string letters(argv[2]);
    unsigned int nb = (unsigned int)letters.size();
    SDL_RWwrite(save, &nb, 4, 1);
    SDL_RWwrite(save, letters.data(), letters.size(), 1);

    /* Open the picture and save it */
    SDL_Surface* pict = IMG_Load(argv[1]);
    if(pict == NULL) {
        std::cout << "Couldn't open " << argv[1] << std::endl;
        return 1;
    }
    SDL_SaveBMP_RW(pict, save, 0);
    SDL_FreeSurface(pict);

    SDL_RWclose(save);
    SDL_Quit();
    return 0;
}

