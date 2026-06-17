#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TARVBM_trab.h"
#include "TG.h"
#include "func_questoes.h"

void acha_campeao_rec(FILE *arq_indices, int offset, FILE *arq_grafo, int t, char *tipo_aresta, int buscar_maior, int *recorde, char *nome_campeao) {
    if (offset == -1) return;
    TARVBM *no = le_no(arq_indices, offset, t);
    if (!no) return;

    if (!no->folha) {
        for (int i = 0; i <= no->nchaves; i++) {
            acha_campeao_rec(arq_indices, no->filho[i], arq_grafo, t, tipo_aresta, buscar_maior, recorde, nome_campeao);
        }
    } else {
        char nome_arq_folha[50];
        sprintf(nome_arq_folha, "bin/folha_%d.bin", no->filho[0]);
        FILE *f_dados = fopen(nome_arq_folha, "rb");

        if (f_dados) {
            dados d;
            for (int i = 0; i < no->nchaves; i++) {
                fseek(f_dados, i * sizeof(dados), SEEK_SET);
                fread(&d, sizeof(dados), 1, f_dados);

                if (strcmp(d.tipo, "Person") == 0 && d.offset_prim_viz != -1) {
                    int contagem = 0;
                    long offset_atual = d.offset_prim_viz;
                    Relacionamentos rel; 

                    while (offset_atual != -1) {
                        fseek(arq_grafo, offset_atual, SEEK_SET);
                        fread(&rel, sizeof(Relacionamentos), 1, arq_grafo);

                        if (strcmp(rel.tipo_relacao, tipo_aresta) == 0) {
                            contagem++;
                        }
                        offset_atual = rel.offset_prox_viz;
                    }

                    if (contagem > 0) {
                        if (buscar_maior && contagem > *recorde) {
                            *recorde = contagem;
                            strcpy(nome_campeao, d.nome);
                        } 
                        else if (!buscar_maior && contagem < *recorde) {
                            *recorde = contagem;
                            strcpy(nome_campeao, d.nome);
                        }
                    }
                }
            }
            fclose(f_dados);
        }
    }
    libera_no(no, t);
}

void relatorio_podio(FILE *arq_indices, int offset_raiz, FILE *arq_grafo, int t, char *tipo_aresta, int buscar_maior) {
    int recorde = buscar_maior ? -1 : 999999;
    char nome_campeao[150] = "";

    acha_campeao_rec(arq_indices, offset_raiz, arq_grafo, t, tipo_aresta, buscar_maior, &recorde, nome_campeao);

    if (strlen(nome_campeao) > 0) {
        if (buscar_maior) {
            printf("O %s que mais trabalhou foi %s são %d registros!\n", tipo_aresta, nome_campeao, recorde);
        } else {
            printf("O %s que menos trabalhou foi %s são %d registros!\n", tipo_aresta, nome_campeao, recorde);
        }
        
    } else {
        printf("Nenhum registro encontrado para a categoria %s.\n", tipo_aresta);
    }
}
