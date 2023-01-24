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
int threads[11] = {0}; // iesimo indice = iesima thread // se ao fim da execucao estiver preenchido de 1`s entao o sudoku esta correto

int correspondencias[] = {1,2,3,4,5,6,7,8,9}; // vetor de correspondecias para verificacao das linhas/colunas

/*
• Um thread para verificar se cada coluna contém os dígitos 1 a 9; - FEITO
• Um thread para verificar se cada linha contém os dígitos 1 a 9; - FEITO
• Nove threads para verificar se cada uma das subgrades 3×3 elementos contém os dígitos 1 a 9. - A FAZER
*/

// THREAD 1
void *verifica_colunas(void *struct_parametros){
    parametros *dados = (parametros *) struct_parametros;
    int numeroDeCorrespondencias = 0;

    int iteradorCorrespondencias = 0;
    int elemento = 0;

    for(int i = 0; i < ALTURA; i++){
        for(int j = 0; j < LARGURA; j++){ // percorre a coluna
            elemento = get_elem(matriz, j, i); // obtem o elemento atual da coluna

            while(iteradorCorrespondencias < 9){ // compara o elemento da coluna com o vetor de correspondencias
                if(elemento == correspondencias[iteradorCorrespondencias]){
                    numeroDeCorrespondencias++; // se encontrar uma correspondencia contabiliza
                }
                iteradorCorrespondencias++; // percorre o vetor de correspondencias
            }
            iteradorCorrespondencias = 0; // reseta o iterador
        }

        if(numeroDeCorrespondencias != 9){
            // se o numero de correspondencias for diferente de 9
            // uma coluna nao contem os digitos corretos
            threads[THREAD_1] = 0;
            return NULL; // existe uma coluna errada, retorna pois nao eh necessario continuar rodando
        }
        numeroDeCorrespondencias = 0; // reseta as correspondencias para a prox linha
    }
    // se passou por todas as colunas e não deu return dentro do for
    // entao as colunas estao corretas e pode dar return com 1 (ok)
    threads[THREAD_1] = 1;
}

// THREAD 2 - É a mesma logica de percorrer as colunas porém porcorre as linhas;
void *verifica_linhas(void *struct_parametros){
    parametros *dados = (parametros *) struct_parametros;
    int numeroDeCorrespondencias = 0;

    int iteradorCorrespondencias = 0;
    int elemento = 0;

    for(int i = 0; i < ALTURA; i++){
        for(int j = 0; j < LARGURA; j++){
            elemento = get_elem(matriz, i, j); // obtem o elemento atual da linha

            while(iteradorCorrespondencias < 9){ // compara o elemento da linha com o vetor de correspondencias
                if(elemento == correspondencias[iteradorCorrespondencias]){
                    numeroDeCorrespondencias++; // se encontrar uma correspondencia contabiliza
                }
                iteradorCorrespondencias++; // percorre o vetor de correspondencias
            }
            iteradorCorrespondencias = 0; // reseta o iterador
        }

        if(numeroDeCorrespondencias != 9){
            // se o numero de correspondencias for diferente de 9
            // uma linha nao contem os digitos corretos
            threads[THREAD_2] = 0;
            return NULL; // existe uma linha errada, retorna pois nao eh necessario continuar rodando
        }
        numeroDeCorrespondencias = 0; // reseta as correspondencias para a prox linha
    }
    // se passou por todas as linhas e não deu return dentro do for
    // entao as linhas estao corretas e pode dar return com 1 (ok)
    threads[THREAD_2] = 1;
}

// THREADS 3 A 11
void *verifca_grade(void *struct_parametros){
    parametros *dados = (parametros *) struct_parametros;
    // a fazer
}

int main(int argc, char **argv){
    matriz = read_file(argv[1]);
    
    pthread_t threadTeste; // teste
    parametros *parametrosTeste = aloca_parametros(0,0);// teste

    if(matriz != NULL){ // se a leitura do txt ocorreu ok 
        // iniciar as threads e fazer o processamento do tabuleiro

    
    // ******TESTES*********
        print_mat(matriz);// teste
        printf("\n");// teste
        pthread_create(&threadTeste, NULL, verifica_colunas, (void *)parametrosTeste);// teste
        pthread_join(threadTeste, NULL);// teste
    }
    int i = 0;// teste
    printf("\n\nIndices 11: ");// teste
    while(i < 11){// teste
        printf("%d ", threads[i]);// teste
        i++;
    }
    printf("\n");// teste

    // ******TESTES*********

    // liberar a memoria alocada
    free_mat(matriz);
    free(parametrosTeste); // teste
    return 0;
}
