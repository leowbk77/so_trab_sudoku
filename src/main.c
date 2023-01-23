#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "txt_input.h"
#include "main.h"

// Parametros (ainda nao foi necessario)
// provavelmente para os quadrantes
struct parametros 
{
    int linha;
    int coluna;
};

parametros *aloca_parametros(int linha, int coluna){
    parametros *dados = (parametros *) malloc(sizeof(parametros));
    if(dados != NULL){
        dados->linha = linha;
        dados->coluna = coluna;
        return dados;
    } else return NULL;
}

void set_parametros(parametros *dados , int linha, int coluna){
    if(dados != NULL){
        dados->linha = linha;
        dados->coluna = coluna;
    }
}

// Globais

int *matriz = NULL; // matriz acessivel a todas as threads
int threads[11] = {0}; // iesimo indice = iesima thread // temp

int correspondencias[] = {1,2,3,4,5,6,7,8,9}; // vetor de correspondecias para verificacao das linhas/colunas

/*
• Um thread para verificar se cada coluna contém os dígitos 1 a 9;
• Um thread para verificar se cada linha contém os dígitos 1 a 9;
• Nove threads para verificar se cada uma das subgrades 3×3 elementos contém os dígitos 1 a 9.
*/

// THREAD 1
void *verifica_colunas(void *struct_parametros){
    parametros *dados = (parametros *) struct_parametros;
    int numeroDeCorrespondencias = 0;

    int iteradorCorrespondencias = 0;
    int elemento = 0;

    for(int i = 0; i < ALTURA; i++){
        for(int j = 0; j < LARGURA; j++){ // percorre a coluna
            elemento = get_elem(matriz, j, i); // consertar para usar i e j do jeito certo (coluna a coluna)(percorrendo todo tabuleiro)

            while(iteradorCorrespondencias < 9){ // compara o elemento da coluna com o vetor de correspondencias
                if(elemento == correspondencias[iteradorCorrespondencias]){
                    numeroDeCorrespondencias++; // se encontrar uma correspondencia contabiliza
                }
                iteradorCorrespondencias++; // percorre o vetor de correspondencias
            }

            iteradorCorrespondencias = 0; // reseta o iterador
            if(numeroDeCorrespondencias != 9){
                // se o numero de correspondencias for diferente de 9
                // uma coluna nao contem os digitos corretos
                threads[THREAD_1] = 0;
                return;
            }
        }
        numeroDeCorrespondencias = 0; // reseta as correspondencias para a prox linha
    }
    // se passou por todas as colunas e não deu return dentro do for
    // entao as colunas estao corretas e pode dar return com 1 (ok)
    threads[THREAD_1] = 1;
    return;
}

// THREAD 2
void *verifica_linhas(void *struct_parametros){
    parametros *dados = (parametros *) struct_parametros;
    // a logica eh a mesma do verifica colunas
}

// THREADS 3 A 11
void *verifca_grade(void *struct_parametros){
    parametros *dados = (parametros *) struct_parametros;
    // a fazer
}

int main(int argc, char **argv){
    matriz = read_file(argv[1]);
    if(matriz != NULL){ // se a leitura do txt ocorreu ok 
        // iniciar as threads e fazer o processamento do tabuleiro
    }
    free_mat(matriz);
    return 0;
}
