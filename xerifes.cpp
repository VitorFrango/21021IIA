//
// Created by Vitor Frango on 06/04/2024.
//
#include <iostream>
#include <array>
#include <iomanip>

const int LARGURA_MAXIMA = 20;
const int NUM_INSTANCIAS = 10;
const int VERBA_DISPONIVEL = 12;

struct Instancia {
    int N, M, Verba, ProtegerA, ProtegerB;
};

const Instancia instancias[NUM_INSTANCIAS] = {
        {5, 5, 4, 19, 20},
        {5, 5, 4, 21, 22},
        {7, 7, 8, 67, 68},
        {7, 7, 8, 59, 60},
        {9, 9, 12, 125, 126},
        {9,9, 12, 57, 58},
        {11, 11, 16, 140, 141},
        {11, 11, 16, 93, 94},
        {13, 13, 20, 211, 212},
        {13, 13, 20, 125, 126}
};

const std::array<std::array<std::array<int, LARGURA_MAXIMA>, LARGURA_MAXIMA>, NUM_INSTANCIAS> mapas = {{
                                                                                                               // Inicialização dos mapas...
                                                                                                               // Exemplo para as primeiras duas instâncias
                                                                                                               {
                                                                                                                       {{0, 7, 0, 0, 4},
                                                                                                                        {0, 0, 0, 4, 0},
                                                                                                                        {1, 0, 0, 0, 0},
                                                                                                                        {4, 4, 1, 0, 0},
                                                                                                                        {6, 0, 3, 4, 4}},
                                                                                                                       // Resto preenchido com zeros
                                                                                                               },
                                                                                                               {
                                                                                                                       {{4, 0, 0, 10, 1},
                                                                                                                        {1, 0, 0, 0, 0},
                                                                                                                        {0, 0, 1, 6, 3},
                                                                                                                        {0, 4, 0, 0, 2}, {8, 0, 6, 3, 0}},
                                                                                                                       // Resto preenchido com zeros
                                                                                                               },
                                                                                                               // ... Complete com os outros mapas
                                                                                                       }};

class CMapa {
    int mapa[LARGURA_MAXIMA][LARGURA_MAXIMA];
    bool protecao[LARGURA_MAXIMA][LARGURA_MAXIMA];  // Nova matriz para rastrear proteção
    Instancia instancia;

public:
    CMapa(int id) : instancia(instancias[id - 1]) {
        inicializarMapa(id - 1);
    }

    void inicializarMapa(int id) {
        for (int i = 0; i < LARGURA_MAXIMA; i++) {
            for (int j = 0; j < LARGURA_MAXIMA; j++) {
                protecao[i][j] = false;  // Inicializa a matriz de proteção
                if (i < instancia.N && j < instancia.M) {
                    mapa[i][j] = mapas[id][i][j];
                } else {
                    mapa[i][j] = 0;
                }
            }
        }
    }

    // Método para calcular o raio de proteção
    int calcularRaio(int deputados) {
        switch (deputados) {
            case 0: return 1;
            case 1: return 2;
            case 5: return 3;
            case 13: return 4;
            default: return 1; // caso padrão para apenas o xerife
        }
    }

    // Método para calcular e imprimir as informações e o mapa
    void calcularEImprimir(int x, int y, int deputados) {
        int raio = calcularRaio(deputados);
        int familiasProtegidas = 0; // Variável para contar famílias protegidas
        int custo = 4 + deputados; // Custo da delegacia e dos deputados
        int moedasRestantes = VERBA_DISPONIVEL - custo; // Moedas restantes após deduzir o custo

        // Marcar zonas protegidas e contar famílias protegidas
        for (int i = std::max(0, x - raio); i <= std::min(x + raio, instancia.N - 1); ++i) {
            for (int j = std::max(0, y - raio); j <= std::min(y + raio, instancia.M - 1); ++j) {
                protecao[i][j] = true;
                familiasProtegidas += mapa[i][j];
            }
        }

        // Imprimir o mapa com as zonas protegidas marcadas
        for (int i = 0; i < instancia.N; ++i) {
            for (int j = 0; j < instancia.M; ++j) {
                if (i == x && j == y) {
                    std::cout << std::setw(2) << mapa[i][j] << "D ";
                } else if (protecao[i][j]) {
                    std::cout << std::setw(2) << mapa[i][j] << "! ";
                } else {
                    std::cout << std::setw(2) << mapa[i][j] << "  ";
                }
            }
            std::cout << std::endl;
        }

        // Imprimir informações adicionais
        std::cout << "Famílias protegidas: " << familiasProtegidas << "/57\n";
        std::cout << "Custo total: " << custo << " moedas de ouro\n";
        std::cout << "Moedas restantes: " << moedasRestantes << " moedas de ouro\n";
        std::cout << "Número de delegacias colocadas: 1\n";
    }
};
int main() {
    CMapa mapa(1); // Utiliza a instância 1

    int x = 1; // Linha da delegacia
    int y = 1; // Coluna da delegacia
    int deputados = 0; // Número de deputados

    mapa.calcularEImprimir(x, y, deputados);

    return 0;
}