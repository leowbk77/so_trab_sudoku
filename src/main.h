#ifndef _MAIN_THREAD_H_
#define _MAIN_THREAD_H_

/*
*   DEFINES
*/
#define N_TRABALHADORES 11
#define N_QUADRANTES 9

#define THREAD_1 0
#define THREAD_2 1

#define THREAD_3 0
#define THREAD_4 1
#define THREAD_5 2
#define THREAD_6 3
#define THREAD_7 4
#define THREAD_8 5
#define THREAD_9 6
#define THREAD_10 7
#define THREAD_11 8

typedef struct parametros parametros; // definir dentro do .c

/* posicoes de inicio dos quadrantes (linha/coluna)
    Q1 (0,0)
    Q2 (0,3)
    Q3 (0,6)
    Q4 (3,0)
    Q5 (3,3)
    Q6 (3,6)
    Q7 (6,0)
    Q8 (6,3)
    Q9 (6,6)
*/

/*
*   FUNCOES
*/

// aloca a struct parametros que vao ser passados para as threads;
parametros *aloca_parametros(int linha, int coluna, int thread);
// seta os parametros de uma struct já criada
void set_parametros(parametros *dados , int linha, int coluna, int thread);


// verifica se uma coluna contém 1 a 9
void *verifica_colunas(void *struct_parametros);
// verifica se uma linha contém 1 a 9
void *verifica_linhas(void *struct_parametros);
// verifica as subgrades
void *verifica_grade(void *struct_parametros);

#endif