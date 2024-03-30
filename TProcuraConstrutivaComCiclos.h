#pragma once
#include "TProcuraConstrutiva.h"

///////////////////////////////////////////////////////////////////////////////
//	TProcuraConstrutivaComCiclos class
///////////////////////////////////////////////////////////////////////////////
//	Author: Jose Coelho
//	Last revision: 2007-08-31
//	Description:
//    Superclasse de procuras no espaco das solucaes parciais.
//    Esta classe tem uma unica variavel, o estado de onde foi gerado,
//    deve criar as variaveis necessarias a implementacao dos metodos virtuais
//    para o problema em concreto.
//    Esta classe assume que ha estados repetidos, utilizar TProcuraConstrutiva
//    caso tal nao seja verdade.
///////////////////////////////////////////////////////////////////////////////
class TProcuraConstrutivaComCiclos :
	public TProcuraConstrutiva
{
public:
	TProcuraConstrutivaComCiclos(void);
	~TProcuraConstrutivaComCiclos(void);

	// estado que o gerou, ou NULL caso seja o estado inicial
	TProcuraConstrutivaComCiclos* pai;

	// Coloca em sucessores a lista de objectos sucessores (sao alocados neste metodo e tem de ser apagados)
	// O custo nao necessita de ser preenchido, caso seja sempre unitario
	// chamar o metodo desta classe apos adicionar os sucessores para actualizar geracoes e expansoes
	// e actualizar pai de todos os sucessores
	// os sucessores que ja foram gerados desde o no inicial sao apagados
	virtual void Sucessores(TVector<TProcuraConstrutiva*>&sucessores, TVector<int>&custo);

	// verificar se e igual ao estado ou diferente
	bool operator!=(TProcuraConstrutivaComCiclos&estado) { return true; }
	virtual bool Distinto(TProcuraConstrutivaComCiclos*estado) { return true; }
};
