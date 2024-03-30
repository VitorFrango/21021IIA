#include "JogoDoGalo.h"
#include <stdio.h>

CJogoDoGalo::CJogoDoGalo(void)
{
}

CJogoDoGalo::~CJogoDoGalo(void)
{
}

// Cria um objecto que e uma copia deste
TProcuraConstrutiva* CJogoDoGalo::Duplicar(void)
{
	CJogoDoGalo *novo=new CJogoDoGalo;
	novo->Copiar(this);
	return novo;
}

// Fica com uma copia de objecto
void CJogoDoGalo::Copiar(TProcuraConstrutiva*objecto)
{
	tabuleiro=((CJogoDoGalo*)objecto)->tabuleiro;
	minimizar=((CJogoDoGalo*)objecto)->minimizar;
}

// Coloca o objecto no estado inicial da procura
void CJogoDoGalo::SolucaoVazia(void)
{
	minimizar=true;
	for(int i=0;i<9;i++)
		tabuleiro[i]='.';
}

// Coloca em sucessores a lista de objectos sucessores (sao alocados neste metodo e tem de ser apagados)
// O custo nao necessita de ser preenchido, caso seja sempre unitario
// chamar o metodo desta classe apos adicionar os sucessores para actualizar geracoes e expansoes
void CJogoDoGalo::Sucessores(TVector<TProcuraConstrutiva*>&sucessores, TVector<int>&custo)
{
	if(!SolucaoCompleta()) {
		for(int i=0;i<9;i++)
			if(tabuleiro[i]=='.') {
				sucessores.Add(Duplicar());
				((CJogoDoGalo*)sucessores.Last())->minimizar=!minimizar;
				((CJogoDoGalo*)sucessores.Last())->tabuleiro[i]=(minimizar?'x':'o');
			}
		TProcuraAdversa::Sucessores(sucessores,custo);
	}
}

// Retorna verdade caso o estado actual seja um estado objectivo
bool CJogoDoGalo::SolucaoCompleta(void)
{
	// verificar se ha tras em linha
	for(int i=0;i<3;i++) // verificar todas as linhas
		if(tabuleiro[i*3]!='.' && tabuleiro[i*3]==tabuleiro[i*3+1] && tabuleiro[i*3+2]==tabuleiro[i*3+1])
			return true;
	for(int i=0;i<3;i++) // verificar todas as colunas
		if(tabuleiro[i]!='.' && tabuleiro[i]==tabuleiro[i+3] && tabuleiro[i+6]==tabuleiro[i+3])
			return true;
	// verificar diagonais
	if(tabuleiro[0]!='.' && tabuleiro[0]==tabuleiro[4] && tabuleiro[4]==tabuleiro[8])
		return true;
	if(tabuleiro[3]!='.' && tabuleiro[3]==tabuleiro[4] && tabuleiro[4]==tabuleiro[6])
		return true;
	// verificar se ha espaco para mais jogadas
	for(int i=0;i<9;i++)
		if(tabuleiro[i]=='.')
			return false; // podem ser feitas mais jogadas
	return true; // nao ha hipotese de mais jogadas
}

// Escrever informacao de debug sobre o objecto currente
// (utilizar variavel TProcuraConstrutiva::debug para seleccionar o detalhe pretendido)
void CJogoDoGalo::Debug(void)
{
	for(int i=0;i<3;i++) {
		NovaLinha();
		for(int j=0;j<3;j++)
			printf(" %c",tabuleiro[i*3+j]);
	}
}

// Redefinir para poder utilizar os algoritmos informados
// O custo desde o no inicial e tido em conta, esta funcao deve devolver o custo estimado
// nunca sobre estimando, deste estado ate ao no final mais proximo (e um minimo)
// chamar para actualiacao de avaliacoes
int CJogoDoGalo::Heuristica(void)
{
	TProcuraAdversa::Heuristica();
	// jogo muito pequeno, fazer so o obrigatorio
	// ver quem ganhou
	for(int i=0;i<3;i++) // verificar todas as linhas
		if(tabuleiro[i*3]!='.' && tabuleiro[i*3]==tabuleiro[i*3+1] && tabuleiro[i*3+2]==tabuleiro[i*3+1])
			return tabuleiro[i*3]=='x'?-infinito:infinito;
	for(int i=0;i<3;i++) // verificar todas as colunas
		if(tabuleiro[i]!='.' && tabuleiro[i]==tabuleiro[i+3] && tabuleiro[i+6]==tabuleiro[i+3])
			return tabuleiro[i]=='x'?-infinito:infinito;
	// verificar diagonais
	if(tabuleiro[0]!='.' && tabuleiro[0]==tabuleiro[4] && tabuleiro[4]==tabuleiro[8])
		return tabuleiro[0]=='x'?-infinito:infinito;
	if(tabuleiro[2]!='.' && tabuleiro[2]==tabuleiro[4] && tabuleiro[4]==tabuleiro[6])
		return tabuleiro[2]=='x'?-infinito:infinito;
	// ninguem ganhou, retornar 0
	return 0;
}
