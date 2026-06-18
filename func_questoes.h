void relatorio_podio(FILE *arq_indices, int offset_raiz, FILE *arq_grafo, int t, char *tipo_aresta, int buscar_maior);
void atores_mais(FILE *arq_indices, int offset_raiz, FILE *arq_grafo, int t, char *req1, char *req2);
void filmes_mais(FILE *arq_indices, int offset_raiz, FILE *arq_grafo, int t, int exige_escreveu);
int retira_equipe_filme(FILE *arq_indices, int offset_raiz, FILE *arq_grafo, int t, char *filme);
void filmes_continuacoes(FILE *arq_indices, int offset_raiz, int t);
void atores_mesmo_ano(FILE *arq_indices, int offset_raiz, int t);
void atores_nascidos_ano_filme(FILE *arq_indices, int offset_raiz, int t);
