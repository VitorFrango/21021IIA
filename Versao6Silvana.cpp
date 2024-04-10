#include <iostream>
#include <array>
#include <queue>
#include <iomanip>
#include <chrono> // Para medir o tempo

const int LARGURA_MAXIMA = 20;
const int NUM_INSTANCIAS = 10;
const int VERBA_DISPONIVEL = 12;

struct Instancia {
    int N, M, Verba, ProtegerA, ProtegerB;
};

struct Estado {
    int x, y; // Posição da delegacia
    int deputados; // Número de deputados
    int familiasProtegidas; // Famílias protegidas
    int custo; // Custo total até agora
    int numDelegacias; // Número de delegacias colocadas
    bool solucaoValida; // Indica se a solução é válida
    int geracao; // Número da geração
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
                                                                                                                        {0, 4, 0, 0, 2},
                                                                                                                        {8, 0, 6, 3, 0}},
                                                                                                                       // Resto preenchido com zeros
                                                                                                               },
                                                                                                               {   {{0,8,0,4,5,10,0},
                                                                                                                    {0,4,0,7,0,4,0},
                                                                                                                    {0,2,4,2,0,0,2},
                                                                                                                    {0,7,0,1,2,0,0},
                                                                                                                    {2,4,0,0,3,0,2},
                                                                                                                    {0,4,0,0,3,0,0},
                                                                                                                    {2,0,0,0,0,0,0}},
                                                                                                               },
                                                                                                               {   {{0,0,1,0,7,0,1},
                                                                                                                    {0,1,4,0,0,0,4},
                                                                                                                    {0,0,0,0,2,0,0},
                                                                                                                    {3,1,0,8,5,7,7},
                                                                                                                    {0,4,0,3,0,0,0},
                                                                                                                    {0,0,0,3,2,4,2},
                                                                                                                    {0,8,3,6,3,0,0}},
                                                                                                               },
                                                                                                               {   {{6,7,2,0,0,0,0,0,0},
                                                                                                                    {3,3,6,0,8,4,3,1,0},
                                                                                                                    {0,0,8,0,0,0,2,4,0},
                                                                                                                    {0,0,0,1,0,3,2,0,0},
                                                                                                                    {0,0,0,7,4,0,1,0,0},
                                                                                                                    {12,8,0,5,4,1,4,3,4},
                                                                                                                    {8,0,1,2,4,3,3,0,0},
                                                                                                                    {1,1,0,0,0,0,5,0,0},
                                                                                                                    {4,0,0,0,4,6,0,13,2}},
                                                                                                               },
                                                                                                               {   {{0,0,0,0,0,0,0,0,0},
                                                                                                                    {4,0,8,4,0,0,0,0,0},
                                                                                                                    {0,0,0,0,0,0,0,0,0},
                                                                                                                    {0,0,0,0,3,0,0,1,0},
                                                                                                                    {0,3,0,0,0,0,0,0,0},
                                                                                                                    {0,0,0,1,1,0,0,3,0},
                                                                                                                    {0,0,2,4,0,0,0,1,0},
                                                                                                                    {0,2,0,0,8,0,4,3,10},
                                                                                                                    {0,0,3,0,0,4,0,0,0}},
                                                                                                               },
                                                                                                               {    {{0,0,0,0,0,3,0,0,0,0,0},
                                                                                                                     {0,0,11,2,0,0,9,3,0,0,3},
                                                                                                                     {0,0,0,3,1,0,2,0,0,0,0},
                                                                                                                     {4,1,2,3,0,4,0,0,4,0,0},
                                                                                                                     {5,0,0,0,4,0,1,0,4,3,0},
                                                                                                                     {0,0,0,7,4,0,1,0,0,7,0},
                                                                                                                     {0,8,0,0,0,0,3,0,1,0,3},
                                                                                                                     {0,3,0,0,5,2,3,0,0,0,2},
                                                                                                                     {0,0,0,3,1,0,2,8,0,0,0},
                                                                                                                     {0,3,4,0,7,0,0,7,0,0,0},
                                                                                                                     {4,2,0,4,0,3,0,0,5,7,0}},
                                                                                                               },
                                                                                                               {
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
                                                                                                                        {0,0,0,0,0,0,0,0,0,0,0}},
                                                                                                               },
                                                                                                               {  {{2,4,0,0,6,7,3,4,0,0,3,0,1},
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
                                                                                                                   {0,0,4,7,7,0,0,2,0,2,5,0,1}},
                                                                                                               },
                                                                                                               {
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
                                                                                                                        {0,0,0,0,0,0,0,0,0,4,0,0,0}},
                                                                                                               },

                                                                                                       }};

class CMapa {
private:
    int mapa[LARGURA_MAXIMA][LARGURA_MAXIMA];
    bool protecao[LARGURA_MAXIMA][LARGURA_MAXIMA];  // To track protection
    Instancia instancia;

public:
    explicit CMapa(int id) : instancia(instancias[id - 1]) {
        inicializarMapa(id - 1);
    }

    void inicializarMapa(int id) {
        for (int i = 0; i < instancia.N; ++i) {
            for (int j = 0; j < instancia.M; ++j) {
                mapa[i][j] = mapas[id][i][j];
                protecao[i][j] = false;  // Initializing protection tracking
            }
        }
    }

    int calcularRaio(int deputados) {
        if (deputados == 0) return 1;
        if (deputados == 1) return 2;
        if (deputados == 5) return 3;
        if (deputados == 13) return 4;
        return 1;  // Default case
    }

    std::vector<Estado> gerarSucessores(Estado estado) {
        std::vector<Estado> sucessores;
        // For each possible position (x, y) in the map:
        for (int x = 0; x < LARGURA_MAXIMA; ++x) {
            for (int y = 0; y < LARGURA_MAXIMA; ++y) {
                // If the position is valid for placing a deputy:
                if (x >= 0 && x < instancia.N && y >= 0 && y < instancia.M && mapa[x][y] == 0 && estado.custo + calcularRaio <= instancia.Verba) {
                    // Create a new `Estado` with the new position and updated values.
                    Estado novoEstado = estado;
                    novoEstado.x = x;
                    novoEstado.y = y;
                    novoEstado.custo += deputados; // assuming COST_OF_DEPUTY is the cost to place a deputy
                    // Add the new `Estado` to `sucessores`.
                    sucessores.push_back(novoEstado);
                }
            }
        }
        return sucessores;
    }

    void BFS(int objetivoProtecao) {
        auto inicio = std::chrono::high_resolution_clock::now();
        std::queue<Estado> fila;
        Estado estadoInicial = {0, 0, 0, 0, 4, 1, false, 0};
        fila.push(estadoInicial);
        int numExpansoes = 0;
        bool tempoExcedido = false;

        while (!fila.empty() && !tempoExcedido) {
            auto agora = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> tempo_gasto = agora - inicio;
            double tempo_gasto_s = tempo_gasto.count() / 1000.0; // Convertendo para segundos

            if (tempo_gasto.count() > 60000) { // Checa se excedeu 1 minuto (60.000 milissegundos)
                tempoExcedido = true;
                // break; // Encerra o loop se o tempo limite for excedido
            }

            Estado atual = fila.front();
            fila.pop();

            if (atual.familiasProtegidas >= objetivoProtecao && atual.custo <= instancia.Verba) {
                atual.solucaoValida = true;
                calcularEImprimir(atual, numExpansoes, tempo_gasto_s, atual.geracao);
                break;  // Add this line to stop as soon as a valid solution is found.
            }

            auto sucessores = gerarSucessores(atual);
            for (const Estado& sucessor : sucessores) {
                Estado novoSucessor = sucessor;
                novoSucessor.geracao = atual.geracao + 1; // Incrementando a geração
                fila.push(novoSucessor);
            }
            numExpansoes += sucessores.size(); // Contando as expansões
        }

        if (tempoExcedido) {
            std::cout << "Tempo limite de 1 minuto excedido." << std::endl;
        }
    }



    // Método para calcular e imprimir as informações e o mapa
    void calcularEImprimir(const Estado& estado, int numExpansoes, double tempoGasto, int geracao) {
    int raio = calcularRaio(estado.deputados);
       // bool solucaoValida = (estado.familiasProtegidas >= instancia.ProtegerA) && (estado.custo <= instancia.Verba);

        // Reset da matriz de proteção
        for (int i = 0; i < LARGURA_MAXIMA; ++i) {
            std::fill_n(protecao[i], LARGURA_MAXIMA, false);
        }

        // Calcular famílias protegidas e marcar zonas de proteção
        int familiasProtegidasCalc = 0;
        for (int i = std::max(0, estado.x - raio); i <= std::min(estado.x + raio, instancia.N - 1); ++i) {
            for (int j = std::max(0, estado.y - raio); j <= std::min(estado.y + raio, instancia.M - 1); ++j) {
                if (mapa[i][j] > 0) {
                    familiasProtegidasCalc += mapa[i][j];
                    protecao[i][j] = true;
                }
            }
        }

        // Verifica se a solução atende aos requisitos após o cálculo correto das famílias protegidas.
        bool solucaoValida = (familiasProtegidasCalc >= instancia.ProtegerA) && (estado.custo <= instancia.Verba);

        // Imprimir o mapa e outras informações
        std::cout << "Mapa com a delegacia e raio de proteção:\n";
        for (int i = 0; i < instancia.N; ++i) {
            for (int j = 0; j < instancia.M; ++j) {
                std::cout << std::setw(2) << mapa[i][j];
                if (i == estado.x && j == estado.y) {
                    std::cout << "D";
                } else if (protecao[i][j]) {
                    std::cout << "!";
                } else {
                    std::cout << " ";
                }
                std::cout << " ";
            }
            std::cout << std::endl;
        }

        std::cout << "\nFamílias protegidas: " << familiasProtegidasCalc << "/" << instancia.ProtegerA << "\n";
        std::cout << "Custo total: " << estado.custo << " moedas de ouro\n";
        std::cout << "Dinheiro disponível: " << instancia.Verba - estado.custo << "\n";
        std::cout << "Número de deputados colocados: " << estado.deputados << "\n";
        std::cout << "Número de delegacias: " << estado.numDelegacias << "\n";
        std::cout << "Número de expansões: " << numExpansoes << std::endl;
        std::cout << "Geração: " << geracao << std::endl;
        std::cout << "Tempo gasto: " << tempoGasto << " segundos" << std::endl;

        if (solucaoValida) {
            std::cout << "Solução válida: Sim\n";
        } else {
            std::cout << "Solução válida: Não\n";
        }
        std::cout << std::endl;
    }
};

int main() {
    for (int id = 1; id <= NUM_INSTANCIAS; id++) {
        std::cout << "Instancia " << id << " - ProtegerA:\n";
        CMapa mapaA(id);
        mapaA.BFS(instancias[id - 1].ProtegerA);

        std::cout << "\n------------------\n";

        std::cout << "Instancia " << id << " - ProtegerB:\n";
        CMapa mapaB(id);
        mapaB.BFS(instancias[id - 1].ProtegerB);

        std::cout << "\n==================\n";
    }
    return 0;
}