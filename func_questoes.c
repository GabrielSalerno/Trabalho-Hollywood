#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TARVBM_trab.h"
#include "TG.h"
#include "func_questoes.h"

struct FilmeCount {
    char nome[150];
    int d, p, e;
};

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
        
        brincar_com_grafo(arq_indices, offset_raiz, nome_campeao, t);
        
    } else {
        printf("Nenhum registro encontrado para a categoria %s.\n", tipo_aresta);
    }
}

void acha_multitarefas_pessoa(FILE *arq_indices, int offset, FILE *arq_grafo, int t, char *req1, char *req2) {
    if (offset == -1) return;
    TARVBM *no = le_no(arq_indices, offset, t);
    if (!no) return;

    if (!no->folha) {
        for (int i = 0; i <= no->nchaves; i++) {
            acha_multitarefas_pessoa(arq_indices, no->filho[i], arq_grafo, t, req1, req2);
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
                    int tem_req1 = 0, tem_req2 = 0;
                    long offset_atual = d.offset_prim_viz;
                    Relacionamentos rel; 

                    while (offset_atual != -1) {
                        fseek(arq_grafo, offset_atual, SEEK_SET);
                        fread(&rel, sizeof(Relacionamentos), 1, arq_grafo);

                        if (strcmp(rel.tipo_relacao, req1) == 0) tem_req1 = 1;
                        if (strcmp(rel.tipo_relacao, req2) == 0) tem_req2 = 1;

                        offset_atual = rel.offset_prox_viz;
                    }

                    if (tem_req1 && tem_req2) {
                        printf("- %s\n", d.nome);
                    }
                }
            }
            fclose(f_dados);
        }
    }
    libera_no(no, t);
}

void acha_faz_tudo_filme(FILE *arq_indices, int offset, FILE *arq_grafo, int t, int exige_escreveu) {
    if (offset == -1) return;
    TARVBM *no = le_no(arq_indices, offset, t);
    if (!no) return;

    if (!no->folha) {
        for (int i = 0; i <= no->nchaves; i++) {
            acha_faz_tudo_filme(arq_indices, no->filho[i], arq_grafo, t, exige_escreveu);
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
                    struct FilmeCount filmes[150];
                    int qtd = 0;
                    long offset_atual = d.offset_prim_viz;
                    Relacionamentos rel; 

                    while (offset_atual != -1) {
                        fseek(arq_grafo, offset_atual, SEEK_SET);
                        fread(&rel, sizeof(Relacionamentos), 1, arq_grafo);

                        int idx = -1;
                        for (int j = 0; j < qtd; j++) {
                            if (strcmp(filmes[j].nome, rel.nome_destino) == 0) {
                                idx = j; 
                                break;
                            }
                        }
                        
                        if (idx == -1) {
                            idx = qtd;
                            strcpy(filmes[idx].nome, rel.nome_destino);
                            filmes[idx].d = 0; filmes[idx].p = 0; filmes[idx].e = 0;
                            qtd++;
                        }

                        if (strcmp(rel.tipo_relacao, "DIRECTED") == 0) filmes[idx].d = 1;
                        if (strcmp(rel.tipo_relacao, "PRODUCED") == 0) filmes[idx].p = 1;
                        if (strcmp(rel.tipo_relacao, "WROTE") == 0) filmes[idx].e = 1;

                        offset_atual = rel.offset_prox_viz;
                    }

                    for (int j = 0; j < qtd; j++) {
                        if (exige_escreveu && filmes[j].d && filmes[j].p && filmes[j].e) {
                            printf("- Filme: %s - Escreveu, dirigiu e produziu: %s\n", filmes[j].nome, d.nome);
                        } else if (!exige_escreveu && filmes[j].d && filmes[j].p) {
                            printf("- Filme: %s - Dirigiu e produziu: %s\n", filmes[j].nome, d.nome);
                        }
                    }
                }
            }
            fclose(f_dados);
        }
    }
    libera_no(no, t);
}

void atores_mais(FILE *arq_indices, int offset_raiz, FILE *arq_grafo, int t, char *req1, char *req2) {
    printf("Atores:\n");
    acha_multitarefas_pessoa(arq_indices, offset_raiz, arq_grafo, t, req1, req2);
}

void filme_faz_tudo(FILE *arq_indices, int offset_raiz, FILE *arq_grafo, int t, int exige_escreveu) {
    printf("Filmes:\n");
    acha_faz_tudo_filme(arq_indices, offset_raiz, arq_grafo, t, exige_escreveu);
}

void coleta_equipe(FILE *arq_indices, int offset, FILE *arq_grafo, int t, char *filme, char nomes[][150], int *qtd) {
    if (offset == -1) return;
    TARVBM *no = le_no(arq_indices, offset, t);
    if (!no) return;

    if (!no->folha) {
        for (int i = 0; i <= no->nchaves; i++) {
            coleta_equipe(arq_indices, no->filho[i], arq_grafo, t, filme, nomes, qtd);
        }
    } else {
        char nome_arq_folha[50];
        sprintf(nome_arq_folha, "bin/folha_%d.bin", no->filho[0]);
        FILE *arq_dados = fopen(nome_arq_folha, "rb");

        if (arq_dados) {
            dados d;
            for (int i = 0; i < no->nchaves; i++) {
                fseek(arq_dados, i * sizeof(dados), SEEK_SET);
                fread(&d, sizeof(dados), 1, arq_dados);

                if (strcmp(d.tipo, "Person") == 0 && d.offset_prim_viz != -1) {
                    int offset_atual = d.offset_prim_viz;
                    Relacionamentos rel;
                    int trabalhou = 0;

                    while (offset_atual != -1) {
                        fseek(arq_grafo, offset_atual, SEEK_SET);
                        fread(&rel, sizeof(Relacionamentos), 1, arq_grafo);

                        if (strcmp(rel.nome_destino, filme) == 0) {
                            trabalhou = 1;
                            break;
                        }
                        offset_atual = rel.offset_prox_viz;
                    }

                    if (trabalhou) {
                        strcpy(nomes[*qtd], d.nome);
                        (*qtd)++;
                    }
                }
            }
            fclose(arq_dados);
        }
    }
    libera_no(no, t);
}

int retira_equipe_filme(FILE *arq_indices, int offset_raiz, FILE *arq_grafo, int t, char *filme) {
    char nomes[500][150];
    int qtd = 0;

    coleta_equipe(arq_indices, offset_raiz, arq_grafo, t, filme, nomes, &qtd);

    int novo_offset = offset_raiz;
    for (int i = 0; i < qtd; i++) {
        novo_offset = TARVBM_retira(arq_indices, novo_offset, nomes[i], t);
        printf("%s removido(a) do banco.\n", nomes[i]);
    }

    if (qtd == 0) {
        printf("Nenhum membro da equipe encontrado para o filme '%s'.\n", filme);
    } else {
        printf("Total: %d pessoas removidas.\n", qtd);
    }

    return novo_offset;
}
