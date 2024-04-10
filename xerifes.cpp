#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <iomanip>
#include <chrono>
#include <numeric>

using namespace std;

int raio_protecao(int deputados) {
    switch (deputados) {
        case 0: return 1;
        case 1: return 2;
        case 5: return 3;
        case 13: return 4;
        default: return 0;
    }
}

struct Estado {
    vector<tuple<int, int, int>> delegacoes;
    int familas_protegidas;
    int cost;

    Estado(vector<tuple<int, int, int>> delegacoes = {}, int familas_protegidas = 0, int cost = 0)  // Updated this line
            : delegacoes(delegacoes), familas_protegidas(familas_protegidas), cost(cost) {}  // Updated this line
};

int calcular_familas_protegidas(const vector<vector<int>>& map, const Estado& estado) {
    int familas_protegidas = 0;
    vector<vector<bool>> protegida_por_delegados(map.size(), vector<bool>(map[0].size(), false));

    for (const auto& delegacao : estado.delegacoes) {
        int x, y, deputados;
        tie(x, y, deputados) = delegacao;
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

vector<Estado> gerar_successores(const Estado& estado_atual, const vector<vector<int>>& map) {
    vector<Estado> sucessores;
    vector<int> deputados_permitidos = {0, 1, 5, 13};

    for (int deputados : deputados_permitidos) {
        for (size_t i = 0; i < map.size(); ++i) {
            for (size_t j = 0; j < map[i].size(); ++j) {
                Estado novo_estado = estado_atual;
                novo_estado.delegacoes.emplace_back(i, j, deputados);
                novo_estado.familas_protegidas = calcular_familas_protegidas(map, novo_estado);
                novo_estado.cost += deputados;  // Assuming the cost is equal to the number of deputies
                sucessores.push_back(novo_estado);
            }
        }
    }

    return sucessores;
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

void instancias(int instancia_id, const vector<vector<int>>& map, int budget, int min_familiasA, int min_familiasB) {
    cout << "----------------------------------------" << endl;
    cout << "Processando a instância ID: " << instancia_id << endl;
    cout << "----------------------------------------" << endl;

    queue<Estado> fila;
    fila.push(Estado());

    while (!fila.empty()) {
        Estado estado_atual = fila.front();
        fila.pop();

        if (estado_atual.familas_protegidas >= min_familiasA || estado_atual.familas_protegidas >= min_familiasB) {
            imprime_mapa(map, estado_atual, budget, 0, 0);  // Updated this line
            break;
        }

        vector<Estado> sucessores = gerar_successores(estado_atual, map);
        for (auto& sucessor : sucessores) {
            fila.push(sucessor);
        }
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
