#ifndef PSIN_H_
#define PSIN_H_

#include "analise.h"

#define TAM_DADOS 1000000 	/*numero de itens no arquivo */
#define TAM_NOME 15 		/* nome sugerido para item 2 */
#define ITENS_POR_PAGINA 4 	/* itens em cada pagina */

	/* nome da estrutura utilizada para guardar dados */
	typedef struct registro Registro;

/* metodo cria um registro e aloca dinamicamente */
Registro* cria_registro();

/* metodo deleta espaço alocado dinamicamente para registro utilizado em tempo de execução */
void deleta_registro(Registro* A);

/* preenche um vetor dinamicamente com as chaves (indices) do inicio de cada pagina. A quantidade é determinada pela constante ITENS_POR_PAGINA */
int paginacao_em_MP(FILE* arqb, int& quant_indices, int* tabela, long TAM, Analise* estatistica);

/* faz uma varredura na estrutura previamente criada e e retorna se encontrou ou nao o registro solicitado pelo usuario */
int pesquisa(FILE* arqb, Registro* buscar, int quant_indices, int* tabela, int ordem, Analise* estatistica);

/*faz a impressão completa de todos os dados do registro */
void imprime(Registro* A);

/* faz a manutencção da chave do registro, preservando o acesso direto aos dados */
void set_chave(Registro* A, int x);

#endif
