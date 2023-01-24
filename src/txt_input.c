#include <stdio.h>
#include <stdlib.h>
#include "txt_input.h"

int *read_file(char *caminho){
    FILE *txt = NULL; // arquivo de dados
    int *matriz = malloc(LARGURA * ALTURA * sizeof(int)); // aloca o espaco da matriz

    if(matriz != NULL){
        txt = fopen(caminho, "r");
        if(txt == NULL){
            free(matriz);
            return NULL;
        }

        int posicao = 0;
        int valor = 0;
        for(int i = 0; i < ALTURA; i++){
            for(int j = 0; j < LARGURA; j++){
                posicao = i * LARGURA + j;
                fscanf(txt, "%d", &valor);
                matriz[posicao] = valor;
            }
        }
        fclose(txt);
        return matriz;
    } else return NULL;
}

int get_elem(int *matriz, int linha, int coluna){
    int posicao = linha * LARGURA + coluna;
    return matriz[posicao];
}

void print_mat(int *matriz){
    int posicao = 0;
    for(int i = 0; i < ALTURA; i++){
        for(int j = 0; j < LARGURA; j++){
            posicao = i * LARGURA + j;
            printf("%d ", matriz[posicao]);
        }
        printf("\n");
    }
}

void free_mat(int *matriz){
    if(matriz != NULL){
        free(matriz);
    }
}