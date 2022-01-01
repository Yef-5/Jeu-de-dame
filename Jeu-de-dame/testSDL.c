#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_W 800
#define WINDOW_H 600


//Premier test :
/*int main(int argc, char **argv){
    SDL_version nb;
    SDL_VERSION(&nb);
    printf("Tu as réussi %d.%d.%d !\n", nb.major, nb.minor, nb.patch);
    return 0;
}*/

//2eme test et plus :

/*void SDL_ExitError(const char *message);

int main(int argc, char **argv){
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    //On lance SDL :
    if(SDL_Init(SDL_INIT_VIDEO) !=0){
        SDL_ExitError("Initialisation SDL");
    }

    //On créer la fenêtre :
    window = SDL_CreateWindow("Première fenêtre SDL2", 
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                WINDOW_W, //Largeur en pixel
                                WINDOW_H, //Hauteur en pixel
                                0 ); //Gerer par defaut
    if(window == NULL){
        SDL_ExitError("Création fenêtre échouée");
    }
    
    /*----------------Création Rendu---------*/
/*    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if(renderer == NULL){
        SDL_ExitError("Création rendu échouée");
    }

    SDL_RenderPresent(renderer); //maj du rendu

    /*-------------Fin Création Rendu---------*/
/*    SDL_Delay(3000); //On met Attente pour voir la fenêtre car détruite juste après la création comme on fait rien.

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS; //return 0;
}*/


//3,5 eme test :
//On peut créer la fenêtre et le rendu en même temps :
void SDL_ExitError(const char *message);

int main(int argc, char **argv){
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    //On lance SDL :
    if(SDL_Init(SDL_INIT_VIDEO) !=0){
        SDL_ExitError("Initialisation SDL");
    }

    //On créer la fenêtre + rendu :
    if(SDL_CreateWindowAndRenderer(WINDOW_W, WINDOW_H, 0, &window, &renderer) !=0){
        SDL_ExitError("Impossible de créer la fenêtre et le rendu");
    }
    
/*-----------------------Dessiner des truc------------------------*/
 /*   if(SDL_SetRenderDrawColor(renderer, 237, 28, 36, SDL_ALPHA_OPAQUE) != 0){
        SDL_ExitError("Impossible de changer la couleur");
    }

    if(SDL_RenderDrawLine(renderer, 112, 112, 500, 500) != 0){
        SDL_ExitError("Impossible de dessiner une ligne");
    }
    //On peut initialiser le rectangle avec le reste mais je le mets ici pour que ça soit plus clair.
    SDL_Rect rectangle;
    rectangle.x = 2;
    rectangle.y = 2;
    rectangle.w = 200;
    rectangle.h = 100;
    //Si on veut changer la couleur, on peut mettre le SDL_SetRenderDrawColor() ici en changeant les para
    if(SDL_RenderDrawRect(renderer, &rectangle) != 0){
        SDL_ExitError("Impossible de dessiner un rectangle");
    }
    //Un rectangle rempli
    if(SDL_RenderFillRect(renderer, &rectangle) != 0){
        SDL_ExitError("Impossible de dessiner un rectangle");
    }
    //Supperposition au niveau de ce qu'on dessine, donc ordre important


    SDL_RenderPresent(renderer); //maj du rendu
    SDL_Delay(6000); //On met Attente pour voir la fenêtre car détruite juste après la création comme on fait rien.
*/
/*----------------------------------------------------------------*/

/*--------------------------Création texture---------------------*/
/*
    SDL_Surface *image = NULL;
    SDL_Texture *texture = NULL;

    image = SDL_LoadBMP("pionN.bmp");
    if(image == NULL){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitError("Impossible de charger l'image");
    }

    texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);

    if(texture == NULL){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitError("Impossible de créer la texture");
    }
    SDL_Rect rectangle;
    if(SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitError("Impossible de charger la texture");
    }

    rectangle.x =(WINDOW_W - rectangle.w) / 2; //Pour centrer élement par rapport à la fenêtre
    rectangle.y = (WINDOW_H - rectangle.h) / 2;

    if(SDL_RenderCopy(renderer, texture, NULL, &rectangle) != 0){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitError("Impossible d'afficher la texture");
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(3000);
*/
/*---------------------------------------------------------------*/

/*------------------------Gestion d'événement-------------------*/

SDL_bool program_launched = SDL_TRUE;

while(program_launched){
    SDL_Event event;

    while(SDL_PollEvent(&event)){
        switch(event.type){
            //Clavier
            case SDL_KEYDOWN:
            //https://wiki.libsdl.org/SDL_Keycode
                switch(event.key.keysym.sym){
                    case SDLK_b:
                        printf("Appuyé sur touche B\n");
                        continue;
                    default:
                        continue;
                }
            //souris
            //coordonnées de la souris
            case SDL_MOUSEMOTION:
                printf("%d / %d\n", event.motion.x, event.motion.y);
                //position relative avec event.motion.xrel
                break;
            //coordonnées de où on clic
            case SDL_MOUSEBUTTONDOWN:
                printf("Clic en %dx/ %dy\n", event.button.x, event.button.y);
                break;
            
            //Fenêtre
            //gerer une sous fenêtre :
            case SDL_WINDOWEVENT:
                if(event.window.event == SDL_WINDOWEVENT_CLOSE){
                    printf("la 2eme fenêtre est fermé (pas vraiment");
                }//A chercher comment mieux faire ça
                break;
            case SDL_QUIT:
                program_launched = SDL_FALSE;
                break;

            default:
                break;
        }
    }
}


/*--------------------------------------------------------------*/



    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS; //return 0;
}


void SDL_ExitError(const char *message){
    SDL_Log("erreur : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}




//gcc testSDL.c -o bin/prog -I include -L lib -lSDL2main -lSDL2