/*
// Created by Vitor Frango on 06/04/2024.

 O código apresentado implementa um algoritmo de busca de custo uniforme. A busca de custo uniforme é um algoritmo de busca que expande o nó de menor custo, garantindo que o primeiro nó a atingir o objetivo seja o de menor custo.
*/


#include <iostream>
#include <ostream>
#include <vector>
#include <tuple>  // utilizado para criar tuplas para armazenar as coordenadas e o numero de deputados
#include <queue>  // utilizado para a fila de prioridade para os estados
#include <iomanip> // utilizado para formatar a saída
#include <numeric>  // utilizado para somar os deputados
#include <chrono>  // utilizado para medir o tempo

using namespace std;


// função para calcular o raio de proteção, baseado no número de deputados
int raio_protecao(int deputados) {
    // O raio de proteção é calculado com base no número de deputados
    switch (deputados) {
        case 0: return 1;   // 3x3 zones
        case 1: return 2;   // 5x5 zones
        case 5: return 3;   // 7x7 zones
        case 13: return 4;  // 9x9 zones
        default: return 0;
    }
}

// Estrutura para armazenar o estado atual
struct Estado {
    vector<tuple<int, int, int>> delegacoes;
    int cost;
    int familas_protegidas;

    // construtor para inicializar o estado
    Estado(vector<tuple<int, int, int>> delegacoes = {}, int cost = 0, int familas_protegidas = 0)
            : delegacoes(delegacoes), cost(cost), familas_protegidas(familas_protegidas) {}

    // operador para comparar os estados com base no custo, necessário para a fila de prioridade
    bool operator>(const Estado& other) const {
        return cost > other.cost;
    }
};

// função para calcular o número de famílias protegidas com base no mapa e no estado atual
int calcular_familas_protegidas(const vector<vector<int>>& map, const Estado& Estado) {
    int familas_protegidas = 0;
    vector<vector<bool>> protegida_por_delegados(map.size(), vector<bool>(map[0].size(), false));

    for (const auto& Delegacao : Estado.delegacoes) {
        int x, y, deputados;
        tie(x, y, deputados) = Delegacao;
        int radius = raio_protecao(deputados);

        for (int i = max(0, x - radius); i <= min(static_cast<int>(map.size()) - 1, x + radius); ++i) {
            for (int j = max(0, y - radius); j <= min(static_cast<int>(map[0].size()) - 1, y + radius); ++j) {
                if (!protegida_por_delegados[i][j]) {
                    familas_protegidas += map[i][j];
                    protegida_por_delegados[i][j] = true;
                }
            }
        }
    }
    return familas_protegidas;
}

// função para gerar os sucessores do estado atual a partir do estado atual, do mapa e do orçamento
vector<Estado> gerar_successores(const Estado& currente_Estado, const vector<vector<int>>& map, int budget) {
    vector<Estado> successores;
    vector<int> deputados_permitidos = {0, 1, 5, 13};

    for (int deputados : deputados_permitidos) { // 0, 1, 5, 13
        int custo_da_delegacao = 4 + deputados; // 4 para a delegação e 1 para cada deputado
        if (currente_Estado.cost + custo_da_delegacao <= budget) {
            for (size_t i = 0; i < map.size(); ++i) {
                for (size_t j = 0; j < map[i].size(); ++j) {
                    Estado novo_Estado = currente_Estado;
                    novo_Estado.delegacoes.emplace_back(i, j, deputados);
                    novo_Estado.cost += custo_da_delegacao;
                    novo_Estado.familas_protegidas = calcular_familas_protegidas(map, novo_Estado);
                    if (novo_Estado.familas_protegidas >= currente_Estado.familas_protegidas) {
                        successores.push_back(novo_Estado);
                    }
                }
            }
        }
    }

    return successores;
}

// Função para imprimir o mapa com o estado atual e o orçamento as delegações e as famílias protegidas
void imprime_mapa(const vector<vector<int>>& map, const Estado& Estado, int budget) {
    for (size_t i = 0; i < map.size(); ++i) {
        for (size_t j = 0; j < map[i].size(); ++j) {
            bool isDelegacao = false;
            bool isProtegida = false;

            for (const auto& Delegacao : Estado.delegacoes) {
                int x, y, deputados;
                tie(x, y, deputados) = Delegacao;
                int radius = raio_protecao(deputados);

                if (i >= x - radius && i <= x + radius && j >= y - radius && j <= y + radius) {
                    isProtegida = true;
                }

                if (i == x && j == y) {
                    isDelegacao = true;
                    cout << " " << deputados << "D"; // delegacao com deputados e sem protecao
                    break;
                }
            }

            if (!isDelegacao) {
                cout << setw(2) << map[i][j]; // mostrar o numero de familias
                if (isProtegida) {
                    cout << "\033[1;31mX\033[0m"; // adiciona "X" vermelho se Protegida; // adiciona "x" se Protegida
                } else {
                    cout << " "; // mantem "  " se nao Protegida
                }
            }
        }
        cout << endl;
    }

    // Calcula o total de familias protegidas e o custo total
    int total_Protegida = calcular_familas_protegidas(map, Estado);

    cout << "\nFamílias protegidas: " << Estado.familas_protegidas << "/" << total_Protegida << endl;
    cout << "Custo total: " << Estado.cost << " moedas de ouro" << endl;
    int moedas_restantes = budget - Estado.cost;
    cout << "Moedas restantes: " << moedas_restantes << " moedas de ouro" << endl;
    int num_deputados = accumulate(begin(Estado.delegacoes), end(Estado.delegacoes), 0,
                                   [](int acc, const tuple<int, int, int>& Delegacao) { return acc + get<2>(Delegacao); });
    cout << "Número de deputados: " << num_deputados << endl;
    cout << "Número de delegacias colocadas: " << Estado.delegacoes.size() << endl;
}

// Função para processar as instâncias do problema tentando encontrar uma solução dentro do tempo limite
void instancias(int instancia_id, const vector<vector<int>>& map, int budget, int min_familiasA, int min_familiasB) {
    using namespace std::chrono;
    auto start = high_resolution_clock::now();

    cout << "----------------------------------------" << endl; // Linha de separação entre as instâncias
    cout << "Processando a instância ID: " << instancia_id << endl;
    cout << "Verba disponível: " << budget << " moedas de ouro" << endl;
    cout << " D -> Delegado colocado \n X -> Família protegida\n";
    cout << "----------------------------------------" << endl; // Linha de separação entre as instâncias

    Estado last_state; // Último estado para exibição
    for (int min_families : {min_familiasA, min_familiasB}) {
        bool solution_found = false;
        int num_expansoes = 0;
        int num_geracoes = 0;

        // Fila de prioridade para os estados e o estado de menor custo é sempre removido primeiro.
        priority_queue<Estado, vector<Estado>, greater<Estado>> pq;
        pq.push(Estado());

        while (!pq.empty()) {
            auto current_duration = duration_cast<milliseconds>(high_resolution_clock::now() - start);
            if (current_duration.count() > 59590) { // Verifica o limite de tempo de 59.59 segundos

                break;
            }

            Estado currente_Estado = pq.top();
            pq.pop();
            last_state = currente_Estado; // Atualiza o último estado para exibição
            num_expansoes++;

            if (currente_Estado.familas_protegidas >= min_families) {
                solution_found = true;
                break; // Encontrou uma solução, pode parar para essa configuração
            }

            // Gera os sucessores do estado atual e adiciona-os à fila de prioridade
            // Cada sucessor é um novo estado com adiciona um delegação adicional
            vector<Estado> successores = gerar_successores(currente_Estado, map, budget);
            num_geracoes += successores.size();
            for (const auto& successor : successores) {
                pq.push(successor);
            }
        }

        // Imprime o mapa com o último estado ou o estado de solução
        imprime_mapa(map, last_state, budget);

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);

        if (solution_found) {
            cout << "Resultado: Solução encontrada." << endl;
        } else if (duration.count() >= 60000) {
            cout << "Resultado: Não resolvido (tempo excedido)." << endl;
        } else {
            cout << "Resultado: Impossível." << endl;
        }


        cout << "Número de expansões: " << num_expansoes << endl;
        cout << "Número de gerações: " << num_geracoes << endl;
        cout << "Tempo gasto: " << duration.count() / 1000.00 << " segundos." << endl;
        cout << "----------------------------------------" << endl;
    }
}


int main() {
    vector<tuple<int, vector<vector<int>>, int, int, int>> instances = {
            // ID, Mapa, Verba, Min_FamiliasA, Min_FamiliasB
            {1,
                    {{0, 7, 0, 0, 4},
                            {0, 0, 0, 4, 0},
                            {1, 0, 0, 0, 0},
                            {4, 4, 1, 0, 0},
                            {6, 0, 3, 4, 4}},4,19,20,

            },
            {2,
                    {{4, 0, 0, 10, 1},
                            {1, 0, 0, 0, 0},
                            {0, 0, 1, 6, 3},
                            {0, 4, 0, 0, 2},
                            {8, 0, 6, 3, 0}},4,21,22,

            },
            {3,
                    {{0,8,0,4,5,10,0},
                            {0,4,0,7,0,4,0},
                            {0,2,4,2,0,0,2},
                            {0,7,0,1,2,0,0},
                            {2,4,0,0,3,0,2},
                            {0,4,0,0,3,0,0},
                            {2,0,0,0,0,0,0}},8,67,68,
            },
            {4,
                    {{0,0,1,0,7,0,1},
                            {0,1,4,0,0,0,4},
                            {0,0,0,0,2,0,0},
                            {3,1,0,8,5,7,7},
                            {0,4,0,3,0,0,0},
                            {0,0,0,3,2,4,2},
                            {0,8,3,6,3,0,0}},8,59,60,
            },
            {5,
                    {{6,7,2,0,0,0,0,0,0},
                            {3,3,6,0,8,4,3,1,0},
                            {0,0,8,0,0,0,2,4,0},
                            {0,0,0,1,0,3,2,0,0},
                            {0,0,0,7,4,0,1,0,0},
                            {12,8,0,5,4,1,4,3,4},
                            {8,0,1,2,4,3,3,0,0},
                            {1,1,0,0,0,0,5,0,0},
                            {4,0,0,0,4,6,0,13,2}},12,125,126,
            },
            {6,
                    {{0,0,0,0,0,0,0,0,0},
                            {4,0,8,4,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,3,0,0,1,0},
                            {0,3,0,0,0,0,0,0,0},
                            {0,0,0,1,1,0,0,3,0},
                            {0,0,2,4,0,0,0,1,0},
                            {0,2,0,0,8,0,4,3,10},
                            {0,0,3,0,0,4,0,0,0}},12,57,58,
            },
            {7,
                    {{0,0,0,0,0,3,0,0,0,0,0},
                            {0,0,11,2,0,0,9,3,0,0,3},
                            {0,0,0,3,1,0,2,0,0,0,0},
                            {4,1,2,3,0,4,0,0,4,0,0},
                            {5,0,0,0,4,0,1,0,4,3,0},
                            {0,0,0,7,4,0,1,0,0,7,0},
                            {0,8,0,0,0,0,3,0,1,0,3},
                            {0,3,0,0,5,2,3,0,0,0,2},
                            {0,0,0,3,1,0,2,8,0,0,0},
                            {0,3,4,0,7,0,0,7,0,0,0},
                            {4,2,0,4,0,3,0,0,5,7,0}},16,140,142,
            },
            {8,
                    {{1,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,10,10,0,0,0,4,5,0,0},
                            {0,4,1,0,8,0,0,0,0,0,5},
                            {8,0,0,0,0,0,6,0,0,0,0},
                            {0,0,0,0,13,0,0,0,2,0,3},
                            {0,0,0,0,4,0,0,0,0,1,0},
                            {0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,4,0,0,0,0,3,0,0,0},
                            {4,1,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0}},16,93,94,
            },
            {9,
                    {{2,4,0,0,6,7,3,4,0,0,3,0,1},
                            {0,0,2,0,3,0,0,6,0,0,8,11,3},
                            {0,3,0,8,0,0,2,0,0,0,0,0,4},
                            {2,0,0,0,0,0,0,0,0,3,2,0,0},
                            {0,6,0,8,0,3,0,0,0,0,0,0,1},
                            {0,3,0,2,0,0,9,0,0,0,0,5,6},
                            {1,9,4,0,0,2,4,0,0,0,3,2,0},
                            {2,3,0,4,0,0,0,6,2,0,1,0,3},
                            {0,0,0,0,0,6,0,0,0,2,2,0,8},
                            {7,2,4,2,0,0,6,4,1,0,0,0,7},
                            {0,0,0,11,0,0,0,0,3,4,0,9,0},
                            {0,0,0,0,1,4,3,4,0,0,0,3,11},
                            {0,0,4,7,7,0,0,2,0,2,5,0,1}},20,211,212,
            },
            {10,
                    {{0,0,1,4,0,0,9,0,0,0,12,0,1},
                            {0,0,0,0,0,0,0,0,0,1,0,0,0},
                            {1,0,0,0,0,0,2,0,0,2,0,0,0},
                            {0,0,0,0,0,9,4,0,0,0,6,0,0},
                            {0,6,9,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,1,6,10,0,1,4},
                            {0,3,0,0,0,1,0,0,0,0,0,2,0},
                            {0,0,0,1,3,0,0,0,0,9,0,0,0},
                            {9,0,0,3,3,0,0,0,0,3,4,0,0},
                            {0,1,4,0,0,0,0,0,0,5,0,1,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {2,0,0,0,0,3,3,0,0,0,0,0,10},
                            {0,0,0,0,0,0,0,0,0,4,0,0,0}},20,125,126,
            },
    };

    for (const auto& instancia: instances) {
        auto& [instancia_id, map, budget, min_familiasA, min_familiasB] = instancia;
        instancias(instancia_id, map, budget, min_familiasA, min_familiasB);

    }

    return 0;
}
