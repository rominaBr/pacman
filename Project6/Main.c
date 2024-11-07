#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL_ttf.h>

#define ANCHO 530
#define ALTURA 630
#define CELDA_TAM 25
#define PACMAN_TAM 23
#define COMIDA_TAM 12
#define COMIDA_TAM_ESP 24
#define N 23
#define M 19

#define MARGEN_SUPERIOR 20
#define ANCHO_LABERINTO (M * CELDA_TAM)
#define ALTURA_LABERINTO (N * CELDA_TAM)
#define DURACION_INMUNIDAD 500 

int offsetX = (ANCHO - ANCHO_LABERINTO) / 2;
int offsetY = MARGEN_SUPERIOR + (ALTURA - MARGEN_SUPERIOR - ALTURA_LABERINTO) / 2;
int vidas = 3;
int puntaje = 0;
bool inmune = false;
int contadorInmunidad = 0; 
int pacmanX;
int pacmanY;
int movimientoX = 0;
int movimientoY = 0;
int comida;
int cantComida;

int laberinto[N][M] = {
    {3, 2, 2, 2, 2, 2, 2, 2, 2, 9, 2, 2, 2, 2, 2, 2, 2, 2, 4},
    {1, 20, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 20, 0, 0, 1},
    {1, 0, 3, 4, 0, 3, 9, 4, 0, 1, 0, 3, 9, 4, 0, 3, 4, 0, 1},
    {1, 0, 5, 6, 0, 5, 10, 6, 0, 12, 0, 5, 10, 6, 0, 5, 6, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 13, 14, 0, 11, 0, 13, 2, 9, 2, 14, 0, 11, 0, 13, 14, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {5, 2, 2, 4, 0, 7, 2, 14, 0, 12, 20, 13, 2, 8, 0, 3, 2, 2, 6},
    {21, 21, 21, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 21, 21, 21},
    {2, 2, 2, 6, 0, 12, 0, 3, 2, -1, 2, 4, 0, 12, 0, 5, 2, 2, 2},
    {0, 0, 0, 0, 0, 0, 0, 1, 21, 21, 21, 1, 0, 0, 0, 0, 0, 0, 0},
    {2, 2, 2, 4, 0, 11, 0, 1, 21, 21, 21, 1, 0, 11, 0, 3, 2, 2, 2},
    {21, 21, 21, 1, 0, 1, 0, 5, 2, 2, 2, 6, 0, 1, 0, 1, 21, 21, 21},
    {21, 21, 21, 1, 0, 1, 0, 0, 0, -1, 0, 0, 0, 1, 0, 1, 21, 21, 21},
    {3, 2, 2, 6, 0, 12, 0, 13, 2, 9, 2, 14, 0, 12, 0, 5, 2, 2, 4},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 13, 4, 0, 13, 2, 14, 0, 12, 0, 13, 2, 14, 0, 3, 14, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {7, 14, 0, 12, 0, 11, 0, 13, 2, 9, 2, 14, 0, 11, 0, 12, 0, 13, 8},
    {1, 0, 0, 0, 0, 1, 0, 0, 20, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 13, 2, 2, 10, 2, 14, 0, 12, 0, 13, 2, 10, 2, 2, 14, 0, 1},
    {1, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 1},  
    {5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 6}
};


void dibujarLaberinto(SDL_Renderer* renderer, SDL_Texture* texturaVertical, SDL_Texture* texturaVertical7, SDL_Texture* texturaVertical8, 
    SDL_Texture* texturaHorizontal, SDL_Texture* texturaEsquina3,SDL_Texture* texturaEsquina4, SDL_Texture* texturaEsquina5,
    SDL_Texture* texturaEsquina6, SDL_Texture* texturaHorizontal9, 
    SDL_Texture* texturaHorizontal10, SDL_Texture* texturaVertical11, SDL_Texture* texturaVertical12, SDL_Texture* texturaHorizontal13,
    SDL_Texture* texturaHorizontal14, SDL_Texture* texturaComida) {    

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            
            if (laberinto[i][j] != 0 && laberinto[i][j] != 20) {
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
            else {
                if (laberinto[i][j] == 0 || laberinto[i][j] == 20)
                {
                    SDL_Rect rectComida;    

                    comida = (laberinto[i][j] == 20) ? COMIDA_TAM_ESP : COMIDA_TAM; 

                    rectComida.w = comida;
                    rectComida.h = comida;
                    rectComida.x = offsetX + j * CELDA_TAM + (CELDA_TAM - comida) / 2;
                    rectComida.y = offsetY + i * CELDA_TAM + (CELDA_TAM - comida) / 2;

                    SDL_RenderCopy(renderer, texturaComida, NULL, &rectComida);

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

    return ((laberinto[celY1][celX1] == 0 || laberinto[celY1][celX1] == 20 || laberinto[celY1][celX1] == -1) &&
        (laberinto[celY1][celX2] == 0 || laberinto[celY1][celX2] == 20 || laberinto[celY1][celX2] == -1) &&
        (laberinto[celY2][celX1] == 0 || laberinto[celY2][celX1] == 20 || laberinto[celY2][celX1] == -1) &&
        (laberinto[celY2][celX2] == 0 || laberinto[celY2][celX2] == 20 || laberinto[celY2][celX2] == -1));
}

void inicializarComida() {
    cantComida = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (laberinto[i][j] == 0 || laberinto[i][j] == 20) {
                cantComida++;
            }
        }
    }
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
    
    int celdaX = pacmanX / CELDA_TAM;
    int celdaY = pacmanY / CELDA_TAM;
    
    if (laberinto[celdaY][celdaX] == 0 || laberinto[celdaY][celdaX] == 20) {
        if (laberinto[celdaY][celdaX] == 20) {
            inmune = true;
            printf("inmunidad!\n");
            contadorInmunidad = DURACION_INMUNIDAD; 
        }

        puntaje += (laberinto[celdaY][celdaX] == 20) ? 50 : 10; 
        laberinto[celdaY][celdaX] = -1; 
        cantComida--;

        if (cantComida == 0) {
            printf("GANÓ!");
        }
    }
    
    if (inmune) {
        contadorInmunidad--;
        if (contadorInmunidad <= 0) {
            inmune = false; 
        }
    }
    

}


void mostrarPuntaje(SDL_Renderer* render, TTF_Font* fuente, int puntaje) {
    SDL_Color color = { 255, 255, 255 }; 
    char puntajeTexto[20];
    snprintf(puntajeTexto, sizeof(puntajeTexto), "Puntaje: %d", puntaje);

    SDL_Surface* surfaceTexto = TTF_RenderText_Solid(fuente, puntajeTexto, color);
    SDL_Texture* texturaTexto = SDL_CreateTextureFromSurface(render, surfaceTexto);

    SDL_Rect rectTexto = { 20, 10, surfaceTexto->w, surfaceTexto->h }; 
    SDL_RenderCopy(render, texturaTexto, NULL, &rectTexto);

    SDL_FreeSurface(surfaceTexto);
    SDL_DestroyTexture(texturaTexto);

}



int main(int argc, char* argv[]) {

    SDL_Init(SDL_INIT_EVERYTHING); 


    SDL_Window* ventana = SDL_CreateWindow("Pac Man", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ANCHO, ALTURA, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);

    if (TTF_Init() == -1) {
        printf("Error al inicializar SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("assets/fonts/FreeSans.ttf", 24);
    if (!font) {
        printf("Error al cargar la fuente: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }


    pacmanX = (ANCHO - PACMAN_TAM) / 2 - offsetX;
    pacmanY = 363 - offsetY;

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
    SDL_Texture* texturaComida = crearTextura(renderer, "assets/pacman/comida.png");

    if (!texturaVertical || !texturaHorizontal || !texturaEsquina3 || !texturaEsquina4 || !texturaEsquina5 || 
        !texturaEsquina6 || !texturaHorizontal9 || !texturaHorizontal10 || !texturaVertical11 || !texturaVertical12|| !texturaHorizontal13 || !texturaHorizontal14
        || !texturaVertical7 || !texturaVertical8 || !texturaComida) {
        printf("Error al cargar las texturas: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
   
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
    SDL_RenderClear(renderer);
    dibujarLaberinto(renderer, texturaVertical, texturaVertical7, texturaVertical8, texturaHorizontal, texturaEsquina3, texturaEsquina4, texturaEsquina5,
        texturaEsquina6, texturaHorizontal9, texturaHorizontal10, texturaVertical11, texturaVertical12, texturaHorizontal13, texturaHorizontal14, texturaComida);
    SDL_RenderPresent(renderer);

    SDL_Texture* pacmanDer = IMG_LoadTexture(renderer, "assets/pacman/pacmanDer.png");
    SDL_Texture* pacmanIzq = IMG_LoadTexture(renderer, "assets/pacman/pacmanIzq.png");
    SDL_Texture* pacmanArriba = IMG_LoadTexture(renderer, "assets/pacman/pacmanArriba.png");
    SDL_Texture* pacmanAbajo = IMG_LoadTexture(renderer, "assets/pacman/pacmanAbajo.png");

    SDL_Texture* texturaPacmanActual = pacmanDer;
   


    if (!texturaPacmanActual) {
        printf("Error al cargar la textura: %s \n", SDL_GetError());
        SDL_Quit();
        return 1;
    }    

    SDL_Rect pacmanRect;

    SDL_RenderCopy(renderer, texturaPacmanActual, NULL, &pacmanRect);

    pacmanRect.w = PACMAN_TAM;
    pacmanRect.h = PACMAN_TAM;
    pacmanRect.x = (ANCHO - pacmanRect.w) / 2;
    pacmanRect.y = 457;

    SDL_RenderCopy(renderer, texturaPacmanActual, NULL, &pacmanRect);
    SDL_RenderPresent(renderer);

    inicializarComida();

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

        if (movimientoX > 0) texturaPacmanActual = pacmanDer;
        else if (movimientoX < 0) texturaPacmanActual = pacmanIzq;
        else if (movimientoY < 0) texturaPacmanActual = pacmanArriba;
        else if (movimientoY > 0) texturaPacmanActual = pacmanAbajo;

        moverPacman();

        pacmanRect.w = PACMAN_TAM;
        pacmanRect.h = PACMAN_TAM;
        pacmanRect.x = offsetX + pacmanX;
        pacmanRect.y = offsetY + pacmanY;

        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        dibujarLaberinto(renderer, texturaVertical, texturaVertical7, texturaVertical8, texturaHorizontal,
            texturaEsquina3, texturaEsquina4, texturaEsquina5, texturaEsquina6,
            texturaHorizontal9, texturaHorizontal10, texturaVertical11, texturaVertical12,
            texturaHorizontal13, texturaHorizontal14, texturaComida);
        SDL_RenderCopy(renderer, texturaPacmanActual, NULL, &pacmanRect);
        
        mostrarPuntaje(renderer, font, puntaje);

        SDL_RenderPresent(renderer);               
        
        SDL_Delay(1000 / 50);
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
    SDL_DestroyTexture(texturaPacmanActual);
    SDL_DestroyTexture(texturaComida);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
    
    SDL_Quit();

    return 0;
}
