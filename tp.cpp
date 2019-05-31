#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdio>

#include "analise.h"	/* trata todas as contagem necessarias para estatistica de execucao */
#include "psin.h" 		/* trata os metodos da pesquisa sequencial indexada */
#include "arvore.h" 	/* trata os metodos da árvore binaria gerada em arquivo */
#include "arvoreb.h" 	/* trata os metodos da árvore B gerada em memoria principal */
#include "arvoreb2.h" 	/* trata os metodos da árvore B* gerada em memória principal */

using namespace std;

/* testa todos os erros que podem ser cometidos pelo usuário ao executar o programa e suas passagens de argumento */
int teste_arg(int argc, char* argv[]);

/* aberturas de arquivo */
FILE* crescente();	//abre o arquivo crescente 
FILE* decrescente();//abre o arquivo decrescente 
FILE* aleatorio(); 	//abre o arquivo aleatório


/* estruturas propostas */
void PSIN(FILE* arqb, char* argv[], Analise* estatistica);		/* pesquisa sequencial indexada */
void ARV_EXT(FILE* arqb, char* argv[], Analise* estatistica); 	/* arvore binária em arquivo */
void ARV_B(FILE* arqb, char* argv[], Analise* estatistica); 	/* árvore B em memória principal */
void ARV_B2(FILE* arqb, char* argv[], Analise* estatistica); 	/* árvore B* em memória principal */

int main(int argc, char* argv[])
{
	srand(time(NULL));
	/*validando argumentos de entrada */
	if(!teste_arg(argc, argv))
		return 0;

	/* variavel ponteiro de arquivo */
	FILE* arqb;

 	/* estrutura que captura dados para análise posterior (criação por default zera todos os indices) */ 
	Analise* estatistica;
	estatistica = cria_analise();

	/* menu interno que trata leva a cada uma das funções */
	switch(atoi(argv[3]))
	{
		case 1: 
		{ 
			if((arqb = crescente()) == NULL)
				return 0; 

		} break;

		case 2: 
		{ 
			if((arqb = decrescente()) == NULL)
				return 0; 
		} break;

		case 3: 
		{ 
			if((arqb = aleatorio()) == NULL)
				return 0; 
		} break;

		default: 
		{
			cout <<"\n\nDESCULPE DISPONIBILIZAMOS APENAS"
				 <<"\n[1] - ONDENADO ASCENDENTEMENTE"
				 <<"\n[2] - ORDENADO DESCENDENTEMENTE"
				 <<"\n[3] - DESORDENADO ALEATORIAMENTE"
				 <<"\n\nTENTE NOVAMENTE.\n\n";

			fflush(stdout);
			return 0;
		} break;
	}

	/* menu interno que leva a cada uma das funções */
	switch(atoi(argv[1]))
	{
		case 1: 
		{ 
			if(atoi(argv[3]) == 3) //tratamento de erro ĺógico, extrutura solicitada não suporta esse tipo de arquivo.
			{
				cout <<"\n\nDESCULPE A PESQUISA SEQUENCIAL INDEXADA SO PODE SER FEITA COM ARQUIVOS ORDENADOS!"
					 <<"\n\nTENTE NOVAMENTE.\n\n"; 

				fflush(stdout);
				return 0;				
			} 
			else
				PSIN(arqb, argv, estatistica);
		} break;

		case 2: { ARV_EXT(arqb, argv, estatistica); } break;

		case 3: { ARV_B(arqb, argv, estatistica); } break;

		case 4: { ARV_B2(arqb, argv, estatistica); } break; 

		default:
		{
			cout <<"\n\nDESCULPE DISPONIBILIZAMOS APENAS"
				 <<"\n[1] - PESQUISA SEQ. INDEXADA"
				 <<"\n[2] - PESQUISA EM ARQUIVO EXTERNO"
				 <<"\n[3] - PESQUISA EM ARVORE B"
				 <<"\n[4] - PESQUISA EM ARVORE B*"
				 <<"\n\nTENTE NOVAMENTE.\n\n";

			fflush(stdout);
		} break;
	}

	fclose(arqb); /* ao fim da execução fecha o arquivo */
	deleta_analise(estatistica); /* deleta a estrutura de analise dinamicamente */
	return 0;
}

/* menu teste de argumentos */
int teste_arg(int argc, char* argv[])
{	
	/* menu para leigos */
	if(argc == 1)
	{
		cout <<"\n\nBEM VINDO AO PROGRAMA - ANALISE ENTRE METODOS DE PESQUISA EXTERNA -\n"
			 <<"POR: GABRIEL CARVALHO DE SOUZA."
			 <<"\nMAT: 17.1.4029"
			 <<"\nLEIA E SIGA OS PASSOS ABAIXO PARA COMPREENSÃO DA EXECUÇÃO.";

		cout <<"\n\n\t******** MENU ********"
			 <<"\n\n-O EXECUTAVEL DEVE SER INICIALIZADO COM OS SEGUINTES PARAMENTROS: "
			 <<"\n\n./<EXECUTAVEL> <MÉTODO> <QUANTIDADE> <SITUACAO> <CHAVE> [-P] "
			 <<"\n\nONDE: "
			 <<"\n\nMETODO: "
			 <<"\n[1] - PESQUISA SEQUENCIAL INDEXADA. "
			 <<"\n[2] - ARVORE BINARIA EM ARQUIVO. "
			 <<"\n[3] - ARVORE B EM MEMORIA PRINCIPAL. "
			 <<"\n[4] - ARVORE B* EM MEMORIA PRINCIPAL. "
			 <<"\n\nQUANTIDADE: "
			 <<"\n-UM INTEIRO ENTRE 1 E 1000000(1 MILHAO). "
			 <<"\n\nSITUACAO: "
		 	 <<"\n[1] - ARQUIVO ORDENADO ASCENDENTEMENTE. "
			 <<"\n[2] - ARQUIVO ORDENADO DESCENDENTEMENTE."
			 <<"\n[3] - ARQUIVO DESORDENADO ALEATORIAMENTE."
			 <<"\n\nCHAVE: "
			 <<"\n-UM NUMERO DENTRO DO INTERVALO QUE VOCE PREENCHEU EM <QUANTIDADE>. "
			 <<"\n\n[-P]: "
			 <<"\n- É OPCIONAL, COM ELE A CHAVE BUSCADA SERA IMPRESSA NA TELA. "
			 <<"\n- SEM ELE, APENAS AS ESTATISTICAS SERAO MOSTRADAS. "
			 <<"\n\nEXEMPLO DE EXECUCAO: ./tp 2 5000 3 38 -p\n\nBOA SORTE!" <<endl;

		return 0;
	}

	/* trata a opção opcional [-P]*/
	if(argv[5] == NULL)
		argv[5] =(char*)"flag";

	/* 5 sem o -p, 6 com o -p */
	if(argc != 5 && argc != 6)
	{
		cout <<"\nVOCE NAO DIGITOU O NUMERO CORRETO DE ARGUMENTOS NECESSARIOS PARA A PESQUISA\n\n"
		<<"TENTE NOVAMENTE\n" <<endl;
		return 0;
	}

	return 1;
}

/* abre arquivo crescente devidamente trantando erro */
FILE* crescente()
{
	FILE* arqb;

	if((arqb = fopen("crescente.bin", "rb")) == NULL)
	{
		cout <<"\n\nFALHA NA ABERTURA DO ARQUIVO\n";
		arqb = NULL;
		return arqb;
	}
	return arqb;
}

/* abre arquivo crescente devidamente trantando erro */
FILE* decrescente()
{
	FILE* arqb;

	if((arqb = fopen("decrescente.bin", "rb")) == NULL)
	{
		cout <<"\n\nFALHA NA ABERTURA DO ARQUIVO\n";
		arqb = NULL;
		return arqb;
	}
	return arqb;	
}

/* abre arquivo crescente devidamente trantando erro */
FILE* aleatorio()
{
	FILE* arqb;

	if((arqb = fopen("aleatorio.bin", "rb")) == NULL)
	{
		cout <<"\n\nFALHA NA ABERTURA DO ARQUIVO\n";
		arqb = NULL;
		return arqb;
	}
	return arqb;	
}

/* criação e pesquisa do método com os parametros solicitados na execução */
void PSIN(FILE* arqb, char* argv[], Analise* estatistica)
{
	int quant_indices = 0;

	int* tabela;
	tabela = new int[TAM_DADOS/ITENS_POR_PAGINA];

	/*iniciando time para paginacao*/
	start_time(estatistica);

	if(!paginacao_em_MP(arqb, quant_indices, tabela, atoi(argv[2]), estatistica))
	{
		/* arquivo e fechado no main */
		cout <<"FALHA NA CRIACAO DA TABELA DE INDICES\n"; 
		return;
	}

	/* gravando tempo de paginacao */
	stop_time(estatistica);
	/* mostrar na tela estatistica de criacaco de indices */
	cout <<"\n\n\tANALISE DE CRIACAO ( PSIN )\n";
	mostrar_est(estatistica);

	/* zera o mesmo para colher dados de pesquisa */
	zera_analise(estatistica);


	/* chave do resgistro a ser buscado foi passado por argumento externo */
	Registro* buscar = cria_registro();
	set_chave(buscar, atoi(argv[4]));

	/* iniciando  tempo de pesquisa */
	start_time(estatistica);

	if(pesquisa(arqb, buscar, quant_indices, tabela, atoi(argv[3]), estatistica))
	{
		if((!strcmp(argv[5], "-p")) || (!strcmp(argv[5], "-P")))
			imprime(buscar);
	}
	else
		cout <<"\nDESCULPE, O REGISTRO NAO FOI ENCONTRADO NA NOSSA BASE DE DADOS.\n\n";

	stop_time(estatistica); /*gravando tempo de pesquisa */
	cout <<"\n\n\tANALISE DE PESQUISA ( PSIN )\n";
	mostrar_est(estatistica); /* mostrar resultados */


	deleta_registro(buscar);
	delete tabela;
}

/* criação e pesquisa do método com os parametros solicitados na execução */
void ARV_EXT(FILE* arqb, char* argv[], Analise* estatistica)
{
	FILE* arv;

	if((arv = fopen("arvore.bin", "w+b")) == NULL)
	{
		cout <<"FALHA NA CRIACAO DO ARQUIVO\n";
		fclose(arqb); 
		return;
	}

	/*iniciando time para criacao da arvore externa */
	start_time(estatistica);
	
	/* criacao da arvore em arquivo */
	if(!cria_arvore_externa(arqb, arv, atoi(argv[2]), estatistica))
	{
		cout <<"\nFALHA NA CRIACAO DO ARQUIVO ARVORE\n";
		fclose(arqb);
		fclose(arv);
		return;
	}

	/* gravando tempo de criacao da arvore externa */
	stop_time(estatistica);
	/* mostrar na tela estatistica de criacaco de indices */
	cout <<"\n\n\tANALISE DE CRIACAO ( ARVORE EXTERNA )\n";
	mostrar_est(estatistica);

	/* zera o mesmo para colher dados de pesquisa */
	zera_analise(estatistica);

	/* chave do resgistro a ser buscado foi passado por argumento externo */
	Registro* retorno = cria_registro();
	set_chave(retorno, atoi(argv[4]));

	Nodo* filho = cria_nodo();

	t_hit(estatistica);
	rewind(arv);
	/* colher dados de pesquisa, iniciando time */
	start_time(estatistica);

	if(pesquisa_arv_externa(arv, filho, retorno, estatistica))
	{
		if((!strcmp(argv[5], "-p")) || (!strcmp(argv[5], "-P")))
			imprime(retorno);
	}
	else
		cout <<"\nDESCULPE, O REGISTRO NAO FOI ENCONTRADO NA NOSSA BASE DE DADOS.\n\n";

	stop_time(estatistica); /*gravando tempo de pesquisa */
	cout <<"\n\n\tANALISE DE PESQUISA ( ARVORE EXTERNA )\n";
	mostrar_est(estatistica); /* mostrar resultados */

	fclose(arv);
	deleta_nodo(filho);
	deleta_registro(retorno);
}

/* criação e pesquisa do método com os parametros solicitados na execução */
void ARV_B(FILE* arqb, char* argv[], Analise* estatistica)
{
	/* criacao de variaveis */
	Tipo_apontador arvore = NULL;

	/*iniciando time para criacao da arvore B */
	start_time(estatistica);

	/* cria arvore B em memoria principal */
	cria_arvoreb(arqb, &arvore, atoi(argv[2]), estatistica); 

	/* gravando tempo de criacao da arvore B */
	stop_time(estatistica);

	/* mostrar na tela estatistica de criacaco de indices */
	cout <<"\n\n\tANALISE DE CRIACAO ( ARVORE B )\n";

	/* imprime na tela a estatistica */
	mostrar_est(estatistica); 

	/* zera o mesmo para colher dados de pesquisa */
	zera_analise(estatistica);

	/* chave do resgistro a ser buscado foi passado por argumento externo */
	Registro* buscar_aux = cria_registro();
	set_chave(buscar_aux,  atoi(argv[4]));

	/* colher dados de pesquisa, iniciando time */
	start_time(estatistica);

	if(pesquisa_em_arvb(buscar_aux, arvore, estatistica))
	{
		if((!strcmp(argv[5], "-p")) || (!strcmp(argv[5], "-P")))
			imprime(buscar_aux);
	}
	else
		cout <<"\n\nDESCULPE, O REGISTRO NAO FOI ENCONTRADO NA NOSSA BASE DE DADOS.\n\n";

	stop_time(estatistica); /*gravando tempo de pesquisa */
	cout <<"\n\n\tANALISE DE PESQUISA ( ARVORE B )\n";
	mostrar_est(estatistica); /* mostrar resultados */

	deleta_registro(buscar_aux);
}

/* criação e pesquisa do método com os parametros solicitados na execução */
void ARV_B2(FILE* arqb, char* argv[], Analise* estatistica)
{
	/* criacao de variaveis */
	Tipo_apontador2 arvore = NULL;	

	/*iniciando time para criacao da arvore B* */
	start_time(estatistica);

	/* cria arvore B* em memoria principal */
	cria_arvoreb2(arqb, &arvore, atoi(argv[2]), estatistica);

	/* gravando tempo de criacao da arvore B* */
	stop_time(estatistica);

	/* mostrar na tela estatistica de criacaco de indices */
	cout <<"\n\n\tANALISE DE CRIACAO ( ARVORE B* )\n";

	/* imprime na tela a estatistica */
	mostrar_est(estatistica); 

	/* zera o mesmo para colher dados de pesquisa */
	zera_analise(estatistica);

	/* chave do resgistro a ser buscado foi passado por argumento externo */
	Registro* item_pesquisa = cria_registro();
	set_chave(item_pesquisa, atoi(argv[4]));

	/* colher dados de pesquisa, iniciando time */
	start_time(estatistica);

	if(pesquisa_b2(item_pesquisa, &arvore, estatistica))
	{
		if((!strcmp(argv[5], "-p")) || (!strcmp(argv[5], "-P")))
			imprime(item_pesquisa);
	}
	else
		cout <<"\n\nDESCULPE, O REGISTRO NAO FOI ENCONTRADO NA NOSSA BASE DE DADOS.\n\n";

	stop_time(estatistica); /*gravando tempo de pesquisa */
	cout <<"\n\n\tANALISE DE PESQUISA ( ARVORE B* )\n";
	mostrar_est(estatistica); /* mostrar resultados */

	deleta_registro(item_pesquisa);
}