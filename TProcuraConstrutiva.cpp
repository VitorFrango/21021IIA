#include "TProcuraConstrutiva.h"
#include <stdio.h>
#include <time.h>


// nivel de debug: 0 - nada; 1 - actividade; 2 - passos; 3 - detalhe
int TProcuraConstrutiva::debug=0;
// numero de geracaes de estados
int TProcuraConstrutiva::geracoes=0;
// numero de expansoes de estados
int TProcuraConstrutiva::expansoes=0;
// numero de chamadas a funcao heuristica
int TProcuraConstrutiva::avaliacoes=0;
// auxiliar para construcao da arvore de procura
TVector<unsigned char> TProcuraConstrutiva::ramo;
// espacamento entre ramos da arvore de debug
int TProcuraConstrutiva::espacosRamo=1;
// construir ou nao o caminho do estado inicial ate a solucao
bool TProcuraConstrutiva::calcularCaminho=false;
// valor retornado pela procura (tem de ser libertado)
TVector<TProcuraConstrutiva*> TProcuraConstrutiva::caminho;
// valor retornado pela procura (tem de ser libertado)
TProcuraConstrutiva* TProcuraConstrutiva::solucao=NULL;

// metodo interno para libertar objectos nao necessarios
void TProcuraConstrutiva::LibertarVector(TVector<TProcuraConstrutiva*>&vector, int excepto)
{
	for(int i=0;i<vector.Count();i++)
		if(i!=excepto && vector[i]!=NULL)
			delete vector[i];
	vector.Count(0);
}

// retorna o valor da solucao e coloca o caminho no vector, ou -1 caso nao encontre solucao
int TProcuraConstrutiva::LarguraPrimeiro(int limite)
{
	TVector<TProcuraConstrutiva*> lista;
	TVector<int> pais,custoTotal;
	lista.Add(this);
	pais.Add(-1);
	custoTotal.Add(0);
	for(int i=0;i<lista.Count();i++) {
		lista[i]->DebugPasso(custoTotal[i]);
		if(lista[i]->SolucaoCompleta()) { // teste da funcao Objectivo
			if(calcularCaminho) { // retornar todo o caminho
				caminho.Add(lista[i]);
				// obter caminho para a solucao
				for(int j=pais[i];j>0;j=pais[j]) {
					caminho.Add(lista[j]);
					if(debug>1) printf(" (%d) ",custoTotal[j]);
					if(debug>0) lista[j]->Debug();
					lista[j]=NULL;
				}
			}
			solucao=lista[i];
			lista[i]->DebugSolucao();
			lista[i]=NULL;
			LibertarVector(lista,0);
			return custoTotal[i];
		} else { // expansao do no
			TVector<TProcuraConstrutiva*>sucessores;
			TVector<int> custo;
			lista[i]->Sucessores(sucessores,custo);
			if(limite) {
				// remover elementos a mais da lista de sucessores
				if(limite<lista.Count()-i+sucessores.Count()) {
					int maximo=limite-lista.Count()+i;
					if(maximo>=0 && maximo<sucessores.Count()) {
						for(int i=maximo;i<sucessores.Count();i++)
							delete sucessores[i];
						sucessores.Count(maximo);
					} else if(maximo<0) {
						LibertarVector(sucessores);
					}
				}
			}
			for(int j=0;j<sucessores.Count();j++) {
				pais.Add(i);
				custoTotal.Add(custoTotal[i]+(custo.Count()>0?custo[j]:1));
			}
			lista+=sucessores;
			// Nao se pode libertar estados ja expandidos porque nao se sabe se
			// os pais sao necessarios ou nao.
		}
	}
	LibertarVector(lista,0);
	return -1; // falhou
}


// retorna o valor da solucao e coloca o caminho no vector, ou -1 caso nao encontre solucao
// 090318: bug ao remover elementos da lista
int TProcuraConstrutiva::CustoUniforme(int limite)
{
	TVector<TProcuraConstrutiva*> lista;
	TVector<int> pais,custoTotal;
	TVector<int> id;
	lista.Add(this);
	pais.Add(-1);
	custoTotal.Add(0);
	id.Add(0); // primeiro elemento e o 0
	for(int i=0;i<id.Count();i++) {
		lista[id[i]]->DebugPasso(custoTotal[id[i]]);
		if(lista[id[i]]->SolucaoCompleta()) { // teste da funcao Objectivo
			if(calcularCaminho) { // retornar todo o caminho
				caminho.Add(lista[id[i]]);
				// obter caminho para a solucao
				for(int j=pais[id[i]];j>0;j=pais[j]) {
					caminho.Add(lista[j]);
					if(debug>1) printf(" (%d) ",custoTotal[j]);
					if(debug>0) lista[j]->Debug();
					lista[j]=NULL;
				}
			}
			solucao=lista[id[i]];
			lista[id[i]]->DebugSolucao();
			lista[id[i]]=NULL;
			LibertarVector(lista,0);
			return custoTotal[id[i]];
		} else { // expansao do no
			TVector<TProcuraConstrutiva*>sucessores;
			TVector<int> custo;
			lista[id[i]]->Sucessores(sucessores,custo);
			for(int j=0;j<sucessores.Count();j++) {
				pais.Add(id[i]);
				custoTotal.Add(custoTotal[id[i]]+(custo.Count()>0?custo[j]:1));
				// procurar o elemento com maior valor para inserir este elemento antes
				int l=i+1;
				while(l<id.Count() && custoTotal[id[l]]<=custoTotal.Last())
					l++;
				if(l<id.Count())
					id.Insert(custoTotal.Count()-1,l);
				else id.Add(custoTotal.Count()-1);
			}
			lista+=sucessores; // nao pode apagar nada porque podem ser pais, sempre a somar no fim da lista
			if(limite) {
				// remover elementos a mais da lista de sucessores, com a estimativa mais elevada
				while(lista.Count()-i>limite) {
					// como o ultimo elemento esta apos i, nao foi expandido, pelo que nao tem filhos
					int idRemover=id.Pop();
					delete lista[idRemover];
					lista.Delete(idRemover);
					custoTotal.Delete(idRemover);
					pais.Delete(idRemover);
					// ids com valor superior a idRemover, devem ser decrementados
					for(int j=0;j<id.Count();j++)
						if(id[j]>idRemover)
							id[j]--;
				}
			}
		}
	}
	LibertarVector(lista,0);
	return -1; // falhou
}

// retorna o valor da solucao e coloca o caminho no vector, ou -1 caso nao encontre solucao
// caso o nivel=-1, e feita uma procura em profunidade normal
// caso o nivel>0, e feita uma procura em profundidade limitada
// caso o nivel=0, e feita uma procura em profundidade iterativa, sem limite;
int TProcuraConstrutiva::ProfundidadePrimeiro(int nivel)
{
	DebugChamada();
	if(nivel==0) { // metodo iterativo
		if(debug>=2) printf("\nNivel 1\n");
		int resultado=ProfundidadePrimeiro(++nivel);
		while(resultado==-1) {
			if(debug>=2) printf("\nNivel %d\n",nivel+1);
			resultado=ProfundidadePrimeiro(++nivel);
		}
		return resultado;
	} else { // metodo normal
		if(SolucaoCompleta()) { // um no objectivo!
			DebugSolucao();
			solucao=this;
			return 0;
		}
		if(nivel>1 || nivel<0) { // continuar a procura
			// ainda nao e o no objectivo
			TVector<TProcuraConstrutiva*> sucessores;
			TVector<int> custo;
			Sucessores(sucessores,custo);
			// tentar todos os sucessores, um de cada vez
			for(int i=0;i<sucessores.Count();i++) {
				DebugExpansao(i,sucessores.Count());
				int resultado=sucessores[i]->ProfundidadePrimeiro(nivel-1);
				if(resultado>=0) { // este sucessor resolveu o problema, devolver
					LibertarVector(sucessores,i);
					if(calcularCaminho)
						caminho.Add(sucessores[i]); // adicionar este no ao caminho
					if(custo.Count()>i) // caso exista custo, adiciona-lo
						resultado+=custo[i];
					else resultado++;
					if(debug>1)
						printf(" (%d) ",resultado);
					DebugSolucao();
					return resultado;
				}
			}
			DebugCorte(sucessores.Count());
			LibertarVector(sucessores);
		} else DebugCorte();
		// percorreram-se todos os sucessores e nada (ou atingiu-se o limite), devolver -1
		return -1;
	}
}

// identico ao algoritmo em profundidade, mas os sucessores sao expandidos por ordem da funcao heuristica
// nao e necessario implementar custo nos sucessores para este algoritmo, dado que as decisoes sao sempre locais
// retorna o valor da solucao e coloca o caminho no vector, ou -1 caso nao encontre solucao
int TProcuraConstrutiva::MelhorPrimeiro(int nivel)
{
	DebugChamada();

	if(SolucaoCompleta()) { // um no objectivo!
		DebugSolucao();
		solucao=this;
		return 0;
	}
	if(nivel<0 || nivel>1) { // continuar a procura
		// ainda nao e o no objectivo
		TVector<TProcuraConstrutiva*> sucessores;
		TVector<int> custo;
		TVector<int> id;
		Sucessores(sucessores,custo);
		// calcular as heuristicas
		{
			TVector<int> heuristicas;
			for(int i=0;i<sucessores.Count();i++) {
				heuristicas.Add(sucessores[i]->Heuristica());
			}
			heuristicas.Remove(-1);
			sucessores.Remove(NULL);
			custo.Remove(-1);
			heuristicas.Sort(&id);
		}
		// tentar todo os sucessores, um de cada vez
		for(int i=0;i<sucessores.Count();i++) {
			DebugExpansao(i,sucessores.Count());
			int resultado=sucessores[id[i]]->MelhorPrimeiro(nivel-1);
			if(resultado>=0) { // este sucessor resolveu o problema, devolver
				LibertarVector(sucessores,id[i]);
				if(calcularCaminho)
					caminho.Add(sucessores[id[i]]); // adicionar este no ao caminho
				if(custo.Count()>id[i]) // caso exista custo, adiciona-lo
					resultado+=custo[id[i]];
				else resultado++;
				if(debug>1)
					printf(" (%d) ",resultado);
				DebugSolucao();
				return resultado;
			}
		}
		DebugCorte(sucessores.Count());
		LibertarVector(sucessores);
	} else DebugCorte();
	// percorreram-se todos os sucessores e nada (ou atingiu-se o limite), devolver -1
	return -1;
}

// identico ao algoritmo Custo Uniforme, mas o custo e contabilizado tambem com a funcao heuristica
// retorna o valor da solucao e coloca o caminho no vector, ou -1 caso nao encontre solucao
// retorna o valor da solucao e coloca o caminho no vector, ou -1 caso nao encontre solucao
// 090318: bug ao remover elementos da lista
int TProcuraConstrutiva::AStar(int limite)
{
	TVector<TProcuraConstrutiva*> lista;
	TVector<int> pais,custoTotal,estimativaTotal;
	TVector<int> id;
	lista.Add(this);
	pais.Add(-1);
	custoTotal.Add(0);
	estimativaTotal.Add(0);
	id.Add(0); // primeiro elemento e o 0
	for(int i=0;i<id.Count();i++) {
		lista[id[i]]->DebugPasso(custoTotal[id[i]],estimativaTotal[id[i]]);
		if(lista[id[i]]->SolucaoCompleta()) { // teste da funcao Objectivo
			if(calcularCaminho) {
				caminho.Add(lista[id[i]]);
				// obter caminho para a solucao
				for(int j=pais[id[i]];j>0;j=pais[j]) {
					caminho.Add(lista[j]);
					if(debug>1) printf(" (%d) ",custoTotal[j]);
					if(debug>0) lista[j]->Debug();
					lista[j]=NULL;
				}
			}
			solucao=lista[id[i]];
			lista[id[i]]->DebugSolucao();
			lista[id[i]]=NULL;
			LibertarVector(lista,0);
			return custoTotal[id[i]];
		} else { // expansao do no
			TVector<TProcuraConstrutiva*>sucessores;
			TVector<int> custo;
			lista[id[i]]->Sucessores(sucessores,custo);
			for(int j=0;j<sucessores.Count();j++) {
				pais.Add(id[i]);
				custoTotal.Add(custoTotal[id[i]]+(custo.Count()>0?custo[j]:1));
				estimativaTotal.Add(custoTotal.Last()+sucessores[j]->Heuristica());
				// procurar o elemento com maior valor para inserir este elemento antes
				int l=i+1;
				while(l<id.Count() && estimativaTotal[id[l]]<=estimativaTotal.Last())
					l++;
				if(l<id.Count())
					id.Insert(estimativaTotal.Count()-1,l);
				else id.Add(estimativaTotal.Count()-1);
			}
			lista+=sucessores;
			if(limite) {
				// remover elementos a mais da lista de sucessores, com a estimativa mais elevada
				while(lista.Count()-i>limite) {
					// como o ultimo elemento esta apos i, nao foi expandido, pelo que nao tem filhos
					int idRemover=id.Pop();
					delete lista[idRemover];
					lista.Delete(idRemover);
					custoTotal.Delete(idRemover);
					estimativaTotal.Delete(idRemover);
					pais.Delete(idRemover);
					// ids com valor superior a idRemover, devem ser decrementados
					for(int j=0;j<id.Count();j++)
						if(id[j]>idRemover)
							id[j]--;
				}
			}
		}
	}
	LibertarVector(lista,0);
	return -1; // falhou
}



// Escrever informacao de debug sobre o objecto currente (utilizar variavel TProcuraConstrutiva::debug para seleccionar o detalhe pretendido)
void TProcuraConstrutiva::Debug(void)
{
	if(debug>0)
		printf("\nTProcuras::Debug nao definido, nao e assim possivel mostrar informacao relativa a este estado.");
}

// Metodo para ser chamado antes de analisar cada sucessor
void TProcuraConstrutiva::DebugExpansao(int sucessor, int sucessores,bool duplo)
{
	if(debug>=2) {
		if(sucessor>0) {
			NovaLinha(false);
		}

#ifdef WINDOWS_VS
		if (sucessor == 0 && sucessores == 1) { // só um ramo
			printf("%*c%c", espacosRamo, 196, (duplo ? 186 : 197));
			ramo.Add(' '); // a ser impresso nesta posição nas linhas seguintes
	}
		else if (sucessor == 0) { // início e continua
			printf("%*c%c", espacosRamo, 196, (duplo ? 203 : 194));
			ramo.Add(duplo ? 186 : 179); // a ser impresso nesta posição nas linhas seguintes
		}
		else if (sucessor > 0 && sucessor < sucessores - 1) { // no meio e continua
			printf("%*s%c", espacosRamo, " ", (duplo ? 186 : 195));
			ramo.Last() = (duplo ? 186 : 179);
		}
		else {
			printf("%*s%c", espacosRamo, " ", (duplo ? 200 : 192)); // no fim, vai acabar 
			ramo.Last() = ' '; // a ser impresso nesta posição nas linhas seguintes
		}

#else
		if (sucessor == 0 && sucessores == 1) { // só um ramo
			printf("%*c%c", espacosRamo, '#', (duplo ? '#' : '+'));
			ramo.Add(' '); // a ser impresso nesta posição nas linhas seguintes
		}
		else if (sucessor == 0) { // início e continua
			printf("%*c%c", espacosRamo, '-', (duplo ? '#' : '+'));
			ramo.Add(duplo ? '/' : '|'); // a ser impresso nesta posição nas linhas seguintes
		}
		else if (sucessor > 0 && sucessor < sucessores - 1) { // no meio e continua
			printf("%*s%c", espacosRamo, " ", (duplo ? '#' : '+'));
			ramo.Last() = (duplo ? '/' : '|');
		}
		else {
			printf("%*s%c", espacosRamo, " ", (duplo ? '#' : '+')); // no fim, vai acabar 
			ramo.Last() = ' '; // a ser impresso nesta posição nas linhas seguintes
		}
#endif
	}
}

// Metodo para ser chamado quando nao ha sucessores ou ha um corte de profundidade
void TProcuraConstrutiva::DebugCorte(int sucessores,bool duplo)
{
	if(debug>=2) {
#ifdef WINDOWS_VS
		if (sucessores < 0) {
			printf("%c %d/%d", 221,expansoes,geracoes); // corte de profundidade  
			if (avaliacoes)
				printf("/%d", avaliacoes);
			if (debug >= 3)
				Debug();
		}
		else if (sucessores > 0)
			ramo.Pop();
		else { // ramo em que nao e possivel continuar
			printf("%c %d/%d", 177,expansoes,geracoes);
			if (avaliacoes)
				printf("/%d", avaliacoes);
			if (debug >= 3)
				Debug();
		}
#else
		if(sucessores<0) {
			printf("%c %d/%d", '=', expansoes, geracoes); // corte de profundidade  
			if(avaliacoes) 
				printf("/%d",avaliacoes);
			if(debug>=3)
				Debug();
		} else if(sucessores>0)
			ramo.Pop();
		else { // ramo em que nao e possivel continuar
			printf("%c %d/%d", '&', expansoes, geracoes);
			if(avaliacoes) 
				printf("/%d",avaliacoes);
			if(debug>=3)
				Debug();
		}
#endif
	}
}

// Encontrou uma solucao
void TProcuraConstrutiva::DebugSolucao(void)
{
	if(debug>0 && SolucaoCompleta()) {
		printf(" Solucao encontrada!");
		ramo.Count(0);
		Debug();
	} else {
		if(debug>1) 
			Debug();
		if(debug>=2) 
			ramo.Pop();
	}
}

// Informacao de debug na chamada ao metodo recursivo
void TProcuraConstrutiva::DebugChamada(void)
{
	if(debug==1 && expansoes%1000==0)
		printf("#");
}

// Chamar sempre que se quer uma nova linha com a arvore em baixo
void TProcuraConstrutiva::NovaLinha(bool tudo)
{
	printf("\n");
	for(int i=0;i<ramo.Count()-(tudo?0:1);i++)
		printf("%*s%c",espacosRamo," ",ramo[i]);
}

// Passo no algoritmo em largura
void TProcuraConstrutiva::DebugPasso(int custo, int heuristica)
{
	if(debug==1 && expansoes%1000==0)
		printf("#");
	if(debug>=2) {
		printf("\n%02d: %d/%d",custo,expansoes,geracoes);
		if(avaliacoes) 
			printf("/%d",avaliacoes);
		if(heuristica) 
			printf("(%d)",heuristica);
	}
	if(debug>2)
		Debug();
}

// Chamar antes de iniciar uma procura
void TProcuraConstrutiva::LimparEstatisticas(void)
{
	expansoes=geracoes=avaliacoes=0;
	ramo.Count(0);
	if(calcularCaminho)
		LibertarVector(caminho,0);
	if(solucao!=NULL)
		delete solucao;
	solucao=NULL;
}

// Metodo para teste manual do objecto (chamadas aos algoritmos, construcao de uma solucao manual)
// Este metodo destina-se a testes preliminares, e deve ser redefinido apenas se forem definidos novos algoritmos
void TProcuraConstrutiva::TesteManual(const char *nome, int seed)
{
	char str[256];
	int limiteNivel=10;
	clock_t instante;
	LimparEstatisticas();
	TRand::srand(seed);
	SolucaoVazia();
	while(true) {
		printf("\n\
%s (TProcuraConstrutiva)\n\
[Configuracoes] debug %d | calcularCaminho %d | limiteNivel %d\n\
[Estatisticas] expansoes %d | geracoes %d | avaliacoes %d ",
			nome,debug,calcularCaminho?1:0,limiteNivel,expansoes,geracoes,avaliacoes);
		Debug();
		printf("\n\
_______________________________________________________________________________\n\
1 - LimparEstatisticas | 2 - SolucaoVazia | 3 - Heuristica      [Inicializacao]\n\
4 - LarguraPrimeiro | 5 - CustoUniforme | 6 - Prof.Primeiro    [Procuras Cegas]\n\
7 - MelhorPrimeiro | 8 - AStar                            [Procuras Informadas]\n\
9 - debug | 10 - calcularCaminho | 11 - limiteNivel             [Configuracoes]\n\
12 - Sucessores                                                        [Testes]\n\
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
						LibertarVector(sucessores);
					} while(opcao!=0);
				}
				break;
			case 13: return;
			default: printf("\nOpcao nao definida."); break;
		}
	}
}
