#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TARVBM_trab.h"
#define N 210

char *divide_string(char **linha){
	if((*linha)==NULL || (**linha)=='\0'){
		return NULL;
	}
	
	char *inicio_linha = (*linha);
	
	char *pos_separador = strstr(inicio_linha," | ");
	
	if(pos_separador){
		(*pos_separador) = '\0';
		(*linha) = pos_separador + 3;
	}else{
		pos_separador = strchr(inicio_linha,'\n');
		
		if(pos_separador){
			(*pos_separador) = '\0';
		}
		
		*linha = NULL;
	}
	
	return inicio_linha;
}

int main(void){
	int t = 2;
	
	FILE *arq_nodes = fopen("data/Nodes.txt","r");
	if(!arq_nodes) exit(1);
	FILE *arq_relationships = fopen("data/Relationships.txt","r");
	if(!arq_relationships) exit(1);
	FILE *arq_indices = fopen("bin/indices","r+b");
	if(!arq_indices){
		TARVBM_cria("bin/indices", t);
		arq_indices = fopen("bin/indices","r+b");
		if(!arq_indices) exit(1);
	}
	
	int t_lido, offset;
	fseek(arq_indices, 0, SEEK_SET);
    fread(&t_lido, sizeof(int), 1, arq_indices);
    fread(&offset, sizeof(int), 1, arq_indices);//pula o t que ta no inicio do arquivo
    
	char *tipo, *nome, *ano, *frase_filme;
	char *linha = (char*)malloc(sizeof(char)*N);
	if(!linha) exit(1);
	
	int contador = 0;
	
	while(fgets(linha, N, arq_nodes)){
		char *ponteiro_linha = linha;
	
		tipo = divide_string(&ponteiro_linha);
		if(strcmp(tipo,"Person") == 0){
			nome = divide_string(&ponteiro_linha);
			ano = divide_string(&ponteiro_linha);
			
			offset = TARVBM_insere(arq_indices, offset, nome, t);
		}else{
			nome = divide_string(&ponteiro_linha);
			ano = divide_string(&ponteiro_linha);
			frase_filme = divide_string(&ponteiro_linha);
			
			offset = TARVBM_insere(arq_indices, offset, nome, t);
		}
		
		contador++;
        // Feedback visual para você saber que o programa não travou!
        if (contador % 500 == 0) {
            printf("Sucesso: %d registros processados...\n", contador);
        }
	}
	
	fseek(arq_indices, sizeof(int), SEEK_SET);
    fwrite(&offset, sizeof(int), 1, arq_indices);
    
    printf("\nImportacao concluida com sucesso! Total: %d registros inseridos.\n", contador);
	
	free(linha);
	fclose(arq_nodes);
	fclose(arq_relationships);
	fclose(arq_indices);
	
	return 0;
}
