#include <stdio.h>
#include <stdlib.h>
#include "txt_input.h"

int read_file(char *caminho, int *matriz){
    FILE *txt = NULL; // arquivo de dados
    matriz = malloc(LARGURA * ALTURA * sizeof(int)); // aloca o espaco da matriz

    if(matriz != NULL){
        txt = fopen(caminho, "r");
        if(txt == NULL) return ERRO_ABERTURA_ARQUIVO;

        int posicao = 0;
        for(int i = 0; i < ALTURA; i++){
            for(int j = 0; j < LARGURA; j++){
                posicao = i * LARGURA + j;
                fscanf(txt, "%d", &matriz[posicao]);
            }
        }
        fclose(txt);
        return SUCESSO;
    } else return FALHA_ALOCACAO_MATRIZ;
}

int get_elem(int *matriz, int linha, int coluna){
    int posicao = linha * LARGURA + coluna;
    return matriz[posicao];
}

void print_mat(int *matriz){
    for(int i = 0; i < ALTURA; i++){
        for(int j = 0; j < LARGURA; j++){
            posicao = i * LARGURA + j;
            printf("%d ", matriz[posicao]);
        }
        printf("\n");
    }
}