#ifndef _TXT_INPUT_H_
#define _TXT_INPUT_H_

/*
*   DEFINES
*/

// tabuleiro 9x9
#define LARGURA 9
#define ALTURA 9

typedef struct parametros parametros; // definir dentro do .c

/*
*   FUNCOES
*/

/*
* Para acessar um elemento em alguma posicao da matriz:
* posicao = linha * 9 (largura) + coluna
* get_elem(); <- faz esse trabalho
*/

// retorna um array com os dados do arquivo no caminho especificado
int *read_file(char *caminho);
// retorna o valor armazenado na matriz da linha x coluna
int get_elem(int *matriz, int linha, int coluna);
// printa a matriz
void print_mat(int *matriz);

#endif