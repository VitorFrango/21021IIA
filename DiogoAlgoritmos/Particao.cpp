#include "Particao.h"
#include <stdio.h>

CParticao::CParticao(void)
{
}

CParticao::~CParticao(void)
{
}

// Cria um objecto que e uma copia deste
TProcuraConstrutiva* CParticao::Duplicar(void)
{
	CParticao *copia=new CParticao;
	copia->Copiar(this);
	return copia;
}

// Coloca o objecto no estado inicial da procura
void CParticao::SolucaoVazia(void)
{
	direita.Count(0);
	esquerda.Count(0);
	numeros.Count(0);
	totalDireita=totalEsquerda=0;

	// gerar uma instancia possivel
	int soma1,soma2;
	soma1=soma2=0;
	TRand::srand(20);
	for(int i=0;i<20;i++) {
		numeros.Add(TRand::rand()%10000);
		if(soma1<soma2)
			soma1+=numeros.Last();
		else soma2+=numeros.Last();
	}
	if(soma1<soma2) numeros.Add(soma2-soma1);
	if(soma2<soma1) numeros.Add(soma1-soma2);
	numeros.Remove(0);
	numeros.Sort();
}

// Coloca em sucessores a lista de objectos sucessores (sao alocados neste metodo e tem de ser apagados)
// O custo nao necessita de ser preenchido, caso seja sempre unitario
// chamar o metodo desta classe apos adicionar os sucessores para actualizar geracoes e expansoes
void CParticao::Sucessores(TVector<TProcuraConstrutiva*>&sucessores, TVector<int>&custo)
{
	int faltaDistribuir=0;
	for(int i=0;i<numeros.Count();i++)
		faltaDistribuir+=numeros[i];
	if(faltaDistribuir<abs(totalEsquerda-totalDireita)) { // ja nao ha hipotese
		TProcuraConstrutiva::Sucessores(sucessores,custo);
		return;
	}

	if(numeros.Count()>0) {
		CParticao *esquerda,*direita;
		sucessores.Add(esquerda=(CParticao *)Duplicar());
		sucessores.Add(direita=(CParticao *)Duplicar());
		esquerda->esquerda.Push(esquerda->numeros.Pop());
		esquerda->totalEsquerda+=esquerda->esquerda.Last();
		direita->direita.Push(direita->numeros.Pop());
		direita->totalDireita+=direita->direita.Last();
		custo.Add(0);
		custo.Add(0);
		TProcuraConstrutiva::Sucessores(sucessores,custo);
	}
}

// Escrever informacao de debug sobre o objecto currente
// (utilizar variavel TProcuraConstrutiva::debug para seleccionar o detalhe pretendido)
void CParticao::Debug(void)
{
	NovaLinha();
	printf("Nao colocados: ");
	for(int i=0;i<numeros.Count();i++)
		printf("%d ",numeros[i]);
	NovaLinha();
	printf("Esquerda (total %d): ", totalEsquerda);
	for(int i=0;i<esquerda.Count();i++)
		printf("%d ",esquerda[i]);
	NovaLinha();
	printf("Direita (total %d): ", totalDireita);
	for(int i=0;i<direita.Count();i++)
		printf("%d ",direita[i]);
}
