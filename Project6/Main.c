#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

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
#define DURACION_POWERUP 500 

typedef struct {
    int x, y;               // Posición en el laberinto
    int dx, dy;             // Dirección de movimiento
    bool vivo;  
    SDL_Texture* textura; 
} Fantasma;

bool powerUp = false;
int pacmanX, pacmanY, comida, cantComida;
int offsetX = (ANCHO - ANCHO_LABERINTO) / 2;
int offsetY = MARGEN_SUPERIOR + (ALTURA - MARGEN_SUPERIOR - ALTURA_LABERINTO) / 2;
int vidas = 3;
int puntaje = 0;
int contadorPowerUp = 0; 
int movimientoX = 0;
int movimientoY = 0;
int nivel = 1;
int contadorBoca = 0;

int laberinto[N][M] = {
    {3, 2, 2, 2, 2, 2, 2, 2, 2, 9, 2, 2, 2, 2, 2, 2, 2, 2, 4},
    {1, 20, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 20, 0, 0, 1},
    {1, 0, 3, 4, 0, 3, 9, 4, 0, 1, 0, 3, 9, 4, 0, 3, 4, 0, 1},
    {1, 0, 5, 6, 0, 5, 10, 6, 0, 12, 0, 5, 10, 6, 0, 5, 6, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 13, 14, 0, 11, 0, 13, 2, 9, 2, 14, 0, 11, 0, 13, 14, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 20, 0, 1, 0, 0, 0, 0, 1},
    {5, 2, 2, 4, 0, 7, 2, 14, -4, 12, -4, 13, 2, 8, 0, 3, 2, 2, 6},
    {21, 21, 21, 1, 0, 1, -4, -4, -4, 25, -4, -4, -4, 1, 0, 1, 21, 21, 21},
    {3, 2, 2, 6, 0, 12, -4, 3, 2, -3, 2, 4, -4, 12, 0, 5, 2, 2, 4},
    {1, 0, 0, 0, 0, -4, -4, 1, 26, 26, 26, 1, -4, -4, 0, 0, 0, 0, 1},
    {5, 2, 2, 4, 0, 11, -4, 1, 22, 23, 24, 1, -4, 11, 0, 3, 2, 2, 6},
    {21, 21, 21, 1, 0, 1, -4, 5, 2, 2, 2, 6, -4, 1, 0, 1, 21, 21, 21},
    {21, 21, 21, 1, 0, 1, -4, -4, -4, -4, -4, -4, -4, 1, 0, 1, 21, 21, 21},
    {3, 2, 2, 6, 0, 12, -4, 13, 2, 9, 2, 14, -4, 12, 0, 5, 2, 2, 4},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 13, 4, 0, 13, 2, 14, 0, 12, 0, 13, 2, 14, 0, 3, 14, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {7, 14, 0, 12, 0, 11, 0, 13, 2, 9, 2, 14, 0, 11, 0, 12, 0, 13, 8},
    {1, 0, 0, 0, 0, 1, 0, 0, 20, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 13, 2, 2, 10, 2, 14, 0, 12, 0, 13, 2, 10, 2, 2, 14, 0, 1},
    {1, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 1},  
    {5, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 6}
};


SDL_Window* ventana;
SDL_Texture* texturaPacmanActual;
SDL_Texture* pacmanDer;
SDL_Texture* pacmanIzq;
SDL_Texture* pacmanArriba;
SDL_Texture* pacmanAbajo;
SDL_Texture* pacmanBocaCerrada;
SDL_Texture* texturaVertical;
SDL_Texture* texturaVertical7;
SDL_Texture* texturaVertical8;
SDL_Texture* texturaHorizontal;
SDL_Texture* texturaHorizontal9;
SDL_Texture* texturaHorizontal10;
SDL_Texture* texturaHorizontal13;
SDL_Texture* texturaHorizontal14;
SDL_Texture* texturaEsquina3;
SDL_Texture* texturaEsquina4;
SDL_Texture* texturaEsquina5;
SDL_Texture* texturaEsquina6;
SDL_Texture* texturaVertical11;
SDL_Texture* texturaVertical12;
SDL_Texture* texturaComida;
SDL_Texture* texturaAmarillo;
SDL_Texture* texturaRosa;
SDL_Texture* texturaCeleste;
SDL_Texture* texturaRoja;
SDL_Renderer* renderer;
Mix_Chunk* musicaComer;
Mix_Chunk* musicaPrincipal;
Mix_Chunk* musicaPowerUp;
SDL_Rect pacmanRect;
Fantasma fantasmaRojo, fantasmaRosa, fantasmaCeleste, fantasmaAmarillo;


SDL_Texture* crearTextura(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* tempSurface = IMG_Load(path);
    SDL_Texture* textura = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return textura;
}

void cargarTexturas(SDL_Renderer* renderer) {

    pacmanDer = crearTextura(renderer, "assets/pacman/pacmanDer.png");
    pacmanIzq = crearTextura(renderer, "assets/pacman/pacmanIzq.png");
    pacmanArriba = crearTextura(renderer, "assets/pacman/pacmanArriba.png");
    pacmanAbajo = crearTextura(renderer, "assets/pacman/pacmanAbajo.png");
    pacmanBocaCerrada = crearTextura(renderer, "assets/pacman/pacmanBocaCerrada.png");

    texturaPacmanActual = pacmanDer;

    if (!texturaPacmanActual) {
        printf("Error al cargar la textura: %s \n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_RenderCopy(renderer, texturaPacmanActual, NULL, &pacmanRect);

    texturaVertical = crearTextura(renderer, "assets/laberinto/vertical.png");
    texturaVertical7 = crearTextura(renderer, "assets/laberinto/vertical7.png");
    texturaVertical8 = crearTextura(renderer, "assets/laberinto/vertical8.png");
    texturaHorizontal = crearTextura(renderer, "assets/laberinto/horizontal.png");
    texturaHorizontal9 = crearTextura(renderer, "assets/laberinto/horizontal9.png");
    texturaHorizontal10 = crearTextura(renderer, "assets/laberinto/horizontal10.png");
    texturaHorizontal13 = crearTextura(renderer, "assets/laberinto/horizontal13.png");
    texturaHorizontal14 = crearTextura(renderer, "assets/laberinto/horizontal14.png");
    texturaEsquina3 = crearTextura(renderer, "assets/laberinto/esquina3.png");
    texturaEsquina4 = crearTextura(renderer, "assets/laberinto/esquina4.png");
    texturaEsquina5 = crearTextura(renderer, "assets/laberinto/esquina5.png");
    texturaEsquina6 = crearTextura(renderer, "assets/laberinto/esquina6.png");
    texturaVertical11 = crearTextura(renderer, "assets/laberinto/vertical11.png");
    texturaVertical12 = crearTextura(renderer, "assets/laberinto/vertical12.png");
    texturaComida = crearTextura(renderer, "assets/otros/comida.png");

    if (!texturaVertical || !texturaHorizontal || !texturaEsquina3 || !texturaEsquina4 || !texturaEsquina5 ||
        !texturaEsquina6 || !texturaHorizontal9 || !texturaHorizontal10 || !texturaVertical11 || !texturaVertical12 || !texturaHorizontal13 ||
        !texturaHorizontal14 || !texturaVertical7 || !texturaVertical8 || !texturaComida) {
        printf("Error al cargar las texturas: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    texturaAmarillo = crearTextura(renderer, "assets/fantasmas/fantasma-amarillo.png");
    texturaCeleste = crearTextura(renderer, "assets/fantasmas/fantasma-celeste.png");
    texturaRoja = crearTextura(renderer, "assets/fantasmas/fantasma-rojo.png");
    texturaRosa = crearTextura(renderer, "assets/fantasmas/fantasma-rosa.png");

}

void actualizarTexturaPacman() {

    if (movimientoX == 0 && movimientoY == 0) {
        texturaPacmanActual = pacmanBocaCerrada;  
        return;
    }

    SDL_Texture* texturaAbierta = pacmanDer;
    SDL_Texture* texturaCerrada = pacmanBocaCerrada;

    if (movimientoX > 0) {
        texturaAbierta = pacmanDer;
        texturaCerrada = pacmanBocaCerrada;
    }
    else if (movimientoX < 0) {
        texturaAbierta = pacmanIzq;
        texturaCerrada = pacmanBocaCerrada;
    }
    else if (movimientoY < 0) {
        texturaAbierta = pacmanArriba;
        texturaCerrada = pacmanBocaCerrada;
    }
    else if (movimientoY > 0) {
        texturaAbierta = pacmanAbajo;
        texturaCerrada = pacmanBocaCerrada;
    }

    texturaPacmanActual = (contadorBoca / 5) % 2 == 0 ? texturaAbierta : texturaCerrada;
    contadorBoca++;

}


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

    return ((laberinto[celY1][celX1] == 0 || laberinto[celY1][celX1] == 20 || laberinto[celY1][celX1] == -1 || laberinto[celY1][celX1] == -2 || laberinto[celY1][celX1] == -4) &&
        (laberinto[celY1][celX2] == 0 || laberinto[celY1][celX2] == 20 || laberinto[celY1][celX2] == -1 || laberinto[celY1][celX2] == -2 || laberinto[celY1][celX2] == -4) &&
        (laberinto[celY2][celX1] == 0 || laberinto[celY2][celX1] == 20 || laberinto[celY2][celX1] == -1 || laberinto[celY2][celX1] == -2 || laberinto[celY2][celX1] == -4) &&
        (laberinto[celY2][celX2] == 0 || laberinto[celY2][celX2] == 20 || laberinto[celY2][celX2] == -1 || laberinto[celY2][celX2] == -2 || laberinto[celY2][celX2] == -4));
}

void inicializarComida() {
    cantComida = 0;

    if (nivel > 1) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                if (laberinto[i][j] == -1) {
                    laberinto[i][j] = 0;
                }
                else if (laberinto[i][j] == -2) {
                    laberinto[i][j] = 20;
                }
            }
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (laberinto[i][j] == 0 || laberinto[i][j] == 20) {
                cantComida++;
            }
        }
    }
}

void inicializarFantasma(Fantasma* fantasma, int x, int y, SDL_Texture* textura) {
    fantasma->x = x;
    fantasma->y = y;
    fantasma->dx = 0;
    fantasma->dy = 0;
    fantasma->vivo = true;
    fantasma->textura = textura;
}

void crearFantasmas() {
    for (int i = 1; i < N; i++) {
        for (int j = 1; j < M; j++) {
            int x = j * CELDA_TAM + offsetX;
            int y = i * CELDA_TAM + offsetY;
            switch (laberinto[i][j]) {
            case 22: inicializarFantasma(&fantasmaAmarillo, x, y, texturaAmarillo);
                break;
            case 23: inicializarFantasma(&fantasmaCeleste, x, y, texturaCeleste);
                break;
            case 24: inicializarFantasma(&fantasmaRosa, x, y, texturaRosa);
                break;
            case 25: inicializarFantasma(&fantasmaRojo, x, y, texturaRoja);
                break;
            }

        }
    }
}

void moverFantasma(Fantasma* fantasma) {
    int nuevaX = fantasma->x + fantasma->dx;
    int nuevaY = fantasma->y + fantasma->dy;

    if (esPosicionValida(nuevaX, nuevaY)) {
        fantasma->x = nuevaX;
        fantasma->y = nuevaY;
    }
    else {
        // Cambiar dirección de manera aleatoria o con lógica específica
        fantasma->dx = -fantasma->dx;
        fantasma->dy = -fantasma->dy;
    }
}

void dibujarFantasma(SDL_Renderer* renderer, Fantasma* fantasma) {
    SDL_Rect rect = { fantasma->x, fantasma->y, CELDA_TAM, CELDA_TAM };
    SDL_RenderCopy(renderer, fantasma->textura, NULL, &rect);
}

bool colisionPacmanFantasma(Fantasma* fantasma) {
    return pacmanX == fantasma->x && pacmanY == fantasma->y;
}

void subirNivel() {


    nivel++;
    printf("GANÓ! Nivel %d\n", nivel);     

    texturaPacmanActual = pacmanDer;
    
    movimientoX = 0;
    movimientoY = 0;
    
    SDL_RenderCopy(renderer, texturaPacmanActual, NULL, &pacmanRect);
    SDL_RenderPresent(renderer);

    SDL_Delay(1000);

    inicializarComida();

    pacmanX = (ANCHO - PACMAN_TAM) / 2 - offsetX;
    pacmanY = 363 - offsetY;      

    powerUp = false;
    contadorPowerUp = 0;
    
    Mix_HaltMusic();

    if (!Mix_PlayingMusic()) {
        Mix_PlayMusic(musicaPrincipal, -1);
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

        Mix_PlayChannel(1, musicaComer, 0);

        if (laberinto[celdaY][celdaX] == 20) {
            Mix_HaltMusic();
            Mix_PlayMusic(musicaPowerUp, -1);

            if (musicaPowerUp) {
                printf("Reproduciendo la música Power Up...\n");
                Mix_PlayMusic(musicaPowerUp, -1);
            }
            else {
                printf("Error: La música de Power Up no se cargó correctamente.\n");
            }

            powerUp = true;
            printf("Power Up\n");
            contadorPowerUp = DURACION_POWERUP; 
            laberinto[celdaY][celdaX] = -2;
            puntaje += 50;
        }
        else {
            laberinto[celdaY][celdaX] = -1;
            puntaje += 10;
        }

        cantComida--;    

        if (cantComida == 0) {
            subirNivel();
        }
    }
    
    if (powerUp) {
        contadorPowerUp--;
        if (contadorPowerUp <= 0) {
            powerUp = false; 
            printf("Terminó power up\n");
            Mix_HaltMusic();
            Mix_PlayMusic(musicaPrincipal, -1);
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

void cerrarSDL() {

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

    Mix_FreeChunk(musicaComer);
    Mix_FreeMusic(musicaPowerUp);
    Mix_FreeMusic(musicaPrincipal);
    Mix_CloseAudio();

    SDL_Quit();

}

int main(int argc, char* argv[]) {

    SDL_Init(SDL_INIT_EVERYTHING); 

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Error de audio: %s\n", Mix_GetError());
        return 1;
    }

    Mix_VolumeMusic(MIX_MAX_VOLUME * 0.8);
    Mix_VolumeChunk(musicaComer, MIX_MAX_VOLUME * 0.3);


    musicaComer = Mix_LoadWAV("assets/musica/pacman-waka-waka.wav");
    musicaPrincipal = Mix_LoadMUS("assets/musica/pacman-siren.mp3");
    musicaPowerUp = Mix_LoadMUS("assets/musica/pacman-powerup3.mp3");

    if (!musicaComer || !musicaPrincipal || !musicaPowerUp) {
        printf("Error al cargar la música: %s\n", Mix_GetError());
        return 1;
    }  

    ventana = SDL_CreateWindow("Pac Man", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ANCHO, ALTURA, 0);
    renderer = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);

    if (!ventana || !renderer) {
        printf("Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    Mix_HaltMusic();
    Mix_PlayMusic(musicaPrincipal, -1);

    if (musicaPrincipal) {
        printf("Reproduciendo la música principal...\n");
        Mix_PlayMusic(musicaPrincipal, -1);
    }
    else {
        printf("Error: La música principal no se cargó correctamente.\n");
    }

    
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
    
    cargarTexturas(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    dibujarLaberinto(renderer, texturaVertical, texturaVertical7, texturaVertical8, texturaHorizontal, texturaEsquina3, texturaEsquina4, texturaEsquina5,
        texturaEsquina6, texturaHorizontal9, texturaHorizontal10, texturaVertical11, texturaVertical12, texturaHorizontal13, texturaHorizontal14, texturaComida);
    SDL_RenderPresent(renderer);

    crearFantasmas();
    dibujarFantasma(renderer, &fantasmaAmarillo);

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


        actualizarTexturaPacman();
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
        
        dibujarFantasma(renderer, &fantasmaAmarillo);
        dibujarFantasma(renderer, &fantasmaCeleste);
        dibujarFantasma(renderer, &fantasmaRosa);
        dibujarFantasma(renderer, &fantasmaRojo);
        
        mostrarPuntaje(renderer, font, puntaje);

        SDL_RenderPresent(renderer);               
        
        SDL_Delay(1000 / 50);
    }
    
    cerrarSDL();

    return 0;
}
