#include <iostream>

#include "arvoreb2.h"

using namespace std;

	/* tipo enumerado que serve como direcionamento de uma página, sendo Externa ou Interna */
	enum Tipo
	{
		INT, /* página marcada como INT será tratada como interna e armazenará apenas a chave de um registro */
		EXT	 /* página marcada como EXT será tratada como externa e armazenará o registro como um todo (apenas Nodos folhas são externos)*/	
	};

	/* estrutura e seus dados que seram utilizados */
	struct registro
	{
		long int chave;  	 /* a chave, ÚNICA, que será utilizada como referencia para todos os métodos apresentados.*/
		long int idade; 	 /* dado 1, que representa a idade do usuário registrado */
		char nome[TAM_NOME]; /* dado 2 que representa o nome do usuário registrado */		
	};

	/* União dos atributos de uma página interna e externa, o que diferenciará cada uma é o seu TIPO (explicado acima em "enum")*/
	struct tipo_pagina2
	{
		Tipo type;
		union //uni duas estruturas
		{
			struct //estrutura de uma página interna
			{
				int n_filhos_int; 					 	 /* número de filhos de uma página */
				long chave[ORDEM_MAX];					 /* chaves que representam registros que podem ou não estar contidos na árvore */
				Tipo_apontador2 apont_int[ORDEM_MAX +1]; /* apontadores de página (interna/externa) */	

			}Pagina_interna;

			struct //estrutura de uma página externa
			{
				int n_filhos_ext; 				/*número de filhos de uma página */
				Registro reg_ext[ORDEM_MAX]; 	/* registros completos existentes na página */
				Tipo_pagina2* prox; 			/* ponteiro para a página que mostra onde se encontra a próxima página ordenadamente */

			}Pagina_externa;
			
		}Uniao;		
	};

/* recebe um arquivo binário, a raiz da árvore e a quantidade de itens a serem manipulados pelo método, a fim de criar a estrutura árvore B* 
	loop que lê registro a registro do arquivo e envia para o método que trata a inserção devida dele na árvore */
void cria_arvoreb2(FILE* arv, Tipo_apontador2* Ap, long TAM, Analise* estatistica)
{
	Registro A;
	long int x = 0; /* contador de registros a serem lidos de acordo com parâmetro passado */

	
	while(fread(&A, sizeof(Registro), 1, arv) == 1 && x < TAM) //uma leitura por vez dentro do arquivo.
	{
		t_hit(estatistica); /* referente aos proximos fread do while */
		
		/*envia registro e raiz da árvore para a devida inserção */
		insercao_em_arv_b2(&A, Ap, estatistica);
		x++;
		
	}
}

/* método recursivo recebe a chave do registro a ser pesquisado, a raiz da árvore e usa seus apontadores
	para se chegar a uma página folha onde se encontra os registros completos, se encontrado o registro e retornado */
int pesquisa_b2(Registro* A, Tipo_apontador2 *Ap, Analise* estatistica)
{
	int i;
	
	Tipo_apontador2 pag;
	pag = *Ap;

	if((*Ap)->type == INT) //verificação do tipo da página 
	{
		i = 1;
		c_hit(estatistica); /*primeira comparação do while */
		while(i < pag->Uniao.Pagina_interna.n_filhos_int && A->chave > pag->Uniao.Pagina_interna.chave[i-1])
		{
			i++;
			c_hit(estatistica);
		}

		c_hit(estatistica);
		/* inicia a recursão com seus apontadores, descendo na árvore */
		if(A->chave < pag->Uniao.Pagina_interna.chave[i-1])
			return pesquisa_b2(A, &pag->Uniao.Pagina_interna.apont_int[i-1], estatistica);
		else
			return pesquisa_b2(A, &pag->Uniao.Pagina_interna.apont_int[i], estatistica);
	}

	i = 1;
	c_hit(estatistica); /*primeira comparação do while */
	while(i < pag->Uniao.Pagina_externa.n_filhos_ext && A->chave > pag->Uniao.Pagina_externa.reg_ext[i-1].chave)
	{
		i++;
		c_hit(estatistica);
	}

	c_hit(estatistica);
	/* caso encontrado, registro é copiado e retornado */
	if(A->chave == pag->Uniao.Pagina_externa.reg_ext[i-1].chave)
	{
		*A = pag->Uniao.Pagina_externa.reg_ext[i-1];
		return 1;
	}
	else
		return 0;
}

/* insere um registro dentro de uma página ordenadamente ajusta seu apontador no caso de uma página interna */
void insercao_em_pagina_b2(Tipo_apontador2 Ap, Registro A, Tipo_apontador2 Ap_direita, Analise* estatistica)
{
	short nao_achou;
	int num_filhos;

	//inserção em página externa
	if(Ap->type == EXT)
	{
		num_filhos = Ap->Uniao.Pagina_externa.n_filhos_ext;

		/* if ternário, verificando se tem filhos ou não, valor 0 ou 1 é booleano para o próximo teste */
		nao_achou = (num_filhos > 0) ? 1 : 0;

		while(nao_achou) // igual a 1.
		{	
			c_hit(estatistica);
			if(A.chave >= Ap->Uniao.Pagina_externa.reg_ext[num_filhos - 1].chave)
				nao_achou = 0;
			else
			{
				Ap->Uniao.Pagina_externa.reg_ext[num_filhos] = Ap->Uniao.Pagina_externa.reg_ext[num_filhos - 1];
				num_filhos--;

				nao_achou = (num_filhos < 1) ? 0 : 1;	
			}
		}

		Ap->Uniao.Pagina_externa.reg_ext[num_filhos] = A;	
		Ap->Uniao.Pagina_externa.n_filhos_ext++;
		return;		
	}
	else //inserção em página interna
	{
		num_filhos = Ap->Uniao.Pagina_interna.n_filhos_int;

		/* if ternário, verificando se tem filhos ou nao */
		nao_achou = (num_filhos > 0) ? 1 : 0;

		while(nao_achou)
		{
			c_hit(estatistica);
			if(A.chave >= Ap->Uniao.Pagina_interna.chave[num_filhos - 1])
				nao_achou = 0;
			else
			{
				Ap->Uniao.Pagina_interna.chave[num_filhos] = Ap->Uniao.Pagina_interna.chave[num_filhos -1];
				Ap->Uniao.Pagina_interna.apont_int[num_filhos + 1] = Ap->Uniao.Pagina_interna.apont_int[num_filhos];
				num_filhos--;

				nao_achou = (num_filhos < 1) ? 0 : 1;	
			}
		}

		Ap->Uniao.Pagina_interna.chave[num_filhos] = A.chave;
		Ap->Uniao.Pagina_interna.apont_int[num_filhos + 1] = Ap_direita;
		Ap->Uniao.Pagina_interna.n_filhos_int++;
		return;
	}
}

/* método auxiliar de inserção que recebe o registro a ser inserido, a raiz e trata a de pesquisar a posição correta e trata 
	os possíveis problemas encontrados como deslocamento e fusão de páginas */
void insercao_aux_b2(Registro A, Tipo_apontador2 Ap, short* cresceu, Registro *A_retorno, Tipo_apontador2* Ap_retorno, Analise* estatistica)
{
	/* criando variaveis */
	long cont = 1, j;
	Tipo_apontador2 Ap_aux;

	/* verificando se e página externa e se cabe a inserção */
	if(Ap->type == EXT)
	{
		/* testa se a página consegue receber o registro sem extourar a capacidade total */
		if(Ap->Uniao.Pagina_externa.n_filhos_ext < ORDEM_MAX) 
		{		
			(*A_retorno) = A;
			insercao_em_pagina_b2(Ap, *A_retorno, *Ap_retorno, estatistica);
			*cresceu = 0;
			return;
		}
		else /* página cheia */
		{
			(*A_retorno) = A;
			(*Ap_retorno) = NULL;

			c_hit(estatistica); /* primeira comparação do while */
			while(cont < Ap->Uniao.Pagina_externa.n_filhos_ext && A.chave > Ap->Uniao.Pagina_externa.reg_ext[cont-1].chave)
			{
				cont++;	
				c_hit(estatistica);
			}

			c_hit(estatistica);
			if(A.chave < Ap->Uniao.Pagina_externa.reg_ext[cont-1].chave)
				cont--;

			/* página sem espaço, tera que ser dividida */
			Ap_aux = new Tipo_pagina2;
			Ap_aux->type = EXT;

			Ap_aux->Uniao.Pagina_externa.prox = Ap->Uniao.Pagina_externa.prox;
			Ap->Uniao.Pagina_externa.prox = Ap_aux; /* encadeando lista */

			Ap_aux->Uniao.Pagina_externa.n_filhos_ext = 0;

			/* verificando posição para inserção */
			if(cont < (ORDEM + 1))
			{
				insercao_em_pagina_b2(Ap_aux, Ap->Uniao.Pagina_externa.reg_ext[ORDEM_MAX -1], *Ap_retorno, estatistica);
				Ap->Uniao.Pagina_externa.n_filhos_ext--;
				insercao_em_pagina_b2(Ap, *A_retorno, *Ap_retorno, estatistica);
			}
			else
				insercao_em_pagina_b2(Ap_aux, *A_retorno, *Ap_retorno, estatistica);

			for(j = 1; j <= ORDEM; j++)
				insercao_em_pagina_b2(Ap_aux, Ap->Uniao.Pagina_externa.reg_ext[ORDEM_MAX-j], *Ap_retorno, estatistica);

			Ap->Uniao.Pagina_externa.n_filhos_ext = ORDEM;

			*A_retorno = Ap->Uniao.Pagina_externa.reg_ext[ORDEM];
			*Ap_retorno = Ap_aux;

			*cresceu = 1;
			return;
		}
	}

	/*verificacao página interna */
	c_hit(estatistica); /* primeira comparação do while */
	while(cont < Ap->Uniao.Pagina_interna.n_filhos_int && (long)A.chave > Ap->Uniao.Pagina_interna.chave[cont - 1])
	{
		cont++;
		c_hit(estatistica);
	}

	c_hit(estatistica);
	if(A.chave < Ap->Uniao.Pagina_interna.chave[cont -1])
		cont--;

	/* recursão com o apontador correto a fim de encontrar o local de inserção do registro */
	insercao_aux_b2(A, Ap->Uniao.Pagina_interna.apont_int[cont], cresceu, A_retorno, Ap_retorno, estatistica);

	if(!(*cresceu))
		return;

	/* a página INTERNA ainda tem espaco para inserção */
	if(Ap->Uniao.Pagina_interna.n_filhos_int < ORDEM_MAX)
	{
		insercao_em_pagina_b2(Ap, *A_retorno, *Ap_retorno, estatistica); //insere na página 
		*cresceu = 0;
		return;
	}
	/* página sem espaço, tera que ser dividida */
	Ap_aux = new Tipo_pagina2;
	Ap_aux->type = INT;

	Ap_aux->Uniao.Pagina_interna.n_filhos_int = 0;
	Ap_aux->Uniao.Pagina_interna.apont_int[0] = NULL;

	/*cria registro auxiliar para armazenar a chave que sera inserida na página interna especificada */
	Registro carrega_chave;	

	/* verificando posição para inserção */
	if(cont < (ORDEM + 1))
	{
		carrega_chave.chave = Ap->Uniao.Pagina_interna.chave[ORDEM_MAX -1];

		insercao_em_pagina_b2(Ap_aux, carrega_chave, Ap->Uniao.Pagina_interna.apont_int[ORDEM_MAX], estatistica);
		Ap->Uniao.Pagina_interna.n_filhos_int--;
		insercao_em_pagina_b2(Ap, *A_retorno, *Ap_retorno, estatistica);
	}
	else
		insercao_em_pagina_b2(Ap_aux, *A_retorno, *Ap_retorno, estatistica);

	/* envia os registros para a nova página */
	for(j = (ORDEM + 2); j <= ORDEM_MAX; j++)
	{
		carrega_chave.chave = Ap->Uniao.Pagina_interna.chave[j-1];
		insercao_em_pagina_b2(Ap_aux, carrega_chave, Ap->Uniao.Pagina_interna.apont_int[j], estatistica);
	}

	Ap->Uniao.Pagina_interna.n_filhos_int = ORDEM;
	Ap_aux->Uniao.Pagina_interna.apont_int[0] = Ap->Uniao.Pagina_interna.apont_int[ORDEM + 1];

	A_retorno->chave = Ap->Uniao.Pagina_interna.chave[ORDEM];
	*Ap_retorno = Ap_aux;

	return;
}

/* função que inicia a inserção de um registro na arvoreB* recebe o registro a ser inserido, a raiz da árvore */
void insercao_em_arv_b2(Registro* A, Tipo_apontador2 *Ap, Analise* estatistica)
{
	Registro aux = *A;
	short cresceu;
	Registro A_retorno;
	Tipo_pagina2 *Ap_retorno, *Ap_aux;

	/* tratando primeira inserção */
	if((*Ap) == NULL)
	{
		Ap_aux = new Tipo_pagina2;
		Ap_aux->type = EXT;		
		Ap_aux->Uniao.Pagina_externa.n_filhos_ext = 1;
		Ap_aux->Uniao.Pagina_externa.reg_ext[0] = aux;
		*Ap = Ap_aux;
		Ap_aux->Uniao.Pagina_externa.prox = NULL;

	}
	/* procurar página folha para inserção */
	else
	{
		insercao_aux_b2(aux, *Ap, &cresceu, &A_retorno, &Ap_retorno, estatistica);

		/*verifica se foi atingido o nível raiz da árvore */
		if(cresceu)
		{
			Ap_aux = new Tipo_pagina2;
			Ap_aux->type = INT;
		
			Ap_aux->Uniao.Pagina_interna.n_filhos_int = 1;
			Ap_aux->Uniao.Pagina_interna.chave[0] = A_retorno.chave;
			Ap_aux->Uniao.Pagina_interna.apont_int[1] = Ap_retorno;
			Ap_aux->Uniao.Pagina_interna.apont_int[0] = *Ap;
			*Ap = Ap_aux;		
		}		
	}
	return;
}

/* percorre recursivamente as página da árvore afim de se encontrar o primeira mais a esquerda, onde se encontra
	o menor registro, após isso e feita uma impressão dos registros linearmente, como uma varredura em lista
	com o apoio dos apontadores de página (próx) dentro das páginas externas */
void percorre(Tipo_apontador2 *Ap)
{
	Tipo_apontador2 pag;
	pag = *Ap;

	if((*Ap)->type == INT) /* esgotamento a esquerda a encontrar a primeira página externa */
		return percorre(&pag->Uniao.Pagina_interna.apont_int[0]);
	
	if((*Ap)->type == EXT) /* página externa, loop ate chegar ao NULL último no folha */
		for(Tipo_apontador2 aux = pag; aux != NULL; aux = aux->Uniao.Pagina_externa.prox) 
			for(int i=0; i < aux->Uniao.Pagina_externa.n_filhos_ext; i++)/* loop em paginas folha para imprimir seus registros */
				imprime(&aux->Uniao.Pagina_externa.reg_ext[i]); /*imprimindo registros */

	return;
}