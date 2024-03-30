#pragma once
#include "TProcuraConstrutiva.h"

///////////////////////////////////////////////////////////////////////////////
//	TProcuraAdversa class
///////////////////////////////////////////////////////////////////////////////
//	Author: Jose Coelho
//	Last revision: 2007-09-04
//	Description:
//    Superclasse de procuras no espaco das solucaes parciais (a solucao e construida).
//    A procura adversa so pode ser feita no espaco das solucaes parciais, e a subclasse
//    da TProcuraConstrutiva. Os custos devem ser ignorados, apenas o valor da funcao
//	  Heuristica e utilizado. O valor infinito e definido numa variavel global a
//    classe e deve ser alterado conforme o problema, significando vitoria/derrota.
//    Se nao houver sucessores, o estado e terminal e o retorno da funcao heuristica
//    deve ser exacto: -infinito ganham pretas, 0 empate, +infinito ganham as brancas
///////////////////////////////////////////////////////////////////////////////
class TProcuraAdversa :
	public TProcuraConstrutiva
{
public:
	// o jogador actual deve minimizar o custo (ou maximizar caso tenha o valor falso)
	bool minimizar;
	// valor de infinito (vitoria/derrota), omissao 1000
	static int infinito;
public:
	TProcuraAdversa(void);
	~TProcuraAdversa(void);

	///////////////////////////////////////////////////////////////////////////////
	// Algoritmo MiniMax
	///////////////////////////////////////////////////////////////////////////////

	// retorna o valor do estado actual, apos procura de profundidade nivel
	int MiniMax(int nivel=4);

	// retorna o valor do estado actual, apos procura de profundidade nivel
	int MiniMaxAlfaBeta(int nivel=4,int alfa=-infinito,int beta=+infinito);

	// Metodo para teste manual do objecto (chamadas aos algoritmos, construcao de uma solucao manual)
	// Este metodo destina-se a testes preliminares, e deve ser redefinido apenas se forem definidos novos algoritmos
	void TesteManual(const char *nome, int seed=1);
};
