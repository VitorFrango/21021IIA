#include "TProcuraMelhorativa.h"

#include <stdio.h>
#include <time.h>

// numero de maximo de avaliacoes (omissao 10000)
int TProcuraMelhorativa::maximoAvaliacoes=10000;

TProcuraMelhorativa::TProcuraMelhorativa(void) : custo(-1)
{
}

TProcuraMelhorativa::~TProcuraMelhorativa(void)
{
}

// metodo interno para libertar objectos nao necessarios
void TProcuraMelhorativa::LibertarVector(TVector<TProcuraMelhorativa*>&vector, int excepto)
{
	for(int i=0;i<vector.Count();i++)
		if(i!=excepto && vector[i]!=NULL)
			delete vector[i];
	vector.Count(0);
}

// e gerada uma nova solucao aleatoriamente. Este metodo e definido iniciamente como uma procura em
// profundidade em que o sucessor e escolhido aleatoriamente e sem retrocesso (caso falhe o processo re-inicia)
void TProcuraMelhorativa::NovaSolucao(void)
{
	SolucaoVazia();
	TVector<TProcuraConstrutiva*> sucessores;
	TVector<int> custo;
	while(!SolucaoCompleta()) { // escolher um sucessor aleatoriamente
		Sucessores(sucessores,custo);
		if(sucessores.Count()==0) { // falhou, re-iniciar
			SolucaoVazia();
		} else {
			*this=*((TProcuraMelhorativa*)sucessores.Random());
			TProcuraConstrutiva::LibertarVector(sucessores);
		}
	}
}

// Retorna o valor da solucao completa actual.
int TProcuraMelhorativa::Avaliar(void)
{
	avaliacoes++;
	return 0;
}

// Operador de vizinhanca (apenas em solucoes completas)
void TProcuraMelhorativa::Vizinhanca(TVector<TProcuraMelhorativa*>&vizinhos)
{
	expansoes++;
	geracoes+=vizinhos.Count();
}

// retorna a avaliacao do resultado actual
int TProcuraMelhorativa::EscaladaDoMonte(bool movePrimeiro)
{
	int procuras=0;
	Avaliar(); // avaliar o proprio
	// gerar uma solucao aleatoria
	TProcuraMelhorativa *solucao=(TProcuraMelhorativa *)Duplicar();
	while(!Parar()) { // iniciar num qualquer ponto e escalar o monte mais perto
		if(debug>1)
			printf("\nEscalada %d (%d/%d/%d)",++procuras,geracoes,expansoes,avaliacoes);
		solucao->NovaSolucao();
		solucao->Avaliar();
		if(debug>2)
			printf(" base %d",solucao->custo);
		while(!Parar()) { // escalar o monte mais perto
			DebugChamada();
			if(movePrimeiro) {
				// avanca logo que encontre um vizinho melhor
				//(nao olha em todas as direccoes antes de comecar a subir)
				TVector<TProcuraMelhorativa*> vizinhos;
				solucao->Vizinhanca(vizinhos);
				while(vizinhos.Count()>0)
					if(solucao->custo > vizinhos.Last()->Avaliar()) {
						TProcuraMelhorativa *trocar=solucao;
						solucao=vizinhos.Last();
						vizinhos.Last()=trocar;
						LibertarVector(vizinhos);
						// verificar se esta solucao e melhor que a actual
						if(custo>solucao->custo) {
							Copiar(solucao);
							DebugMelhorEncontrado();
						}
						break;
					} else delete vizinhos.Pop();
				// percorridos todos os vizinhos e nao ha nenhum melhor
				// altura para recomecar de novo
				if(debug>2)
					printf(" topo %d",solucao->custo);
				break;
			} else { // analisa todos os vizinhos e avanca apenas para o melhor vizinho
				TVector<TProcuraMelhorativa*> vizinhos;
				int melhorCusto,melhorVizinho=-1;
				solucao->Vizinhanca(vizinhos);
				for(int i=0;i<vizinhos.Count();i++) {
					vizinhos[i]->Avaliar();
					if(i==0 || melhorCusto>vizinhos[i]->custo) {
						melhorCusto=vizinhos[i]->custo;
						melhorVizinho=i;
					}
				}
				// trocar caso melhore
				if(melhorVizinho>=0 && melhorCusto < solucao->custo) {
					TProcuraMelhorativa *trocar=solucao;
					solucao=vizinhos[melhorVizinho];
					vizinhos[melhorVizinho]=trocar;
					LibertarVector(vizinhos);
					// verificar se esta solucao e melhor que a actual
					if(custo>solucao->custo) {
						Copiar(solucao);
						DebugMelhorEncontrado();
					}
				} else {
					// nao melhorou, recomecar
					LibertarVector(vizinhos);
					if(debug>2)
						printf(" topo %d",solucao->custo);
					break;
				}
			}
		}
	}
	return custo;
}

// retorna a avaliacao do resultado final
// parametros para a mutacao e cruzamento podem ser dados em variaveis globais
int TProcuraMelhorativa::AlgoritmoGenetico(int populacao,int probablidadeMutacao,int distanciaMinima)
{
	TVector<TProcuraMelhorativa*> geracao1,geracao2;
	TVector<int> custos1,custos2,id;
	Avaliar(); // avaliar o proprio
	if(debug>1)
		printf("\nConstruir populacao inicial (%d elementos)",populacao);
	// construir a geracao inicial
	for(int i=0;i<populacao;i++) {
		geracao1.Add((TProcuraMelhorativa*)Duplicar());
		geracao1.Last()->NovaSolucao();
		custos1.Add(geracao1.Last()->Avaliar());
		if(custos1.Last() < custo) { // melhor encontrado, copiar
			Copiar(geracao1.Last());
			DebugMelhorEncontrado();
		}
	}
	while(!Parar()) { // uma geracao
		expansoes++;
		if(debug==1) printf(".");
		else if(debug==2) printf("\nG%d #%d (%d a %d)",expansoes,custos1.Count(),custos1.First(),custos1.Last());
		else printf("\nGeracao %d (%d/%d) populacao %d (custos %d a %d):",
			expansoes,geracoes,avaliacoes,custos1.Count(),custos1.First(),custos1.Last());
		////////////////////////////
		// cruzar conforme o custo
		////////////////////////////
		int minimo,maximo,total;
		for(int i=0;i<custos1.Count();i++) {
			if(i==0 || custos1[i]<minimo)
				minimo=custos1[i];
			if(i==0 || custos1[i]>maximo)
				maximo=custos1[i];
		}
		TVector<int> probabilidade;
		total=0;
		for(int i=0;i<custos1.Count();i++) {
			// cada elemento tem de ter sempre probabilidade de ser escolhido
			// por isso adiciona-se 1 para que o custo maximo tenha probabilidade minima
			probabilidade.Add(maximo-custos1[i]+1);
			total+=probabilidade.Last();
		}
		while(geracao2.Count()<2*populacao) { // gera-se o doubro para poder seleccionar os melhores
			// escolher dois individuos aleatoriamente de acordo com as probabilidades
			int a,b;
			int valor=TRand::rand()%total;
			a=0;
			while(valor>=0)
				valor-=probabilidade[a++];
			a--;
			valor=TRand::rand()%total;
			b=0;
			while(valor>=0)
				valor-=probabilidade[b++];
			b--;
			// gerar um novo individuo por cruzamento
			geracao2.Add((TProcuraMelhorativa*)Duplicar());
			geracao2.Last()->Cruzamento(geracao1[a],geracao1[b]);
			if(TRand::rand()%100<probablidadeMutacao)
				geracao2.Last()->Mutar();
			custos2.Add(geracao2.Last()->Avaliar());
			if(debug>2)
				printf("\n  Cruzar %dx%d (custos %d x %d -> %d)",a,b,geracao1[a]->custo,geracao1[b]->custo,custos2.Last());
			if(custos2.Last() < custo) { // melhor encontrado, copiar
				Copiar(geracao2.Last());
				DebugMelhorEncontrado();
			}
		}

		// seleccao: os melhores, ignorando os pais
		custos2.Sort(&id);
		custos1.Count(0);
		LibertarVector(geracao1);
		for(int i=0;i<populacao && i<custos2.Count();i++) {
			if(i>0 && distanciaMinima>0) { // verificar se ha outro elemento muito parecido la
				bool diferente=true;
				for(int j=0;j<geracao1.Count();j++)
					if(geracao1[j]->Distancia(geracao2[id[i]])<distanciaMinima) {
						diferente=false;
						break;
					}
				if(!diferente)
					continue;
			}
			geracao1.Add(geracao2[id[i]]);
			custos1.Add(custos2[id[i]]);
			geracao2[id[i]]=NULL;
		}
		LibertarVector(geracao2);
		custos2.Count(0);
	}
	LibertarVector(geracao1);
	return custo;
}
// Chamar sempre que uma solucao melhor que a actual e encontrada
void TProcuraMelhorativa::DebugMelhorEncontrado(void)
{
	if(debug>0) {
		NovaLinha();
		printf("Melhor: %d",custo);
	}
	if(debug>1)
		printf(" %d/%d",geracoes,avaliacoes);
	if(debug>2)
		Debug();
}

// Metodo para teste manual do objecto (chamadas aos algoritmos, construcao de uma solucao manual)
// Este metodo destina-se a testes preliminares, e deve ser redefinido apenas se forem definidos novos algoritmos
void TProcuraMelhorativa::TesteManual(const char *nome,int seed)
{
	char str[256];
	int limiteNivel=10;
	bool movePrimeiro=true;
	int populacao=20;
	int probablidadeMutacao=50;
	int distanciaMinima=0;
	clock_t instante;
	LimparEstatisticas();
	TRand::srand(seed);
	SolucaoVazia();
	while(true) {
		printf("\n\
%s (TProcuraMelhorativa)\n\
[Configuracoes] debug %d | calcularCaminho %d | limiteNivel %d\n\
                maximoAvaliacoes %d | movePrimeiro %d | populacao %d\n\
                probablidadeMutacao %d | distanciaMinima %d\n\
[Estatisticas] expansoes %d | geracoes %d | avaliacoes %d ",
			nome,debug,calcularCaminho?1:0,limiteNivel,maximoAvaliacoes,movePrimeiro?1:0,populacao,
			probablidadeMutacao,distanciaMinima,expansoes,geracoes,avaliacoes);
		Debug();
		printf("\n\
__________________________Solucoes Parciais____________________________________\n\
1 - LimparEstatisticas | 2 - SolucaoVazia | 3 - Heuristica      [Inicializacao]\n\
4 - LarguraPrimeiro | 5 - CustoUniforme | 6 - Prof.Primeiro    [Procuras Cegas]\n\
7 - MelhorPrimeiro | 8 - AStar                            [Procuras Informadas]\n\
9 - debug | 10 - calcularCaminho | 11 - limiteNivel             [Configuracoes]\n\
12 - Sucessores                                                        [Testes]\n\
__________________________Solucoes Completas___________________________________\n\
13 - NovaSolucao | 14 - Avaliar                                 [Inicializacao]\n\
15 - EscaladaDoMonte | 16 - AlgoritmoGenetico           [Procuras Melhorativas]\n\
17 - maximoAvaliacoes | 18 - movePrimeiro                       [Configuracoes]\n\
19 - popopulacao | 20 - prob.Mutacao | 21 - dist.Minima         [Configuracoes]\n\
22 - Vizinhos | 23 - Mutar                                             [Testes]\n\
Opcao:");

		fgets(str,256,stdin);
		instante = clock();
		switch(atoi(str)) {
			case 0: return;
			case 1: LimparEstatisticas(); break;
			case 2: TRand::srand(seed); SolucaoVazia(); break;
			case 3: printf("\nHeuristica: %d\n",Heuristica()); break;
			case 4:
				LimparEstatisticas();
				printf("\nResultado LarguraPrimeiro(%d): %d",limiteNivel,LarguraPrimeiro(limiteNivel));
				if(solucao!=NULL) {
					Copiar(solucao);
					delete solucao;
					solucao=NULL;
				}
				printf(" (%.3fs)", 1.0 * (clock() - instante) / CLOCKS_PER_SEC);
				break;
			case 5:
				LimparEstatisticas();
				printf("\nResultado CustoUniforme(%d): %d",limiteNivel,CustoUniforme(limiteNivel));
				if(solucao!=NULL) {
					Copiar(solucao);
					delete solucao;
					solucao=NULL;
				}
				printf(" (%.3fs)", 1.0 * (clock() - instante) / CLOCKS_PER_SEC);
				break;
			case 6:
				LimparEstatisticas();
				printf("\nResultado ProfundidadePrimeiro(%d): %d",limiteNivel,ProfundidadePrimeiro(limiteNivel));
				if(solucao!=NULL) {
					Copiar(solucao);
					delete solucao;
					solucao=NULL;
				}
				printf(" (%.3fs)", 1.0 * (clock() - instante) / CLOCKS_PER_SEC);
				break;
			case 7:
				LimparEstatisticas();
				printf("\nResultado MelhorPrimeiro(%d): %d",limiteNivel,MelhorPrimeiro(limiteNivel));
				if(solucao!=NULL) {
					Copiar(solucao);
					delete solucao;
					solucao=NULL;
				}
				printf(" (%.3fs)", 1.0 * (clock() - instante) / CLOCKS_PER_SEC);
				break;
			case 8:
				LimparEstatisticas();
				printf("\nResultado AStar(%d): %d",limiteNivel,AStar(limiteNivel));
				if(solucao!=NULL) {
					Copiar(solucao);
					delete solucao;
					solucao=NULL;
				}
				printf(" (%.3fs)", 1.0 * (clock() - instante) / CLOCKS_PER_SEC);
				break;
			case 9: printf("\nNovo valor para debug: "); fgets(str, 256, stdin); debug=atoi(str); break;
			case 10: printf("\nNovo valor para cacularCaminho: "); fgets(str, 256, stdin); calcularCaminho=(atoi(str)!=0); break;
			case 11: printf("\nNovo valor para limiteNivel: "); fgets(str, 256, stdin); limiteNivel=atoi(str); break;
			case 12: // sucessores
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
						TProcuraConstrutiva::LibertarVector(sucessores);
					} while(opcao!=0);
				}
				break;
			case 13: TRand::srand(seed); NovaSolucao(); break;
			case 14: printf("\nAvaliar: %d\n",Avaliar()); break;
			case 15:
				LimparEstatisticas();
				printf("\nResultado EscaladaDoMonte(%d): %d",movePrimeiro?1:0,EscaladaDoMonte(movePrimeiro));
				if(solucao!=NULL) {
					Copiar(solucao);
					delete solucao;
					solucao=NULL;
				}
				break;
			case 16:
				LimparEstatisticas();
				printf("\nResultado AlgoritmoGenetico(%d,%d,%d): %d",populacao,probablidadeMutacao,distanciaMinima,
					AlgoritmoGenetico(populacao,probablidadeMutacao,distanciaMinima));
				if(solucao!=NULL) {
					Copiar(solucao);
					delete solucao;
					solucao=NULL;
				}
				break;
			case 17: printf("\nNovo valor para maximoAvaliacoes: "); fgets(str, 256, stdin); maximoAvaliacoes=atoi(str); break;
			case 18: printf("\nNovo valor para movePrimeiro: "); fgets(str, 256, stdin); movePrimeiro=(atoi(str)!=0); break;
			case 19: printf("\nNovo valor para populacao: "); fgets(str, 256, stdin); populacao=atoi(str); break;
			case 20: printf("\nNovo valor para probablidadeMutacao: "); fgets(str, 256, stdin); probablidadeMutacao=atoi(str); break;
			case 21: printf("\nNovo valor para distanciaMinima: "); fgets(str, 256, stdin); distanciaMinima=atoi(str); break;
			case 22: // vizinhos
				{
					TVector<TProcuraMelhorativa*> vizinhos;
					int opcao;
					do {
						Vizinhanca(vizinhos);
						printf("\nEstado actual:");
						Debug();
						printf("\nAvaliar: %d\n",Avaliar());
						printf("\nVizinhos: %d",vizinhos.Count());
						printf("\nMenu:\n1 a %d - vizualizar vizinho\n-1 a -%d - mover para vizinho\n 0 - sair.\nOpcao:",
							vizinhos.Count(),vizinhos.Count());
						fgets(str, 256, stdin);
						opcao=atoi(str);
						if(opcao>0 && opcao<=vizinhos.Count()) {
							printf("\nVizinho %d",opcao);
							vizinhos[opcao-1]->Debug();
							printf("\nAvaliar: %d\n",vizinhos[opcao-1]->Avaliar());
						} else if(opcao<0 && opcao>=-vizinhos.Count()) {
							Copiar(vizinhos[-opcao-1]);
						}
						LibertarVector(vizinhos);
					} while(opcao!=0);
				}
				break;
			case 23: Mutar(); break;
			case 24: return;
			default: printf("\nOpcao nao definida."); break;
		}
	}
}
