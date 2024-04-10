#include <iostream>
#include <array>
#include <queue>
#include <iomanip>
#include <chrono> // Para medir o tempo
#include <sstream> // Para otimizar a impressão

const int LARGURA_MAXIMA = 13;
const int NUM_INSTANCIAS = 10;
//const int VERBA_DISPONIVEL = 12;

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
    int mapa[LARGURA_MAXIMA][LARGURA_MAXIMA];
    bool protecao[LARGURA_MAXIMA][LARGURA_MAXIMA];  // Nova matriz para rastrear proteção
    Instancia instancia;
    Estado melhorEstado;

public:
    CMapa(int id) : instancia(instancias[id - 1]) {
        inicializarMapa(id - 1);
    }

    std::vector<Estado> gerarSucessores(const Estado &estado) {
        std::vector<Estado> sucessores;
        for (int x = 0; x < instancia.N; x++) {
            for (int y = 0; y < instancia.M; y++) {
                int maxDeputados = (instancia.Verba - 4) - estado.custo;  // Limitando os deputados com base no custo restante
                for (int deputados = 0; deputados <= maxDeputados; deputados++) {
                    int raio = calcularRaio(deputados);
                    int familiasProtegidas = 0;

                    for (int i = std::max(0, x - raio); i <= std::min(x + raio, instancia.N - 1); ++i) {
                        for (int j = std::max(0, y - raio); j <= std::min(y + raio, instancia.M - 1); ++j) {
                            familiasProtegidas += mapa[i][j];
                        }
                    }

                    int custo = 4 + deputados + estado.custo;
                    if (custo <= instancia.Verba) {
                        Estado novoEstado = {x, y, deputados, familiasProtegidas, custo, estado.numDelegacias + 1, false, estado.geracao + 1};
                        sucessores.push_back(novoEstado);
                    }
                }
            }
        }
        return sucessores;
    }



    void BFS(int objetivoProtecao) {
        auto inicio = std::chrono::high_resolution_clock::now();
        std::queue<Estado> fila;
        Estado estadoInicial = {0, 0, 0, 0, 0, 0, true, 0}; // Custos e delegacias ajustados, considerado inicialmente como válido
        fila.push(estadoInicial);
        int numExpansoes = 0;
        Estado melhorEstado = estadoInicial; // Inicializa o melhor estado como o estado inicial

        while (!fila.empty()) {
            Estado atual = fila.front();
            fila.pop();

            // Atualização da melhor solução
            if (atual.familiasProtegidas > melhorEstado.familiasProtegidas ||
                (atual.familiasProtegidas == melhorEstado.familiasProtegidas && atual.custo < melhorEstado.custo)) {
                melhorEstado = atual;
            }

            if (atual.familiasProtegidas >= objetivoProtecao) {
                melhorEstado = atual; // Atualiza o melhor estado se uma solução foi encontrada
                break; // Encerra se a proteção desejada foi alcançada
            }

            auto sucessores = gerarSucessores(atual);
            for (const Estado &sucessor: sucessores) {
                fila.push(sucessor);


            }

            numExpansoes += sucessores.size();

            // Verificação do limite de tempo
            auto agora = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> tempo_gasto = agora - inicio;
            if (tempo_gasto.count() > 60000) { // Limite de tempo
                break; // Encerra se o tempo excedeu
            }
        }

        // Após sair do loop, imprimir o melhor estado
        auto fim = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> tempo_gasto = fim - inicio;
        calcularEImprimir(melhorEstado, numExpansoes, tempo_gasto.count() / 1000.0, melhorEstado.geracao, "Resumo após o término:");
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
            case 0:
                return 1;
            case 1:
                return 2;
            case 5:
                return 3;
            case 13:
                return 4;
            default:
                return 1; // caso padrão para apenas o xerife
        }
    }

    // Método para calcular e imprimir as informações e o mapa
    // Método para calcular e imprimir as informações e o mapa
    void calcularEImprimir(const Estado &estado, int numExpansoes, double tempoGasto, int geracao, const std::string& mensagem) {
        std::ostringstream output;

        if (estado.solucaoValida) {
            output << "Mapa com a delegacia e áreas protegidas:\n";

            // Inicializa uma cópia do mapa para marcar proteções e delegacia
            int mapaVisual[LARGURA_MAXIMA][LARGURA_MAXIMA];
            for (int i = 0; i < instancia.N; ++i) {
                for (int j = 0; j < instancia.M; ++j) {
                    mapaVisual[i][j] = mapa[i][j];
                }
            }

            // Marca a delegacia no mapa
            mapaVisual[estado.x][estado.y] = -1; // -1 denota a delegacia

            // Marca as áreas protegidas no mapa
            int raio = calcularRaio(estado.deputados);
            for (int i = std::max(0, estado.x - raio); i <= std::min(instancia.N - 1, estado.x + raio); ++i) {
                for (int j = std::max(0, estado.y - raio); j <= std::min(instancia.M - 1, estado.y + raio); ++j) {
                    if (mapaVisual[i][j] >= 0) { // Apenas marca se não for a delegacia
                        mapaVisual[i][j] = -2; // -2 denota área protegida
                    }
                }
            }

            // Imprime o mapa
            for (int i = 0; i < instancia.N; ++i) {
                for (int j = 0; j < instancia.M; ++j) {
                    if (mapaVisual[i][j] == -1) {
                        output << "D "; // Delegacia
                    } else if (mapaVisual[i][j] == -2) {
                        output << "* "; // Área protegida
                    } else {
                        output << ". "; // Área não protegida
                    }
                }
                output << "\n";
            }
        }

        output << mensagem << "\n";

        if (estado.solucaoValida || (!estado.solucaoValida && geracao > 0)) {
            output << "\nFamílias protegidas: " << estado.familiasProtegidas << "/" << instancia.ProtegerA
                   << "\nCusto total: " << estado.custo << " moedas de ouro\n"
                   << "Número de deputados colocados: " << estado.deputados << "\n"
                   << "Número de delegacias: " << estado.numDelegacias << "\n"
                   << "Número de expansões: " << numExpansoes << "\n"
                   << "Geração: " << geracao << "\n"
                   << "Tempo gasto: " << tempoGasto << " segundos\n";
        }

        std::cout << output.str();
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