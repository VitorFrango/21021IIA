#include "OitoDamas.h"
#include <stdio.h>

//#define WINDOWS_VS 1

COitoDamas::COitoDamas(void)
{
}

COitoDamas::~COitoDamas(void)
{
}

// Cria um objecto que e uma copia deste
TProcuraConstrutiva* COitoDamas::Duplicar(void)
{
	COitoDamas *clone=new COitoDamas;
	clone->damas=damas;
	return clone;
}

// Coloca em sucessores a lista de objectos sucessores (sao alocados neste metodo e tem de ser apagados)
// O custo nao necessita de ser preenchido, caso seja sempre unitario
void COitoDamas::Sucessores(TVector<TProcuraConstrutiva*>&sucessores, TVector<int>&custo)
{
	int novaLinha=damas.Count();
	// tentar colocar damas em todas as colunas
	for(int i=0;i<8;i++) {
		int j=0;
		// verificar se ha uma dama a atacar esta posicao
		for(;j<novaLinha;j++)
			if(i==damas[j] || i==damas[j]+(novaLinha-j) || i==damas[j]-(novaLinha-j))
				break;
		if(j==novaLinha) { // e possivel, adicionar sucessor
			COitoDamas *sucessor=(COitoDamas*)Duplicar();
			sucessor->damas.Add(i);
			sucessores.Add(sucessor);
		}
	}
	TProcuraConstrutiva::Sucessores(sucessores,custo);
}

// Escrever informacao de debug sobre o objecto currente
// (utilizar variavel TProcuraConstrutiva::debug para seleccionar o detalhe pretendido)
void COitoDamas::Debug(void)
{
	for(int i=0;i<8;i++) {
		NovaLinha();
		for(int j=0;j<8;j++) {
#ifdef WINDOWS_VS
			int cor = ((i + j) % 2 ? 176 : 178);  
			if (damas.Count() > i && damas[i] == j)
				printf("%c%c", 16, 17);
			else printf("%c%c", cor, cor);
#else
			int cor=((i+j)%2?' ' : '#');
			if(damas.Count()>i && damas[i]==j)
				printf("%c%c",'X', 'X');
			else printf("%c%c",cor,cor);
#endif
		}
	}
	NovaLinha();
	if(damas.Count()==8 && custo>=0) {
		printf("Custo: %d",custo);
		NovaLinha();
	}
}

// e gerada uma nova solucao aleatoriamente.
void COitoDamas::NovaSolucao(void)
{
	damas.Count(8);
	for(int i=0;i<8;i++)
		damas[i]=TRand::rand()%8;
	custo=-1;
}

// Retorna o valor da solucao completa actual.
int COitoDamas::Avaliar(void)
{
	TProcuraMelhorativa::Avaliar();
	custo=0;
	// calcular o numero de pares de damas atacadas
	for(int i=0;i<7;i++)
		for(int j=i+1;j<8;j++)
			if(damas[i]==damas[j] || damas[i]-damas[j]==i-j || damas[i]-damas[j]==j-i)
				custo++;
	return custo;
}

// Operador de vizinhanca (apenas em solucaes completas)
// chamar este operador para actualizacao de estatisticas
void COitoDamas::Vizinhanca(TVector<TProcuraMelhorativa*>&vizinhos)
{
	// trocar a posicao de cada dama
	for(int i=0;i<8;i++)
		for(int j=0;j<8;j++)
			if(j!=damas[i]) {
				COitoDamas *vizinho=(COitoDamas*)Duplicar();
				vizinho->damas[i]=j;
				vizinhos.Add(vizinho);
			}
	TProcuraMelhorativa::Vizinhanca(vizinhos);
}

// muta o estado actual (de acordo com a parametrizacao global)
void COitoDamas::Mutar(void)
{
	// trocar a posicao de uma das damas (50%)
	damas[TRand::rand()%8]=TRand::rand()%8;
}

// cruza os dois elementos a e b, colocando o resultado neste
void COitoDamas::Cruzamento(TProcuraMelhorativa *a, TProcuraMelhorativa *b)
{
	int divisao=1+TRand::rand()%6; // divisao tem de ter elementos de um e de outro
	damas.Count(8);
	for(int i=0;i<divisao;i++)
		damas[i]=((COitoDamas*)a)->damas[i];
	for(int i=divisao;i<8;i++)
		damas[i]=((COitoDamas*)b)->damas[i];
	TProcuraMelhorativa::Cruzamento(a,b);
}

// retorna a distancia deste elemento ao elemento a
// (necessario para manter os elementos diversos)
int COitoDamas::Distancia(TProcuraMelhorativa*a)
{
	int distancia=0;
	for(int i=0;i<8;i++)
		if(damas[i]!=((COitoDamas*)a)->damas[i])
			distancia++;
	return distancia;
}
