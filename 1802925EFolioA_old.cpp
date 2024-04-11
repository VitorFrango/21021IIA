/*
// Created by Vitor Frango on 06/04/2024.

 O código implementa um algoritmo de busca de custo uniforme. Esse algoritmo expande, no espaço de busca,
 o nó de menor custo de caminho desde a origem, o que garante que o primeiro nó a atingir o objetivo seja
 o de caminho mais curto (ou de menor custo), caso exista um caminho até o objetivo. É um algoritmo completo
 e otimizado
*/


#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <iomanip>
#include <numeric>
#include <chrono>

using namespace std;

// Função para calcular o raio de proteção de acordo com o número de deputados
int raio_protecao(int deputados) {
    switch (deputados) {
        case 0: return 1;
        case 1: return 2;
        case 5: return 3;
        case 13: return 4;
        default: return 0;
    }
}

// Estrutura para representar o estado do problema
struct Estado {
    vector<tuple<int, int, int>> delegacoes;
    int cost;
    int familas_protegidas;

    Estado(vector<tuple<int, int, int>> delegacoes = {}, int cost = 0, int familas_protegidas = 0)
            : delegacoes(delegacoes), cost(cost), familas_protegidas(familas_protegidas) {}

    bool operator>(const Estado& other) const {
        return cost > other.cost;
    }
};

// Função para calcular o número de famílias protegidas por delegações
int calcular_familas_protegidas(const vector<vector<int>>& map, const Estado& Estado) {
    int familas_protegidas = 0;
    vector<vector<bool>> protegida_por_delegados(map.size(), vector<bool>(map[0].size(), false));

    // Itera sobre as delegações e calcula o número de famílias protegidas
    for (const auto& Delegacao : Estado.delegacoes) {
        int x, y, deputados;
        tie(x, y, deputados) = Delegacao;
        int radius = raio_protecao(deputados);

        // Calcula o número de famílias protegidas por cada delegação
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

// Função para gerar os sucessores do estado atual do problema
vector<Estado> gerar_successores(const Estado& currente_Estado, const vector<vector<int>>& map, int budget) {
    vector<Estado> successores;
    vector<int> deputados_permitidos = {0, 1, 5, 13};

    // Gera os sucessores para cada número de deputados permitidos
    for (int deputados : deputados_permitidos) {
        int custo_da_delegacao = 4 + deputados;
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
void imprime_mapa(const vector<vector<int>>& map, const Estado& Estado, int budget, int num_expansoes, int num_geracoes) {

    for (size_t i = 0; i < map.size(); ++i) {
        for (size_t j = 0; j < map[i].size(); ++j) {
            bool isDelegacao = false;
            bool isProtegida = false;

            for (const auto& Delegacao : Estado.delegacoes) {
                int x, y, deputados;
                tie(x, y, deputados) = Delegacao;  // obtem as coordenadas e o número de deputados
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
                                   [](int acc, const tuple<int, int, int>& Delegacao) {
                                       return acc + get<2>(Delegacao); });
    cout << "Número de deputados: " << num_deputados << endl;
    cout << "Número de delegacias colocadas: " << Estado.delegacoes.size() << endl;
}

// Função para processar as instâncias do problema tentando encontrar uma solução dentro do tempo limite
void instancias(int instancia_id, const vector<vector<int>>& map, int budget, int min_familiasA, int min_familiasB) {
    using namespace std::chrono;
    auto start = high_resolution_clock::now();

    cout << "----------------------------------------" << endl;
    cout << "Processando a instância ID: " << instancia_id << endl;
    cout << "Verba disponível: " << budget << " moedas de ouro" << endl;
    cout << " D -> Delegado colocado \n X -> Família protegida\n";
    cout << "----------------------------------------" << endl;

    // Estrutura para armazenar o estado do último estado processado
    Estado last_state;
    for (int min_families : {min_familiasA, min_familiasB}) {
        bool solution_found = false;
        int num_expansoes = 0;
        int num_geracoes = 0;

        queue<Estado> q;
        q.push(Estado());

        while (!q.empty()) {
            auto current_duration = duration_cast<microseconds>(high_resolution_clock::now() - start);
            if (current_duration.count() > 60000000) break;

            Estado currente_Estado = q.front();
            q.pop();
            last_state = currente_Estado;
            num_expansoes++;

            if (currente_Estado.familas_protegidas >= min_families) {
                solution_found = true;
                break;
            }

            vector<Estado> successores = gerar_successores(currente_Estado, map, budget);
            num_geracoes += successores.size();
            for (const auto& successor : successores) {
                q.push(successor);
            }
        }

        imprime_mapa(map, last_state, budget, num_expansoes, num_geracoes);
        // Calcula o tempo gasto em microssegundos para processar a instância
        auto duration = duration_cast<microseconds>(high_resolution_clock::now() - start);

        // Detalhes adicionais sobre o resultado
        cout << "Minimo de familias protegidas necessárias: " << min_families << endl;
        cout << "Famílias protegidas alcançadas: " << last_state.familas_protegidas << endl;
        cout << "Custo total: " << last_state.cost << " moedas de ouro" << endl;
        cout << "Moedas restantes: " << (budget - last_state.cost) << " moedas de ouro" << endl;
        // Calcula o número de deputados e delegacias colocadas
        int num_deputados = accumulate(begin(last_state.delegacoes), end(last_state.delegacoes), 0,
                                       [](int acc, const tuple<int, int, int>& Delegacao) {
                                           return acc + get<2>(Delegacao);
                                       });
        cout << "Número de deputados: " << num_deputados << endl;
        cout << "Número de delegacias colocadas: " << last_state.delegacoes.size() << endl;

        if (solution_found) {
            cout << "Resultado: Solução encontrada." << endl;
        } else if (duration.count() > 60000) { // define o tempo limite de 60 segundos
            cout << "Resultado: Não resolvido (tempo excedido)." << endl;
        } else {
            cout << "Resultado: Impossível." << endl;
        }

        cout << "Número de expansões: " << num_expansoes << endl;
        cout << "Número de gerações: " << num_geracoes << endl;
        // Imprime o tempo gasto em microssegundos e converte para segundos
        cout << "Tempo gasto: " << fixed << setprecision(6) << duration.count() / 1000000.0 << " segundos." << endl;
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

    // Estrutura para armazenar os resultados de cada instância para a tabela
    struct Resultado {
        int id;
        string estado;
        int familias_protegidas;
        int custo;
        int moedas_restantes;
        int num_deputados;
        int delegacias_colocadas;
    };

    // Vetor para armazenar os resultados de todas as instâncias
    vector<Resultado> resultados;

    // Processa cada instância
    for (const auto& instancia: instances) {
        auto& [instancia_id, map, budget, min_familiasA, min_familiasB] = instancia;
        instancias(instancia_id, map, budget, min_familiasA, min_familiasB);
    }

    return 0;
}