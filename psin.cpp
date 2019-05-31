#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdio>

#include "psin.h"

using namespace std;

	/* estrutura e seus dados que seram utilizados */
	struct registro
	{
		long int chave;  	 /* a chave, ÚNICA, que será utilizada como referencia para todos os métodos apresentados.*/
		long int idade; 	 /* dado 1, que representa a idade do usuário registrado */
		char nome[TAM_NOME]; /* dado 2 que representa o nome do usuário registrado */		
	};

/* não recebe parametros, cria aloca dinamicamente e retorna um registro  */
Registro* cria_registro()
{
	Registro *A;
	A = new Registro;

	return A;
}

/*trata a exclusão de um registro, recebido como parametro */
void deleta_registro(Registro* A)
{
	delete A;
}

/*recebe o arquivo binario, o numero de indices paginados o vetor onde serão salvos os indices/chaves e a quantidade de dados manipulados, esta, passada pelo usuário previamente */
int paginacao_em_MP(FILE* arqb, int& quant_indices, int* tabela, long TAM, Analise* estatistica)
{
	if(tabela == NULL)
		return 0;

	Registro A;

	/* tratando leitura de arquivo para numeros menores que 1 milhão */
	t_hit(estatistica);
	while(fread(&A, sizeof(A), 1, arqb) == 1 && quant_indices < (TAM/ITENS_POR_PAGINA)+1)
	{
		tabela[quant_indices++] = A.chave;

		t_hit(estatistica);
		fseek(arqb, sizeof(Registro)*(ITENS_POR_PAGINA-1), 1);

		t_hit(estatistica);
	}

	return 1;
}

/* Recebe o arquivo onde se possivelmente se encontra o registro a ser pesquisado, um registro de retorno que salva os dados do registro pesquisado para retorno, 
a quantidade de indices do vetor de paginas, a ordem (crescente ou decrescente)*/
int pesquisa(FILE* arqb, Registro* buscar, int quant_indices, int* tabela, int ordem, Analise* estatistica)
{
	/* variaveis utilitarias */
	int i=0;
	long desloc;
	int tam_pagina;

	/* vetor de estrutura que armazenará todos os registros que contiverem em uma página */
	Registro pagina_completa[ITENS_POR_PAGINA];

	/* procurando na tabela de indice a pagina onde o item pode ser encontrado */
	/* tratamento para pesquisa ascendente ou descendente ordenada */
	if(ordem == 1) //crescente
	{
		/* primeira comparacao do while */
		c_hit(estatistica);
		while(i < quant_indices && tabela[i] <= buscar->chave)
		{
			i++;
			c_hit(estatistica); /*proximas comparacoes do while */
		}
	}
	else //decrescente
	{	/* primeira comparacao do while */
		c_hit(estatistica);
		while(i < quant_indices && tabela[i] >= buscar->chave)
		{
			i++;
			c_hit(estatistica); /* proximas comparacoes do while */
		}
	}

	/*caso a chave seja maior que o primeiro indice ou nao exista */
	if(i == 0)
		return 0;
	else
	{
		/* adaptacao para ultima pagina incompleta */
		if(i <= quant_indices)
			tam_pagina = ITENS_POR_PAGINA;
		else
		{
			t_hit(estatistica);
			fseek(arqb, 0, 2);
			/*salto para a pagina */
			tam_pagina = (ftell(arqb)/sizeof(Registro))%ITENS_POR_PAGINA;
		}

		/*leitura de toda a pagina tamanho = tam_pagina */
		desloc =  (i-1)*(ITENS_POR_PAGINA)*sizeof(Registro);

		t_hit(estatistica);
		fseek(arqb, desloc, 0);
		
		t_hit(estatistica);
		/*leitura da pagina completa de uma vez */
		fread(&pagina_completa, sizeof(Registro), tam_pagina, arqb);	

		/* pesquisa pelo item dentro da pagina */
		for(i = 0; i<tam_pagina; i++)
		{
			c_hit(estatistica);
			if(pagina_completa[i].chave == buscar->chave)
			{
				*buscar = pagina_completa[i];
				return 1;
			}
		}

		return 0;
	}
}

/*recebe como parametro o registro e faz a impressao do mesmo formatada e com todos os campos necessários */
void imprime(Registro* A)
{
	cout <<"\nCHAVE:\t" <<A->chave
		 <<"\nIDADE:\t" <<"(" <<A->idade <<")" <<" Anos"
		 <<"\nNOME :\t"  <<A->nome <<endl;
}

/*faz a alteração da chave do registro, recebe como parametro o registro e a nova chave desejada */
void set_chave(Registro* A, int x)
{
	A->chave = x;	
}
