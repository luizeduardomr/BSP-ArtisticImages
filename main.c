#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>		// Para usar strings

#ifdef WIN32
#include <windows.h>    // Apenas para Windows
#endif

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>     // Funções da OpenGL
#include <GL/glu.h>    // Funções da GLU
#include <GL/glut.h>   // Funções da FreeGLUT
#endif

// SOIL é a biblioteca para leitura das imagens
#include "SOIL.h"

// Um pixel Pixel (24 bits)
typedef struct
{
    unsigned char r, g, b;
} Pixel;

// Uma imagem Pixel
typedef struct
{
    int width, height;
    Pixel* img;
} Img;

// Protótipos
void load(char* name, Img* pic);
void valida();

// Funções da interface gráfica e OpenGL NÃO MEXER
void init();
void draw();
void keyboard(unsigned char key, int x, int y);

// Largura e altura da janela
int width, height;

// Identificadores de textura
GLuint tex[2];

// As 2 imagens
Img pic[2];

// Imagem selecionada (0,1)
int sel;

// Carrega uma imagem para a struct Img
void load(char* name, Img* pic)
{
    int chan;
    pic->img = (Pixel*) SOIL_load_image(name, &pic->width, &pic->height, &chan, SOIL_LOAD_RGB);
    if(!pic->img)
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
        exit(1);
    }
    printf("Load: %d x %d x %d\n", pic->width, pic->height, chan);
}

//Basicamente modificar o main APENAS. Talvez alguma outra coisinha
int main(int argc, char** argv)
{
    if(argc < 1)
    {
        printf("artistic [im. entrada]\n");
        exit(1);
    }
    glutInit(&argc,argv);

    // Define do modo de operacao da GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // pic[0] -> imagem de entrada
    // pic[1] -> imagem de saida

    // Carrega a imagem
    load(argv[1], &pic[0]);

    width = pic[0].width;
    height = pic[0].height;

    // A largura e altura da imagem de saída são iguais às da imagem de entrada (0)
    pic[1].width  = pic[0].width;
    pic[1].height = pic[0].height;
    pic[1].img = calloc(pic[1].width * pic[1].height, 3); // W x H x 3 bytes (Pixel)

    // Especifica o tamanho inicial em pixels da janela GLUT
    glutInitWindowSize(width, height);

    // Cria a janela passando como argumento o titulo da mesma
    glutCreateWindow("Arte Computacional");

    // Registra a funcao callback de redesenho da janela de visualizacao
    glutDisplayFunc(draw);

    // Registra a funcao callback para tratamento das teclas ASCII
    glutKeyboardFunc (keyboard);

    // Exibe as dimensões na tela, para conferência
    printf("Origem  : %s %d x %d\n", argv[1], pic[0].width, pic[0].height);
    sel = 0; // entrada

    // Define a janela de visualizacao 2D
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0,width,height,0.0);
    glMatrixMode(GL_MODELVIEW);

    // Converte para interpretar como matriz
    Pixel (*in)[width] = (Pixel(*)[width]) pic[0].img;
    Pixel (*out)[width] = (Pixel(*)[width]) pic[1].img;

    // Aplica o algoritmo e gera a saida em out (pic[1].img)
    // ...
    // ...
    // Exemplo: copia apenas o componente vermelho para a saida

    //Generating random numbers
    /* int numSeeds = 1000;
    //int sementes[numSeeds][height][width];
    //char sementes[numSeeds][8];
    Pixel sementes[numSeeds];
    int i;
    for(i=0; i<numSeeds; i++)
    {
    int upperH = height;
    int lowerH = 0;
    int seedH = (rand() % (upperH - lowerH + 1)) + lowerH;
    int upperW = width;
    int lowerW = 0;
    int seedW = (rand() % (upperW - lowerW + 1)) + lowerW;
    out[seedH][seedW].r = in[seedH][seedW].r;
    out[seedH][seedW].g = in[seedH][seedW].g;
    out[seedH][seedW].b = in[seedH][seedW].b;
    sementes[i] = out[seedH][seedW];
    }

    */

    //Generating random numbers
    int numSeeds = 5000; //após o número 1163, começa a dar erro e a imagem não é gerada (não sei pq)
    int matriz[numSeeds][2];
    Pixel sementes[numSeeds];
    int i;
    for(i = 0; i<numSeeds; i++)
    {
        int upperH = height;
        int lowerH = 0;
        int seedH = (rand() % (upperH - lowerH )) + lowerH;
        int upperW = width;
        int lowerW = 0;
        int seedW = (rand() % (upperW - lowerW + 1)) + lowerW;
        out[seedH][seedW].r = in[seedH][seedW].r;
        out[seedH][seedW].g = in[seedH][seedW].g;
        out[seedH][seedW].b = in[seedH][seedW].b;
        sementes[i] = out[seedH][seedW];
        matriz[i][0] = seedH;
        matriz[i][1] = seedW;
    }

    /*for(i=0; i<numSeeds; i++)
    {
        printf("Height: %d - ", matriz[i][0]);
        printf("Width: %d; \n", matriz[i][1]);
    }
    */

    int indice = 0;
    int y;
    int x;
    int j;
    int menorDistancia;

    for(y=0; y<height; y++)
    {
        for(x=0; x<width; x++)
        {
            menorDistancia = 500;

            for(j=0; j<numSeeds; j++)
            {

                int x1 = matriz[j][1];
                int y1 = matriz[j][0];
                int distancia = sqrt(((x - x1) * (x - x1)) + ((y - y1) * (y - y1)));
                if(distancia<menorDistancia)
                {
                    menorDistancia = distancia;
                    indice = j;
                }
            }
            //printf("Menor distancia: %d; \n", menorDistancia);
            out[y][x].r = sementes[indice].r;
            out[y][x].g = sementes[indice].g;
            out[y][x].b = sementes[indice].b;
        }
    }


    /* for(i = 0; i<numSeeds; i++)
     {
         printf("Out = %x  ", sementes[i]);
     }
    */

    //  for(int y=0; y<height; y++)
    //     for(int x=0; x<width; x++)
    //      out[y][x].r = in[y][x].r;

    // Cria texturas em memória a partir dos pixels das imagens
    tex[0] = SOIL_create_OGL_texture((unsigned char*) pic[0].img, width, height, SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);
    tex[1] = SOIL_create_OGL_texture((unsigned char*) pic[1].img, width, height, SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

    // Entra no loop de eventos, não retorna
    glutMainLoop();
}

// Gerencia eventos de teclado
void keyboard(unsigned char key, int x, int y)
{
    if(key==27)
    {
        // ESC: libera memória e finaliza
        free(pic[0].img);
        free(pic[1].img);
        exit(1);
    }
    if(key >= '1' && key <= '2')
        // 1-2: seleciona a imagem correspondente (origem ou destino)
        sel = key - '1';
    glutPostRedisplay();
}

// Callback de redesenho da tela
void draw()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Preto
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // Para outras cores, veja exemplos em /etc/X11/Pixel.txt

    glColor3ub(255, 255, 255);  // branco

    // Ativa a textura corresponde à imagem desejada
    glBindTexture(GL_TEXTURE_2D, tex[sel]);
    // E desenha um retângulo que ocupa toda a tela
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);

    glTexCoord2f(0,0);
    glVertex2f(0,0);

    glTexCoord2f(1,0);
    glVertex2f(pic[sel].width,0);

    glTexCoord2f(1,1);
    glVertex2f(pic[sel].width, pic[sel].height);

    glTexCoord2f(0,1);
    glVertex2f(0,pic[sel].height);

    glEnd();
    glDisable(GL_TEXTURE_2D);

    // Exibe a imagem
    glutSwapBuffers();
}
