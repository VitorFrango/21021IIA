//
// Created by Vitor Frango on 06/04/2024.
//
#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <iomanip>
#include <numeric>

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
    // Imprimir o mapa atual com as delegacias e zonas protegidas
    for (size_t i = 0; i < map.size(); ++i) {
        for (size_t j = 0; j < map[i].size(); ++j) {
            // Check for delegation or protection
            bool isDelegation = false;
            for (const auto& delegation : state.delegations) {
                int x, y, deputies;
                tie(x, y, deputies) = delegation;
                int radius = protection_radius(deputies);
                if (i == x && j == y) {
                    cout << " " << deputies << "D" << " ";
                    isDelegation = true;
                    break;
                } else if (i >= x - radius && i <= x + radius && j >= y - radius && j <= y + radius) {
                    cout << " ! ";
                    isDelegation = true;
                    break;
                }
            }
            if (!isDelegation) {
                cout << setw(2) << map[i][j] << " ";
            }
        }
        cout << endl;
    }

    // imprimir informações e estatísticas
    cout << "Famílias protegidas: " << state.protected_families << "/" << calculate_protected_families(map, state) << endl;
    cout << "Custo total: " << state.cost << " moedas de ouro" << endl;
    int remaining_coins = budget - state.cost;
    cout << "Moedas restantes: " << remaining_coins << " moedas de ouro" << endl;
    cout << "Número de deputados: " << accumulate(begin(state.delegations), end(state.delegations), 0,
                                                  [](int acc, const tuple<int, int, int>& delegation) { return acc + get<2>(delegation); }) << endl;
    cout << "Número de delegacias colocadas: " << state.delegations.size() << endl;
}
void process_instance(const vector<vector<int>>& map, int budget, int minimum_protected_families) {
    priority_queue<State, vector<State>, greater<State>> pq;
    pq.push(State());
    while (!pq.empty()) {
        State current_state = pq.top();
        pq.pop();
        if (current_state.cost > budget) {
            break;
        }
        if (current_state.protected_families >= minimum_protected_families) {
            print_map(map, current_state, budget);
            break;
        }
        vector<State> successors = generate_successors(current_state, map, budget);
        for (const auto& successor : successors) {
            pq.push(successor);
        }
    }
}

int main() {
    vector<tuple<vector<vector<int>>, int, int>> instances = {
            // ID 1, N = 5x5
            {{{0, 7, 0, 0, 4}, {0, 0, 0, 4, 0}, {1, 0, 0, 0, 0}, {4, 4, 1, 0, 0}, {6, 0, 3, 4, 4}}, 4, 19},
            {{{0, 7, 0, 0, 4}, {0, 0, 0, 4, 0}, {1, 0, 0, 0, 0}, {4, 4, 1, 0, 0}, {6, 0, 3, 4, 4}}, 4, 20},
            // ID 2, N = 5x5
            {{{4, 0, 0, 10, 1}, {1, 0, 0, 0, 0}, {0, 0, 1, 6, 3}, {0, 4, 0, 0, 2}, {8, 0, 6, 3, 0}}, 4, 21},
            {{{4, 0, 0, 10, 1}, {1, 0, 0, 0, 0}, {0, 0, 1, 6, 3}, {0, 4, 0, 0, 2}, {8, 0, 6, 3, 0}}, 4, 22},
            // ID 3, N = 7x7
            {{{0, 8, 0, 4, 5, 10, 0}, {0, 4, 0, 7, 0, 4, 0}, {0, 2, 4, 2, 0, 0, 2}, {0, 7, 0, 1, 2, 0, 0}, {2, 4, 0, 0, 3, 0, 2}, {0, 4, 0, 0, 3, 0, 0}, {2, 0, 0, 0, 0, 0, 0}}, 8, 67},
            {{{0, 8, 0, 4, 5, 10, 0}, {0, 4, 0, 7, 0, 4, 0}, {0, 2, 4, 2, 0, 0, 2}, {0, 7, 0, 1, 2, 0, 0}, {2, 4, 0, 0, 3, 0, 2}, {0, 4, 0, 0, 3, 0, 0}, {2, 0, 0, 0, 0, 0, 0}}, 8, 68},
            // ID 4, N = 7x7
            {{{0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}}, 8, 59},
            {{{0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}}, 8, 60},
            // ID 5, N= 8x8
            {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}, 12, 125},
            {{{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}}, 12, 126},
            // ID 6, N = 9x9
            {{{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}}, 12, 57},
            {{{0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0}}, 12, 58}
    };

    // declaração estado
    State state;

    // declaração de mapa
    vector<vector<int>> map {
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

    // Declare 'minimum_protected_families' or use 'calculate_protected_families' function
    int minimum_protected_families = calculate_protected_families(map, state);
    cout << "Famílias protegidas: " << state.protected_families << "/" << minimum_protected_families << endl;

    // Use 'greater<State>' template
    priority_queue<State, vector<State>, greater<State>> pq;

    for (auto& instance : instances) {
        auto& [map, budget, min_families] = instance;
        cout << "Processando a instância com orçamento " << budget << " e mínimo de famílias " << min_families << " " << endl;
        process_instance(map, budget, min_families);
    }

    return 0;
}
