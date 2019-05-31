#ifndef ARVOREB_H_
#define ARVOREB_H_

#define ORDEM 2 			/* Tamanho da ordem da arvore */
#define ORDEM_MAX ORDEM*2 	/* tamanho máximo de registros que uma página pode ter */

#include "psin.h"

	/* estrutura que armazenará registros, a quantidade e regida pela constante ORDEM e ORDEM_MAX */
	typedef struct tipo_pagina Tipo_pagina;
	/*estrutura que apontará as páginas filho do Tipo_página */
	typedef struct tipo_pagina* Tipo_apontador;

/* recebe um arquivo e cria uma árvore B em memória principal */
void cria_arvoreb(FILE* arv, Tipo_apontador* Ap, long TAM, Analise* estatistica);

/*pesquisa um registro desejado na estrutura árvore B, préviamente criada */
int pesquisa_em_arvb(Registro *A, Tipo_apontador Ap, Analise* estatistica);

/* insere um Registro na posição correta da página em uma árvore B*/
void insercao_em_pagina(Tipo_apontador Ap, Registro A, Tipo_apontador Ap_direita, Analise* estatistica);

/* método auxiliar de inserção de um registro em uma árvore B */
void insercao_aux(Registro A, Tipo_apontador Ap, short* cresceu, Registro *A_retorno, Tipo_apontador* Ap_retorno, Analise* estatistica);

/*método inicial de inserção de registros em árvore B */
void insercao_em_arv(Registro* A, Tipo_apontador *Ap, Analise* estatistica);

/* método trata a reconstituição de uma árvore B, após uma remoção de registro */
void reconstitui(Tipo_apontador Ap_pagina, Tipo_apontador Ap_pai, int pos_pai, short* diminuiu, Analise* estatistica);

/* pesquisa o antecessor dentro de uma árvore B para não perder a propriedade da árvore após a remoção de um registro */
void antecessor(Tipo_apontador Ap, int ind, Tipo_apontador Ap_pai, short *diminuiu, Analise* estatistica);

/*método auxiliar que remove um registro da árvore B */
void retira_aux(Registro A, Tipo_apontador *Ap, short *diminuiu, Analise* estatistica);

/* método inicial que remoção de um registro em árvore B */
void retira(Registro A, Tipo_apontador *Ap, Analise* estatistica);

/* Percorre uma árvore B a fim de imprimir todos os seus registros ordenadamente */
void percorre(Tipo_apontador *Ap);

#endif