#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <iomanip>
#include <numeric>
#include <chrono>

using namespace std;

class Estado {
public:
    vector<tuple<int, int, int>> delegacoes;
    int custo;
    int familiasProtegidas;

    Estado(vector<tuple<int, int, int>> delegacoes = {}, int custo = 0, int familiasProtegidas = 0)
            : delegacoes(delegacoes), custo(custo), familiasProtegidas(familiasProtegidas) {}

    bool operator>(const Estado& outro) const {
        return custo > outro.custo;
    }
};

class Buscador {
private:
    vector<vector<int>> mapa;
    int orcamento;

    int calcularRaioProtecao(int numDeputados) {
        switch (numDeputados) {
            case 0: return 1;
            case 1: return 2;
            case 5: return 3;
            case 13: return 4;
            default: return 0;
        }
    }

    int calcularFamiliasProtegidas(const Estado& estado) {
        int familiasProtegidas = 0;
        vector<vector<bool>> zonaProtegida(mapa.size(), vector<bool>(mapa[0].size(), false));

        for (const auto& delegacao : estado.delegacoes) {
            int x, y, deputados;
            tie(x, y, deputados) = delegacao;
            int raio = calcularRaioProtecao(deputados);

            for (int i = max(0, x - raio); i <= min(static_cast<int>(mapa.size()) - 1, x + raio); ++i) {
                for (int j = max(0, y - raio); j <= min(static_cast<int>(mapa[0].size()) - 1, y + raio); ++j) {
                    if (!zonaProtegida[i][j]) {
                        familiasProtegidas += mapa[i][j];
                        zonaProtegida[i][j] = true;
                    }
                }
            }
        }
        return familiasProtegidas;
    }

public:
    Buscador(const vector<vector<int>>& mapa, int orcamento) : mapa(mapa), orcamento(orcamento) {}

    vector<Estado> gerarSucessores(const Estado& estadoAtual) {
        vector<Estado> sucessores;
        vector<int> deputadosPermitidos = {0, 1, 5, 13};

        for (int numDeputados : deputadosPermitidos) {
            int custoDelegacao = 4 + numDeputados;
            if (estadoAtual.custo + custoDelegacao <= orcamento) {
                for (size_t i = 0; i < mapa.size(); ++i) {
                    for (size_t j = 0; j < mapa[i].size(); ++j) {
                        Estado novoEstado = estadoAtual;
                        novoEstado.delegacoes.emplace_back(i, j, numDeputados);
                        novoEstado.custo += custoDelegacao;
                        novoEstado.familiasProtegidas = calcularFamiliasProtegidas(novoEstado);
                        if (novoEstado.familiasProtegidas >= estadoAtual.familiasProtegidas) {
                            sucessores.push_back(novoEstado);
                        }
                    }
                }
            }
        }
        return sucessores;
    }

    void imprimirMapa(const Estado& estado) {
        for (size_t i = 0; i < mapa.size(); ++i) {
            for (size_t j = 0; j < mapa[i].size(); ++j) {
                bool isDelegacao = false;
                bool isProtegida = false;

                for (const auto& delegacao : estado.delegacoes) {
                    int x, y, deputados;
                    tie(x, y, deputados) = delegacao;
                    int raio = calcularRaioProtecao(deputados);

                    if (i >= x - raio && i <= x + raio && j >= y - raio && j <= y + raio) {
                        isProtegida = true;
                    }

                    if (i == x && j == y) {
                        isDelegacao = true;
                        cout << " " << deputados << "D"; // delegacao com deputados e sem protecao
                        break;
                    }
                }

                if (!isDelegacao) {
                    cout << setw(2) << mapa[i][j]; // mostrar o numero de familias
                    if (isProtegida) {
                        cout << "\033[1;31mX\033[0m"; // adiciona "X" vermelho se Protegida
                    } else {
                        cout << " "; // mantem "  " se nao Protegida
                    }
                }
            }
            cout << endl;
        }

        cout << "\nFamílias protegidas: " << estado.familiasProtegidas << "/" << calcularFamiliasProtegidas(estado) << endl;
        cout << "Custo total: " << estado.custo << " moedas de ouro" << endl;
        cout << "Moedas restantes: " << orcamento - estado.custo << " moedas de ouro" << endl;
        int num_deputados = accumulate(begin(estado.delegacoes), end(estado.delegacoes), 0,
                                       [](int acc, const tuple<int, int, int>& delegacao) {
                                           return acc + get<2>(delegacao);
                                       });
        cout << "Número de deputados: " << num_deputados << endl;
        cout << "Número de delegações colocadas: " << estado.delegacoes.size() << endl;
    }

    void processarInstancias(int idInstancia, int minFamiliasA, int minFamiliasB) {
        using namespace std::chrono;
        auto start = high_resolution_clock::now();

        cout << "----------------------------------------" << endl;
        cout << "Processando a instância ID: " << idInstancia << endl;
        cout << "Verba disponível: " << orcamento << " moedas de ouro" << endl;
        cout << "D -> Delegado colocado \nX -> Família protegida\n";
        cout << "----------------------------------------" << endl;

        Estado last_state;
        for (int min_families : {minFamiliasA, minFamiliasB}) {
            bool solution_found = false;
            int num_expansoes = 0;
            int num_geracoes = 0;

            priority_queue<Estado, vector<Estado>, greater<Estado>> pq;
            pq.push(Estado());

            while (!pq.empty()) {
                auto current_duration = duration_cast<milliseconds>(high_resolution_clock::now() - start);
                if (current_duration.count() > 59590) {
                    break;
                }

                Estado current_estado = pq.top();
                pq.pop();
                last_state = current_estado;
                num_expansoes++;

                if (current_estado.familiasProtegidas >= min_families) {
                    solution_found = true;
                    break;
                }

                vector<Estado> sucessores = gerarSucessores(current_estado);
                num_geracoes += sucessores.size();
                for (const auto& sucessor : sucessores) {
                    pq.push(sucessor);
                }
            }

            imprimirMapa(last_state);

            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(stop - start);

            cout << "Resultado: " << (solution_found ? "Solução encontrada." : "Não resolvido.") << endl;
            cout << "Número de expansões: " << num_expansoes << endl;
            cout << "Número de gerações: " << num_geracoes << endl;
            cout << "Tempo gasto: " << duration.count() / 1000.0 << " segundos." << endl;
            cout << "----------------------------------------" << endl;
        }
    }
};

int main() {
    vector<vector<int>> mapa = {
            {0, 2, 3},
            {2, 1, 4},
            {3, 0, 1}
    };
    int orcamento = 10;
    Buscador buscador(mapa, orcamento);
    buscador.processarInstancias(1, 5, 6);

    return 0;
}
