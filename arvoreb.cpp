#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdio>

#include "arvoreb.h"

using namespace std;

	/* estrutura e seus dados que seram utilizados */
	struct registro
	{
		long int chave;  	 /* a chave, ÚNICA, que será utilizada como referencia para todos os métodos apresentados.*/
		long int idade; 	 /* dado 1, que representa a idade do usuário registrado */
		char nome[TAM_NOME]; /* dado 2 que representa o nome do usuário registrado */		
	};

	/* estrutura página que representa os Nodos da árvore */
	struct tipo_pagina
	{
		short n_filhos; 					 /* número de registros/filhos que a página contém */
		Registro reg[ORDEM_MAX]; 			 /* os registros contidos na página */
		Tipo_apontador apont[ORDEM_MAX + 1]; /* os apontadores de registros (quantos filhos) a página tem */
	};
		

/* recebe um arquivo binário, a raiz da árvore e a quantidade de registros que seram manipulados no decorrer do programa 
	a cada iteração do while envia um registro novo que será devidamente acrescentado a uma página da árvore */
void cria_arvoreb(FILE* arv, Tipo_apontador* Ap, long TAM, Analise* estatistica)
{
	Registro A;
	long int x = 0; /* contador de registros a serem lidos de acordo com parametro passado */


	while(fread(&A, sizeof(Registro), 1, arv) == 1 && x < TAM)
	{
		t_hit(estatistica); /* referente aos proximos fread do while */
		
		/*envia registro lido para inserção em página da árvore */
		insercao_em_arv(&A, Ap, estatistica);
		x++;

	}
}

/* pesquisa recursiva em arvore afim de se encontrar o registro desejado */
int pesquisa_em_arvb(Registro *A, Tipo_apontador Ap, Analise* estatistica)
{
	/* registro sem referencia ou nao encontrado na arvore */
	if(Ap == NULL)
		return 0;

	long cont = 1;

	/* pesquisa para encontrar o intervalo desejado */
	c_hit(estatistica); //trata a primeira comparação do while 
	while(cont < Ap->n_filhos && A->chave > Ap->reg[cont-1].chave)
	{
		cont++;
		c_hit(estatistica); //trata as próximas comparações do while
	}

	/* verifica se a chave é a desejada */
	c_hit(estatistica);
	if(A->chave == Ap->reg[cont-1].chave)
	{
		*A = Ap->reg[cont-1];
		return 1; //encontrou
	}

	/* iniciando recursao a direita ou a esquerda */
	c_hit(estatistica);
	if(A->chave < Ap->reg[cont-1].chave)
		return pesquisa_em_arvb(A, Ap->apont[cont-1], estatistica);
	else
		return pesquisa_em_arvb(A, Ap->apont[cont], estatistica);
}

/* método é chamada quando sabe-se qual página o registro vai ser inserido, e trata a inserção na posição correta */
void insercao_em_pagina(Tipo_apontador Ap, Registro A, Tipo_apontador Ap_direita, Analise* estatistica)
{
	short nao_achou;
	int num_filhos;

	num_filhos = Ap->n_filhos;

	/* if ternário, verificando se tem filhos ou nao */
	nao_achou = (num_filhos > 0) ? 1 : 0;

	while(nao_achou)
	{
		c_hit(estatistica);
		if(A.chave >= Ap->reg[num_filhos - 1].chave)
			nao_achou = 0;
		else
		{
			Ap->reg[num_filhos] = Ap->reg[num_filhos - 1];
			Ap->apont[num_filhos + 1] = Ap->apont[num_filhos];
			num_filhos--;

			nao_achou = (num_filhos < 1) ? 0 : 1;	
		}
	}

	Ap->reg[num_filhos] = A;
	Ap->apont[num_filhos + 1] = Ap_direita;
	Ap->n_filhos++;
}

/* pesquisa em qual página o registro vai ser inserido e trata os problemas de página cheia (deslocamentos) */
void insercao_aux(Registro A, Tipo_apontador Ap, short* cresceu, Registro *A_retorno, Tipo_apontador* Ap_retorno, Analise* estatistica)
{
	/* criando variaveis */
	long cont = 1, j;
	Tipo_apontador Ap_aux;

	/* verificando entrada nula */
	if(Ap == NULL)
	{
		*cresceu = 1;
		(*A_retorno) = A;
		(*Ap_retorno) = NULL;
		return;
	}

	c_hit(estatistica); /* primeira comparacao do while */
	while(cont < Ap->n_filhos && A.chave > Ap->reg[cont - 1].chave)
	{
		cont++;
		c_hit(estatistica); /* proximas comparacoes */
	}

	c_hit(estatistica);
	if(A.chave < Ap->reg[cont -1].chave)
		cont--;

	/* recursão a fim de se econtrar um Nodo folha para inserção */
	insercao_aux(A, Ap->apont[cont], cresceu, A_retorno, Ap_retorno, estatistica); 

	if(!(*cresceu))
		return;

	/* a página ainda tem espaco para insercao_em_arv */
	if(Ap->n_filhos < ORDEM_MAX)
	{
		insercao_em_pagina(Ap, *A_retorno, *Ap_retorno, estatistica);
		*cresceu = 0;
		return;
	}

	/* página sem espaço, tera que ser dividida */
	Ap_aux = new Tipo_pagina;

	Ap_aux->n_filhos = 0;
	Ap_aux->apont[0] = NULL;

	/* verificando posicao para inserção */
	if(cont < (ORDEM + 1))
	{
		insercao_em_pagina(Ap_aux, Ap->reg[ORDEM_MAX -1], Ap->apont[ORDEM_MAX], estatistica);
		Ap->n_filhos--;
		insercao_em_pagina(Ap, *A_retorno, *Ap_retorno, estatistica);
	}
	else
		insercao_em_pagina(Ap_aux, *A_retorno, *Ap_retorno, estatistica);

	for(j = (ORDEM + 2); j <= ORDEM_MAX; j++)
		insercao_em_pagina(Ap_aux, Ap->reg[j-1], Ap->apont[j], estatistica);

	Ap->n_filhos = ORDEM;
	Ap_aux->apont[0] = Ap->apont[ORDEM + 1];

	*A_retorno = Ap->reg[ORDEM];
	*Ap_retorno = Ap_aux;

	return;
}

/* função que inicia a inserção de um registro na arvore B*/
void insercao_em_arv(Registro* A, Tipo_apontador *Ap, Analise* estatistica)
{
	short cresceu;

	Registro A_retorno;
	Tipo_pagina *Ap_retorno, *Ap_aux;

	/* chama auxiliar que trata os possiveis problemas de inserção */
	insercao_aux(*A, *Ap, &cresceu, &A_retorno, &Ap_retorno, estatistica);	

	/*verifica se foi atingido o nivel raiz da arvore */
	if(cresceu)
	{
		Ap_aux = new Tipo_pagina;
		
		Ap_aux->n_filhos = 1;
		Ap_aux->reg[0] = A_retorno;
		Ap_aux->apont[1] = Ap_retorno;
		Ap_aux->apont[0] = *Ap;
		*Ap = Ap_aux;
	}
}

/* trata da fusã ou empréstimo de registros */
void reconstitui(Tipo_apontador Ap_pagina, Tipo_apontador Ap_pai, int pos_pai, short* diminuiu, Analise* estatistica)
{
	Tipo_pagina *aux;
	long Disp_aux, j;

	/* aux = Pagina a direita de Ap_pagina */
	if(pos_pai < Ap_pai->n_filhos)
	{
		aux = Ap_pai->apont[pos_pai +1];
		Disp_aux = (aux->n_filhos - ORDEM + 1)/2;
		Ap_pagina->reg[Ap_pagina->n_filhos] = Ap_pai->reg[pos_pai];
		Ap_pagina->apont[Ap_pagina->n_filhos +1] = aux->apont[0];
		Ap_pagina->n_filhos++;

		/* Existe uma folga > transferencia de aux para Ap_pagina */
		if(Disp_aux > 0)
		{
			for(j = 1; j< Disp_aux; j++)
				insercao_em_pagina(Ap_pagina, aux->reg[j-1], aux->apont[j], estatistica);

			Ap_pai->reg[pos_pai] = aux->reg[Disp_aux -1];
			aux->n_filhos -= Disp_aux;

			for(j = 0; j< aux->n_filhos; j++)
				aux->reg[Disp_aux] = aux->reg[j + Disp_aux];

			for(j = 0; j <= aux->n_filhos; j++)
				aux->apont[j] = aux->apont[j + Disp_aux];

			*diminuiu = 0;
		}
		/* Fusão intercala aux em Ap_pagina e libera aux */
		else
		{
			for(j = 1; j <= ORDEM; j++)
				insercao_em_pagina(Ap_pagina, aux->reg[j-1], aux->apont[j], estatistica);

			delete aux;

			for(j = pos_pai + 1; j<Ap_pai->n_filhos; j++)
			{
				Ap_pai->reg[j-1] = Ap_pai->reg[j];
				Ap_pai->apont[j] = Ap_pai->apont[j+1];
			}

			Ap_pai->n_filhos--;

			if(Ap_pai->n_filhos >= ORDEM)
				*diminuiu = 0;
		}
	}
	/* aux = Pagina a esquerda de Ap_pagina */
	else
	{
		aux = Ap_pai->apont[pos_pai -1];
		Disp_aux = (aux->n_filhos - ORDEM + 1)/2;

		for(j = Ap_pagina->n_filhos; j >= 1; j--)
			Ap_pagina->reg[j] = Ap_pagina->reg[j-1];

		Ap_pagina->reg[0] = Ap_pai->reg[pos_pai -1];

		for(j = Ap_pagina->n_filhos; j>= 0; j--)
			Ap_pagina->apont[j+1] = Ap_pagina->apont[j];

		Ap_pagina->n_filhos++;

		/* existe folga : transferir de aux para ap_pagina */
		if(Disp_aux > 0)
		{
			for(j = 1; j < Disp_aux; j++)
				insercao_em_pagina(Ap_pagina, aux->reg[aux->n_filhos - j], aux->apont[aux->n_filhos + 1], estatistica);

			Ap_pagina->apont[0] = aux->apont[aux->n_filhos - Disp_aux + 1];
			Ap_pai->reg[pos_pai -1] = aux->reg[aux->n_filhos - Disp_aux];
			aux->n_filhos -= Disp_aux;
			*diminuiu = 0;
		}
		/* fusao: intercalar ap_pagina em aux e liberar ap_pagina */
		else
		{
			for(j = 1; j<= ORDEM; j++)
				insercao_em_pagina(aux, Ap_pagina->reg[j-1], Ap_pagina->apont[j], estatistica);

			delete Ap_pagina;

			Ap_pai->n_filhos--;

			if(Ap_pai->n_filhos >= ORDEM)
				*diminuiu = 0;
		}
	}
}

/* verifica qual é o antecessor para substituir o registro retirado sem perder as propriedades da arvore */
void antecessor(Tipo_apontador Ap, int ind, Tipo_apontador Ap_pai, short *diminuiu, Analise* estatistica)
{
	if(Ap_pai->apont[Ap_pai->n_filhos] != NULL)
	{
		antecessor(Ap, ind, Ap_pai->apont[Ap_pai->n_filhos], diminuiu, estatistica);

		if(*diminuiu)
			reconstitui(Ap_pai->apont[Ap_pai->n_filhos], Ap_pai, (long)Ap_pai->n_filhos, diminuiu, estatistica);

		return;
	}

	Ap->reg[ind -1] = Ap_pai->reg[Ap_pai->n_filhos -1];
	Ap_pai->n_filhos--;
	*diminuiu = (Ap_pai->n_filhos < ORDEM);
}

/* funcao que starta todo comando de remocao verificando qtdade de filhos, escolhendo qual antecessor e tratando fusao ou emprestimo */
void retira_aux(Registro A, Tipo_apontador *Ap, short *diminuiu, Analise* estatistica)
{
	long j, ind = 1;
	Tipo_apontador Pag;

	if(*Ap == NULL)
	{
		/* tratar retornar int e avisar que nao entra na arvore */
		return;
	}

	Pag = *Ap;

	c_hit(estatistica); /* primeira comparacao do while */
	while(ind < Pag->n_filhos && A.chave > Pag->reg[ind-1].chave)
	{
		c_hit(estatistica);
		ind++;
	}

	c_hit(estatistica);
	if(A.chave == Pag->reg[ind-1].chave)
	{
		/*Pagina e uma folha */
		if(Pag->apont[ind-1] == NULL)
		{
			Pag->n_filhos--;
			*diminuiu = (Pag->n_filhos < ORDEM);

			for(j = ind; j <= Pag->n_filhos; j++)
			{
				Pag->reg[j-1] = Pag->reg[j];
				Pag->apont[j] = Pag->apont[j+1];
			}

			return;
		}

		/* Pagina nao e folha: trocar com antecessor */
		antecessor(*Ap, ind, Pag->apont[ind-1], diminuiu, estatistica);

		if(*diminuiu)
			reconstitui(Pag->apont[ind-1], *Ap, ind-1, diminuiu, estatistica);

		return;
	}

	c_hit(estatistica);
	if(A.chave > Pag->reg[ind-1].chave)
		ind++;

	retira_aux(A, &Pag->apont[ind-1], diminuiu, estatistica);

	if(*diminuiu)
		reconstitui(Pag->apont[ind-1], *Ap, ind-1, diminuiu, estatistica);
}

/* funcao inicial da remoção recebe o registro com a chave a ser removida e a raiz da arvore */
void retira(Registro A, Tipo_apontador *Ap, Analise* estatistica)
{
	short diminuiu;

	Tipo_apontador aux;

	retira_aux(A, Ap, &diminuiu, estatistica);

	/* arvore vai diminuir altura */
	if(diminuiu && (*Ap)->n_filhos == 0)
	{
		aux = *Ap;
		*Ap = aux->apont[0];
		delete aux;
	}
}

/* impressao ordenada de registros (Central)*/
void percorre(Tipo_apontador *Ap)
{
	/* se o proximo apontador for nulo quer dizer que a recursão chegou em um nó folha (na arvore B todos as folhas estão no mesmo nivel)*/
	if((*Ap)->apont[0] != NULL)
	{
		int i;
		/*faz uma iteração ate o numero de filhos do pai */
		for(i = 0; i<(*Ap)->n_filhos; i++)
		{
			percorre(&(*Ap)->apont[i]);
			imprime(&(*Ap)->reg[i]);
		}

		/* faz a ultima iteração, pois o numero de apontadores é o numero de filhos + 1 */
		percorre(&(*Ap)->apont[i]);

		return;
	}

	/* imprime todas as folhas pelo numero de filhos */
	for(int j=0; j<(*Ap)->n_filhos; j++)
		imprime(&(*Ap)->reg[j]);

	return;
}