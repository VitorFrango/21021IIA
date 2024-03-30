#pragma once
#include "TProcuraMelhorativa.h"

///////////////////////////////////////////////////////////////////////////////
//	COitoDamas class
///////////////////////////////////////////////////////////////////////////////
//	Author: Jose Coelho
//	Last revision: 2007-09-06
//	Description:
//    Implementa o problema das oito damas. Este problema consiste em colocar
//    oito damas de xadrez (movem-se na horizontal, vertical e diagonal), num
//    tabuleiro de xadrez, sem que estas se ataquem umas as outras.
///////////////////////////////////////////////////////////////////////////////
class COitoDamas :
	public TProcuraMelhorativa
{
public:
	COitoDamas(void);
	~COitoDamas(void);

	// posicao de cada dama
	TVector<int> damas;

	// metodos redefinidos de TProcuraConstrutiva

	// Cria um objecto que e uma copia deste
	TProcuraConstrutiva* Duplicar(void);
	void Copiar(TProcuraConstrutiva*objecto) {
		TProcuraMelhorativa::Copiar(objecto);
		damas=((COitoDamas*)objecto)->damas;
	}
	// Coloca o objecto no estado inicial da procura
	void SolucaoVazia(void) { damas.Count(0); }
	// Coloca em sucessores a lista de objectos sucessores (sao alocados neste metodo e tem de ser apagados)
	// O custo nao necessita de ser preenchido, caso seja sempre unitario
	void Sucessores(TVector<TProcuraConstrutiva*>&sucessores, TVector<int>&custo);
	// Retorna verdade caso o estado actual seja um estado objectivo
	bool SolucaoCompleta(void) { return damas.Count()==8; }
	// Escrever informacao de debug sobre o objecto currente
	// (utilizar variavel TProcuraConstrutiva::debug para seleccionar o detalhe pretendido)
	void Debug(void);

	// metodos redefinidos de TProcuraMelhorativa

	// e gerada uma nova solucao aleatoriamente.
	void NovaSolucao(void);
	// Retorna o valor da solucao completa actual.
	int Avaliar(void);
	// Operador de vizinhanca (apenas em solucaes completas)
	// chamar este operador para actualizacao de estatisticas
	void Vizinhanca(TVector<TProcuraMelhorativa*>&vizinhos);
	// muta o estado actual (de acordo com a parametrizacao global)
	void Mutar(void);
	// cruza os dois elementos a e b, colocando o resultado neste
	void Cruzamento(TProcuraMelhorativa*a,TProcuraMelhorativa*b);
	// retorna a distancia deste elemento ao elemento a
	// (necessario para manter os elementos diversos)
	int Distancia(TProcuraMelhorativa*a);

};
