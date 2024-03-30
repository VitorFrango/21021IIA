#include "TProcuraConstrutivaComCiclos.h"

TProcuraConstrutivaComCiclos::TProcuraConstrutivaComCiclos(void) : pai(NULL)
{
}

TProcuraConstrutivaComCiclos::~TProcuraConstrutivaComCiclos(void)
{
}


// chamar o metodo desta classe apos adicionar os sucessores para actualizar geracoes e expansoes
// e actualizar pai de todos os sucessores
void TProcuraConstrutivaComCiclos::Sucessores(TVector<TProcuraConstrutiva*>&sucessores, TVector<int>&custo)
{
	// remover todos os estados que ja tenham sido expandidos neste ramo
	for(int i=0;i<sucessores.Count();i++) {
		TProcuraConstrutivaComCiclos *avo=this;
		TProcuraConstrutivaComCiclos *sucessor=(TProcuraConstrutivaComCiclos *)sucessores[i];
		while(avo!=NULL && avo->Distinto(sucessor))
			avo=avo->pai;
		if(avo!=NULL) {
			delete sucessor;
			sucessores[i]=NULL;
		}
	}
	sucessores.Remove(NULL);
	// actualizar os pais
	for(int i=0;i<sucessores.Count();i++)
		((TProcuraConstrutivaComCiclos*)sucessores[i])->pai=this;
	// actualizar estatisticas
	TProcuraConstrutiva::Sucessores(sucessores,custo);
}
