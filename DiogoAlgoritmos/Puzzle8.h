#pragma once
#include "TProcuraConstrutivaComCiclos.h"

///////////////////////////////////////////////////////////////////////////////
//	CPuzzle8 class
///////////////////////////////////////////////////////////////////////////////
//	Author: Jose Coelho
//	Last revision: 2007-09-06
//	Description:
//    Implementa o puzzle 8. Este jogo consiste em mover numa quadricula de 3x3
//    oito pecas, em que os movimentos permitidos e mover uma peca para a
//    posicao vazia. Pretende-se de uma posicao inicial atingir a posicao final.
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

	// puzzle-8, posicao de cada numero
	TVector<int> puzzle;
	// posicao do zero (espaco vazio)
	int zero;

	// Cria um objecto que e uma copia deste
	TProcuraConstrutiva* Duplicar(void);
	void Copiar(TProcuraConstrutiva*objecto) {
		puzzle=((CPuzzle8*)objecto)->puzzle;
		zero=((CPuzzle8*)objecto)->zero;
	}
	// Coloca o objecto no estado inicial da procura
	void SolucaoVazia(void);
	// Coloca em sucessores a lista de objectos sucessores (sao alocados neste metodo e tem de ser apagados)
	// O custo nao necessita de ser preenchido, caso seja sempre unitario
	// actualizar geracoes e expansoes neste metodo
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


	bool operator!=(CPuzzle8&estado);
	bool Distinto(TProcuraConstrutivaComCiclos*estado);

};
