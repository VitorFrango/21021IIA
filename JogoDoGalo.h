#pragma once
#include "TProcuraAdversa.h"

///////////////////////////////////////////////////////////////////////////////
//	CJogoDoGalo class
///////////////////////////////////////////////////////////////////////////////
//	Author: Jose Coelho
//	Last revision: 2007-09-06
//	Description:
//    Implementa o Jogo do Galo. Este jogo e jogado num tabuleiro 3x3 por dois jogadores
//    em que cada jogador joga a vez numa posicao vazia, colocando a sua marca.
//    Ganha o jogador que conseguir fazer 3 em linha com a sua marca.
///////////////////////////////////////////////////////////////////////////////
class CJogoDoGalo :
	public TProcuraAdversa
{
public:
	TVector<char> tabuleiro;
public:
	CJogoDoGalo(void);
	~CJogoDoGalo(void);

	// Cria um objecto que e uma copia deste
	TProcuraConstrutiva* Duplicar(void);
	// Fica com uma copia de objecto
	void Copiar(TProcuraConstrutiva*objecto);
	// Coloca o objecto no estado inicial da procura
	void SolucaoVazia(void);
	// Coloca em sucessores a lista de objectos sucessores (sao alocados neste metodo e tem de ser apagados)
	// O custo nao necessita de ser preenchido, caso seja sempre unitario
	// chamar o metodo desta classe apos adicionar os sucessores para actualizar geracoes e expansoes
	void Sucessores(TVector<TProcuraConstrutiva*>&sucessores, TVector<int>&custo);
	// Retorna verdade caso o estado actual seja um estado objectivo
	bool SolucaoCompleta(void);
	// Escrever informacao de debug sobre o objecto currente
	// (utilizar variavel TProcuraConstrutiva::debug para seleccionar o detalhe pretendido)
	void Debug(void);

	// Redefinir para poder utilizar os algoritmos informados
	// O custo desde o no inicial e tido em conta, esta funcao deve devolver o custo estimado
	// nunca sobre estimando, deste estado ate ao no final mais proximo (e um minimo)
	// chamar para actualiacao de avaliacoes
	int Heuristica(void);
};
