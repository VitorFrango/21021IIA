#pragma once
#include "TProcuraConstrutiva.h"


///////////////////////////////////////////////////////////////////////////////
//	TProcuraMelhorativa class
///////////////////////////////////////////////////////////////////////////////
//	Author: Jose Coelho
//	Last revision: 2007-09-03
//	Description:
//    Superclasse de procuras no espaco das solucoes completas (a solucao e melhorada).
//    Esta classe nao tem variaveis, deve criar as variaveis necessarias
//    e implementacao dos metodos virtuais para o problema em concreto.
//    As variaveis da classe devem ser apenas de uma solucao, se existir informacao
//    sobre a instancia, esta informacao deve ser colocada em variaveis estiticas
//    para nao serem duplicadas desnecessariamente.
//	  A unica variavel e o valor da solucao que apos ser calculada em Avaliar
//	  deve ser actualizada (se o valor nao estiver actualizado, deve ter -1)
///////////////////////////////////////////////////////////////////////////////
class TProcuraMelhorativa :
	public TProcuraConstrutiva
{
protected:
	// metodo interno para libertar objectos nao necessarios
	void LibertarVector(TVector<TProcuraMelhorativa*>&vector, int excepto=-1);

public:
	int custo;
public:
	TProcuraMelhorativa(void);
	~TProcuraMelhorativa(void);

	// numero de maximo de avaliacoes (omissao 10000)
	static int maximoAvaliacoes;


	///////////////////////////////////////////////////////////////////////////////
	// Metodos para redefinir
	///////////////////////////////////////////////////////////////////////////////

	void Copiar(TProcuraConstrutiva*objecto) {
		custo=((TProcuraMelhorativa*)objecto)->custo;
	}
	// e gerada uma nova solucao aleatoriamente.
	// Este metodo e definido iniciamente como uma procura em profundidade em que o sucessor
	// e escolhido aleatoriamente e sem retrocesso (caso falhe o processo re-inicia)
	virtual void NovaSolucao(void);
	// Retorna o valor da solucao completa actual.
	virtual int Avaliar(void);
	// Metodo que testa se e altura de parar ou nao (assume-se que o custo 0 e o menor possivel)
	virtual bool Parar(void) { return maximoAvaliacoes<=avaliacoes || custo==0; }

	///////////////////////////////////////////////////////////////////////////////
	// Redefinir para poder executar Escalada-do-Monte
	///////////////////////////////////////////////////////////////////////////////

	// Operador de vizinhanca (apenas em solucoes completas)
	// chamar este operador para actualizacao de estatisticas
	virtual void Vizinhanca(TVector<TProcuraMelhorativa*>&vizinhos);

	///////////////////////////////////////////////////////////////////////////////
	// Redefinir para poder executar Algoritmos Geneticos
	///////////////////////////////////////////////////////////////////////////////

	// muta o estado actual (de acordo com a parametrizacao global)
	virtual void Mutar(void) { }
	// cruza os dois elementos a e b, colocando o resultado neste
	virtual void Cruzamento(TProcuraMelhorativa*a,TProcuraMelhorativa*b) { geracoes++; }
	// retorna a distancia deste elemento ao elemento a
	// (necessario para manter os elementos diversos)
	virtual int Distancia(TProcuraMelhorativa*a) { return 0; }

	///////////////////////////////////////////////////////////////////////////////
	// Algoritmo de procura local: Escalada do Monte
	///////////////////////////////////////////////////////////////////////////////

	// retorna a avaliacao do resultado actual
	int EscaladaDoMonte(bool movePrimeiro=true);

	///////////////////////////////////////////////////////////////////////////////
	// Algoritmo de procura local: Algoritmos Geneticos
	///////////////////////////////////////////////////////////////////////////////

	// retorna a avaliacao do resultado final
	// distanciaMinima e o valor minimo que dois elementos podem estar distanciados numa geracao (Distancia)
	int AlgoritmoGenetico(int populacao=20, int probablidadeMutacao=50, int distanciaMinima=0);


	// Metodo para teste manual do objecto (chamadas aos algoritmos, construcao de uma solucao manual)
	// Este metodo destina-se a testes preliminares, e deve ser redefinido apenas se forem definidos novos algoritmos
	void TesteManual(const char *nome, int seed=1);

	// Chamar sempre que uma solucao melhor que a actual e encontrada
	void DebugMelhorEncontrado(void);
};
