#include "TARVBM_trab.h"
#include "TG.h"
#define N 1000

void imprime_folha(int id_folha, int t){
    char nome_arq[50];
    sprintf(nome_arq, "bin/folha_%d.bin", id_folha);
    
    FILE *fp = fopen(nome_arq, "rb");
    if(!fp){
        printf("Arquivo folha_%d.bin nao encontrado\n", id_folha);
        return;
    }
    
    printf("=== folha_%d.bin ===\n", id_folha);
    dados d;
    for(int i = 0; i < (2*t)-1; i++){
        fread(&d, sizeof(dados), 1, fp);
        if(d.nome[0] == '\0'){
            printf("[%d] (vazio)\n", i);
        } else {
            printf("[%d] tipo=%s | nome=%s | ano=%d | prim_viz=%ld |frase=%s\n",
                i, d.tipo, d.nome, d.ano, d.offset_prim_viz, d.frase);
        }
    }
    
    fclose(fp);
}

int main(void){
	int t, offset;
	
	
	FILE *arq_indices = fopen("bin/indices","r+b");
	if(!arq_indices){
		printf("Informe o grau minimo t da arvore: ");
    	scanf("%d", &t);
	
		TARVBM_cria("bin/indices", t);
		arq_indices = fopen("bin/indices","r+b");
		if(!arq_indices) exit(1);
		
		fread(&t, sizeof(int), 1, arq_indices);
    	fread(&offset, sizeof(int), 1, arq_indices);
		
		FILE *arq_nodes = fopen("data/Nodes.txt","r");
		if(!arq_nodes) exit(1);
		
		FILE *arq_rel = fopen("data/Relationships.txt", "r");
		FILE *arq_grafo = fopen("bin/relacionamentos.bin", "w+b");
		
		char *tipo, *nome, *ano, *frase_filme;
		char *linha = (char*)malloc(sizeof(char)*N);
		if(!linha) exit(1);
		
		while(fgets(linha, N, arq_nodes)){
			char *ponteiro_linha = linha;
			
			dados dado = {0};
			dado.offset_prim_viz = -1;
		
			tipo = divide_string(&ponteiro_linha);
			if(!tipo || strlen(tipo) == 0){
				continue;
			}
			strcpy(dado.tipo, tipo);
			
			if(strcmp(tipo,"Person") == 0){
				nome = divide_string(&ponteiro_linha);
				ano = divide_string(&ponteiro_linha);

				if(!nome || !ano) continue;
				
				strcpy(dado.nome, nome);
				dado.ano = atoi(ano);
				
				offset = TARVBM_insere(arq_indices, offset, nome, dado, t);
			}else{
				nome = divide_string(&ponteiro_linha);
				ano = divide_string(&ponteiro_linha);
				if(!nome || !ano) continue;
				frase_filme = divide_string(&ponteiro_linha);
				
				if(!nome || !ano || !frase_filme) continue;
				
				strcpy(dado.nome, nome);
				dado.ano = atoi(ano);
				if(!ano || !isdigit(ano[0]))continue;
				strcpy(dado.frase, frase_filme);
				
				offset = TARVBM_insere(arq_indices, offset, nome, dado, t);
			}
		}

		while(fgets(linha, N, arq_rel)){
			char *ponteiro_linha = linha;
			char *lixo = divide_string(&ponteiro_linha); // start person ou start movie joga fora

			char *nome_origem = divide_string(&ponteiro_linha);
			char *relacao = divide_string(&ponteiro_linha);
			
			lixo = divide_string(&ponteiro_linha); //pular end movie/person

			char *nome_destino = divide_string(&ponteiro_linha);
			if(!nome_origem || !nome_destino) continue;

			char *papel = ponteiro_linha; // pode ser NULL se não houver mais campos
			char papel_copia[150] = {0}; // sempre usa cópia para não depender de linha

			if(papel && strncmp(papel, "role: ", 6) == 0){
				papel += 6;
				char *nl = strchr(papel, '\n');
				if(nl) *nl = '\0';
				char *cr = strchr(papel, '\r');
				if(cr) *cr = '\0';
				strncpy(papel_copia, papel, 149);
			} else {
				strcpy(papel_copia, ""); //nao tem papel
			}
			
			insere_aresta_disco(arq_indices, arq_grafo, offset, nome_origem, nome_destino, relacao, papel_copia, t);
		}

		fseek(arq_indices, sizeof(int), SEEK_SET);
		fwrite(&offset, sizeof(int), 1, arq_indices);
		fclose(arq_nodes);
		fclose(arq_rel);
		fclose(arq_grafo);
		free(linha);
	}else{
		int id;
		fread(&t, sizeof(int), 1, arq_indices);
    	fread(&offset, sizeof(int), 1, arq_indices);
    	fread(&id, sizeof(int), 1, arq_indices);
    	printf("Arvore já existe com t=%d\n", t);
	}
    
	/*
    // imprime as primeiras N folhas para testar
	for(int i = 1; i <= 10; i++){
		imprime_folha(i, t);
	}

	brincar_com_grafo(arq_indices, offset, "Charlize Theron", t);
    brincar_com_grafo(arq_indices, offset, "The Matrix", t);
    brincar_com_grafo(arq_indices, offset, "Tom Hanks", t);
    */
    
	int opcao, sub_opcao;
	printf("(1) - Inserir atores, diretores, produtores ou filmes\n");
	printf("(2) - Retirar atores, diretores, produtores ou filmes\n");
	printf("(3) - Buscar nome\n");
	printf("(4) - Imprimir árvore\n");
	printf("(5) - Sair e Salvar\n");
	printf("Escolha: ");
	
	scanf("%d", &opcao);

	while(opcao !=5) {
		if (opcao == 1 || opcao == 2) {
			printf("(1) - Ator, Diretor, Produtor\n");
			printf("(2) - Filme\n");
			printf("Escolha: ");
			scanf("%d", &sub_opcao);

			char nome_input[150];
			int ano_input;
			char frase_input[240] = {0};
			char tipo_input[15] = {0};

			if (sub_opcao == 1) {
				strcpy(tipo_input, "Person");
			} else if (sub_opcao == 2) {
				strcpy(tipo_input, "Movie");
			} else {
				printf("Opção invalida!\n");
				continue;
			}

			if (opcao == 1) {
				getchar(); // limpa o buffer
			
				printf("Digite o nome: ");
				fgets(nome_input, 150, stdin); 
				nome_input[strcspn(nome_input, "\n")] = '\0';
				
				printf("Digite o ano: ");
				scanf("%d", &ano_input);

				dados novo_dado = {0};
				novo_dado.offset_prim_viz = -1; // Nasce sem relacionamentos
				strcpy(novo_dado.nome, nome_input);
				strcpy(novo_dado.tipo, tipo_input);
				novo_dado.ano = ano_input;

				if (sub_opcao == 2) {
					getchar();
					
					printf("Digite a frase do filme: ");
					fgets(frase_input, 240, stdin);
					frase_input[strcspn(frase_input, "\n")] = '\0';					
					strcpy(novo_dado.frase, frase_input);
				}

				offset = TARVBM_insere(arq_indices, offset, nome_input, novo_dado, t);
				printf("%s foi inserido no banco.\n", nome_input);

			} else if (opcao == 2) {
				getchar();
			
				printf("Digite o nome para retirar: ");
				fgets(nome_input, 150, stdin); 
				nome_input[strcspn(nome_input, "\n")] = '\0';
				
				offset = TARVBM_retira(arq_indices, offset, nome_input, t);
				printf("%s foi retirado do banco.\n", nome_input);
			} 
		} else if (opcao == 3){
			getchar();
		
			printf("Digite o nome para buscar: ");
				char nome_input[150];
				fgets(nome_input, 150, stdin); 
				nome_input[strcspn(nome_input, "\n")] = '\0';
				
				TARVBM *no = TARVBM_busca(arq_indices, offset, nome_input, t);
			
			if (!no) {
				printf("%s nao foi encontrado no banco.\n", nome_input);
			} else {
				int i = 0;
				while (i < no->nchaves && strcmp(no->chave[i], nome_input) != 0) {
					i++;
				}
				
				if (i < no->nchaves && strcmp(no->chave[i], nome_input) == 0) {
					char nome_arq_folha[50];
					sprintf(nome_arq_folha, "bin/folha_%d.bin", no->filho[0]);
					FILE *f_folha = fopen(nome_arq_folha, "rb");
					
					if (f_folha) {
						dados d;
						fseek(f_folha, i * sizeof(dados), SEEK_SET);
						fread(&d, sizeof(dados), 1, f_folha);
						fclose(f_folha);
						
						printf("Tipo:  %s\n", d.tipo);
						printf("Nome:  %s\n", d.nome);
						printf("Ano:   %d\n", d.ano);
						if (strcmp(d.tipo, "Movie") == 0) {
							printf("Frase: %s\n", d.frase);
						}
					}
				} else {
					printf("%s nao foi encontrado no banco.\n", nome_input);
				}

				libera_no(no, t);
			}
		} else if (opcao == 4) {
			TARVBM_imprime(arq_indices, offset,t);
		} else {
			printf("Opcao invalida\n");
		}
		
		printf("(1) - Inserir atores, diretores ou filmes\n");
		printf("(2) - Retirar atores, diretores ou filmes\n");
		printf("(3) - Buscar nome\n");
		printf("(4) - Imprimir árvore\n");
		printf("(5) - Sair e Salvar\n");
		printf("Escolha: ");
		
		scanf("%d", &opcao);
	}
	
	fseek(arq_indices, sizeof(int), SEEK_SET);
	fwrite(&offset, sizeof(int), 1, arq_indices);
	
    fclose(arq_indices);

	return 0;
}
