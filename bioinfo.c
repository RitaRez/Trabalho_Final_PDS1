#include "bioinfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <string.h>

//Imprime celula
void imprime_celula(Celula c){
    printf("%d\nid%d\n%s\n%s\n", c.semelhanca, c.id, c.alinhamento, c.sequencia);
}

//Imprime tabela
void imprime_tabela(Tabela tabela){
    for(int i = 0; i < tabela.qtdSequencias; i++)
        printf("\nid%d %s %d\n", tabela.celulas[i].id, tabela.celulas[i].alinhamento, tabela.celulas[i].semelhanca);
    
}

//Deixa a tabela em ordem de maior semelhancia
void ordena_tabela(Celula *a, int left, int right) {
    int i, j, x;
    Celula y;
     
    i = left;
    j = right;
    x = a[(left + right) / 2].semelhanca;
     
    while(i <= j) {
        while(a[i].semelhanca < x && i < right)
            i++;
        while(a[j].semelhanca > x && j > left)
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
        for(int j = 0; j < col; j++)
            printf("%d ", mat[i][j]);
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

//Libera a memória utilizada pela matriz de alinhamento
void libera_matriz(int lin, int col, int **mat){
    for(int i = 0; i < lin; i++)
        free(mat[i]);
    free(mat);
}

// Gera alinhamento de uma sequencia espeficica com a primeira sequencia
char *gera_alinhamento(int flag, int lin, int col, int **mat, char *str){
    
    int i = lin, j = col, tamanho;   
    if(!flag)
        tamanho = lin;
    else
        tamanho = col;   
    char *alinhamento = (char *) malloc (tamanho*sizeof(char));

    for(int k = 0; k < tamanho - 2; k++)
        alinhamento[k] = '_';
    
    while(i != 0 && j != 0 && tamanho !=0){
        if(mat[i][j] == diagonal){
            alinhamento[tamanho - 1] = str[tamanho - 1];
            i--;
            j--;
        } else if(mat[i][j] == vertical){   
            alinhamento[tamanho - 1] = '_';
            i--;  
        } else {
            alinhamento[tamanho - 1] = '_';
            j--;
        }   
        tamanho--;
    }
    
    return alinhamento;
}

//Algoritmo "Longest Common Sequence"
Tabela lcs(Tabela t, int index){

    int **pontos, **ponteiros; 
    int lin = t.celulas[0].tamanhoSequencia + 1;
    int col = t.celulas[index].tamanhoSequencia + 1;
    
    pontos = monta_matriz(lin, col, pontos);   
    ponteiros = monta_matriz(lin, col, ponteiros);
    
    for(int i = 0; i < lin; i++) {
        for(int j = 0; j < col; j++) {            
            if(i == 0 || j == 0){
                continue;   
            } else if(t.celulas[0].sequencia[i - 1] == t.celulas[index].sequencia[j - 1]){
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

    t.celulas[index].semelhanca = pontos[lin - 1][col - 1];

    if(lin > col)
        t.celulas[index].alinhamento = gera_alinhamento(0, lin - 1, col - 1, ponteiros, t.celulas[0].sequencia);
    else 
        t.celulas[index].alinhamento = gera_alinhamento(1, lin - 1, col - 1, ponteiros, t.celulas[index].sequencia);


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