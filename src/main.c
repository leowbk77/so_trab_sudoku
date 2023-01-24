#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "txt_input.h"
#include "main.h"

// Globais

int *matriz = NULL; // matriz acessivel a todas as threads
int threads[11] = {0}; // iesimo indice = iesima thread // se ao fim da execucao estiver preenchido de 1`s entao o sudoku esta correto
int correspondencias[] = {1,2,3,4,5,6,7,8,9}; // vetor de correspondecias para verificacao das linhas/colunas

// Parametros
struct parametros 
{
    int linha;
    int coluna;
    int numeroDaThread;
};

parametros *aloca_parametros(int linha, int coluna, int thread){
    parametros *dados = (parametros *) malloc(sizeof(parametros));
    if(dados != NULL){
        dados->linha = linha;
        dados->coluna = coluna;
        dados->numeroDaThread = thread;
        return dados;
    } else return NULL;
}

void set_parametros(parametros *dados , int linha, int coluna, int thread){
    if(dados != NULL){
        dados->linha = linha;
        dados->coluna = coluna;
        dados->numeroDaThread = thread;
    }
}

/*
• Um thread para verificar se cada coluna contém os dígitos 1 a 9; - FEITO
• Um thread para verificar se cada linha contém os dígitos 1 a 9; - FEITO
• Nove threads para verificar se cada uma das subgrades 3×3 elementos contém os dígitos 1 a 9. - A FAZER
*/

// THREAD 1
void *verifica_colunas(void *struct_parametros){
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
void *verifica_grade(void *struct_parametros){
    parametros *dados = (parametros *) struct_parametros;
    // a logica tbm eh parecida mas vai ser verificado um quadrante menor (3x3) dentro do tabuleiro
    // as vars de limite dizem até onde deve-se verificar com base nos parametros que foram passados
    // para saber por onde comecar
    int limiteDaColuna = dados->coluna + 3;
    int limiteDaLinha = dados->linha + 3;

    int numeroDeCorrespondencias = 0;
    int iteradorCorrespondencias = 0;
    int elemento = 0;

    for(int i = dados->linha; i < limiteDaLinha; i++){ // quadrante 3x3
        for(int j = dados->coluna; j < limiteDaColuna; j++){ // percorre a linha atual até o ultimo elemento do quadrante
            elemento = get_elem(matriz, i, j);
            
            while(iteradorCorrespondencias < 9){ // compara o elemento da linha com o vetor de correspondencias
                if(elemento == correspondencias[iteradorCorrespondencias]){
                    numeroDeCorrespondencias++; // se encontrar uma correspondencia contabiliza
                }
                iteradorCorrespondencias++; // percorre o vetor de correspondencias
            }
            iteradorCorrespondencias = 0; // reseta o iterador
        }
    }
    
    if(numeroDeCorrespondencias != 9){
        // se o numero de correspondencias no quadrante for diferente de 9
        // o quadrante esta errado
        threads[dados->numeroDaThread] = 0;
    } else threads[dados->numeroDaThread] = 1;
}

int main(int argc, char **argv){
    matriz = read_file(argv[1]);

    if(matriz != NULL){ // se a leitura do txt ocorreu ok 
        // iniciar as threads e fazer o processamento do tabuleiro
        pthread_t trabalhadores[N_TRABALHADORES];
        parametros *parametrosDosTrabalhadores[N_QUADRANTES];

        // cria as duas primeiras threads que verificam linhas e colunas
        trabalhadores[THREAD_1] = pthread_create(&trabalhadores[THREAD_1], NULL, verifica_colunas, NULL);
        trabalhadores[THREAD_2] = pthread_create(&trabalhadores[THREAD_2], NULL, verifica_linhas, NULL);

        // aloca os parametros dos trabalhadores com as coordenadas dos quadrantes
        parametrosDosTrabalhadores[THREAD_3] = aloca_parametros(0,0, THREAD_3);
        parametrosDosTrabalhadores[THREAD_4] = aloca_parametros(0,3, THREAD_4);
        parametrosDosTrabalhadores[THREAD_5] = aloca_parametros(0,6, THREAD_5);
        parametrosDosTrabalhadores[THREAD_6] = aloca_parametros(3,0, THREAD_6);
        parametrosDosTrabalhadores[THREAD_7] = aloca_parametros(3,3, THREAD_7);
        parametrosDosTrabalhadores[THREAD_8] = aloca_parametros(3,6, THREAD_8);
        parametrosDosTrabalhadores[THREAD_9] = aloca_parametros(6,0, THREAD_9);
        parametrosDosTrabalhadores[THREAD_10] = aloca_parametros(6,3, THREAD_10);
        parametrosDosTrabalhadores[THREAD_11] = aloca_parametros(6,6, THREAD_11);

        // cria as threads de trabalhadores dos quadrantes (comeca da thread3)
        for(int i = 2; i < N_TRABALHADORES; i++){
            trabalhadores[i] = pthread_create(&trabalhadores[i], NULL, verifica_grade, (void*)parametrosDosTrabalhadores[i]);
        }
        for(int i = 0; i < N_TRABALHADORES; i++){
            pthread_join(trabalhadores[i], NULL);
        }
        //printa a matriz
        print_mat(matriz);
    }

    printf("Resultados dos testes (Threads): "); // printa os resultados das threads
    printf("T1\tT2\tT3\tT4\tT5\tT6\tT7\tT8\tT9\tT10\tT11\n");
    for(int i = 0; i < N_TRABALHADORES; i++){
        printf("%d\t", threads[i]);
    }
    printf("\n");

    // liberar a memoria alocada
    free_mat(matriz); // libera a matriz
    for(int i = 0; i < N_TRABALHADORES; i++){
        free(parametrosDosTrabalhadores[i]);
    }

    return 0;
}
