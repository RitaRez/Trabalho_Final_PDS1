#include "bioinfo.h"


int main(int argc, char *argv[]){

    Tabela tabela = ler_arquivo(argv[1]);  
    
    for(int i = 1; i < tabela.qtdSequencias; i++)
        tabela = lcs(tabela, i);
    
    ordena_tabela(tabela.celulas, 0, tabela.qtdSequencias - 1);
    imprime_tabela(tabela);

    imprime_celula(tabela.celulas[tabela.qtdSequencias - 1]);

    return 0;
}