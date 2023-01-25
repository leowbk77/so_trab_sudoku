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
            threads[0] = 0;
            pthread_exit(NULL); // existe uma coluna errada, retorna pois nao eh necessario continuar rodando
        }
        numeroDeCorrespondencias = 0; // reseta as correspondencias para a prox linha
    }
    // se passou por todas as colunas e não deu return dentro do for
    // entao as colunas estao corretas e pode dar return com 1 (ok)
    threads[0] = 1;
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
            threads[1] = 0;
            pthread_exit(NULL); // existe uma linha errada, retorna pois nao eh necessario continuar rodando
        }
        numeroDeCorrespondencias = 0; // reseta as correspondencias para a prox linha
    }
    // se passou por todas as linhas e não deu return dentro do for
    // entao as linhas estao corretas e pode dar return com 1 (ok)
    threads[1] = 1;
}

// THREADS 3 A 11 (indices 2 ate 10)
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
    int retornoDoCreateThread;
    pthread_t trabalhadores[N_TRABALHADORES];
    parametros *parametrosDosTrabalhadores[N_QUADRANTES];

    matriz = read_file(argv[1]); // le o arquivo de input
    if(matriz != NULL){ // se a leitura do txt ocorreu ok 
        // iniciar as threads e fazer o processamento do tabuleiro
        // aloca os parametros dos trabalhadores com as coordenadas dos quadrantes
        parametrosDosTrabalhadores[0] = aloca_parametros(0,0, 2); // Q1
        parametrosDosTrabalhadores[1] = aloca_parametros(0,3, 3); // Q2
        parametrosDosTrabalhadores[2] = aloca_parametros(0,6, 4); // Q3

        parametrosDosTrabalhadores[3] = aloca_parametros(3,0, 5); // Q4
        parametrosDosTrabalhadores[4] = aloca_parametros(3,3, 6); // Q5
        parametrosDosTrabalhadores[5] = aloca_parametros(3,6, 7); // Q6
        
        parametrosDosTrabalhadores[6] = aloca_parametros(6,0, 8); // Q7
        parametrosDosTrabalhadores[7] = aloca_parametros(6,3, 9); // Q8
        parametrosDosTrabalhadores[8] = aloca_parametros(6,6, 10);// Q9

        // cria as duas primeiras threads que verificam linhas e colunas
        retornoDoCreateThread = pthread_create(&trabalhadores[0], NULL, verifica_colunas, NULL);
        retornoDoCreateThread = pthread_create(&trabalhadores[1], NULL, verifica_linhas, NULL);

        // cria as threads de trabalhadores dos quadrantes (comeca da thread n3)
        int j = 0; // iterador dos parametros
        for(int i = 2; i < N_TRABALHADORES; i++){
            retornoDoCreateThread = pthread_create(&trabalhadores[i], NULL, verifica_grade, (void*)parametrosDosTrabalhadores[j]);
            j++;
        }// join
        for(int i = 0; i < N_TRABALHADORES; i++){
            pthread_join(trabalhadores[i], NULL);
        }
        //printa a matriz
        print_mat(matriz);
        // printa os resultados das threads
        printf("Resultados dos testes (Threads):\n"); 
        printf("T1 T2 T3 T4 T5 T6 T7 T8 T9 T10 T11\n");
        for(int i = 0; i < N_TRABALHADORES; i++){
            printf(" %d ", threads[i]);
        }
        printf("\n");
        // liberar a memoria alocada
        free_mat(matriz); // libera a matriz
        for(int i = 0; i < N_QUADRANTES; i++){
            free(parametrosDosTrabalhadores[i]); // libera os parametros
        }
    }
    return 0;
}
