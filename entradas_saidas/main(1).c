#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///DEFINICAO DO TIPO C�LULA
struct celula{
    char **s; ///MATRIZ DE SETAS
    int **n; ///MATRIZ DE N�MEROS
};

///DEFINI��O DO TIPO PESSOA
struct pessoa{
    int pontuacao; ///PONTUA��O DO ALINHAMENTO
    int linhas; ///TAMANHO DO DNA (LINHAS DA MATRIZ DE ALINHAMENTO)
    char id[10]; ///IDENTIFICADOR DO SUSPEITO
    char *dna; ///DNA DO SUSPEITO
    char *seq0; ///DNA DO ASSASSINO ALINHADO (COM GAPS)
    char *seq; ///DNA DO SUSPEITO ALINHADO (COM GAPS)
};

///PROT�TIPO DAS FUN��ES
int alinhar_sequencias(char *dna1, char *dna2, int **n, char **s, int linhas, int colunas);
void imprimir_matriz_alinhamento(char *dna1, char *dna2, int **n, char **s, int linas, int colunas);
int tamanho_alinhamento(char *dna1, char *dna2, int **n, char **s, int linhas, int colunas);
void sequencias_alinhadas(char *dna1, char *dna2, int **n, char **s, char *seq1, char *seq2, int linhas, int colunas, int cont);
void ordenacao(int tam, struct pessoa *suspeito);
void saida(struct pessoa *suspeito, int tam);

int main()
{
    struct pessoa *suspeito; ///STRUCT CONTENDO OS DADOS DO SUSPEITO
    struct celula mat; ///STRUCT CONTENDO A MATRIZ DE ALINHAMENTO
    char *dna1; ///DNA DO ASSASSINO
    char id[10]; ///IDENTIFICADOR DO ASSASSINO
    int colunas; ///TAMANHO DO DNA DO ASSASSINO (COLUNAS DA MATRIZ DE ALINHAMENTO)
    int tam; ///QUANTIDADE DE DNAS NO ARQUIVO
    char *temp; ///STRING TEMPOR�RIA PARA ALOCA��O DOS DNAS
    int i, j, cont;
    FILE *fp;
    ///LENDO O ARQUIVO
    fp = fopen("input_toy.txt", "r");
    if(fp == NULL) { printf("ERRO NA LEITURA!\n"); exit(1); }
    fscanf(fp, "%d %s %d", &tam, id, &colunas);
    ///ALOCANDO DINAMICAMENTE A STRUCT E AS STRINGS
    suspeito = (struct pessoa *)malloc((tam-1)*sizeof(struct pessoa));
    if(suspeito == NULL){ printf("ERRO!\n"); exit(1); }
    colunas += 2; ///1 POSI��O PARA O '\0' E UMA PARA A POSI��O INICIAL 'v'
    dna1 = (char *) malloc(colunas*sizeof(char));
    if(dna1 == NULL){ printf("ERRO!\n"); exit(1); }
    colunas -= 1; ///A TEMPOR�RIA N�O TEM O INICIALIZADOR
    temp = (char *) malloc(colunas*sizeof(char));
    if(temp == NULL){ printf("ERRO!\n"); exit(1); }
    fscanf(fp, "%s", temp);
    dna1[0] = 'v'; dna1[1] = '\0'; ///ISERINDO O CARACTER DE �NICIO
    strcat(dna1, temp); ///CONCATENANDO O CARACTER E O DNA LIDO
    free(temp); /// DESALOCANDO A STRING TEMPOR�RIA
    ///LENDO O ARQUIVO
    for(i = 0; i < (tam-1); i++){
        fscanf(fp, "%s", suspeito[i].id);
        fscanf(fp, "%d", &suspeito[i].linhas);
        suspeito[i].linhas += 2;
        suspeito[i].dna = (char *) malloc(suspeito[i].linhas*sizeof(char));
        if(suspeito[i].dna == NULL){ printf("ERRO!\n"); exit(1); }
        suspeito[i].linhas -= 1;
        temp = (char *) malloc(suspeito[i].linhas*sizeof(char));
        if(temp == NULL){ printf("ERRO!\n"); exit(1); }
        fscanf(fp, "%s", temp);
        suspeito[i].dna[0] = 'w'; suspeito[i].dna[1] = '\0';
        strcat(suspeito[i].dna, temp);
    }
    ///CHAMADAS DE FUN��O
    for(i = 0; i < (tam-1); i++){
        //printf("%d\n", i);
        ///ALOCANDO AS MATRIZES DE ALINHAMENTO
        mat.s = (char **) malloc(suspeito[i].linhas*sizeof(char*));
        if(mat.s == NULL){ printf("ERRO!\n"); exit(1); }
        mat.n = (int **) malloc(suspeito[i].linhas*sizeof(int*));
        if(mat.n == NULL){ printf("ERRO!\n"); exit(1); }
        for(j = 0; j < suspeito[i].linhas; j++){
            mat.s[j] = (char *) malloc(colunas*sizeof(char));
            if(mat.s[j] == NULL){ printf("ERRO!\n"); exit(1); }
            mat.n[j] = (int *) malloc(colunas*sizeof(int));
            if(mat.n[j] == NULL){ printf("ERRO!\n"); exit(1); }
        }
        suspeito[i].pontuacao = alinhar_sequencias(dna1, suspeito[i].dna, mat.n, mat.s, suspeito[i].linhas, colunas);
        //imprimir_matriz_alinhamento(dna1, suspeito[i].dna, mat.n, mat.s, suspeito[i].linhas, colunas);
        cont = tamanho_alinhamento(dna1, suspeito[i].dna, mat.n, mat.s, suspeito[i].linhas, colunas);
        ///ALOCANDO AS STRINGS DO ALINHAMENTO
        suspeito[i].seq0 = (char *) malloc((cont + 1)*sizeof(char));
        suspeito[i].seq = (char *) malloc((cont + 1)*sizeof(char));
        ///VERIFICANDO SE A ALOCA��O FOI POSS�VEL
        if(suspeito[i].seq0 == NULL){ printf("ERRO!\n"); exit(1); }
        if(suspeito[i].seq == NULL){ printf("ERRO!\n"); exit(1); }
        ///INSERINDO TERMINADOR NAS STRINGS
        suspeito[i].seq0[cont] = '\0'; suspeito[i].seq[cont] = '\0'; ///INSERINDO TERMINADORES PARA INDICAR FINAL DA SEQU�NCIA
        sequencias_alinhadas(dna1, suspeito[i].dna, mat.n, mat.s, suspeito[i].seq0, suspeito[i].seq, suspeito[i].linhas, colunas, cont);
    }
    ordenacao(tam, suspeito);
    saida(suspeito, tam);
    fclose(fp);
    ///DESALOCANDO OS VETORES E MATRIZES
    free(dna1);
    /*for(i = 0; i < (tam-1); i++){
        for(i = 0; i < strlen(suspeito[i].dna + 1); i++){
            free(mat.s[j]); free(mat.n[j]);
        }
        free(mat.s);
        free(mat.n);
    }
    for(i = 0; i < (tam-1); i++){
        free(suspeito[i].dna);
        free(suspeito[i].seq0);
        free(suspeito[i].seq);
    }
    free(suspeito);*/
    return 0;
}

int alinhar_sequencias(char *dna1, char *dna2, int **n, char **s, int linhas, int colunas)
{
    int i, j, score;
    ///INICIALIZANDO A PRIMEIRA LINHA
    for(i = 0; i < colunas; i++){
        s[0][i] = '*'; n[0][i] = 0;
    }
    ///INICIALIZANDO A PRIMEIRA COLUNA
    for(j = 0; j < linhas; j++){
        s[j][0] = '*'; n[j][0] = 0;
    }
    ///FAZENDO O ALINHAMENTO
    for(i = 1; i < linhas; i++)
       for(j = 1; j < colunas; j++)
       {
            if(dna2[i] == dna1[j])
            {
                n[i][j] = n[i-1][j-1] + 1;
                if(n[i-1][j-1] >= n[i][j-1] && n[i-1][j-1] >= n[i-1][j])
                    s[i][j] = '`';
                else if(n[i-1][j] > n[i][j-1] && n[i-1][j] > n[i-1][j-1])
                    s[i][j] = '|';
                else if(n[i][j-1] >= n[i-1][j] && n[i][j-1] >= n[i-1][j-1])
                    s[i][j] = '-';
            }
            else if(n[i-1][j] > n[i][j-1] && n[i-1][j] > n[i-1][j-1])
            {
                s[i][j] = '|';
                n[i][j] = n[i-1][j];
            }
            else if(n[i][j-1] >= n[i-1][j] && n[i][j-1] >= n[i-1][j-1])
            {
                s[i][j] = '-';
                n[i][j] = n[i][j-1];
            }
        }
    ///RETORNANDO A PONTUA��O DO ALINHAMENTO
    score = n[linhas-1][colunas-1];
    return score;
}

void imprimir_matriz_alinhamento(char *dna1, char *dna2, int **n, char **s, int linhas, int colunas)
{
    int i, j;
    printf("  ");
    for(i = 0; i < colunas; i++)
       printf("%c  ", dna1[i]);
    printf("\n");
    for(i = 0; i < linhas; i++)
    {
       printf("%c ", dna2[i]);
       for(j = 0; j < colunas; j++)
       {
          printf("%d%c ",n[i][j],s[i][j]);
       }
       printf("\n");
    }
}

int tamanho_alinhamento(char *dna1, char *dna2, int **n, char **s, int linhas, int colunas)
{
    int l = linhas - 1, c = colunas - 1, cont = 0;
    ///CONTANDO O TAMANHO DAS SEQU�NCIAS ALINHADAS E COM GAPS
    while(s[l][c] != '*'){
        if(s[l][c] == '-'){ c--; cont++; }
        if(s[l][c] == '`'){ l--; c--; cont++; }
        if(s[l][c] == '|'){ l--; cont++; }
    }
    ///TRATANDO O CASO DE "CAUDAS"
    if(c != 0 || l != 0){
        while(l > 0) { cont++; l--; c--; }
        while(c > 0) { cont++; c--; l--; }
    }
    return cont;
}

void sequencias_alinhadas(char *dna1, char *dna2, int **n, char **s, char *seq1, char *seq2, int linhas, int colunas, int cont)
{
    int l = linhas - 1, c = colunas - 1;
    ///PREENCHENDO AS STRINGS COM OS ALINHAMENTOS
    while(s[l][c] != '*'){
        if(s[l][c] == '-'){
            seq1[cont-1] = dna1[c];
            seq2[cont-1] = '-';
            c--; cont--;
        }
        if(s[l][c] == '`'){
            seq1[cont-1] = dna1[c];
            seq2[cont-1] = dna2[l];
            l--; c--; cont--;
        }
        if(s[l][c] == '|'){
            seq1[cont-1] = '-';
            seq2[cont-1] = dna2[l];
            l--; cont--;
        }
    }
    if(c != 0 || l != 0){
        while(l > 0) { seq1[cont-1] = '-'; seq2[cont-1] = dna2[l]; cont--; l--; }
        while(c > 0) { seq1[cont-1] = dna1[c]; seq2[cont-1] = '-'; cont--; c--; }
    }
}

void ordenacao(int tam, struct pessoa *suspeito)
{
    int i, j, min, x;
    char idtemp[10], dnatemp[1100], seqtemp[1100];
    for(i = 0; i < (tam-2); ++i){
        min = i;
        for(j = i + 1; j < (tam-1); ++j)
            if(suspeito[j].pontuacao < suspeito[min].pontuacao) min = j;
        x = suspeito[i].pontuacao; suspeito[i].pontuacao = suspeito[min].pontuacao; suspeito[min].pontuacao = x;
        strcpy(idtemp, suspeito[i].id); strcpy(suspeito[i].id, suspeito[min].id); strcpy(suspeito[min].id, idtemp);
        strcpy(dnatemp, suspeito[i].dna); strcpy(suspeito[i].dna, suspeito[min].dna); strcpy(suspeito[min].dna, dnatemp);
        strcpy(seqtemp, suspeito[i].seq0); strcpy(suspeito[i].seq0, suspeito[min].seq0); strcpy(suspeito[min].seq0, seqtemp);
        strcpy(seqtemp, suspeito[i].seq); strcpy(suspeito[i].seq, suspeito[min].seq); strcpy(suspeito[min].seq, seqtemp);
    }
}

void saida(struct pessoa *suspeito, int tam)
{
    FILE *arq;
    int i;
    ///ABRINDO O ARQUIVO PARA ESCRITA
    arq = fopen("output.txt", "w");
    if(arq == NULL) { printf("ERRO NA ABERTURA!\n"); exit(1); }
    ///IMPRIMINDO A SAIDA
    printf("%d\n%s\n%s\n%s\n\n", suspeito[tam-2].pontuacao, suspeito[tam-2].id, suspeito[tam-2].seq0, suspeito[tam-2].seq);
    for(i = (tam-2); i >= 0; i--)
        fprintf(arq, "%d\n%s\n%s\n%s\n\n", suspeito[i].pontuacao, suspeito[i].id, suspeito[i].seq0, suspeito[i].seq);
    fclose(arq);
}

