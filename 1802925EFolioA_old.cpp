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
#include <numeric>

using namespace std;

int protection_radius(int deputies) {
    switch (deputies) {
        case 0: return 1; // 3x3 zones
        case 1: return 2; // 5x5 zones
        case 5: return 3; // 7x7 zones
        case 13: return 4; // 9x9 zones
        default: return 0;
    }
}

struct State {
    vector<tuple<int, int, int>> delegations;
    int cost;
    int protected_families;

    State(vector<tuple<int, int, int>> delegations = {}, int cost = 0, int protected_families = 0)
            : delegations(delegations), cost(cost), protected_families(protected_families) {}

    bool operator>(const State& other) const {
        return cost > other.cost;
    }
};

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
    for (size_t i = 0; i < map.size(); ++i) {
        for (size_t j = 0; j < map[i].size(); ++j) {
            bool isDelegation = false;
            bool isProtected = false;

            for (const auto& delegation : state.delegations) {
                int x, y, deputies;
                tie(x, y, deputies) = delegation;
                int radius = protection_radius(deputies);

                if (i == x && j == y) {
                    cout << " " << deputies << "D ";
                    isDelegation = true;
                    break;
                }

                if (i >= x - radius && i <= x + radius && j >= y - radius && j <= y + radius) {
                    isProtected = true;
                }
            }

            if (isProtected && !isDelegation) {
                cout << " ! ";
            } else if (!isDelegation) {
                cout << setw(2) << map[i][j] << " ";
            }
        }
        cout << endl;
    }

    cout << "Famílias protegidas: " << state.protected_families << "/" << calculate_protected_families(map, state) << endl;
    cout << "Custo total: " << state.cost << " moedas de ouro" << endl;
    int remaining_coins = budget - state.cost;
    cout << "Moedas restantes: " << remaining_coins << " moedas de ouro" << endl;
    cout << "Número de deputados: " << accumulate(begin(state.delegations), end(state.delegations), 0,
                                                  [](int acc, const tuple<int, int, int>& delegation) { return acc + get<2>(delegation); }) << endl;
    cout << "Número de delegacias colocadas: " << state.delegations.size() << endl;
}

void process_instance(int instance_id, const vector<vector<int>>& map, int budget, int min_familiesA, int min_familiesB) {
    cout << "Processando a instância ID: " << instance_id << endl;
    // Process each case: A and B for the given instance
    for (int min_families : {min_familiesA, min_familiesB}) {
        priority_queue<State, vector<State>, greater<State>> pq;
        pq.push(State());
        while (!pq.empty()) {
            State current_state = pq.top();
            pq.pop();

            if (current_state.protected_families >= min_families) {
                print_map(map, current_state, budget);
                break;
            }

            vector<State> successors = generate_successors(current_state, map, budget);
            for (const auto& successor : successors) {
                pq.push(successor);
            }
        }
    }
}

int main() {
    vector<tuple<int, vector<vector<int>>, int, int, int>> instances = {


            // ID 1, N = 5x5
            {1, {{1, 2, 3, 2, 1}, {2, 0, 4, 0, 2}, {3, 4, 5, 4, 3}, {2, 0, 4, 0, 2}, {1, 2, 3, 2, 1}}, 4, 19, 20},
            // ID 2, N = 5x5
            {2, {{0, 1, 2, 1, 0}, {1, 3, 4, 3, 1}, {2, 4, 0, 4, 2}, {1, 3, 4, 3, 1}, {0, 1, 2, 1, 0}}, 4, 21, 22},
            // ID 3, N = 7x7
            {3, {{1, 0, 2, 0, 1, 0, 2}, {0, 3, 0, 4, 0, 3, 0}, {2, 0, 5, 0, 2, 0, 1}, {0, 4, 0, 6, 0, 4, 0}, {1, 0, 2, 0, 3, 0, 2}, {0, 3, 0, 4, 0, 1, 0}, {2, 1, 0, 2, 0, 2, 1}}, 8, 67, 68},
            // ID 4, N = 7x7
            {4, {{2, 1, 0, 1, 2, 1, 0}, {1, 0, 3, 0, 1, 0, 3}, {0, 3, 1, 4, 1, 3, 0}, {1, 0, 4, 0, 4, 0, 1}, {2, 1, 1, 4, 1, 1, 2}, {0, 3, 0, 1, 0, 3, 0}, {1, 2, 0, 2, 0, 2, 1}}, 8, 59, 60},
            // ID 5, N = 9x9
            {5, {{0, 1, 0, 2, 1, 0, 1, 0, 2}, {1, 0, 3, 0, 2, 3, 0, 1, 0}, {0, 3, 0, 1, 4, 1, 0, 3, 0}, {2, 0, 1, 3, 0, 3, 1, 0, 2}, {1, 4, 0, 0, 5, 0, 0, 4, 1}, {0, 2, 3, 0, 0, 3, 2, 0, 1}, {1, 0, 0, 3, 0, 1, 0, 0, 2}, {0, 1, 2, 0, 4, 0, 2, 1, 0}, {2, 0, 1, 3, 1, 3, 0, 1, 2}}, 12, 125, 126},
            // ID 6, N = 9x9
            {6, {{1, 0, 2, 0, 1, 0, 2, 0, 1}, {0, 3, 0, 4, 0, 3, 0, 4, 0}, {2, 0, 5, 0, 2, 0, 5, 0, 2}, {0, 4, 0, 6, 0, 4, 0, 6, 0}, {1, 0, 2, 0, 3, 0, 2, 0, 1}, {0, 3, 0, 4, 0, 1, 0, 4, 0}, {2, 0, 1, 0, 2, 0, 1, 0, 2}, {0, 1, 0, 2, 0, 1, 0, 2, 0}, {1, 2, 0, 2, 1, 2, 0, 2, 1}}, 12, 57, 58},
            // ID 7. N = 10x10
            {7, {{0, 1, 0, 2, 0, 1, 0, 2, 0, 1}, {1, 0, 3, 0, 2, 0, 3, 0, 2, 0}, {0, 3, 0, 1, 0, 4, 0, 1, 0, 3}, {2, 0, 1, 3, 0, 3, 1, 0, 2, 0}, {1, 2, 0, 0, 5, 0, 0, 4, 0, 1}, {0, 3, 4, 0, 0, 0, 0, 3, 0, 0}, {1, 0, 0, 3, 0, 1, 0, 0, 2, 0}, {0, 1, 2, 0, 4, 0, 2, 1, 0, 0}, {2, 0, 1, 3, 1, 3, 0, 1, 2, 0}, {0, 1, 0, 2, 0, 1, 0, 2, 0, 1}}, 12, 57, 58},
    };

    for (auto& instance : instances) {
        auto& [instance_id, map, budget, min_familiesA, min_familiesB] = instance;
        process_instance(instance_id, map, budget, min_familiesA, min_familiesB);
    }

    return 0;
}
// Created by Vitor Frango on 06/04/2024.