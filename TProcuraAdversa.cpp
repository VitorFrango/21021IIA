#include "TProcuraAdversa.h"
#include <stdio.h>
#include <time.h>

// valor de infinito (vitoria/derrota), omissao 1000
int TProcuraAdversa::infinito=1000;

TProcuraAdversa::TProcuraAdversa(void)
{
}

TProcuraAdversa::~TProcuraAdversa(void)
{
}

// retorna o valor do estado actual, apos procura de profundidade nivel
int TProcuraAdversa::MiniMax(int nivel)
{
	DebugChamada();
	if(nivel==0) {
		DebugCorte(-1,minimizar);
		int resultado=Heuristica();
		if(debug>1) printf(" %d",resultado);
		return resultado;
	} else {
		int resultado=0, melhor=-1;
		TVector<TProcuraConstrutiva*> sucessores;
		TVector<int> custo;
		Sucessores(sucessores,custo);
		if(sucessores.Count()==0) { // no final
			DebugCorte(0,minimizar);
			int resultado=Heuristica();
			if(debug>1) printf(" %d",resultado);
			return resultado;
		}
		for(int i=0;i<sucessores.Count();i++) {
			DebugExpansao(i,sucessores.Count(),minimizar);
			int valor=((TProcuraAdversa*)sucessores[i])->MiniMax(nivel-1);
			if(i==0 || (minimizar?resultado>valor:resultado<valor)) {
				resultado=valor;
				melhor=i;
				if(nivel>1 && debug>=2) { // colocar valor actual alterado
					NovaLinha();
					printf("(%d)",resultado);
				}
			}
		}
		if(melhor>=0) {
			if(solucao!=NULL)
				delete solucao;
			solucao=sucessores[melhor];
		}
		DebugCorte(sucessores.Count(),minimizar);
		LibertarVector(sucessores,melhor);
		return resultado;
	}
}

// retorna o valor do estado actual, apos procura de profundidade nivel
int TProcuraAdversa::MiniMaxAlfaBeta(int nivel,int alfa,int beta)
{
	DebugChamada();
	if(nivel==0) {
		DebugCorte(-1,minimizar);
		int resultado=Heuristica();
		if(debug>1) printf(" %d",resultado);
		return resultado;
	} else {
		int resultado=0,melhor=-1;
		TVector<TProcuraConstrutiva*> sucessores;
		TVector<int> custo;
		Sucessores(sucessores,custo);
		if(sucessores.Count()==0) { // no final
			DebugCorte(0);
			int resultado=Heuristica();
			if(debug>1) printf(" %d",resultado);
			return resultado;
		}
		for(int i=0;i<sucessores.Count();i++) {
			DebugExpansao(i,sucessores.Count(),minimizar);
			int valor=((TProcuraAdversa*)sucessores[i])->MiniMaxAlfaBeta(nivel-1,alfa,beta);
			if(i==0 || (minimizar?resultado>valor:resultado<valor)) {
				resultado=valor;
				melhor=i;
				if(nivel>1 && debug>=2) { // colocar valor actual alterado
					NovaLinha();
					printf("(%d)",resultado);
				}
			}
			// cortes e actualizacao
			if(i<sucessores.Count()-1) { // nao interessa cortar quando nao ha mais nada para cortar
				if(minimizar) { // pretas
					if(alfa>=resultado) {
						if(debug>1) {
							// substituir o ultimo caracter por um corte
#ifdef WINDOWS_VS
							ramo.Last() = 223;
#else
							ramo.Last() = '>';
#endif
							NovaLinha();
							printf(" alfa");
						}
						break; // as brancas tem uma alternativa, e escusado continuar a procurar aqui
					}
					if(beta>resultado)
						beta=resultado;
				} else { // brancas
					if(beta<=resultado) {
						if(debug>1) {
#ifdef WINDOWS_VS
							ramo.Last() = 223;
#else
							ramo.Last() = '>';
#endif
							NovaLinha();
							printf(" beta");
						}
						break; // as pretas tem uma alternativa, e escusado continuar a procurar aqui
					}
					if(alfa<resultado)
						alfa=resultado;
				}
			}
		}
		if(melhor>=0) {
			if(solucao!=NULL)
				delete solucao;
			solucao=sucessores[melhor];
		}
		DebugCorte(sucessores.Count(),minimizar);
		LibertarVector(sucessores,melhor);
		return resultado;
	}
}

// Metodo para teste manual do objecto (chamadas aos algoritmos, construcao de uma solucao manual)
// Este metodo destina-se a testes preliminares, e deve ser redefinido apenas se forem definidos novos algoritmos
void TProcuraAdversa::TesteManual(const char *nome,int seed)
{
	char str[256];
	int limiteNivel=4;
	clock_t instante;
	LimparEstatisticas();
	TRand::srand(seed);
	SolucaoVazia();
	while(true) {
		printf("\n\
%s (TProcuraAdversa)\n\
[Configuracoes] debug %d | limiteNivel %d\n\
[Estatisticas] expansoes %d | geracoes %d | avaliacoes %d ",
			nome,debug,limiteNivel,expansoes,geracoes,avaliacoes);
		Debug();
		printf("\n\
_______________________________________________________________________________\n\
1 - LimparEstatisticas | 2 - SolucaoVazia | 3 - Heuristica      [Inicializacao]\n\
4 - MiniMax | 5 - MiniMaxAlfaBeta                           [Procuras Adversas]\n\
6 - debug | 7 - limiteNivel                                     [Configuracoes]\n\
8 - Sucessores                                                         [Testes]\n\
Opcao:");
		fgets(str, 256, stdin);
		instante = clock();
		switch(atoi(str)) {
			case 0: return;
			case 1: LimparEstatisticas(); break;
			case 2: TRand::srand(seed); SolucaoVazia(); break;
			case 3: printf("\nHeuristica: %d\n",Heuristica()); break;
			case 4:
				LimparEstatisticas();
				printf("\nResultado MiniMax(%d): %d",limiteNivel,MiniMax(limiteNivel));
				if(solucao!=NULL) {
					Copiar(solucao);
					delete solucao;
				}
				solucao=NULL;
				printf(" (%.3fs)", 1.0 * (clock() - instante) / CLOCKS_PER_SEC);
				break;
			case 5:
				LimparEstatisticas();
				printf("\nResultado MiniMaxAlfaBeta(%d): %d",limiteNivel,MiniMaxAlfaBeta(limiteNivel));
				if(solucao!=NULL) {
					Copiar(solucao);
					delete solucao;
				}
				solucao=NULL;
				printf(" (%.3fs)", 1.0 * (clock() - instante) / CLOCKS_PER_SEC);
				break;
			case 6: printf("\nNovo valor para debug: "); fgets(str, 256, stdin); debug=atoi(str); break;
			case 7: printf("\nNovo valor para limiteNivel: "); fgets(str, 256, stdin); limiteNivel=atoi(str); break;
			case 8: // sucessores
				{
					TVector<TProcuraConstrutiva*> sucessores;
					TVector<int> custos;
					int opcao;
					do {
						Sucessores(sucessores,custos);
						printf("\nEstado actual:");
						Debug();
						printf("\nHeuristica: %d\n",Heuristica());
						printf("\nSucessores: %d",sucessores.Count());
						printf("\nMenu:\n1 a %d - vizualizar sucessor\n-1 a -%d - mover para sucessor\n 0 - sair.\nOpcao:",
							sucessores.Count(),sucessores.Count());
						fgets(str, 256, stdin);
						opcao=atoi(str);
						if(opcao>0 && opcao<=sucessores.Count()) {
							printf("\nSucessor %d",opcao);
							sucessores[opcao-1]->Debug();
							printf("\nHeuristica: %d\n",sucessores[opcao-1]->Heuristica());
						} else if(opcao<0 && opcao>=-sucessores.Count()) {
							Copiar(sucessores[-opcao-1]);
						}
						LibertarVector(sucessores);
					} while(opcao!=0);
				}
				break;
			case 9: return;
			default: printf("\nOpcao nao definida."); break;
		}
	}
}
