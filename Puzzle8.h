#pragma once
#include "tprocuraconstrutivacomciclos.h"

///////////////////////////////////////////////////////////////////////////////
//	CPuzzle8 class
///////////////////////////////////////////////////////////////////////////////
//	Author: Jos� Coelho
//	Last revision: 2007-09-06
//	Description: 
//    Implementa o puzzle 8. Este jogo consiste em mover numa quadricula de 3x3
//    oito pe�as, em que os movimentos permitidos � mover uma pe�a para a 
//    posi��o vazia. Pretende-se de uma posi��o inicial atingir a posi��o final. 
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

	// puzzle-8, posi��o de cada n�mero
	TVector<int> puzzle; 
	// posi��o do zero (espa�o vazio)
	int zero; 

	// Cria um objecto que � uma c�pia deste
	TProcuraConstrutiva* Duplicar(void);
	void Copiar(TProcuraConstrutiva*objecto) {  
		puzzle=((CPuzzle8*)objecto)->puzzle; 
		zero=((CPuzzle8*)objecto)->zero; 
	}
	// Coloca o objecto no estado inicial da procura
	void SolucaoVazia(void);
	// Coloca em sucessores a lista de objectos sucessores (s�o alocados neste m�todo e t�m de ser apagados)
	// O custo n�o necessita de ser preenchido, caso seja sempre unit�rio
	// actualizar geracoes e expansoes neste m�todo
	void Sucessores(TVector<TProcuraConstrutiva*>&sucessores, TVector<int>&custo);
	// Retorna verdade caso o estado actual seja um estado objectivo
	bool SolucaoCompleta(void);	
	// Escrever informa��o de debug sobre o objecto currente 
	// (utilizar vari�vel TProcuraConstrutiva::debug para seleccionar o detalhe pretendido)
	void Debug(void);

	// Redefinir para poder utilizar os algoritmos informados
	// O custo desde o n� inicial � tido em conta, esta fun��o deve devolver o custo estimado
	// nunca sobre estimando, deste estado at� ao n� final mais pr�ximo (� um m�nimo)
	// chamar para actualia��o de avaliacoes
	int Heuristica(void);


	bool operator!=(CPuzzle8&estado);
	bool Distinto(TProcuraConstrutivaComCiclos*estado);

};
