#include "Puzzle8.h"
#include <stdio.h>

CPuzzle8::CPuzzle8(void)
{
}

CPuzzle8::~CPuzzle8(void)
{
}

// Cria um objecto que � uma c�pia deste
TProcuraConstrutiva* CPuzzle8::Duplicar(void)
{
	CPuzzle8 *clone=new CPuzzle8;
	clone->Copiar(this);
	return clone;
}

// Coloca o objecto no estado inicial da procura
void CPuzzle8::SolucaoVazia(void)
{
	// colocar a posi��o final
	puzzle.Count(9);
	for(int i=0;i<9;i++)
		puzzle[i]=i;
	zero=0;
	// efectuar trocas ao acaso
	for(int i=0;i<30;i++) {
		TVector<TProcuraConstrutiva*> sucessores;
		TVector<int> custo;
		Sucessores(sucessores,custo);
		CPuzzle8 *sucessor=(CPuzzle8 *)sucessores.Random();
		puzzle=sucessor->puzzle;
		zero=sucessor->zero;
		LibertarVector(sucessores);
	}

}

// Coloca em sucessores a lista de objectos sucessores (s�o alocados neste m�todo e t�m de ser apagados)
// O custo n�o necessita de ser preenchido, caso seja sempre unit�rio
// actualizar geracoes e expansoes neste m�todo
void CPuzzle8::Sucessores(TVector<TProcuraConstrutiva*>&sucessores, TVector<int>&custo)
{
	if(zero>=3) { // subir
		CPuzzle8 *sucessor=(CPuzzle8*)Duplicar();
		sucessor->Trocar(zero,zero-3);
		sucessores.Add(sucessor);
	}
	if(zero<6) { // baixar
		CPuzzle8 *sucessor=(CPuzzle8*)Duplicar();
		sucessor->Trocar(zero,zero+3);
		sucessores.Add(sucessor);
	}
	if(zero%3>0) { // esquerda
		CPuzzle8 *sucessor=(CPuzzle8*)Duplicar();
		sucessor->Trocar(zero,zero-1);
		sucessores.Add(sucessor);
	}
	if(zero%3<2) { // direita
		CPuzzle8 *sucessor=(CPuzzle8*)Duplicar();
		sucessor->Trocar(zero,zero+1);
		sucessores.Add(sucessor);
	}
	TProcuraConstrutivaComCiclos::Sucessores(sucessores,custo);
}

// Retorna verdade caso o estado actual seja um estado objectivo
bool CPuzzle8::SolucaoCompleta(void)
{
	for(int i=0;i<9;i++)
		if(puzzle[i]!=i)
			return false;
	return true;
}

// Redefinir para poder utilizar os algoritmos informados
// O custo desde o n� inicial � tido em conta, esta fun��o deve devolver o custo estimado
// nunca sobre estimando, deste estado at� ao n� final mais pr�ximo (� um m�nimo)
// chamar para actualia��o de avaliacoes
int CPuzzle8::Heuristica(void)
{
	int resultado=0;
	TProcuraConstrutiva::Heuristica();

	// dist�ncia de manhatan de cada posi��o � sua possi��o final
	for(int i=1;i<9;i++) {
		int posicaoI=0;
		while(puzzle[posicaoI]!=i)
			posicaoI++;
		resultado+=abs(posicaoI%3-i%3)+abs(posicaoI/3-i/3);
	}

	return resultado;
}


// Escrever informa��o de debug sobre o objecto currente 
// (utilizar vari�vel TProcuraConstrutiva::debug para seleccionar o detalhe pretendido)
void CPuzzle8::Debug(void)
{
	for(int i=0;i<3;i++) {
		NovaLinha();
		for(int j=0;j<3;j++)
			if(puzzle[i*3+j]==0)
				printf(" . ");
			else printf(" %d ",puzzle[i*3+j]);
	}
	NovaLinha();
}

bool CPuzzle8::operator!=(CPuzzle8&estado)
{
	if(zero!=estado.zero) 
		return true;
	for(int i=0;i<9;i++)
		if(puzzle[i]!=estado.puzzle[i])
			return true;
	return false;
}

bool CPuzzle8::Distinto(TProcuraConstrutivaComCiclos*estado)
{
	// se quiser saber o que seria a procura se n�o se impedir ciclos,
	// simplesmente retorne sempre true e comente o restante c�digo 

	// para n�o produzir ciclos:
	CPuzzle8 *aux=(CPuzzle8*)estado;
	return (*this)!=(*aux);
}

