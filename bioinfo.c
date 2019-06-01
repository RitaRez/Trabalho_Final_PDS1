#include "bioinfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <string.h>

// Lê o arquivo e retorna as sequencias em uma estrutura nomeada tabela
void lerArquivo(char* nomeArquivo){
	FILE *f;		
    Tabela tabela;
    
    char *id;
    int tamanho;
    char *sequencia;
				 	
	f = fopen(nomeArquivo, "r");
	if(f == NULL){
        printf("\nArquivo não encontrado\n");
        exit(1);
    } 

    fscanf(f, "%d\n", tabela.qtdSequencias);

    id = (char *) malloc ((tabela.qtdSequencias + 2) * sizeof(char));
	Celula *celulas = (Celula *) calloc(tabela.qtdSequencias, sizeof(Celula));
	for (int i = 0; i < tabela.qtdSequencias; i++) {
	    fscanf(f,"%s %d %s\n", id, tamanho, sequencia);
	    celulas[i] = lerCelula(id, tamanho, sequencia);
	}
    tabela.celulas = celulas;
	fclose(f);
    free(f);

	return tabela; 
}

void imprime_mat(int lin, int col, int **mat){
    for(int i = 0; i < lin; i++){
        for(int j = 0; j < col; j++)
            printf("%d ", mat[i][j]);
        printf("\n"); 
    }
}

int **monta_matriz(int lin, int col, int **mat){
    mat = (int **) calloc(lin, sizeof(int*));
    for(int i = 0; i < lin; i++)
        mat[i] = (int *) calloc(col, sizeof(int)); 
    return mat;    
}

void libera_matriz(int lin, int col, int **mat){
    for(int i = 0; i < lin; i++)
        free(mat[i]);
    free(mat);
}
void lcs(char *str, char *str2, int lin, int col){

    int **pontos, **ponteiros; 
    
    pontos = monta_matriz(lin, col, pontos);   
    ponteiros = monta_matriz(lin, col, ponteiros);
    
    for(int i = 0; i < lin; i++) {
        for(int j = 0; j < col; j++) {            
            if(i == 0 || j == 0) {
                continue;   
            } else if(str[i - 1] == str2[j - 1]) {
                pontos[i][j] = pontos[i - 1][j - 1] + 1;
                ponteiros[i][j] = diagonal;          
            } else { 
                if(pontos[i - 1][j] >= pontos[i][j - 1]){    
                    pontos[i][j] = pontos[i - 1][j];
                    ponteiros[i][j] = vertical;
                } else {
                    pontos[i][j] = pontos[i][j - 1];
                    ponteiros[i][j] = horizontal;
                }  
            }         
        }
    }  
    imprime_mat(lin, col, pontos);  
    libera_matriz(lin, col, pontos);
    libera_matriz(lin, col, ponteiros);
}