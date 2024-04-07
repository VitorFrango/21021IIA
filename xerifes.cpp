#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <array>

using namespace std;

const int LARGURA_MAXIMA = 20;
const int NUM_INSTANCIAS = 10;

struct Estado {
    vector<vector<char>> mapaProtecao;
    vector<vector<bool>> protegido;
    int verbaRestante;
    int familiasProtegidas;
};

struct Protecao {
    vector<vector<char>> mapaProtecao;
    int familiasProtegidas;
    int verbaRestante;
};

struct Comparador {
    bool operator()(const Estado& a, const Estado& b) {
        return a.verbaRestante > b.verbaRestante;
    }
};

struct Instancia {
    int N, M, Verba, ProtegerA, ProtegerB;
    vector<vector<int>> territorio;
};

std::vector<Instancia> instancias = {
        {5, 5, 10, 15, 20, {{0, 2, 0, 3, 0}, {2, 1, 0, 1, 2}, {0, 0, 0, 2, 0}, {3, 1, 2, 0, 1}, {0, 1, 0, 3, 0}}},
        {6, 6, 12, 20, 25, {{1, 0, 2, 0, 1, 3}, {0, 2, 0, 1, 0, 2}, {2, 0, 3, 2, 0, 1}, {0, 1, 0, 0, 2, 0}, {1, 0, 2, 1, 0, 3}, {0, 2, 0, 3, 1, 0}}},
        {7, 7, 15, 25, 30, {{0, 2, 0, 3, 0, 1, 0}, {2, 1, 0, 1, 2, 0, 2}, {0, 0, 0, 2, 0, 1, 0}, {3, 1, 2, 0, 1, 0, 3}, {0, 1, 0, 3, 0, 2, 0}, {1, 0, 1, 0, 2, 0, 1}, {0, 2, 0, 3, 0, 1, 0}}},
        {8, 8, 20, 30, 35, {{0, 2, 0, 3, 0, 1, 0, 2}, {2, 1, 0, 1, 2, 0, 2, 1}, {0, 0, 0, 2, 0, 1, 0, 3}, {3, 1, 2, 0, 1, 0, 3, 0}, {0, 1, 0, 3, 0, 2, 0, 1}, {1, 0, 1, 0, 2, 0, 1, 0}, {0, 2, 0, 3, 0, 1, 0, 2}, {2, 1, 0, 1, 2, 0, 2, 1}}},
        {9, 9, 25, 35, 40, {{0, 2, 0, 3, 0, 1, 0, 2, 0}, {2, 1, 0, 1, 2, 0, 2, 1, 0}, {0, 0, 0, 2, 0, 1, 0, 3, 0}, {3, 1, 2, 0, 1, 0, 3, 0, 1}, {0, 1}}} ,

        };

        class CMapa {
            public:
            static int calcularRaio(int deputados) {
                switch (deputados) {
                    case 0:
                        return 1;
                    case 1:
                        return 2;
                    case 5:
                        return 3;
                    case 13:
                        return 4;
                    default:
                        return 1;
                }
            }

            static Estado expandirEstado(const Estado &estado, const Instancia &instancia, int x, int y) {
                Estado novoEstado = estado;
                if (instancia.territorio[x][y] > 0 && !estado.protegido[x][y]) {
                    int maxDeputadosPossiveis = min(novoEstado.verbaRestante - 4, 13);
                    int deputados = maxDeputadosPossiveis >= 5 ? (maxDeputadosPossiveis >= 13 ? 13 : 5) : 1;
                    novoEstado.verbaRestante -= (4 + deputados);
                    int raio = calcularRaio(deputados);

                    for (int i = max(0, x - raio); i <= min(instancia.N - 1, x + raio); i++) {
                        for (int j = max(0, y - raio); j <= min(instancia.M - 1, y + raio); j++) {
                            if (!novoEstado.protegido[i][j] && instancia.territorio[i][j] > 0) {
                                novoEstado.mapaProtecao[i][j] = 'P';
                                novoEstado.protegido[i][j] = true;
                                novoEstado.familiasProtegidas += instancia.territorio[i][j];
                            }
                        }
                    }
                    novoEstado.mapaProtecao[x][y] = 'D';
                }
                return novoEstado;
            }

            static Protecao calculaProtecaoUniforme(const Instancia &instancia) {
                priority_queue<Estado, vector<Estado>, Comparador> fronteira;
                Estado inicial{vector<vector<char>>(instancia.N, vector<char>(instancia.M, ' ')),
                               vector<vector<bool>>(instancia.N, vector<bool>(instancia.M, false)),
                               instancia.Verba, 0};

                fronteira.push(inicial);
                while (!fronteira.empty()) {
                    Estado atual = fronteira.top();
                    fronteira.pop();
                    if (atual.familiasProtegidas >= instancia.ProtegerA) {
                        return {atual.mapaProtecao, atual.familiasProtegidas, atual.verbaRestante};
                    }

                    for (int i = 0; i < instancia.N; i++) {
                        for (int j = 0; j < instancia.M; j++) {
                            if (instancia.territorio[i][j] > 0 && !atual.protegido[i][j] && atual.verbaRestante >= 4) {
                                Estado novoEstado = expandirEstado(atual, instancia, i, j);
                                if (novoEstado.verbaRestante >= 0) {
                                    fronteira.push(novoEstado);
                                }
                            }
                        }
                    }
                }
                return {inicial.mapaProtecao, inicial.familiasProtegidas, inicial.verbaRestante};
            }
        };

        int main() {

            int id = 1; // Escolher a instância
            if (id > 0 && id <= instancias.size()) {
                cout << "Processando instância " << id << "..." << endl;
                Protecao protecao = CMapa::calculaProtecaoUniforme(instancias[id - 1]);

                cout << "-- Mapa de Proteção --" << endl;
                for (int i = 0; i < instancias[id - 1].N; ++i) {
                    for (int j = 0; j < instancias[id - 1].M; ++j) {
                        if (protecao.mapaProtecao[i][j] == 'D') {
                            cout << setw(3) << 'D';
                        } else if (protecao.mapaProtecao[i][j] == 'P') {
                            cout << setw(3) << 'P';
                        } else {
                            cout << setw(3) << '.';
                        }
                    }
                    cout << endl;
                }
                cout << "Famílias Protegidas: " << protecao.familiasProtegidas << endl;
                cout << "Verba Restante: " << protecao.verbaRestante << " moedas" << endl;
            } else {
                cout << "ID da instância inválido." << endl;
            }
            return 0;
        }

