#ifndef ARVORE_H_
#define ARVORE_H_

#include "psin.h"

	/*estrutura nodo sera guardada no arquivo arvore */
	typedef struct nodo Nodo;

/*cria um Nodo dinamicamente */
Nodo* cria_nodo();

/* deleta um nodo dinamicamente */
void deleta_nodo(Nodo* A);

/* cria toda a estrutura da arvore em arquivo com seus indices (direita/esquerda)*/
int cria_arvore_externa(FILE* arqb, FILE* arv, long TAM, Analise* estatistica);

/*preenche um Nodo a ser guardado na arvore em arquivo */
void preenche_no(Nodo* filho, Registro A);

/* faz uma pesquisa em arquivo a fim de encontrar a chave desejada */
int pesquisa_arv_externa(FILE* arv, Nodo* filho, Registro *retorno, Analise* estatistica);

#endif