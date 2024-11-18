#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>

#define ANCHO 530
#define ALTURA 630
#define CELDA_TAM 25
#define PACMAN_TAM 20
#define COMIDA_TAM 12
#define COMIDA_TAM_ESP 24
#define N 23
#define M 19

#define MARGEN_SUPERIOR 20
#define ANCHO_LABERINTO (M * CELDA_TAM)
#define ALTURA_LABERINTO (N * CELDA_TAM)
#define DURACION_POWERUP 500 

typedef struct {
    int pacmanX, pacmanY;
    int xInicial, yInicial;
    int movimientoX, movimientoY;
    int vidas;
    SDL_Texture* textura;    
    bool powerUp;
} Pacman;


typedef struct {
    int x, y;               // Posición en el laberinto
    int movimientoX, movimientoY;             // Dirección de movimiento
    int xInicial, yInicial;  //la primera vez calcula su posición inicial con el contenido de la matriz y guarda para los reinicios de nivel y del juego
    bool powerUp;  
    bool muerto;
    bool regresando;
    SDL_Texture* textura; 
    SDL_Texture* texturaOriginal;
    SDL_Texture* texturaIzq;
    SDL_Texture* texturaArr;
    SDL_Texture* texturaAb;
    int pasosRestantes;
} Fantasma;

typedef struct {
    char direccion; // 'D', 'I', 'A', 'B' (Derecha, Izquierda, Arriba, Abajo).
    int pasos;      // Cantidad de pasos en esta dirección.
} Movimiento;

int comida, cantComida;
int offsetX = (ANCHO - ANCHO_LABERINTO) / 2;
int offsetY = MARGEN_SUPERIOR + (ALTURA - MARGEN_SUPERIOR - ALTURA_LABERINTO) / 2;

int puntaje = 0;
int contadorPowerUp = 0; 

int nivel = 1;
int contadorBoca = 0;
int contadorFantasma = 0;
bool reiniciar = false;
int actualizacionesPorLlamada = 1;

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
    {3, 2, 2, 6, 0, 12, -4, 3, 2, 15, 2, 4, -4, 12, 0, 5, 2, 2, 4},
    {1, 0, 0, 0, 0, -4, -4, 1, 26, 26, 26, 1, -4, -4, 0, 0, 0, 0, 1},
    {5, 2, 2, 4, 0, 11, -4, 1, 22, 23, 24, 1, -4, 11, 0, 3, 2, 2, 6},
    {21, 21, 21, 1, 0, 1, -4, 5, 2, 2, 2, 6, -4, 1, 0, 1, 21, 21, 21},
    {21, 21, 21, 1, 0, 1, -4, -4, -4, -3, -4, -4, -4, 1, 0, 1, 21, 21, 21},
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


int div = 20;
int totalMovimientosRojo, totalMovimientosAmarillo, totalMovimientosCeleste, totalMovimientosRosa;
int indiceActualRojo, indiceActualAmarillo, indiceActualCeleste, indiceActualRosa;

Movimiento caminoRojo[13]; 
Movimiento caminoAmarillo[16];
Movimiento caminoCeleste[25];
Movimiento caminoRosa[28];


SDL_Window* ventana;
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
SDL_Texture* texturaHorizontal15;
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

SDL_Texture* texturaAmarilloIzq;
SDL_Texture* texturaRosaIzq;
SDL_Texture* texturaCelesteIzq;
SDL_Texture* texturaRojaIzq;

SDL_Texture* texturaAmarilloArr;
SDL_Texture* texturaRosaArr;
SDL_Texture* texturaCelesteArr;
SDL_Texture* texturaRojaArr;

SDL_Texture* texturaAmarilloAb;
SDL_Texture* texturaRosaAb;
SDL_Texture* texturaCelesteAb;
SDL_Texture* texturaRojaAb;

SDL_Renderer* renderer;
Mix_Chunk* musicaComer;
Mix_Music* musicaInicio;
Mix_Music* musicaPrincipal;
Mix_Music* musicaPowerUp;
Mix_Music* musicaMuerte;
Mix_Chunk* musicaOjosPosicionInicial;
Mix_Chunk* musicaComerFantasma;
Mix_Music* musicaCambioNivel;
TTF_Font* font;
SDL_Rect pacmanRect;
Fantasma fantasmaRojo, fantasmaRosa, fantasmaCeleste, fantasmaAmarillo;
Pacman pacman;


SDL_Texture* crearTextura(const char* path) {
    SDL_Surface* tempSurface = IMG_Load(path);
    SDL_Texture* textura = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return textura;
}

void cargarTexturas() {

    //texturas pacman
    pacmanDer = crearTextura("assets/pacman/pacmanDer.png");
    pacmanIzq = crearTextura("assets/pacman/pacmanIzq.png");
    pacmanArriba = crearTextura("assets/pacman/pacmanArriba.png");
    pacmanAbajo = crearTextura( "assets/pacman/pacmanAbajo.png");
    pacmanBocaCerrada = crearTextura("assets/pacman/pacmanBocaCerrada.png");

    if (!pacmanDer || !pacmanIzq || !pacmanArriba || !pacmanAbajo || !pacmanBocaCerrada) {
        printf("Error al cargar las texturas de Pacman: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    //texturas laberinto
    texturaVertical = crearTextura("assets/laberinto/vertical.png");
    texturaVertical7 = crearTextura("assets/laberinto/vertical7.png");
    texturaVertical8 = crearTextura("assets/laberinto/vertical8.png");
    texturaHorizontal = crearTextura("assets/laberinto/horizontal.png");
    texturaHorizontal9 = crearTextura("assets/laberinto/horizontal9.png");
    texturaHorizontal10 = crearTextura("assets/laberinto/horizontal10.png");
    texturaHorizontal13 = crearTextura("assets/laberinto/horizontal13.png");
    texturaHorizontal14 = crearTextura("assets/laberinto/horizontal14.png");
    texturaEsquina3 = crearTextura("assets/laberinto/esquina3.png");
    texturaEsquina4 = crearTextura("assets/laberinto/esquina4.png");
    texturaEsquina5 = crearTextura("assets/laberinto/esquina5.png");
    texturaEsquina6 = crearTextura("assets/laberinto/esquina6.png");
    texturaVertical11 = crearTextura("assets/laberinto/vertical11.png");
    texturaVertical12 = crearTextura("assets/laberinto/vertical12.png");
    texturaHorizontal15 = crearTextura("assets/laberinto/horizontal15.png");
    texturaComida = crearTextura("assets/otros/comida.png");

    if (!texturaVertical || !texturaHorizontal || !texturaEsquina3 || !texturaEsquina4 || !texturaEsquina5 ||
        !texturaEsquina6 || !texturaHorizontal9 || !texturaHorizontal10 || !texturaVertical11 || !texturaVertical12 || !texturaHorizontal13 ||
        !texturaHorizontal14 || !texturaHorizontal15 || !texturaVertical7 || !texturaVertical8 || !texturaComida) {
        printf("Error al cargar las texturas del laberinto: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    //texturas fantasmas
    texturaAmarillo = crearTextura("assets/fantasmas/fantasma-amarillo.png");
    texturaCeleste = crearTextura("assets/fantasmas/fantasma-celeste.png");
    texturaRoja = crearTextura("assets/fantasmas/fantasma-rojo.png");
    texturaRosa = crearTextura("assets/fantasmas/fantasma-rosa.png");

    texturaAmarilloIzq = crearTextura("assets/fantasmas/fantasma-amarillo-izq.png");
    texturaCelesteIzq = crearTextura("assets/fantasmas/fantasma-celeste-izq.png");
    texturaRojaIzq = crearTextura("assets/fantasmas/fantasma-rojo-izq.png");
    texturaRosaIzq = crearTextura("assets/fantasmas/fantasma-rosa-izq.png");

    texturaAmarilloArr = crearTextura("assets/fantasmas/fantasma-amarillo-arr.png");
    texturaCelesteArr = crearTextura("assets/fantasmas/fantasma-celeste-arr.png");
    texturaRojaArr = crearTextura("assets/fantasmas/fantasma-rojo-arr.png");
    texturaRosaArr = crearTextura("assets/fantasmas/fantasma-rosa-arr.png");

    texturaAmarilloAb = crearTextura("assets/fantasmas/fantasma-amarillo-ab.png");
    texturaCelesteAb = crearTextura("assets/fantasmas/fantasma-celeste-ab.png");
    texturaRojaAb = crearTextura("assets/fantasmas/fantasma-rojo-ab.png");
    texturaRosaAb = crearTextura("assets/fantasmas/fantasma-rosa-ab.png");

    if (!texturaAmarillo || !texturaCeleste || !texturaRosa || !texturaRoja || 
        !texturaAmarilloIzq || !texturaCelesteIzq || !texturaRosaIzq || !texturaRojaIzq ||
        !texturaAmarilloArr || !texturaCelesteArr || !texturaRosaArr || !texturaRojaArr ||
        !texturaAmarilloAb || !texturaCelesteAb || !texturaRosaAb || !texturaRojaAb) {
        printf("Error al cargar las texturas de los fantasmas: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

}

void actualizarTexturaPacman() {

    if (pacman.movimientoX == 0 && pacman.movimientoY == 0) {
        pacman.textura = pacmanBocaCerrada;  
        return;
    }

    SDL_Texture* texturaAbierta = pacmanDer;
    SDL_Texture* texturaCerrada = pacmanBocaCerrada;

    if (pacman.movimientoX > 0) {
        texturaAbierta = pacmanDer;
        texturaCerrada = pacmanBocaCerrada;
    }
    else if (pacman.movimientoX < 0) {
        texturaAbierta = pacmanIzq;
        texturaCerrada = pacmanBocaCerrada;
    }
    else if (pacman.movimientoY < 0) {
        texturaAbierta = pacmanArriba;
        texturaCerrada = pacmanBocaCerrada;
    }
    else if (pacman.movimientoY > 0) {
        texturaAbierta = pacmanAbajo;
        texturaCerrada = pacmanBocaCerrada;
    }

    pacman.textura = (contadorBoca / 5) % 2 == 0 ? texturaAbierta : texturaCerrada;
    contadorBoca++;

}


void dibujarLaberinto() {    

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
                case 15: SDL_RenderCopy(renderer, texturaHorizontal15, NULL, &rect);
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
            pacman.movimientoX = 0;
            pacman.movimientoY = - CELDA_TAM / 10;
            break;
        case SDLK_DOWN:
            pacman.movimientoX = 0;
            pacman.movimientoY = CELDA_TAM / 10;
            break;
        case SDLK_LEFT:
            pacman.movimientoX = -CELDA_TAM / 10;
            pacman.movimientoY = 0;
            break;
        case SDLK_RIGHT:
            pacman.movimientoX = CELDA_TAM / 10;
            pacman.movimientoY = 0;
            break;
    }

}

bool esPosicionValida(int x, int y) {
    int celX1 = x / CELDA_TAM;
    int celY1 = y / CELDA_TAM;
    int celX2 = (x + PACMAN_TAM - 1) / CELDA_TAM;
    int celY2 = (y + PACMAN_TAM - 1) / CELDA_TAM;

    return ((laberinto[celY1][celX1] == 0 || laberinto[celY1][celX1] == 20 || laberinto[celY1][celX1] == -1 || laberinto[celY1][celX1] == -2 || laberinto[celY1][celX1] == -4 || laberinto[celY1][celX1] == 25 || laberinto[celY1][celX1] == -3) &&
        (laberinto[celY1][celX2] == 0 || laberinto[celY1][celX2] == 20 || laberinto[celY1][celX2] == -1 || laberinto[celY1][celX2] == -2 || laberinto[celY1][celX2] == -4 || laberinto[celY1][celX2] == 25 || laberinto[celY1][celX2] == -3) &&
        (laberinto[celY2][celX1] == 0 || laberinto[celY2][celX1] == 20 || laberinto[celY2][celX1] == -1 || laberinto[celY2][celX1] == -2 || laberinto[celY2][celX1] == -4 || laberinto[celY2][celX1] == 25 || laberinto[celY2][celX1] == -3) &&
        (laberinto[celY2][celX2] == 0 || laberinto[celY2][celX2] == 20 || laberinto[celY2][celX2] == -1 || laberinto[celY2][celX2] == -2 || laberinto[celY2][celX2] == -4 || laberinto[celY2][celX2] == 25 || laberinto[celY2][celX2] == -3));
}


void inicializarComida() {
    cantComida = 0;

    if (nivel > 1 || reiniciar) {
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

void inicializarPacman(Pacman* pacman, SDL_Texture* textura) {
    pacman->vidas = 3;
    pacman->textura = textura;
    pacman->powerUp = false;
    pacman->textura = pacmanBocaCerrada;
    pacman->xInicial = (ANCHO - PACMAN_TAM) / 2 - offsetX;
    pacman->yInicial = 363 - offsetY;

    if (!pacman->textura) {
        printf("Error al cargar la textura: %s \n", SDL_GetError());
        SDL_Quit();
        return;
    }


    SDL_RenderCopy(renderer, pacman->textura, NULL, &pacmanRect);
}


void crearCaminos() {

    caminoRojo[0] = (Movimiento){ 'D', 4 * div};
    caminoRojo[1] = (Movimiento){ 'B', 8 * div};
    caminoRojo[2] = (Movimiento){ 'I', 3 * div};
    caminoRojo[3] = (Movimiento){ 'B', 3 * div};
    caminoRojo[4] = (Movimiento){ 'I', 7 * div};
    caminoRojo[5] = (Movimiento){ 'A', 20 * div};
    caminoRojo[6] = (Movimiento){ 'D', 5 * div };
    caminoRojo[7] = (Movimiento){ 'B', 4 * div };
    caminoRojo[8] = (Movimiento){ 'D', 5 * div };
    caminoRojo[9] = (Movimiento){ 'B', 2 * div };
    caminoRojo[10] = (Movimiento){ 'I', 3 * div };
    caminoRojo[11] = (Movimiento){ 'B', 3 * div };
    caminoRojo[12] = (Movimiento){ 'I', 1 * div };
    totalMovimientosRojo = sizeof(caminoRojo) / sizeof(caminoRojo[0]);


    caminoAmarillo[0] = (Movimiento){ 'A', 1 * div };
    caminoAmarillo[1] = (Movimiento){ 'D', 1 * div };
    caminoAmarillo[2] = (Movimiento){ 'A', 3 * div };
    caminoAmarillo[3] = (Movimiento){ 'I', 3 * div };
    caminoAmarillo[4] = (Movimiento){ 'B', 7 * div };
    caminoAmarillo[5] = (Movimiento){ 'D', 7 * div };
    caminoAmarillo[6] = (Movimiento){ 'B', 2 * div };
    caminoAmarillo[7] = (Movimiento){ 'I', 3 * div };
    caminoAmarillo[8] = (Movimiento){ 'B', 2 * div };
    caminoAmarillo[9] = (Movimiento){ 'D', 5 * div };
    caminoAmarillo[10] = (Movimiento){ 'A', 8 * div };
    caminoAmarillo[11] = (Movimiento){ 'I', 2 * div };
    caminoAmarillo[12] = (Movimiento){ 'A', 3 * div };
    caminoAmarillo[13] = (Movimiento){ 'I', 4 * div };
    caminoAmarillo[14] = (Movimiento){ 'B', 4 * div };
    caminoAmarillo[15] = (Movimiento){ 'I', 1 * div };
   
    totalMovimientosAmarillo = sizeof(caminoAmarillo) / sizeof(caminoAmarillo[0]);

    caminoCeleste[0] = (Movimiento){ 'A', 4 * div };
    caminoCeleste[1] = (Movimiento){ 'D', 4 * div };
    caminoCeleste[2] = (Movimiento){ 'B', 3 * div };
    caminoCeleste[3] = (Movimiento){ 'D', 2 * div };
    caminoCeleste[4] = (Movimiento){ 'B', 6 * div };
    caminoCeleste[5] = (Movimiento){ 'D', 4 * div };
    caminoCeleste[6] = (Movimiento){ 'B', 2 * div };
    caminoCeleste[7] = (Movimiento){ 'I', 1 * div };
    caminoCeleste[8] = (Movimiento){ 'B', 3 * div };
    caminoCeleste[9] = (Movimiento){ 'D', 1 * div };
    caminoCeleste[10] = (Movimiento){ 'B', 2 * div };
    caminoCeleste[11] = (Movimiento){ 'I', 20 * div };
    caminoCeleste[12] = (Movimiento){ 'A', 2 * div };
    caminoCeleste[13] = (Movimiento){ 'D', 4 * div };
    caminoCeleste[14] = (Movimiento){ 'A', 19 * div };
    caminoCeleste[15] = (Movimiento){ 'D', 7 * div };
    caminoCeleste[16] = (Movimiento){ 'A', 4 * div };
    caminoCeleste[17] = (Movimiento){ 'D', 9 * div };
    caminoCeleste[18] = (Movimiento){ 'B', 4 * div };
    caminoCeleste[19] = (Movimiento){ 'I', 6 * div };
    caminoCeleste[20] = (Movimiento){ 'B', 3 * div };
    caminoCeleste[21] = (Movimiento){ 'I', 3 * div };
    caminoCeleste[22] = (Movimiento){ 'B', 2 * div };
    caminoCeleste[23] = (Movimiento){ 'I', 1 * div };
    caminoCeleste[24] = (Movimiento){ 'B', 4 * div };

    totalMovimientosCeleste = sizeof(caminoCeleste) / sizeof(caminoCeleste[0]);


    caminoRosa[0] = (Movimiento){ 'A', 2 * div };
    caminoRosa[1] = (Movimiento){ 'I', 1 * div };
    caminoRosa[2] = (Movimiento){ 'A', 2 * div };
    caminoRosa[3] = (Movimiento){ 'I', 1 * div };
    caminoRosa[4] = (Movimiento){ 'A', 2 * div };
    caminoRosa[5] = (Movimiento){ 'I', 3 * div };
    caminoRosa[6] = (Movimiento){ 'A', 3 * div };
    caminoRosa[7] = (Movimiento){ 'I', 3 * div };
    caminoRosa[8] = (Movimiento){ 'B', 3 * div };
    caminoRosa[9] = (Movimiento){ 'I', 3 * div };
    caminoRosa[10] = (Movimiento){ 'A', 7 * div };
    caminoRosa[11] = (Movimiento){ 'D', 8 * div };
    caminoRosa[12] = (Movimiento){ 'B', 4 * div };
    caminoRosa[13] = (Movimiento){ 'D', 3 * div };
    caminoRosa[14] = (Movimiento){ 'A', 4 * div };
    caminoRosa[15] = (Movimiento){ 'D', 9 * div };
    caminoRosa[16] = (Movimiento){ 'B', 6 * div };
    caminoRosa[17] = (Movimiento){ 'I', 4 * div };
    caminoRosa[18] = (Movimiento){ 'B', 6 * div };
    caminoRosa[19] = (Movimiento){ 'D', 3 * div };
    caminoRosa[20] = (Movimiento){ 'I', 3 * div };
    caminoRosa[21] = (Movimiento){ 'B', 6 * div };
    caminoRosa[22] = (Movimiento){ 'I', 2 * div };
    caminoRosa[23] = (Movimiento){ 'A', 9 * div };
    caminoRosa[24] = (Movimiento){ 'I', 4 * div };
    caminoRosa[25] = (Movimiento){ 'B', 2 * div };
    caminoRosa[26] = (Movimiento){ 'D', 1 * div };
    caminoRosa[27] = (Movimiento){ 'B', 2 * div };

    totalMovimientosRosa = sizeof(caminoRosa) / sizeof(caminoRosa[0]);
}

void inicializarFantasma(Fantasma* fantasma, int x, int y, SDL_Texture* textura, SDL_Texture* texturaIzq, SDL_Texture* texturaArr, SDL_Texture* texturaAb) {
    fantasma->x = x;
    fantasma->y = y;
    fantasma->powerUp = false;
    fantasma->textura = textura;
    fantasma->texturaIzq = texturaIzq;
    fantasma->texturaArr = texturaArr;
    fantasma->texturaAb = texturaAb;
    fantasma->texturaOriginal = textura;
    fantasma->muerto = false;
    fantasma->regresando = false;
    fantasma->pasosRestantes = 0;
    fantasma->xInicial = x;
    fantasma->yInicial = y;
}

void crearFantasmas() {
    for (int i = 1; i < N; i++) {
        for (int j = 1; j < M; j++) {
            int x = j * CELDA_TAM + offsetX;
            int y = i * CELDA_TAM + offsetY;
            switch (laberinto[i][j]) {
            case 22: inicializarFantasma(&fantasmaAmarillo, x, y, texturaAmarillo, texturaAmarilloIzq, texturaAmarilloArr, texturaAmarilloAb);
                break;
            case 23: inicializarFantasma(&fantasmaCeleste, x, y, texturaCeleste, texturaCelesteIzq, texturaCelesteArr, texturaCelesteAb);
                break;
            case 24: inicializarFantasma(&fantasmaRosa, x, y, texturaRosa, texturaRosaIzq, texturaRosaArr, texturaRosaAb);
                break;
            case 25: inicializarFantasma(&fantasmaRojo, x, y, texturaRoja, texturaRojaIzq, texturaRojaArr, texturaRojaAb);
                break;
            }

        }
    }
}


void moverFantasma(Fantasma* fantasma, Movimiento* movimientos, int* indiceActual, int totalMovimientos) {    
        
    if (fantasma->muerto) {
        *indiceActual = 0;
        fantasma->pasosRestantes = 0;
        fantasma->muerto = false;
    }

    for (int i = 0; i < actualizacionesPorLlamada; i++) {
        // Si no hay pasos pendientes, toma el siguiente movimiento.
        if (fantasma->pasosRestantes == 0) {
            if (*indiceActual >= totalMovimientos) {
                *indiceActual = 0; // No hay más movimientos.
            }
            // Configura la dirección actual según el movimiento.
            Movimiento movimiento = movimientos[*indiceActual];
            fantasma->pasosRestantes = movimiento.pasos;

            switch (movimiento.direccion) {
            case 'D': // Derecha
                fantasma->movimientoX = CELDA_TAM / div;
                fantasma->movimientoY = 0;
                if(!fantasma->powerUp)
                    fantasma->textura = fantasma->texturaOriginal;
                break;
            case 'I': // Izquierda
                fantasma->movimientoX = -CELDA_TAM / div;
                fantasma->movimientoY = 0;
                if (!fantasma->powerUp)
                    fantasma->textura = fantasma->texturaIzq;
                break;
            case 'A': // Arriba
                fantasma->movimientoX = 0;
                fantasma->movimientoY = -CELDA_TAM / div;
                if (!fantasma->powerUp)
                    fantasma->textura = fantasma->texturaArr;
                break;
            case 'B': // Abajo
                fantasma->movimientoX = 0;
                fantasma->movimientoY = CELDA_TAM / div;
                if (!fantasma->powerUp)
                    fantasma->textura = fantasma->texturaAb;
                break;
            }
            (*indiceActual)++;
        }

        // Realiza un paso en la dirección actual.
        fantasma->x += fantasma->movimientoX;
        fantasma->y += fantasma->movimientoY;
        fantasma->pasosRestantes--;

        if (fantasma->pasosRestantes == 0) {
            break;
        }

        if (fantasma->pasosRestantes == 0) {
            fantasma->x = (fantasma->x / CELDA_TAM) * CELDA_TAM;
            fantasma->y = (fantasma->y / CELDA_TAM) * CELDA_TAM;

        }

    }

}


void dibujarFantasma(SDL_Renderer* renderer, Fantasma* fantasma) {
    SDL_Rect rect = { fantasma->x, fantasma->y, CELDA_TAM, CELDA_TAM };
    SDL_RenderCopy(renderer, fantasma->textura, NULL, &rect);
}

void reiniciarPosiciones() {

    SDL_Delay(1000);    

    pacman.pacmanX = pacman.xInicial;
    pacman.pacmanY = pacman.yInicial;

    pacman.movimientoX = 0;
    pacman.movimientoY = 0;

    Mix_PlayMusic(musicaPrincipal, -1);

    inicializarFantasma(&fantasmaRojo, fantasmaRojo.xInicial, fantasmaRojo.yInicial, texturaRoja, texturaRojaIzq, texturaRojaArr, texturaRojaAb);
    indiceActualRojo = 0;
    fantasmaRojo.pasosRestantes = 0;

    inicializarFantasma(&fantasmaRosa, fantasmaRosa.xInicial, fantasmaRosa.yInicial, texturaRosa, texturaRosaIzq, texturaRosaArr, texturaRosaAb);
    indiceActualRosa = 0; 
    fantasmaRosa.pasosRestantes = 0;

    inicializarFantasma(&fantasmaCeleste, fantasmaCeleste.xInicial, fantasmaCeleste.yInicial, texturaCeleste, texturaCelesteIzq, texturaCelesteArr, texturaCelesteAb);
    indiceActualCeleste = 0; 
    fantasmaCeleste.pasosRestantes = 0;

    inicializarFantasma(&fantasmaAmarillo, fantasmaAmarillo.xInicial, fantasmaAmarillo.yInicial, texturaAmarillo, texturaAmarilloIzq, texturaAmarilloArr, texturaAmarilloAb);
    indiceActualAmarillo = 0; 
    fantasmaAmarillo.pasosRestantes = 0;

 
    SDL_RenderClear(renderer);
    dibujarLaberinto();

    dibujarFantasma(renderer, &fantasmaRojo);
    dibujarFantasma(renderer, &fantasmaRosa);
    dibujarFantasma(renderer, &fantasmaCeleste);
    dibujarFantasma(renderer, &fantasmaAmarillo);
    
    SDL_RenderCopy(renderer, pacman.textura, NULL, &pacmanRect);
    SDL_RenderPresent(renderer);

}


void animarMuertePacman() {

    int duracionMusica = 2000;
    int nroSprites = 8;
    int tiempoPorSprite = duracionMusica / nroSprites;

    SDL_Texture* spritesMuerte[8];
    spritesMuerte[0] = crearTextura("assets/pacman/pacmanBocaCerrada.png");
    spritesMuerte[1] = crearTextura("assets/pacman/pacmanBocaArriba.png");
    spritesMuerte[2] = crearTextura("assets/pacman/pacmanMuerte3.png");
    spritesMuerte[3] = crearTextura("assets/pacman/pacmanMuerte4.png");
    spritesMuerte[4] = crearTextura("assets/pacman/pacmanMuerte5.png");
    spritesMuerte[5] = crearTextura("assets/pacman/pacmanMuerte6.png");
    spritesMuerte[6] = crearTextura("assets/pacman/pacmanMuerte7.png");
    spritesMuerte[7] = crearTextura("assets/pacman/pacmanMuerte8.png");

    Mix_PlayMusic(musicaMuerte, 1);


    for (int i = 0; i < nroSprites; i++) {
        SDL_RenderClear(renderer);

        dibujarLaberinto();


        dibujarFantasma(renderer, &fantasmaAmarillo);
        dibujarFantasma(renderer, &fantasmaCeleste);
        dibujarFantasma(renderer, &fantasmaRosa);
        dibujarFantasma(renderer, &fantasmaRojo);

        SDL_RenderCopy(renderer, spritesMuerte[i], NULL, &pacmanRect);
        SDL_RenderPresent(renderer);

        SDL_Delay(tiempoPorSprite);
    }

}

bool mostrarPantallaGameOver(TTF_Font* font) {
    SDL_Color color = { 255, 255, 255 };  // Color blanco para el texto
    SDL_Surface* surfaceMensaje = TTF_RenderText_Solid(font, "GAME OVER", color);
    SDL_Surface* surfaceOpcion1 = TTF_RenderText_Solid(font, "Presiona R para Reiniciar", color);
    SDL_Surface* surfaceOpcion2 = TTF_RenderText_Solid(font, "Presiona S para Salir", color);

    SDL_Texture* texturaMensaje = SDL_CreateTextureFromSurface(renderer, surfaceMensaje);
    SDL_Texture* texturaOpcion1 = SDL_CreateTextureFromSurface(renderer, surfaceOpcion1);
    SDL_Texture* texturaOpcion2 = SDL_CreateTextureFromSurface(renderer, surfaceOpcion2);

    SDL_Rect rectMensaje = { ANCHO / 2 - surfaceMensaje->w / 2, ALTURA / 3, surfaceMensaje->w, surfaceMensaje->h };
    SDL_Rect rectOpcion1 = { ANCHO / 2 - surfaceOpcion1->w / 2, ALTURA / 2, surfaceOpcion1->w, surfaceOpcion1->h };
    SDL_Rect rectOpcion2 = { ANCHO / 2 - surfaceOpcion2->w / 2, ALTURA / 2 + 40, surfaceOpcion2->w, surfaceOpcion2->h };

    SDL_FreeSurface(surfaceMensaje);
    SDL_FreeSurface(surfaceOpcion1);
    SDL_FreeSurface(surfaceOpcion2);

    bool seleccionHecha = false;    

    while (!seleccionHecha) {
        SDL_Event evento;
        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) {
                seleccionHecha = true;
                reiniciar = false;  // Para salir del juego
            }
            else if (evento.type == SDL_KEYDOWN) {
                if (evento.key.keysym.sym == SDLK_r) {  // Presiona R para reiniciar
                    seleccionHecha = true;
                    reiniciar = true;
                }
                else if (evento.key.keysym.sym == SDLK_s) {  // Presiona S para salir
                    seleccionHecha = true;
                    reiniciar = false;
                }
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texturaMensaje, NULL, &rectMensaje);
        SDL_RenderCopy(renderer, texturaOpcion1, NULL, &rectOpcion1);
        SDL_RenderCopy(renderer, texturaOpcion2, NULL, &rectOpcion2);
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }

    SDL_DestroyTexture(texturaMensaje);
    SDL_DestroyTexture(texturaOpcion1);
    SDL_DestroyTexture(texturaOpcion2);

    return reiniciar;
}

bool detectarColision(int xFantasma, int yFantasma) {

    int margen = 9;
    SDL_Rect pacmanRect = { pacman.pacmanX + offsetX + margen, pacman.pacmanY + offsetY, PACMAN_TAM - margen * 2, PACMAN_TAM };
    SDL_Rect fantasmaRect = { xFantasma, yFantasma, CELDA_TAM, CELDA_TAM };

    return SDL_HasIntersection(&pacmanRect, &fantasmaRect);

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
    SDL_DestroyTexture(pacman.textura);
    SDL_DestroyTexture(texturaComida);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);

    Mix_FreeChunk(musicaComer);
    Mix_FreeMusic(musicaInicio);
    Mix_FreeMusic(musicaPowerUp);
    Mix_FreeMusic(musicaPrincipal);
    Mix_FreeChunk(musicaComerFantasma);
    Mix_CloseAudio();

    IMG_Quit();
    SDL_Quit();

}

void reiniciarJuegoCompleto() {
    pacman.vidas = 3;
    puntaje = 0;
    nivel = 1;
    actualizacionesPorLlamada = 1;
    inicializarComida();
    reiniciarPosiciones();

}

void animarMuerteFantasma(Fantasma* fantasma, int puntajeGanado) {

    // Mostrar el puntaje ganado
    SDL_Color color = { 255, 255, 255 };
    char textoPuntaje[10];
    snprintf(textoPuntaje, sizeof(textoPuntaje), "+%d", puntajeGanado);

    SDL_Surface* surfacePuntaje = TTF_RenderText_Solid(font, textoPuntaje, color);
    SDL_Texture* texturaPuntaje = SDL_CreateTextureFromSurface(renderer, surfacePuntaje);
    SDL_FreeSurface(surfacePuntaje);

    SDL_Rect rectPuntaje = { fantasma->x, fantasma->y - 30, surfacePuntaje->w, surfacePuntaje->h };
    SDL_RenderCopy(renderer, texturaPuntaje, NULL, &rectPuntaje);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);
    SDL_DestroyTexture(texturaPuntaje);

    // Reproducir música de regreso y mover los ojos a la posición inicial
    Mix_PlayChannel(1, musicaOjosPosicionInicial, -1);

    int pasosX = (fantasma->xInicial - fantasma->x) / CELDA_TAM;
    int pasosY = (fantasma->yInicial - fantasma->y) / CELDA_TAM;

    while (fantasma->x != fantasma->xInicial || fantasma->y != fantasma->yInicial) {

        if (fantasma->x < fantasma->xInicial) fantasma->x += CELDA_TAM / div;
        else if (fantasma->x > fantasma->xInicial) fantasma->x -= CELDA_TAM / div;

        if (fantasma->y < fantasma->yInicial) fantasma->y += CELDA_TAM / div;
        else if (fantasma->y > fantasma->yInicial) fantasma->y -= CELDA_TAM / div;

        SDL_RenderClear(renderer);
        dibujarLaberinto();
        dibujarFantasma(renderer, fantasma);
        SDL_RenderPresent(renderer);
        SDL_Delay(1000 / 500);
    }
    
    Mix_HaltChannel(1);
    fantasma->textura = fantasma->texturaOriginal;

}


void muerteFantasma(Fantasma* fantasma) {
    fantasma->powerUp = false;
    fantasma->muerto = true;
    fantasma->regresando = true;

    SDL_Texture* texturaOjos = crearTextura("assets/fantasmas/ojos-fantasma.png");

    if (!texturaOjos) {
        printf("Error al cargar la textura de ojos: %s\n", SDL_GetError());
        return;
    }

    // Iniciar música de "comer fantasma"
    Mix_PlayChannel(1, musicaComerFantasma, 0);
    SDL_Delay(500);

    // Cambiar textura del fantasma a ojos
    fantasma->textura = texturaOjos;

    puntaje += 200;
    animarMuerteFantasma(fantasma, 200);

    SDL_DestroyTexture(texturaOjos);
}



void manejarColisiones(Fantasma* fantasma) {
    if (detectarColision(fantasma->x, fantasma->y)) {
        if (pacman.powerUp && fantasma->powerUp) {
            muerteFantasma(fantasma);
        }
        else {

            animarMuertePacman();
            pacman.vidas--;  

            if (pacman.vidas > 0) {
                reiniciarPosiciones();
            }
            else {
                bool reiniciarJuego = mostrarPantallaGameOver(font);
                if (reiniciarJuego) {
                    reiniciarJuegoCompleto();
                }
                else {
                    cerrarSDL();
                    exit(0);
                }
            }
        }
    }
}

void pausarJuegoMientrasSuena() {
    Mix_PlayMusic(musicaCambioNivel, 1); 
    
    while (Mix_PlayingMusic()) {
        SDL_Event evento;
        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) {

                Mix_HaltMusic();
                cerrarSDL();
                exit(0);
            }
        }
        SDL_Delay(100); 
    }
}

void subirNivel() {

    nivel++;
    actualizacionesPorLlamada++;

    pacman.powerUp = false;
    contadorPowerUp = 0;

    inicializarComida();
    reiniciarPosiciones();
    
    Mix_HaltMusic();

    pausarJuegoMientrasSuena();

    if (!Mix_PlayingMusic()) {
        Mix_PlayMusic(musicaPrincipal, -1);
    }

}


void moverPacman() {

    SDL_Texture* texturaPowerUpBlanca = crearTextura("assets/fantasmas/fantasma-blanco.png");
    SDL_Texture* texturaPowerUpAzul = crearTextura("assets/fantasmas/fantasma-azul.png");

    int nuevaX = pacman.pacmanX + pacman.movimientoX;
    int nuevaY = pacman.pacmanY + pacman.movimientoY;

    if (esPosicionValida(nuevaX, pacman.pacmanY)) {
        pacman.pacmanX = nuevaX;
    }
    if (esPosicionValida(pacman.pacmanX, nuevaY)) {
        pacman.pacmanY = nuevaY;
    }
    
    int celdaX = pacman.pacmanX / CELDA_TAM;
    int celdaY = pacman.pacmanY / CELDA_TAM;
    
    
    if (laberinto[celdaY][celdaX] == 0 || laberinto[celdaY][celdaX] == 20) {

        Mix_PlayChannel(1, musicaComer, 0);

        if (laberinto[celdaY][celdaX] == 20) {
            Mix_HaltMusic();
            Mix_PlayMusic(musicaPowerUp, -1);

            pacman.powerUp = true;
            fantasmaAmarillo.powerUp = true;
            fantasmaCeleste.powerUp = true;
            fantasmaRosa.powerUp = true;
            fantasmaRojo.powerUp = true;

            printf("Power Up\n");
            contadorPowerUp = DURACION_POWERUP; 
            laberinto[celdaY][celdaX] = -2;
            puntaje += 50;
            fantasmaAmarillo.textura = texturaPowerUpAzul;
            fantasmaCeleste.textura = texturaPowerUpAzul;
            fantasmaRosa.textura = texturaPowerUpAzul;
            fantasmaRojo.textura = texturaPowerUpAzul;

        }
        else {
            laberinto[celdaY][celdaX] = -1;
            puntaje += 10;
        }

        cantComida--;   
        printf("Cantidad de comida restante: %d\n", cantComida);
        
        if (cantComida == 0) {
            subirNivel();
        }
    }
    
    if (pacman.powerUp) {
        contadorPowerUp--;

        if (contadorPowerUp <= 150 && contadorPowerUp > 0) {
            if (fantasmaAmarillo.powerUp) {
                fantasmaAmarillo.textura = (contadorFantasma / 10) % 2 == 0 ? texturaPowerUpBlanca : texturaPowerUpAzul;
            }
            if (fantasmaCeleste.powerUp) {
                fantasmaCeleste.textura = (contadorFantasma / 10) % 2 == 0 ? texturaPowerUpBlanca : texturaPowerUpAzul;
            }
            if (fantasmaRosa.powerUp) {
                fantasmaRosa.textura = (contadorFantasma / 10) % 2 == 0 ? texturaPowerUpBlanca : texturaPowerUpAzul;
            }
            if (fantasmaRojo.powerUp) {
                fantasmaRojo.textura = (contadorFantasma / 10) % 2 == 0 ? texturaPowerUpBlanca : texturaPowerUpAzul;
            }

            contadorFantasma++;

        }

        if (contadorPowerUp <= 0) {
            pacman.powerUp = false; 

            fantasmaAmarillo.textura = fantasmaAmarillo.texturaOriginal;
            fantasmaCeleste.textura = fantasmaCeleste.texturaOriginal;
            fantasmaRosa.textura = fantasmaRosa.texturaOriginal;
            fantasmaRojo.textura = fantasmaRojo.texturaOriginal;

            fantasmaAmarillo.powerUp = false;
            fantasmaCeleste.powerUp = false;
            fantasmaRosa.powerUp = false;
            fantasmaRojo.powerUp = false;

            contadorFantasma = 0;

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

void mostrarNivelYVidas(SDL_Renderer* renderer, TTF_Font* fuente, int nivel, int vidas, SDL_Texture* texturaVida) {
    
    SDL_Color color = { 255, 255, 255 }; 
    char textoNivel[20];
    snprintf(textoNivel, sizeof(textoNivel), "Nivel: %d", nivel);

    SDL_Surface* surfaceNivel = TTF_RenderText_Solid(fuente, textoNivel, color);
    SDL_Texture* texturaNivel = SDL_CreateTextureFromSurface(renderer, surfaceNivel);
    SDL_FreeSurface(surfaceNivel);

    SDL_Rect rectNivel = { 300, 10, surfaceNivel->w, surfaceNivel->h };
    SDL_RenderCopy(renderer, texturaNivel, NULL, &rectNivel);
    SDL_DestroyTexture(texturaNivel);

    // Mostrar vidas
    SDL_Rect rectVida = { 400, 10, PACMAN_TAM, PACMAN_TAM };
    for (int i = 0; i < vidas; i++) {
        SDL_RenderCopy(renderer, texturaVida, NULL, &rectVida);
        rectVida.x += PACMAN_TAM + 5;  // Espaciado entre íconos
    }
}



int main(int argc, char* argv[]) {

    //srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error al inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("Error al inicializar SDL_image para PNG: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Error de audio: %s\n", Mix_GetError());
        return 1;
    }

    Mix_VolumeMusic(MIX_MAX_VOLUME * 0.8);
    Mix_VolumeChunk(musicaComer, MIX_MAX_VOLUME * 0.3);


    musicaComer = Mix_LoadWAV("assets/musica/pacman-waka-waka.wav");
    musicaInicio = Mix_LoadMUS("assets/musica/pacman-inicio.mp3");
    musicaPrincipal = Mix_LoadMUS("assets/musica/pacman-siren.mp3");
    musicaPowerUp = Mix_LoadMUS("assets/musica/pacman-powerup3.mp3");
    musicaMuerte = Mix_LoadMUS("assets/musica/pacman-muerte.mp3");
    musicaComerFantasma = Mix_LoadWAV("assets/musica/pacman-eating-ghost.wav");
    musicaOjosPosicionInicial = Mix_LoadWAV("assets/musica/ghost-ojos.wav");
    musicaCambioNivel = Mix_LoadMUS("assets/musica/pacman-cambio-nivel.mp3");
    

    if (!musicaComer || !musicaPrincipal || !musicaPowerUp || !musicaInicio || !musicaMuerte) {
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

    Mix_PlayMusic(musicaInicio, 1);
    
    if (TTF_Init() == -1) {
        printf("Error al inicializar SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    font = TTF_OpenFont("assets/fonts/FreeSans.ttf", 24);

    if (!font) {
        printf("Error al cargar la fuente: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }


    inicializarComida();
    
    cargarTexturas();

    inicializarPacman(&pacman, pacmanDer);

    pacman.pacmanX = pacman.xInicial;
    pacman.pacmanY = pacman.yInicial;


    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    crearFantasmas();

    pacmanRect.w = PACMAN_TAM;
    pacmanRect.h = PACMAN_TAM;
    pacmanRect.x = offsetX + pacman.pacmanX;
    pacmanRect.y = 457;

    SDL_RenderCopy(renderer, pacman.textura, NULL, &pacmanRect);
    SDL_RenderPresent(renderer);

    SDL_Event evento;
    
    bool playing = true;

    while (playing) {        

        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) {
                playing = false;
            }
        }

        actualizarTexturaPacman();
        moverPacman();

        pacmanRect.w = PACMAN_TAM;
        pacmanRect.h = PACMAN_TAM;
        pacmanRect.x = offsetX + pacman.pacmanX;
        pacmanRect.y = offsetY + pacman.pacmanY;

        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        dibujarLaberinto();

        SDL_RenderCopy(renderer, pacman.textura, NULL, &pacmanRect);

        SDL_RenderPresent(renderer);   

        if (!Mix_PlayingMusic()) {
            playing = false;
        }
        
        SDL_Delay(1000 / 50);
    }


    SDL_Delay(1000);
    

    Mix_PlayMusic(musicaPrincipal, -1);
    
    
    bool running = true;

    while (running) {

        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) {
                running = false;
            }
            else if (evento.type == SDL_KEYDOWN) {
                moverSegunTecla(evento);
            }
        }


        actualizarTexturaPacman();
        moverPacman();

        pacmanRect.w = PACMAN_TAM;
        pacmanRect.h = PACMAN_TAM;
        pacmanRect.x = offsetX + pacman.pacmanX;
        pacmanRect.y = offsetY + pacman.pacmanY;


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        dibujarLaberinto();

        SDL_RenderCopy(renderer, pacman.textura, NULL, &pacmanRect);
        
        crearCaminos();

        moverFantasma(&fantasmaRojo, caminoRojo, &indiceActualRojo, totalMovimientosRojo);
        dibujarFantasma(renderer, &fantasmaRojo);
        manejarColisiones(&fantasmaRojo);
        
        moverFantasma(&fantasmaAmarillo, caminoAmarillo, &indiceActualAmarillo, totalMovimientosAmarillo);
        dibujarFantasma(renderer, &fantasmaAmarillo);
        manejarColisiones(&fantasmaAmarillo);
        
        moverFantasma(&fantasmaCeleste, caminoCeleste, &indiceActualCeleste, totalMovimientosCeleste);
        dibujarFantasma(renderer, &fantasmaCeleste);
        manejarColisiones(&fantasmaCeleste);
        
        moverFantasma(&fantasmaRosa, caminoRosa, &indiceActualRosa, totalMovimientosRosa);
        dibujarFantasma(renderer, &fantasmaRosa);
        manejarColisiones(&fantasmaRosa); 

        mostrarPuntaje(renderer, font, puntaje);
        mostrarNivelYVidas(renderer, font, nivel, pacman.vidas, pacmanDer);

        SDL_RenderPresent(renderer);

        SDL_Delay(1000 / 60);

    }

    
    cerrarSDL();

    return 0;
}
