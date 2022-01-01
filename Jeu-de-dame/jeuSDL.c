#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define TAILLEDAMIER 800
#define TAILLEGRILLE 10

SDL_Surface *ecran = NULL;
SDL_Surface *pion[2] = {NULL};
SDL_Surface *damier = NULL;
SDL_Rect fond = {0, 0, 0, 0};

typedef struct _point
{
    int casex;
    int casey;
}Point;

Point mouseinCase={0, 0};
Point startcase={0, 0};
Point destCase={0, 0};

typedef enum {PIONNOIR, PIONBLANC, NOIRE, BLANCHE}Case;
//typedef enum {CUR_NEUTRE, CUR_VALIDE, CUR_NONVALIDE, CUR_LAST}TypeCurseur;
 
char *s_pion[4]={"PION NOIR", "PION BLANC", "CASE NOIRE", "CASE BLANCHE"};

void affiche(Case grille[TAILLEGRILLE][TAILLEGRILLE])
{
    int i, j;
    SDL_Rect positionCase={0,0,0,0};
    SDL_BlitSurface(damier, NULL, ecran, &fond);
    for (i = 0; i < TAILLEGRILLE; i++)
    {
        for (j = 0; j < TAILLEGRILLE; j++)
        {
            switch(grille[i][j])
            {
                case PIONNOIR:
                case PIONBLANC:
                    positionCase.x=j*TAILLEDAMIER/TAILLEGRILLE +5;
                    positionCase.y=i*TAILLEDAMIER/TAILLEGRILLE + 5;
                    SDL_BlitSurface(pion[grille[i][j]], NULL, ecran, &positionCase);
            }
        }
    }
}





void SDL_ExitError(const char *message);

int main(int argc, char **argv){

    //Définition des variables
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    //On lance SDL :
    if(SDL_Init(SDL_INIT_VIDEO) !=0){
        SDL_ExitError("Initialisation SDL");
    }

    /*------------Programme---------------*/

    /*------------------------------------*/



    //Libération
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