#include "bioinfo.h"


int main(int argc, char *argv[]){

    Tabela tabela = ler_arquivo(argv[1]);  
    
    for(int i = 1; i < tabela.qtdSequencias; i++){
        tabela = lcs(tabela, i);
    }         
        

    tabela.celulas[0].semelhanca = 0;
    tabela.celulas[0].alinhamento1 = "-";
    tabela.celulas[0].alinhamento2 = "-";

    ordena_tabela(tabela.celulas, 0, tabela.qtdSequencias - 1);

    imprime_celula(tabela.celulas[tabela.qtdSequencias - 1]);
    //imprime_tabela(tabela);

    free(tabela.celulas);

    return 0;
}