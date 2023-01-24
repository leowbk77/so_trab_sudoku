#ifndef _MAIN_THREAD_H_
#define _MAIN_THREAD_H_

/*
*   DEFINES
*/

#define THREAD_1 0
#define THREAD_2 1

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
void *verifca_grade(void *struct_parametros);

#endif