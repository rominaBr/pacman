#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>


#define ANCHO 640
#define ALTURA 700
#define CELDA_TAM 28
#define PACMAN_TAM 25
#define N 23
#define M 19

#define MARGEN_SUPERIOR 20
#define ANCHO_LABERINTO (M * CELDA_TAM)
#define ALTURA_LABERINTO (N * CELDA_TAM)


int offsetX = (ANCHO - ANCHO_LABERINTO) / 2;
int offsetY = MARGEN_SUPERIOR + (ALTURA - MARGEN_SUPERIOR - ALTURA_LABERINTO) / 2;
int vidas = 3;
int pacmanX;
int pacmanY;
int movimientoX = 0;
int movimientoY = 0;


int laberinto[N][M] = {
    {3, 2, 2, 2, 2, 2, 2, 2, 2, 9, 2, 2, 2, 2, 2, 2, 2, 2, 4},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 3, 4, 0, 3, 9, 4, 0, 1, 0, 3, 9, 4, 0, 3, 4, 0, 1},
    {1, 0, 5, 6, 0, 5, 10, 6, 0, 12, 0, 5, 10, 6, 0, 5, 6, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 13, 14, 0, 11, 0, 13, 2, 9, 2, 14, 0, 11, 0, 13, 14, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {5, 2, 2, 4, 0, 7, 2, 14, 0, 12, 0, 13, 2, 8, 0, 3, 2, 2, 6},
    {0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0},
    {2, 2, 2, 6, 0, 12, 0, 3, 2, -1, 2, 4, 0, 12, 0, 5, 2, 2, 2},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {2, 2, 2, 4, 0, 11, 0, 1, 0, 0, 0, 1, 0, 11, 0, 3, 2, 2, 2},
    {0, 0, 0, 1, 0, 1, 0, 5, 2, 2, 2, 6, 0, 1, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0},
    {3, 2, 2, 6, 0, 12, 0, 13, 2, 9, 2, 14, 0, 12, 0, 5, 2, 2, 4},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 13, 4, 0, 13, 2, 14, 0, 12, 0, 13, 2, 14, 0, 3, 14, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {7, 14, 0, 12, 0, 11, 0, 13, 2, 9, 2, 14, 0, 11, 0, 12, 0, 13, 8},
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 13, 2, 2, 10, 2, 14, 0, 12, 0, 13, 2, 10, 2, 2, 14, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},  
    {5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 6}
};


void dibujarLaberinto(SDL_Renderer* renderer, SDL_Texture* texturaVertical, SDL_Texture* texturaVertical7, SDL_Texture* texturaVertical8, 
    SDL_Texture* texturaHorizontal, SDL_Texture* texturaEsquina3,SDL_Texture* texturaEsquina4, SDL_Texture* texturaEsquina5,
    SDL_Texture* texturaEsquina6, SDL_Texture* texturaHorizontal9, 
    SDL_Texture* texturaHorizontal10, SDL_Texture* texturaVertical11, SDL_Texture* texturaVertical12, SDL_Texture* texturaHorizontal13, SDL_Texture* texturaHorizontal14) {
    

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            
            if (laberinto[i][j] != 0) {
                SDL_Rect rect = { offsetX + j * CELDA_TAM, offsetY + i * CELDA_TAM, CELDA_TAM, CELDA_TAM };
                
                switch (laberinto[i][j])
                {
                case 1: SDL_RenderCopy(renderer, texturaVertical, NULL, &rect);
                    break;
                case 2: SDL_RenderCopy(renderer, texturaHorizontal, NULL, &rect);
                    break;
                case 3: SDL_RenderCopy(renderer, texturaEsquina3, NULL, &rect);
                    break;
                case 4: SDL_RenderCopy(renderer, texturaEsquina4, NULL, &rect);
                    break;
                case 5: SDL_RenderCopy(renderer, texturaEsquina5, NULL, &rect);
                    break;
                case 6: SDL_RenderCopy(renderer, texturaEsquina6, NULL, &rect);
                    break;
                case 7: SDL_RenderCopy(renderer, texturaVertical7, NULL, &rect);
                    break;
                case 8: SDL_RenderCopy(renderer, texturaVertical8, NULL, &rect);
                    break;
                case 9: SDL_RenderCopy(renderer, texturaHorizontal9, NULL, &rect);
                    break;
                case 10: SDL_RenderCopy(renderer, texturaHorizontal10, NULL, &rect);
                    break;
                case 11: SDL_RenderCopy(renderer, texturaVertical11, NULL, &rect);
                    break;
                case 12: SDL_RenderCopy(renderer, texturaVertical12, NULL, &rect);
                    break;
                case 13: SDL_RenderCopy(renderer, texturaHorizontal13, NULL, &rect);
                    break;
                case 14: SDL_RenderCopy(renderer, texturaHorizontal14, NULL, &rect);
                    break;                
                }
               
            }
        }
    }
}

SDL_Texture* crearTextura(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* tempSurface = IMG_Load(path);
    SDL_Texture* textura = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return textura;
}

void moverSegunTecla(SDL_Event evento) {

    switch (evento.key.keysym.sym) {
        case SDLK_UP:
            movimientoX = 0;
            movimientoY = - CELDA_TAM / 10;
            break;
        case SDLK_DOWN:
            movimientoX = 0;
            movimientoY = CELDA_TAM / 10;
            break;
        case SDLK_LEFT:
            movimientoX = -CELDA_TAM / 10;
            movimientoY = 0;
            break;
        case SDLK_RIGHT:
            movimientoX = CELDA_TAM / 10;
            movimientoY = 0;
            break;
    }

}

bool esPosicionValida(int x, int y) {
    int celX1 = x / CELDA_TAM;
    int celY1 = y / CELDA_TAM;
    int celX2 = (x + PACMAN_TAM - 1) / CELDA_TAM;
    int celY2 = (y + PACMAN_TAM - 1) / CELDA_TAM;

    return laberinto[celY1][celX1] == 0 && laberinto[celY1][celX2] == 0 &&
        laberinto[celY2][celX1] == 0 && laberinto[celY2][celX2] == 0;
}

void moverPacman() {
    int nuevaX = pacmanX + movimientoX;
    int nuevaY = pacmanY + movimientoY;

    if (esPosicionValida(nuevaX, pacmanY)) {
        pacmanX = nuevaX;
    }
    if (esPosicionValida(pacmanX, nuevaY)) {
        pacmanY = nuevaY;
    }
}


int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* ventana = SDL_CreateWindow("Pac Man", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ANCHO, ALTURA, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);

    pacmanX = (ANCHO - PACMAN_TAM) / 2 - offsetX;
    pacmanY = 405 - offsetY;

    if (!ventana || !renderer) {
        printf("Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Texture* texturaVertical = crearTextura(renderer, "assets/laberinto/vertical.png");
    SDL_Texture* texturaVertical7 = crearTextura(renderer, "assets/laberinto/vertical7.png");
    SDL_Texture* texturaVertical8 = crearTextura(renderer, "assets/laberinto/vertical8.png");
    SDL_Texture* texturaHorizontal = crearTextura(renderer, "assets/laberinto/horizontal.png");
    SDL_Texture* texturaHorizontal9 = crearTextura(renderer, "assets/laberinto/horizontal9.png");
    SDL_Texture* texturaHorizontal10 = crearTextura(renderer, "assets/laberinto/horizontal10.png");
    SDL_Texture* texturaHorizontal13 = crearTextura(renderer, "assets/laberinto/horizontal13.png");
    SDL_Texture* texturaHorizontal14 = crearTextura(renderer, "assets/laberinto/horizontal14.png");
    SDL_Texture* texturaEsquina3 = crearTextura(renderer, "assets/laberinto/esquina3.png");
    SDL_Texture* texturaEsquina4 = crearTextura(renderer, "assets/laberinto/esquina4.png");
    SDL_Texture* texturaEsquina5 = crearTextura(renderer, "assets/laberinto/esquina5.png");
    SDL_Texture* texturaEsquina6 = crearTextura(renderer, "assets/laberinto/esquina6.png");
    SDL_Texture* texturaVertical11 = crearTextura(renderer, "assets/laberinto/vertical11.png");
    SDL_Texture* texturaVertical12 = crearTextura(renderer, "assets/laberinto/vertical12.png");

    if (!texturaVertical || !texturaHorizontal || !texturaEsquina3 || !texturaEsquina4 || !texturaEsquina5 || 
        !texturaEsquina6 || !texturaHorizontal9 || !texturaHorizontal10 || !texturaVertical11 || !texturaVertical12|| !texturaHorizontal13 || !texturaHorizontal14
        || !texturaVertical7 || !texturaVertical8) {
        printf("Error al cargar las texturas: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
   
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
    SDL_RenderClear(renderer);
    dibujarLaberinto(renderer, texturaVertical, texturaVertical7, texturaVertical8, texturaHorizontal, texturaEsquina3, texturaEsquina4, texturaEsquina5,
        texturaEsquina6, texturaHorizontal9, texturaHorizontal10, texturaVertical11, texturaVertical12, texturaHorizontal13, texturaHorizontal14);
    SDL_RenderPresent(renderer);

    SDL_Texture* texturaPacman = crearTextura(renderer, "assets/pacman/pacman1.png");

    if (!texturaPacman) {
        printf("Error al cargar la textura: %s \n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Rect pacmanRect;
    pacmanRect.w = PACMAN_TAM;
    pacmanRect.h = PACMAN_TAM;
    pacmanRect.x = (ANCHO - pacmanRect.w) / 2;
    pacmanRect.y = 457;

    SDL_RenderCopy(renderer, texturaPacman, NULL, &pacmanRect);
    SDL_RenderPresent(renderer);


    SDL_Event evento;
    bool running = true;

    while (running) {        

        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) {
                running = false;
            }
            else if(evento.type == SDL_KEYDOWN){
                moverSegunTecla(evento);
            }
        }

        moverPacman();

        SDL_Rect pacmanRect;
        pacmanRect.w = PACMAN_TAM;
        pacmanRect.h = PACMAN_TAM;
        pacmanRect.x = offsetX + pacmanX;
        pacmanRect.y = offsetY + pacmanY;

        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        dibujarLaberinto(renderer, texturaVertical, texturaVertical7, texturaVertical8, texturaHorizontal,
            texturaEsquina3, texturaEsquina4, texturaEsquina5, texturaEsquina6,
            texturaHorizontal9, texturaHorizontal10, texturaVertical11, texturaVertical12,
            texturaHorizontal13, texturaHorizontal14);
        SDL_RenderCopy(renderer, texturaPacman, NULL, &pacmanRect);
        SDL_RenderPresent(renderer);
        
        SDL_Delay(1000 / 60);
    }

    SDL_DestroyTexture(texturaVertical);
    SDL_DestroyTexture(texturaVertical7);
    SDL_DestroyTexture(texturaVertical8);
    SDL_DestroyTexture(texturaHorizontal);
    SDL_DestroyTexture(texturaHorizontal9);
    SDL_DestroyTexture(texturaHorizontal10);
    SDL_DestroyTexture(texturaVertical11);
    SDL_DestroyTexture(texturaVertical12);
    SDL_DestroyTexture(texturaHorizontal13);
    SDL_DestroyTexture(texturaHorizontal14);
    SDL_DestroyTexture(texturaEsquina3);
    SDL_DestroyTexture(texturaEsquina4);
    SDL_DestroyTexture(texturaEsquina5);
    SDL_DestroyTexture(texturaEsquina6);
    SDL_DestroyTexture(texturaPacman);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
    SDL_Quit();

    return 0;
}
