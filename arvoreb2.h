#ifndef ARV_B_EST
#define ARV_B_EST

#define ORDEM 2 			/* Tamanho da ordem da arvore */
#define ORDEM_MAX ORDEM*2 	/* tamanho máximo de registros que uma página pode ter */

#include "psin.h"

	/* página de uma árvore B* que guarda chaves ou registros */
	typedef struct tipo_pagina2 Tipo_pagina2;

	/*estrutura que apontará as páginas filho do Tipo_página */
	typedef struct tipo_pagina2* Tipo_apontador2;


/* cria uma estrutura árvore B* de pesquisa em memória principal */
void cria_arvoreb2(FILE* arv, Tipo_apontador2* Ap, long TAM, Analise* estatistica);

/* pesquisa um registro na árvore B* e retona se ele está presente ou não na estrutura */
int pesquisa_b2(Registro* A, Tipo_apontador2 *Ap, Analise* estatistica);

/* trata os métodos de inserção devida em uma página da árvore B* */
void insercao_em_pagina_b2(Tipo_apontador2 Ap, Registro A, Tipo_apontador2 Ap_direita, Analise* estatistica);

/* auxilia o método de inserção de registros na árvore B* em memória principal */
void insercao_aux_b2(Registro A, Tipo_apontador2 Ap, short* cresceu, Registro *A_retorno, Tipo_apontador2* Ap_retorno, Analise* estatistica);

/* método inicial de inserção devida em árvore B* */
void insercao_em_arv_b2(Registro* A, Tipo_apontador2 *Ap, Analise* estatistica);

/* percorre a árvore B* afim de se imprimir todos os seus registros ordenadamente */
void percorre(Tipo_apontador2 *Ap);

#endif