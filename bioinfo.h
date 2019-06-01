#include<stdio.h>
#include<stdlib.h>
#include <stdio_ext.h>
#include<string.h>

struct tipoCelula {
    char *id;
    char *sequencia;
    char *alinhamento;
    int semelhanca;
};

typedef struct tipoCelula Celula;

struct tipoTabela {
	int qtdSequencias;
    Celula *celulas;
};
typedef struct tipoTabela Tabela;

enum direcoes {
    diagonal = 1,  
    horizontal,
    vertical
} dir;


void imprime_mat(int lin, int col, int **mat);
int **monta_matriz(int lin, int col, int **mat);
void libera_matriz(int lin, int col, int **mat);
void lcs(char *str, char *str2, int lin, int col);