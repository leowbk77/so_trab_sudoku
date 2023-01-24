#ifndef _MAIN_THREAD_H_
#define _MAIN_THREAD_H_

/*
*   DEFINES
*/

#define THREAD_1 0
#define THREAD_2 1

#define THREAD_3 2
#define THREAD_4 3
#define THREAD_5 4
#define THREAD_6 5
#define THREAD_7 6
#define THREAD_8 7
#define THREAD_9 8
#define THREAD_10 9
#define THREAD_11 10

typedef struct parametros parametros; // definir dentro do .c

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