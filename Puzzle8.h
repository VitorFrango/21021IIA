#pragma once
#include "tprocuraconstrutivacomciclos.h"

///////////////////////////////////////////////////////////////////////////////
//	CPuzzle8 class
///////////////////////////////////////////////////////////////////////////////
//	Author: José Coelho
//	Last revision: 2007-09-06
//	Description: 
//    Implementa o puzzle 8. Este jogo consiste em mover numa quadricula de 3x3
//    oito peças, em que os movimentos permitidos é mover uma peça para a 
//    posição vazia. Pretende-se de uma posição inicial atingir a posição final. 
///////////////////////////////////////////////////////////////////////////////
class CPuzzle8 :
	public TProcuraConstrutivaComCiclos
{
private:
	void Trocar(int a, int b) {
		if(puzzle[a]==0) {
			puzzle[a]=puzzle[b];
			puzzle[b]=0;
			zero=b;
		} else {
			puzzle[a]=puzzle[b];
			puzzle[b]=0;
			zero=b;
		}
	}
public:
	CPuzzle8(void);
	~CPuzzle8(void);

	// puzzle-8, posição de cada número
	TVector<int> puzzle; 
	// posição do zero (espaço vazio)
	int zero; 

	// Cria um objecto que é uma cópia deste
	TProcuraConstrutiva* Duplicar(void);
	void Copiar(TProcuraConstrutiva*objecto) {  
		puzzle=((CPuzzle8*)objecto)->puzzle; 
		zero=((CPuzzle8*)objecto)->zero; 
	}
	// Coloca o objecto no estado inicial da procura
	void SolucaoVazia(void);
	// Coloca em sucessores a lista de objectos sucessores (são alocados neste método e têm de ser apagados)
	// O custo não necessita de ser preenchido, caso seja sempre unitário
	// actualizar geracoes e expansoes neste método
	void Sucessores(TVector<TProcuraConstrutiva*>&sucessores, TVector<int>&custo);
	// Retorna verdade caso o estado actual seja um estado objectivo
	bool SolucaoCompleta(void);	
	// Escrever informação de debug sobre o objecto currente 
	// (utilizar variável TProcuraConstrutiva::debug para seleccionar o detalhe pretendido)
	void Debug(void);

	// Redefinir para poder utilizar os algoritmos informados
	// O custo desde o nó inicial é tido em conta, esta função deve devolver o custo estimado
	// nunca sobre estimando, deste estado até ao nó final mais próximo (é um mínimo)
	// chamar para actualiação de avaliacoes
	int Heuristica(void);


	bool operator!=(CPuzzle8&estado);
	bool Distinto(TProcuraConstrutivaComCiclos*estado);

};
