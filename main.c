#include <stdio.h>
#include <string.h>

char *divide_string(char **linha){
	if((*linha)==NULL || (*linha)=='\0'){
		return NULL:
	}
	
	char *inicio_linha = (*linha);
	
	char *pos_separador = strstr(inicio_linha," | ");
	
	if(pos_separador){
		(*pos_separador) = '\0';
		
		(*linha) = pos_separador + 3;
	}else{
		(*pos_separador) = strchr(inicio_linha,'\n');
		if(pos_separador){
			(*pos_separador) = '\0';
		}
		
		*linha = NULL;
	}
	
	return inicio_linha;
}

int main(void){
	FILE *arq_nodes = fopen("data/Nodes.txt","r");
	if(!arq_nodes) exit(1);
	FILE *arq_relationships = fopen("data/Relationships.txt","r");
	if(!arq_relationships) exit(1);
	FILE *arq_indices = fopen("bin/indices","r+b");
	if(!arq_indices){
		arq_indices = fopen("bin/indices","w+b");
		if(!arq_indices) exit(1);
	}
	
	while(fgets(linha),sixeof(linha),arq_nodes)){
			
	}
	
	fclose(nodes);
	fclose(relationships);
	fclose(indices);
	
	return 0;
}
