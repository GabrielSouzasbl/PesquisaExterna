/*		GERADOR DE ARQUIVO BINARIO

CRIADOR: GABRIEL CARVALHO DE SOUZA 

GERA ARQUIVOS CRESCENTES DECRESCENTES E ALEATORIOS 
COM TRATAMENTO DE CHAVES IGUAIS 					*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cstring>	

#define TAM_DADOS 1000000 /*numero de itens no arquivo */
#define TAM_NOME 15 /* nome sugerido para item 2 */

using namespace std;

	/* estrutura e seus dados que seram utilizados */
	typedef struct registro
	{
		long int chave;  	 /* a chave, ÚNICA, que será utilizada como referencia para todos os métodos apresentados.*/
		long int idade; 	 /* dado 1, que representa a idade do usuário registrado */
		char nome[TAM_NOME]; /* dado 2 que representa o nome do usuário registrado */		
	}Registro;

/* gera e insere um nome ao registro */
void gera_string(Registro& A, FILE* nomes);

/* gera um arquivo com chaves ordenadas ascendentemente */
int gera_crescente(Registro A, long x, FILE* arqb, FILE* nomes);

/* gera um arquivo com chaves ordenadas descendentemente */
int gera_decrescente(Registro A, long x, FILE* arqb, FILE* nomes);

/* gera um arquivo com chaves desordenadas aleatóriamente */
int gera_aleatorio(FILE* crescente, FILE* aleatorio);

/* método auxiliar para  geração de arquivo aleatório */
void copy(Registro A, FILE* crescente, FILE* aleatorio);


int main()
{
	srand(time(NULL)); /* inicializando semente para gerar numeros aleatorios */

	/*criação de variaveis */
	Registro A;

	FILE* crescente;
	FILE* decrescente;
	FILE* aleatorio;
	FILE* nomes;

	/*-----------------ABERTURA E VALIDACAO------------------*/ /* arquivos */

	//abre arquivo com nomes 
	if((nomes = fopen("nomes.txt", "r+")) == NULL)
	{
		cout <<"\n\nABERTURA DO ARQUIVO\n";
		return 0;
	}
	
	//cria um arquivo crescente.bin
	if((crescente = fopen("crescente.bin", "w+b")) == NULL)
	{
		cout <<"\n\nFALHA NA CRIACAO/ABERTURA DO ARQUIVO\n";
		fclose(nomes);
		return 0;
	}

	//cria um arquivo decrescente.bin
	if((decrescente = fopen("decrescente.bin", "w+b")) == NULL)
	{
		cout <<"\n\nFALHA NA CRIACAO/ABERTURA DO ARQUIVO\n";
		fclose(nomes);
		fclose(crescente);
		return 0;
	}

	//cria um arquivo aleatorio.bin
	if((aleatorio = fopen("aleatorio.bin", "w+b")) == NULL)
	{
		cout <<"\n\nFALHA NA CRIACAO/ABERTURA DO ARQUIVO\n";
		fclose(nomes);
		fclose(crescente);
		fclose(decrescente);
		return 0;
	}



	/*-----------------GERACAO E VALIDACAO------------------*/ /* arquivos */
	if(gera_crescente(A, 1, crescente, nomes))
		cout <<"\n\nARQUIVO PREENCHIDO COM SUCESSO\n";
	else
	{
		cout <<"\n\nFALHA NO PREENCHIMENTO DO ARQUIVO\n";		
		fclose(crescente);
		fclose(decrescente);
		fclose(aleatorio);
	}

	if(gera_decrescente(A, TAM_DADOS, decrescente, nomes))
		cout <<"\n\nARQUIVO PREENCHIDO COM SUCESSO\n";
	else
	{
		cout <<"\n\nFALHA NO PREENCHIMENTO DO ARQUIVO\n";		
		fclose(crescente);
		fclose(decrescente);
		fclose(aleatorio);
	}

	if(gera_aleatorio(crescente, aleatorio))
		cout <<"\n\nARQUIVO PREENCHIDO COM SUCESSO\n";
	else
	{
		cout <<"\n\nFALHA NO PREENCHIMENTO DO ARQUIVO\n";		
		fclose(crescente);
		fclose(decrescente);
		fclose(aleatorio);
	}

	/*--------------------------------------------------------*/

	/* fecha arquivos */
	fclose(crescente);
	fclose(decrescente);
	fclose(aleatorio);

	return 0;
}

/* recebe um registro e o arquivo nomes faz a leitura e atribuição de um no registro recebido */
void gera_string(Registro& A, FILE* nomes)
{
	/*busca um nome no arquivo nomes e atribui ao seu respectivo da estrutura*/
	char aux[TAM_NOME];
	fscanf(nomes, "%s '\n'", aux);
	
	strcpy(A.nome, aux);

	return;
}

/* loop até TAM_DADOS atribuindo valores ao Registro enviado via parametro e gravando em arquivo */
int gera_crescente(Registro A, long x, FILE* arqb, FILE* nomes)
{
	while(x <= TAM_DADOS)
	{
		A.chave = x++;
		A.idade = (rand()%100)+1;
		gera_string(A, nomes);
		fwrite(&A, sizeof(Registro), 1, arqb);
	}

	rewind(arqb);

	return 1;
}

/* loop até TAM_DADOS atribuindo valores ao Registro enviado via parametro e gravando em arquivo */
int gera_decrescente(Registro A, long x, FILE* arqb, FILE* nomes)
{
	while(x > 0) 
	{
		A.chave = x--;
		A.idade = (rand()%100)+1;
		gera_string(A, nomes);
		fwrite(&A, sizeof(Registro), 1, arqb);
	}

	rewind(arqb);

	return 1;
}

/* pega o arquivo crescente faz a leitura de uma linha e grava em arquivo aleatorio */
void copy(Registro A, FILE* crescente, FILE* aleatorio)
{	
	while(fread(&A, sizeof(Registro), 1, crescente) == 1)
		fwrite(&A, sizeof(Registro), 1, aleatorio);
}

/* recebe o arquivo crescente e o arquivo aleatorio da saltos randomicamente no arquivo crescente e envia registro para o arquivo aleatorio */
int gera_aleatorio(FILE* crescente, FILE* aleatorio)
{
	Registro A;
	Registro B;

	/* usa uma funcao auxiliar que faz copia de todos os elementos do arquivo */
	rewind(crescente);
	copy(A, crescente, aleatorio);

	/* embaralhamento */
	rewind(crescente);
	long x = 0, desloc;

	while(x < TAM_DADOS/2)
	{
		/* tratamento de numeros iguais (mesmos saltos)*/
		do{
			desloc = rand()%TAM_DADOS;
			
		}while(x == desloc);

		/*busca dois registros um aleatorio e um ordenado*/

		fseek(aleatorio, desloc*sizeof(Registro), 0);
		fread(&A, sizeof(Registro), 1, aleatorio);

		fseek(aleatorio, x*sizeof(Registro), 0);
		fread(&B, sizeof(Registro), 1, aleatorio);		

		/*troca a posicao deles no arquivo */

		fseek(aleatorio, desloc*sizeof(Registro), 0);
		fwrite(&B, sizeof(Registro), 1, aleatorio);

		fseek(aleatorio, x*sizeof(Registro), 0);
		fwrite(&A, sizeof(Registro), 1, aleatorio);

		x++;					
	}

	rewind(aleatorio);	

	return 1;
}
