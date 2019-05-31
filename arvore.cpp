#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdio>
#include "arvore.h"

using namespace std;

	/* estrutura e seus dados que seram utilizados */
	struct registro
	{
		long int chave;  	 /* a chave, ÚNICA, que será utilizada como referencia para todos os métodos apresentados.*/
		long int idade; 	 /* dado 1, que representa a idade do usuário registrado */
		char nome[TAM_NOME]; /* dado 2 que representa o nome do usuário registrado */		
	};

	/* estrutura que armazena os dados a serem guardados no arquivo arvore externa */
	struct nodo
	{
		long int esq; 		/* um indice que representa em qual linha do arquivo seu filho a esquerda se encontra */
		Registro item;		/* guarda uma estrutura registro */
		long int dir; 		/* um indice que representa em qual linha do arquivo seu filho a direita se encontra */
	};
		
/* Cria aloca dinamicamente e retorna um Nodo corretamente */
Nodo* cria_nodo()
{
	Nodo* A;
	A = new Nodo;

	return A;
}

/* Deleta um Nodo passado como parametro dinamicamente */
void deleta_nodo(Nodo* A)
{
	delete A;
}

/* recebe o arquivo os estão os registros, o arquivo que sera montado como estrutura árvore e a quantidade de dados a serem manipulados 
	retorna êxito na criação da árvore em arquivo */
int cria_arvore_externa(FILE* arqb, FILE* arv, long TAM, Analise* estatistica)
{
	Registro A;
	Nodo auxiliar;
	Nodo filho;

	int contador = 0;
	int posicao_filho=0;
	bool flag = 1;

	int regulador_de_tamanho = 0;


	/*lê registro a registro do arquivo */
	while(fread(&A, sizeof(Registro), 1, arqb) == 1 && regulador_de_tamanho < TAM)
	{
		flag = 1;
		preenche_no(&filho, A);

		t_hit(estatistica);
		/* faz a gravação do Nodo criado no arquivo arv*/
		fwrite(&filho, sizeof(Nodo), 1, arv);

		contador++;

		if(contador != 1)
		{
			t_hit(estatistica);
			rewind(arv);
			posicao_filho = 0;
			
			t_hit(estatistica); /* referente ao primeor fread do while */
			while(fread(&auxiliar, sizeof(Nodo), 1, arv) == 1 && flag)
			{
				c_hit(estatistica);
				/*testa se a o filho e menor ou maior que o pai */
				if(A.chave < auxiliar.item.chave) // menor
					if(auxiliar.esq == -1) //(-1) é default para "sem filho". 
					{
						auxiliar.esq = contador-1;
						t_hit(estatistica);
						fseek(arv, posicao_filho*sizeof(Nodo), 0);
						t_hit(estatistica);
						fwrite(&auxiliar, sizeof(Nodo), 1, arv);
						flag = 0;						
					}
					else //caso tenha filho realiza o salto de acordo com o indice salvo em "esq".
					{
						t_hit(estatistica);
						fseek(arv, (auxiliar.esq)*sizeof(Nodo), 0);
						posicao_filho = auxiliar.esq;
					}
				else //maior 
				{
					if(auxiliar.dir == -1) //(-1) é default para "sem filho". 
					{
						auxiliar.dir = contador-1;
						t_hit(estatistica);
						fseek(arv, posicao_filho*sizeof(Nodo), 0);
						t_hit(estatistica);
						fwrite(&auxiliar, sizeof(Nodo), 1, arv);
						flag = 0;
					}
					else //caso tenha filho realiza o salto de acordo com o indice salvo em "esq".
					{
						t_hit(estatistica);
						fseek(arv, (auxiliar.dir)*sizeof(Nodo), 0);
						posicao_filho = auxiliar.dir;
					}
				}
				t_hit(estatistica); /* referente aos proximos fread do while interno */
			}
		}
		t_hit(estatistica);
		fseek(arv, 0, 2);

		regulador_de_tamanho++;

		t_hit(estatistica); /* referente aos proximos fread do while externo */
	}

	return 1;
}

/*preenche um nó com default -1 para os apontadores dir/esq e item é preenchido com o registro lido do arquivo */
void preenche_no(Nodo* filho, Registro A)
{
	filho->dir = -1;
	filho->esq = -1;
	filho->item = A;
}

/*pesquisa recursiva em arquivo que analisa o registro e segue seus apontadores para os devidos saltos */
int pesquisa_arv_externa(FILE* arv, Nodo* filho, Registro *retorno, Analise* estatistica)
{
	t_hit(estatistica);
	fread(filho, sizeof(Nodo), 1, arv);

	/* caso base */
	c_hit(estatistica);
	if(filho->item.chave == retorno->chave)
	{
		*retorno = filho->item;
		return 1;
	}

	/* aplicando recursão */
	c_hit(estatistica);
	if(retorno->chave > filho->item.chave && filho->dir != -1)
	{
		t_hit(estatistica);
		fseek(arv, (filho->dir)*sizeof(Nodo), 0);
		return pesquisa_arv_externa(arv, filho, retorno,estatistica);
	}

	c_hit(estatistica);
	if(retorno->chave < filho->item.chave && filho->esq != -1)
	{
		t_hit(estatistica);
		fseek(arv, (filho->esq)*sizeof(Nodo), 0);
		return pesquisa_arv_externa(arv, filho, retorno, estatistica);
	}
	
	return 0;
}