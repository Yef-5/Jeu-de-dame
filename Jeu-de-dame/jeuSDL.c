#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_W 800
#define WINDOW_H 880
#define TAILLEDAMIER 800
#define TAILLEGRILLE 10

SDL_Surface *ecran = NULL;
SDL_Surface *pion[2] = {NULL};
SDL_Surface *damier = NULL;
SDL_Surface *START = NULL; //Bouton jouer -> à voir si on garde
//SDL_Surface *curseur[4] = {NULL};
SDL_Rect fond = {0, 0, 0, 0};
SDL_Texture *texture = NULL;
SDL_Texture *texture2 = NULL;

typedef struct _point
{
    int casex;
    int casey;
}Point;

Point mouseinCase={0, 0};
Point startcase={0, 0};
Point destCase={0, 0};

typedef enum {PIONNOIR, PIONBLANC, NOIRE, BLANCHE}Case;
typedef enum {CUR_NEUTRE, CUR_VALIDE, CUR_NONVALIDE, CUR_LAST}TypeCurseur;
 
char *s_pion[4]={"PION NOIR", "PION BLANC", "CASE NOIRE", "CASE BLANCHE"};

void affiche(Case grille[TAILLEGRILLE][TAILLEGRILLE]) {
    int i, j;
    SDL_Rect positionCase={0,0,0,0};
    SDL_BlitSurface(damier, NULL, ecran, &fond);
    for (i = 0; i < TAILLEGRILLE; i++) {
        for (j = 0; j < TAILLEGRILLE; j++) {
            switch(grille[i][j]) { //Je suis pas sûre de ce que c'est
                case PIONNOIR:
                case PIONBLANC:
                    positionCase.x = j*TAILLEDAMIER/TAILLEGRILLE +5;
                    positionCase.y = i*TAILLEDAMIER/TAILLEGRILLE + 5;
                    SDL_BlitSurface(pion[grille[i][j]], NULL, ecran, &positionCase);
                break;
            }
            
        }
    }
}





void SDL_ExitError(const char *message);

int play(Case grille[TAILLEGRILLE][TAILLEGRILLE]){
    int replay = 0;
    int selec = 0;
    SDL_Rect positionCase={0, 0, 0, 0};
    SDL_Event event;

    SDL_bool program_launched = SDL_TRUE;

    while(program_launched){
        SDL_Event event;

        while(SDL_PollEvent(&event)){
            switch(event.type){

                case SDL_MOUSEMOTION:  //Calculer la position de la souris pour afficher le cadre de selection!
                if ( event.motion.x > 0 && event.motion.x < TAILLEDAMIER && event.motion.y > 0 && event.motion.y < TAILLEDAMIER) {
                    mouseinCase.casex = event.motion.x / (TAILLEDAMIER/TAILLEGRILLE);
                    mouseinCase.casey = event.motion.y / (TAILLEDAMIER/TAILLEGRILLE);
                    printf("souris en {%d, %d} sur %s\n", mouseinCase.casex, mouseinCase.casey, s_pion[grille[mouseinCase.casey][mouseinCase.casex]]);
                }
                break;

                case SDL_MOUSEBUTTONDOWN: // on prend avec clickant sur une cas (et en maintenant le click)
                    if (selec==1)
                        break;
                    if (grille[mouseinCase.casey][mouseinCase.casex] != BLANCHE && grille[mouseinCase.casey][mouseinCase.casex] != NOIRE) {
                        selec = 1;
                        startcase = mouseinCase;
                    }
                break;
                case SDL_MOUSEBUTTONUP:
                    if( event.button.button == SDL_BUTTON_LEFT &&               // relache le clic gauche
                        event.button.x>0 && event.button.x<START->w &&   // dans le carré du bouton, on quitte
                        event.button.y>TAILLEDAMIER && event.button.x<TAILLEDAMIER+START->h) {
                        
                        program_launched = SDL_FALSE;
                        replay  = 1;
                        break;
                    }
                    if (grille[mouseinCase.casey][mouseinCase.casex] == NOIRE){   // on pose un pion // faudrait tester la possibilitié de coup multiple!
                        selec=0;
                        destCase = mouseinCase;
                        grille[destCase.casey][destCase.casex] = grille[startcase.casey][startcase.casex];
                        grille[startcase.casey][startcase.casex] = NOIRE;
                        break;
                    }

                case SDL_QUIT:
                    program_launched = SDL_FALSE;
                break;

                default:
                break;
            }
            // On fait maintenant l'affichage :
            SDL_FillRect(ecran, NULL, 0x000000); //tout effacer
            affiche(grille); //Affichage du damier puis des pions
            //Affichage des case qu'on peut jouer -> à venir

            //Affichage des pions ?
            //for(i=0, )
            
            //Affichage du bouton START
            positionCase.x = 0;
            positionCase.y = TAILLEDAMIER;
            
            SDL_BlitSurface(START, NULL, ecran, &positionCase); //J'ai pas l'impression que c'est pas utile

        }

    }
    return replay;

}






int main(int argc, char **argv){

    //Définition des variables
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    int replay = 0;

    //On lance SDL :
    if(SDL_Init(SDL_INIT_VIDEO) !=0){
        SDL_ExitError("Initialisation SDL");
    }

    /*------------Programme---------------*/
    //Création de la fenêtre :
    /*window = SDL_CreateWindow("Première fenêtre SDL2", 
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                WINDOW_W, WINDOW_H, 0 );
    if(window == NULL){
        SDL_ExitError("Création fenêtre échouée");
    }*/

    //On créer la fenêtre + rendu :
    if(SDL_CreateWindowAndRenderer(WINDOW_W, WINDOW_H, 0, &window, &renderer) !=0){
        SDL_ExitError("Impossible de créer la fenêtre et le rendu");
    }

    damier = SDL_LoadBMP("damier.bmp");
    START = SDL_LoadBMP("START.bmp");
    pion[PIONNOIR] = SDL_LoadBMP("pionN.bmp");
    pion[PIONBLANC] = SDL_LoadBMP("pionB.bmp");

    if(damier == NULL || START ==  NULL || pion[PIONNOIR] == NULL || pion[PIONBLANC] == NULL){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitError("Impossible de charger les images");
    }

    texture = SDL_CreateTextureFromSurface(renderer, damier);
    SDL_FreeSurface(damier); //-> à voir
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

    //rectangle.x =(WINDOW_W - rectangle.w) / 2; //Pour centrer élement par rapport à la fenêtre
    //rectangle.y = (WINDOW_H - rectangle.h) / 2;

    if(SDL_RenderCopy(renderer, texture, NULL, &rectangle) != 0){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_ExitError("Impossible d'afficher la texture");
    }

    //for(rectangle.x = 0; rectangle.x >= TAILLEDAMIER; rectangle.x+10){
      //  for(rectangle.y = 0; rectangle.y >= TAILLEDAMIER; rectangle.y+10){    
        texture2 = SDL_CreateTextureFromSurface(renderer, pion[PIONNOIR]);
        SDL_FreeSurface(pion[PIONNOIR]); //-> à voir
        if(texture2 == NULL){
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_ExitError("Impossible de créer la texture");
        }
        if(SDL_QueryTexture(texture2, NULL, NULL, &rectangle.w, &rectangle.h) != 0){
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_ExitError("Impossible de charger la texture");
        }

        if(SDL_RenderCopy(renderer, texture2, NULL, &rectangle) != 0){
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_ExitError("Impossible d'afficher la texture");
        }
        //}
    //}


    SDL_RenderPresent(renderer);

 

    do {
        Case grille[TAILLEGRILLE][TAILLEGRILLE]={
            {BLANCHE, PIONNOIR, BLANCHE, PIONNOIR, BLANCHE, PIONNOIR, BLANCHE, PIONNOIR, BLANCHE, PIONNOIR},
            {PIONNOIR, BLANCHE, PIONNOIR, BLANCHE, PIONNOIR, BLANCHE, PIONNOIR, BLANCHE, PIONNOIR, BLANCHE},
            {BLANCHE, PIONNOIR, BLANCHE, PIONNOIR, BLANCHE, PIONNOIR, BLANCHE, PIONNOIR, BLANCHE, PIONNOIR},
            {PIONNOIR, BLANCHE, PIONNOIR, BLANCHE, PIONNOIR, BLANCHE, PIONNOIR, BLANCHE, PIONNOIR, BLANCHE},
            {BLANCHE, NOIRE, BLANCHE, NOIRE, BLANCHE, NOIRE, BLANCHE, NOIRE, BLANCHE, NOIRE},
            {BLANCHE, NOIRE, BLANCHE, NOIRE, BLANCHE, NOIRE, BLANCHE, NOIRE, BLANCHE, NOIRE},
            {BLANCHE, PIONBLANC, BLANCHE, PIONBLANC, BLANCHE, PIONBLANC, BLANCHE, PIONBLANC, BLANCHE, PIONBLANC},
            {PIONBLANC, BLANCHE, PIONBLANC, BLANCHE, PIONBLANC, BLANCHE, PIONBLANC, BLANCHE, PIONBLANC, BLANCHE},
            {BLANCHE, PIONBLANC, BLANCHE, PIONBLANC, BLANCHE, PIONBLANC, BLANCHE, PIONBLANC, BLANCHE, PIONBLANC},
            {PIONBLANC, BLANCHE, PIONBLANC, BLANCHE, PIONBLANC, BLANCHE, PIONBLANC, BLANCHE, PIONBLANC, BLANCHE}
        };

        replay = play(grille);
    }while(replay);

    /*------------------------------------*/


    //Libération
    SDL_FreeSurface(ecran);
    SDL_FreeSurface(damier);
    SDL_FreeSurface(pion[PIONNOIR]);
    SDL_FreeSurface(pion[PIONBLANC]);
    SDL_FreeSurface(START);

    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(texture2);

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
/*void SDL_Destroy(const char *message){
        SDL_DestroyRenderer("erreur : %s > %s\n", message, SDL_GetError());
        SDL_DestroyWindow("erreur : %s > %s\n", message, SDL_GetError());
        SDL_ExitError("Impossible de charger l'image");
}*/

//gcc jeuSDL.c -o bin/jeu -I include -L lib -lSDL2main -lSDL2