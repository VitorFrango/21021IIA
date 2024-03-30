#pragma once

#include "TVector.h"

//#define WINDOWS_VS 1

///////////////////////////////////////////////////////////////////////////////
//	TProcuraConstrutiva class
///////////////////////////////////////////////////////////////////////////////
//	Author: Jose Coelho
//	Last revision: 2007-08-31
//	Description:
//    Superclasse de procuras no espaco das solucaes parciais (a solucao e construida).
//    Esta classe nao tem variaveis, deve criar as variaveis necessarias
//    a implementacao dos metodos virtuais para o problema em concreto.
//    Esta classe assume que nao ha ciclos, utilizar TProcuraConstrutivaComCiclos
//    caso tal nao seja verdade.
///////////////////////////////////////////////////////////////////////////////
class TProcuraConstrutiva
{
protected:
	// metodo interno para libertar objectos nao necessarios
	void LibertarVector(TVector<TProcuraConstrutiva*>&vector, int excepto=-1);
public:
	// classe sem variaveis, construtor e destrutores sao vazios
	TProcuraConstrutiva(void) {}
	virtual ~TProcuraConstrutiva(void) {}


	///////////////////////////////////////////////////////////////////////////////
	// Variaveis globais a classe. Estas variaveis sao reutilizadas para cada corrida.
	// O facto de serem globais evita que sejam copiadas vezes sem sentido, mas impede que
	// se possa fazer mais que uma corrida em simultaneo.
	// Se necessitar de duas ou mais corridas em simultaneo, tem de remover estas variaveis globais.
	///////////////////////////////////////////////////////////////////////////////

	// nivel de debug: 0 - nada; 1 - actividade; 2 - passos; 3 - detalhe
	static int debug;
	// numero de geracaes de estados
	static int geracoes;
	// numero de expansoes de estados
	static int expansoes;
	// numero de chamadas a funcao heuristica
	static int avaliacoes;
	// auxiliar para construcao da arvore de procura
	static TVector<unsigned char> ramo;
	// espacamento entre ramos da arvore de debug
	static int espacosRamo;
	// construir ou nao o caminho do estado inicial ate a solucao
	static bool calcularCaminho;
	// valor retornado pela procura (tem de ser libertado)
	static TVector<TProcuraConstrutiva*> caminho;
	// valor retornado pela procura (tem de ser libertado)
	static TProcuraConstrutiva* solucao;


	///////////////////////////////////////////////////////////////////////////////
	// Metodos para redefinir conforme o problema
	///////////////////////////////////////////////////////////////////////////////

	// Cria um objecto que e uma copia deste
	virtual TProcuraConstrutiva* Duplicar(void) = 0;
	// Fica com uma copia de objecto
	virtual void Copiar(TProcuraConstrutiva*objecto) { }
	// Coloca o objecto no estado inicial da procura
	virtual void SolucaoVazia(void) {}
	// Coloca em sucessores a lista de objectos sucessores (sao alocados neste metodo e tem de ser apagados)
	// O custo nao necessita de ser preenchido, caso seja sempre unitario
	// chamar o metodo desta classe apos adicionar os sucessores para actualizar geracoes e expansoes
	virtual void Sucessores(TVector<TProcuraConstrutiva*>&sucessores, TVector<int>&custo) {
		expansoes++;
		geracoes+=sucessores.Count();
	}
	// Retorna verdade caso o estado actual seja um estado objectivo
	virtual bool SolucaoCompleta(void) { return false; }
	// Escrever informacao de debug sobre o objecto currente
	// (utilizar variavel TProcuraConstrutiva::debug para seleccionar o detalhe pretendido)
	virtual void Debug(void);

	// Redefinir para poder utilizar os algoritmos informados
	// O custo desde o no inicial e tido em conta, esta funcao deve devolver o custo estimado
	// nunca sobre estimando, deste estado ate ao no final mais proximo (e um minimo)
	// chamar para actualiacao de avaliacoes
	virtual int Heuristica(void) { avaliacoes++; return 0; }


	///////////////////////////////////////////////////////////////////////////////
	// Algoritmos de procura cega (chamar apos colocar o estado inicial no objecto)
	// Nao necessitam da implementacao da funcao heuristica
	///////////////////////////////////////////////////////////////////////////////

	// retorna o valor da solucao e coloca o caminho no vector (se calcularCaminho=true), ou -1 caso nao encontre solucao
	// limite e o numero de estados gerados nao expandidos, que nao pode ultrapassar esse limite
	// os que ultrapassarem sao deitados fora (se 0 este limite nao importa, podendo haver problemas de memoria)
	int LarguraPrimeiro(int limite=0);

	// retorna o valor da solucao e coloca o caminho no vector, ou -1 caso nao encontre solucao
	int CustoUniforme(int limite=0);

	// retorna o valor da solucao e coloca o caminho no vector, ou -1 caso nao encontre solucao
	// caso o nivel=-1, e feita uma procura em profunidade normal
	// caso o nivel>0, e feita uma procura em profundidade limitada
	// caso o nivel=0, e feita uma procura em profundidade iterativa, sem limite;
	int ProfundidadePrimeiro(int nivel=-1);


	///////////////////////////////////////////////////////////////////////////////
	// Algoritmos de procura informados (chamar apos colocar o estado inicial no objecto)
	// Necessitam da implementacao da funcao Heuristica
	///////////////////////////////////////////////////////////////////////////////

	// identico ao algoritmo em profundidade, mas os sucessores sao expandidos por ordem da funcao heuristica
	int MelhorPrimeiro(int nivel=-1);

	// identico ao algoritmo Custo Uniforme, mas o custo e contabilizado tambem com a funcao heuristica
	// retorna o valor da solucao e coloca o caminho no vector, ou -1 caso nao encontre solucao
	int AStar(int limite=0);

protected:
	// Metodo para ser chamado antes de analisar cada sucessor
	void DebugExpansao(int sucessor, int sucessores,bool duplo=false);
	// Metodo para ser chamado quando nao ha sucessores ou ha um corte de profundidade
	void DebugCorte(int sucessores=-1,bool duplo=false);
	// Encontrou uma solucao
	void DebugSolucao(void);
	// Informacao de debug na chamada ao metodo recursivo
	void DebugChamada(void);
	// Chamar sempre que se quer uma nova linha com a arvore em baixo
	void NovaLinha(bool tudo=true);
	// Passo no algoritmo em largura
	void DebugPasso(int custo=0, int heuristica=0);
public:
	// Chamar antes de iniciar uma procura
	void LimparEstatisticas(void);
	// Metodo para teste manual do objecto (chamadas aos algoritmos, construcao de uma solucao manual)
	// Este metodo destina-se a testes preliminares, e deve ser redefinido apenas se forem definidos novos algoritmos
	virtual void TesteManual(const char *nome, int seed=1);
};
