//
// Created by Vitor Frango on 06/04/2024.
//
#include <iostream>
#include <vector>
#include <queue>
#include <map>

using namespace std;

typedef pair<int, int> pii; // Par de custo, nó

int uniform_cost_search(vector<vector<int>>& graph, int start, int goal) {
    priority_queue<pii, vector<pii>, greater<pii>> frontier; // fila de prioridade minima
    frontier.push({0, start}); // Inicializa com o nó de início e custo 0

    map<int, int> cost_so_far; // Custo acumulado para alcançar cada nó
    cost_so_far[start] = 0;

    while (!frontier.empty()) {
        pii current = frontier.top();
        frontier.pop();

        int current_node = current.second;

        // Objetivo alcançado
        if (current_node == goal) {
            return current.first; // Retorna o custo para alcançar o objetivo
        }

        // Explorar vizinhos
        for (int i = 0; i < graph[current_node].size(); i++) {
            int next = i;
            int new_cost = cost_so_far[current_node] + graph[current_node][i];

            if (cost_so_far.find(next) == cost_so_far.end() || new_cost < cost_so_far[next]) {
                cost_so_far[next] = new_cost;
                frontier.push({new_cost, next});
            }
        }
    }

    return -1; // Retornar -1 se o objetivo não é alcançável
}

int main() {
    // Exemplo de grafo como matriz de adjacência (custos)
    vector<vector<int>> graph = {
            {0, 2, 5},
            {2, 0, 3},
            {5, 3, 0}
    };

    int start = 0; // Nó inicial
    int goal = 2;  // Nó objetivo

    int result = uniform_cost_search(graph, start, goal);
    cout << "Custo para alcançar o objetivo: " << result << endl;

    return 0;
}
