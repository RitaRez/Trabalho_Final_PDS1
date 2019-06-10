#include "bioinfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <string.h>

//Imprime celula
void imprime_celula(Celula c){
    printf("\nCaso mais semelhante:\n");
    printf("%d\nid%d\n%s\n%s\n\n\n", c.semelhanca, c.id, c.alinhamento1, c.alinhamento2);
}

//Imprime tabela
void imprime_tabela(Tabela t){
    FILE *fp;
    fp = fopen("output.txt", "w");
    for(int i = 0; i < t.qtdSequencias; i++)
        fprintf(fp, "%d\nid%d\n%s\n%s\n\n", t.celulas[i].semelhanca, t.celulas[i].id, t.celulas[i].alinhamento1, t.celulas[i].alinhamento2);
    fclose(fp);
    free(fp);
}

//Deixa a tabela em ordem de maior semelhancia
void ordena_tabela(Celula *a, int left, int right) {
    int i, j, x;
    Celula y;

    i = left;
    j = right;
    x = a[(left + right) / 2].semelhanca;

    while(i <= j) {
        while(a[i].semelhanca > x && i < right)
            i++;
        while(a[j].semelhanca < x && j > left)
            j--;
        if(i <= j) {
            y = a[i];
            a[i] = a[j];
            a[j] = y;
            i++;
            j--;
        }
    }

    if(j > left)
        ordena_tabela(a, left, j);
    if(i < right)
        ordena_tabela(a, i, right);
}

// Imprime a matriz de alinhamento, função não é mais utilizada
void imprime_mat(int lin, int col, int **mat){
    for(int i = 0; i < lin; i++){
        for(int j = 0; j < col; j++){
            if(mat[i][j] == 1)
                printf("\\ ");
            if(mat[i][j] == 2)
                printf("- ");
            if(mat[i][j] == 3)
                printf("| ");
        }
        printf("\n");
    }
}

//Aloca a matriz de alinhamento
int **monta_matriz(int lin, int col, int **mat){
    mat = (int **) calloc(lin, sizeof(int*));
    for(int i = 0; i < lin; i++)
        mat[i] = (int *) calloc(col, sizeof(int));
    return mat;
}

int **seta_matriz(int lin, int col, int **mat){
    for(int i = 0; i < lin; i++)
        for(int j = 0; j < col; j++)
            mat[i][j] = horizontal;
    return mat;
}

//Libera a memória utilizada pela matriz de alinhamento
void libera_matriz(int lin, int col, int **mat){
    for(int i = 0; i < lin; i++)
        free(mat[i]);
    free(mat);
}

// Gera primeiro alinhamento de uma sequencia espeficica com a primeira sequencia
Tabela gera_alinhamento(Tabela t, int index, int lin, int col, int **mat, char *str1, char *str2){

    int i = lin, j = col, tamanho = 0;
    while(i != 0 && j != 0){
        if(mat[i][j] == diagonal){
            i--;
            j--;
        } else if(mat[i][j] == vertical){
            i--;
        } else {
            j--;
        }
        tamanho++;
    }
    if(i == 0)
        tamanho += j;
    else
        tamanho += i;

    char *alinhamento1 = (char *) malloc (tamanho*sizeof(char));
    char *alinhamento2 = (char *) malloc (tamanho*sizeof(char));
    if(index == 2){
        printf("DNA1: %s\n", str1);
        printf("DNA2: %s\n", str2);
    }
    while(mat[lin][col] != 0){
        if(mat[lin][col] == horizontal){
            if(index == 2){
              printf("\n[lin: %d, col: %d] Horizontal\n", lin, col);
            }
            alinhamento1[tamanho-1] = '_';
            alinhamento2[tamanho-1] = str2[col-1];
            if(index == 2){
              printf("Alinhamento 1 recebe _\n");
              printf("Alinhamento 2 recebe %c (DNA2[%d])\n", str2[col-1], col-1);
            }
            col--;
            tamanho--;
        }
        if(mat[lin][col] == diagonal){
            if(index == 2){
              printf("\n[lin: %d, col: %d] Diagonal\n", lin, col);
            }
            alinhamento1[tamanho-1] = str1[lin-1];
            alinhamento2[tamanho-1] = str2[col-1];
            if(index == 2){
              printf("Alinhamento 1 recebe %c (DNA1[%d])\n", str1[lin-1], lin-1);
              printf("Alinhamento 2 recebe %c (DNA2[%d])\n", str2[col-1], col-1);
            }
            lin--;
            col--;
            tamanho--;
        }
        if(mat[lin][col] == vertical){
            if(index == 2){
              printf("\n[lin: %d, col: %d] Vertical\n", lin, col);
            }
            alinhamento1[tamanho-1] = str1[lin-1];
            alinhamento2[tamanho-1] = '_';
            if(index == 2){
              printf("Alinhamento 1 recebe %c (DNA1[%d])\n", str1[lin-1], lin-1);
              printf("Alinhamento 2 recebe _\n");
            }
            lin--;
            tamanho--;
        }
    }
    if(index == 2){
      printf("\nTamanho: %d\n", tamanho);
      printf("Col: %d\n", col);
      printf("Lin: %d\n", lin);
    }
    while(lin > 0) {
        alinhamento1[tamanho-1] = str1[lin-1];
        alinhamento2[tamanho-1] = '_';
        tamanho--;
        lin--;
    }
    while(col > 0) {
        alinhamento1[tamanho-1] = '_';
        alinhamento2[tamanho-1] = str2[col-1];
        tamanho--;
        col--;
    }

    t.celulas[index].alinhamento1 = alinhamento1;
    t.celulas[index].alinhamento2 = alinhamento2;

    return t;
}

//Algoritmo "Longest Common Sequence"
Tabela lcs(Tabela t, int index){

    int **pontos, **ponteiros;
    int lin = t.celulas[0].tamanhoSequencia + 1;
    int col = t.celulas[index].tamanhoSequencia + 1;
    int *str_aux1 = (int*)calloc((lin - 1),sizeof(int));
    int *str_aux2 = (int*)calloc((col - 1),sizeof(int));

    pontos = monta_matriz(lin, col, pontos);
    ponteiros = monta_matriz(lin, col, ponteiros);

    for(int i = 1; i < lin; i++) {
        for(int j = 1; j < col; j++) {
            if(t.celulas[0].sequencia[i - 1] == t.celulas[index].sequencia[j - 1]){
                pontos[i][j] = pontos[i - 1][j - 1] + 1;
                if(pontos[i-1][j-1] >= pontos[i][j-1] && pontos[i-1][j-1] >= pontos[i-1][j])
                    ponteiros[i][j] = diagonal;
                else if(pontos[i-1][j] > pontos[i][j-1] && pontos[i-1][j] > pontos[i-1][j-1])
                    ponteiros[i][j] = horizontal;
                else if(pontos[i][j-1] >= pontos[i-1][j] && pontos[i][j-1] >= pontos[i-1][j-1])
                    ponteiros[i][j] = vertical;

            } else if(pontos[i - 1][j] >= pontos[i][j - 1] && pontos[i-1][j] > pontos[i-1][j-1]){
                    pontos[i][j] = pontos[i - 1][j];
                    ponteiros[i][j] = vertical;
            } else {
                pontos[i][j] = pontos[i][j - 1];
                ponteiros[i][j] = horizontal;
            }
        }
    }

    // printf("\nID%d\n", index);
    if(index == 2){
      imprime_mat(lin, col, ponteiros);
    }

    t.celulas[index].semelhanca = pontos[lin - 1][col - 1];
    t = gera_alinhamento(t, index, lin - 1, col - 1, ponteiros, t.celulas[0].sequencia, t.celulas[index].sequencia);

    libera_matriz(lin, col, pontos);
    libera_matriz(lin, col, ponteiros);

    return t;
}

//Monta a estrutura celula
Celula monta_celula(int id, int tamanho, char *sequencia ){
    Celula c;

    sequencia = (char*) realloc (sequencia, tamanho);
    c.id = id;
    c.sequencia = sequencia;
    c.tamanhoSequencia = tamanho;

    return c;
}

// Lê o arquivo e retorna as sequencias em uma estrutura nomeada tabela
Tabela ler_arquivo(char *nomeArquivo){
	FILE *f;
    Tabela tabela;

    int id;
    int tamanho;
    char *sequencia;

	f = fopen(nomeArquivo, "r");
	if(f == NULL){
        printf("Arquivo não encontrado\n");
        exit(1);
    }

    fscanf(f, "%d\n", &tabela.qtdSequencias);
	Celula *celulas = (Celula *) malloc((tabela.qtdSequencias) * sizeof(Celula));
	for (int i = 0; i < tabela.qtdSequencias; i++) {
	    fscanf(f,"id%d %d ",&id, &tamanho);
        sequencia = (char*) malloc ((tamanho + 1) * sizeof(char));
        fscanf(f,"%s\n", sequencia);
	    celulas[i] = monta_celula(id, tamanho, sequencia);
	}
    tabela.celulas = celulas;

    fclose(f);
    free(f);
	return tabela;
}
