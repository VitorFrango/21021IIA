//
// Created by Vitor Frango on 06/04/2024.
//
// opt/homebrew/Cellar/libvorbis/1.3.7
// g++ /Users/vitorfrango/Library/CloudStorage/OneDrive-Personal/programacao/21071IIA/1802925EFolioA.cpp -std=c++11 -I /opt/homebrew/Cellar/libvorbis/1.3.7/include -L /opt/homebrew/Cellar/libvorbis/1.3.7/lib -lsfml-graphics -lsfml-window -lsfml-system -o 1802925EFolioA

#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <iomanip>

using namespace std;

// Calcular o raio de proteção com base no número de deputados
int protection_radius(int deputies) {
    switch (deputies) {
        case 0: return 1; // 3x3 zones
        case 1: return 2; // 5x5 zones
        case 5: return 3; // 7x7 zones
        case 13: return 4; // 9x9 zones
        default: return 0;
    }
}

// Estrutura para representar um estado
struct State {
    vector<tuple<int, int, int>> delegations; // Posição X, Posição Y, Deputados
    int cost;
    int protected_families;

    State(vector<tuple<int, int, int>> delegations = {}, int cost = 0, int protected_families = 0)
    : delegations(delegations), cost(cost), protected_families(protected_families) {}

    bool operator>(const State& other) const {
        return cost > other.cost;
    }
};

// Calcular famílias protegidas para um determinado estado
int calculate_protected_families(const vector<vector<int>>& map, const State& state) {
    int protected_families = 0;
    vector<vector<bool>> protected_by_delegation(map.size(), vector<bool>(map[0].size(), false));

    for (const auto& delegation : state.delegations) {
        int x, y, deputies;
        tie(x, y, deputies) = delegation;
        int radius = protection_radius(deputies);

        for (int i = max(0, x - radius); i <= min(static_cast<int>(map.size()) - 1, x + radius); ++i) {
            for (int j = max(0, y - radius); j <= min(static_cast<int>(map[0].size()) - 1, y + radius); ++j) {
                if (!protected_by_delegation[i][j]) {
                    protected_families += map[i][j];
                    protected_by_delegation[i][j] = true;
                }
            }
        }
    }
    return protected_families;
}

// Gerar estados sucessores
vector<State> generate_successors(const State& current_state, const vector<vector<int>>& map, int budget) {
    vector<State> successors;
    vector<int> allowed_deputies = {0, 1, 5, 13};

    for (int deputies : allowed_deputies) {
        int cost_of_delegation = 4 + deputies; // 4 for the delegation and 1 for each deputy
        if (current_state.cost + cost_of_delegation <= budget) {
            for (size_t i = 0; i < map.size(); ++i) {
                for (size_t j = 0; j < map[i].size(); ++j) {
                    State new_state = current_state;
                    new_state.delegations.emplace_back(i, j, deputies);
                    new_state.cost += cost_of_delegation;
                    new_state.protected_families = calculate_protected_families(map, new_state);
                    if (new_state.protected_families >= current_state.protected_families) {
                        successors.push_back(new_state);
                    }
                }
            }
        }
    }

    return successors;
}

void print_map(const vector<vector<int>>& map, const State& state, int budget) {
    // Criar uma cópia do mapa para marcar áreas protegidas e delegacias
    vector<vector<string>> visual_map(map.size(), vector<string>(map[0].size()));
    for (size_t i = 0; i < map.size(); ++i) {
        for (size_t j = 0; j < map[i].size(); ++j) {
            visual_map[i][j] = to_string(map[i][j]);
        }
    }

    // Marcar áreas protegidas com 'P' e delegacias com 'D'
    for (const auto& delegation : state.delegations) {
        int x, y, deputies;
        tie(x, y, deputies) = delegation;
        int radius = protection_radius(deputies);
        visual_map[x][y] = "D" + to_string(deputies); // Mark the delegation
        for (int i = max(0, x - radius); i <= min(static_cast<int>(map.size()) - 1, x + radius); ++i) {
            for (int j = max(0, y - radius); j <= min(static_cast<int>(map[0].size()) - 1, y + radius); ++j) {
                if (visual_map[i][j][0] != 'D') {
                    visual_map[i][j] = "P"; // Mark the protected area
                }
            }
        }
    }

    // Print the visual map
    for (const auto& row : visual_map) {
        for (const auto& cell : row) {
            cout << setw(2) << cell << " ";
        }
        cout << endl;
    }
    // Imprimir as estatísticas abaixo do mapa
    cout << "Famílias protegidas: " << state.protected_families << "/57" << endl;
    cout << "Custo total: " << state.cost << " moedas de ouro" << endl;
    int remaining_coins = budget - state.cost;
    cout << "Moedas restantes: " << remaining_coins << " moedas de ouro" << endl;
    cout << "Número de delegacias colocadas: " << state.delegations.size() << endl;
    int total_deputies = 0;
    for (const auto& delegation : state.delegations) {
        total_deputies += get<2>(delegation);
    }
    cout << "Número de deputados: " << total_deputies << endl;
}

int main() {
    // Mapa de exemplo da instância do problema ID 6
    vector<vector<int>> map = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {4, 0, 8, 4, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 3, 0, 0, 1, 0},
            {0, 3, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 1, 0, 0, 3, 0},
            {0, 0, 2, 4, 0, 0, 0, 1, 0},
            {0, 2, 0, 0, 8, 0, 4, 3, 10},
            {0, 0, 3, 0, 0, 4, 0, 0, 0},
    };

    // Orçamento e o número mínimo de famílias a proteger para este exemplo

    int minimum_protected_families = 57;
    int budget = 12;

    // Iniciar a busca com o estado inicial
    State initial_state;
    queue<State> search_queue;
    search_queue.push(initial_state);

    // Espaço reservado para a solução final
    State solution;

    // Realizar a busca cega
    while (!search_queue.empty()) {
        State current_state = search_queue.front();
        search_queue.pop();

        if (current_state.protected_families >= minimum_protected_families && current_state.cost <= budget) {
            solution = current_state;
            break;
        }

        vector<State> successors = generate_successors(current_state, map, budget);
        for (const auto& successor : successors) {
            search_queue.push(successor);
        }
    }

    // Saída da solução
    if (!solution.delegations.empty()) {
        cout << "Solução encontrada!" << endl;
        cout << "Delegacias:" << endl;
        for (const auto& delegation : solution.delegations) {
            cout << "(" << get<0>(delegation) << ", " << get<1>(delegation) << ") with " << get<2>(delegation) << " deputados" << endl;
        }
        cout << "Custo: " << solution.cost << " moedas" << endl;
        cout << "Familias protegidass: " << solution.protected_families << "/" << minimum_protected_families << endl;
        print_map(map, solution, budget);
    } else {
        cout << "Solução não encontrada." << endl;
    }

    return 0;
}
