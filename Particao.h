#pragma once

#include "TProcuraConstrutiva.h"

///////////////////////////////////////////////////////////////////////////////
//	CParticao class
///////////////////////////////////////////////////////////////////////////////
//	Author: Jose Coelho
//	Last revision: 2007-09-06
//	Description:
//    Implementa o problema da particao. Este problema consiste em dividir
//    um conjunto de numeros naturais em duas partes cuja soma dos numeros em cada
//    parte seja igual.
///////////////////////////////////////////////////////////////////////////////
class CParticao :
	public TProcuraConstrutiva
{
public:
	CParticao(void);
	~CParticao(void);

	TVector<int> numeros, esquerda, direita;
	int totalEsquerda, totalDireita;

	// Metodos redefinidos de TProcuraConstrutiva

	// Cria um objecto que e uma copia deste
	TProcuraConstrutiva* Duplicar(void);
	void Copiar(TProcuraConstrutiva*objecto) {
		numeros=((CParticao*)objecto)->numeros;
		esquerda=((CParticao*)objecto)->esquerda;
		direita=((CParticao*)objecto)->direita;
		totalEsquerda=((CParticao*)objecto)->totalEsquerda;
		totalDireita=((CParticao*)objecto)->totalDireita;
	}
	// Coloca o objecto no estado inicial da procura
	void SolucaoVazia(void);
	// Coloca em sucessores a lista de objectos sucessores (sao alocados neste metodo e tem de ser apagados)
	// O custo nao necessita de ser preenchido, caso seja sempre unitario
	// chamar o metodo desta classe apos adicionar os sucessores para actualizar geracoes e expansoes
	void Sucessores(TVector<TProcuraConstrutiva*>&sucessores, TVector<int>&custo);
	// Retorna verdade caso o estado actual seja um estado objectivo
	bool SolucaoCompleta(void) { return numeros.Count()==0 && totalEsquerda==totalDireita; }
	// Escrever informacao de debug sobre o objecto currente
	// (utilizar variavel TProcuraConstrutiva::debug para seleccionar o detalhe pretendido)
	void Debug(void);

};
