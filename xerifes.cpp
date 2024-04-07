#include <iostream>
#include <array>
#include <queue>
#include <vector>
#include <iomanip>
#include <chrono>

const int LARGURA_MAXIMA = 20;
const int NUM_INSTANCIAS = 10;

struct Instancia {
    int N, M, Verba, ProtegerA, ProtegerB;
};

struct Estado {
    int x, y;
    int deputados;
    int familiasProtegidas;
    int custo;
    bool solucaoValida;
    int geracao;

    // Operador para comparação de custo (para UCS)
    bool operator>(const Estado& outro) const {
        return custo > outro.custo;
    }
};

struct Comparador {
    bool operator()(const Estado& a, const Estado& b) const {
        return a.custo > b.custo;  // Menor custo tem maior prioridade
    }
};

int raio_protecao(int deputados) {
    switch (deputados) {
        case 0: return 1;
        case 1: return 2;
        case 5: return 3;
        case 13: return 4;
        default: return 0;
    }
}

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
    bool protecao[LARGURA_MAXIMA][LARGURA_MAXIMA];
    Instancia instancia;


public:
    CMapa(int id) : instancia(instancias[id - 1]) {
        inicializarMapa(id - 1);
    };



    void inicializarMapa(int id);  // Declaration of the method
    void calcularEImprimir(const Estado& estado, int numExpansoes, double tempoGasto, int geracao);  // Declaration of the method
    int calcularRaio(int deputados);  // Declaration of the method


    void UCS(int objetivoProtecao) {
        auto inicio = std::chrono::high_resolution_clock::now();
        std::priority_queue<Estado, std::vector<Estado>, Comparador> fila;
        Estado estadoInicial = {0, 0, 0, 0, 4, 0, false};
        fila.push(estadoInicial);
        int numExpansoes = 0;
        bool tempoExcedido = false;

        while (!fila.empty() && !tempoExcedido) {
            Estado atual = fila.top();
            fila.pop();
            numExpansoes++;

            if (atual.familiasProtegidas >= objetivoProtecao && atual.custo <= instancia.Verba) {
                auto agora = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> tempo_gasto = agora - inicio;
                atual.solucaoValida = true;
                calcularEImprimir(atual, numExpansoes, tempo_gasto.count() / 1000.0, atual.geracao);
                return;
            }

            auto sucessores = gerarSucessores(atual);
            for (auto& sucessor : sucessores) {
                fila.push(sucessor);
            }

            if (std::chrono::high_resolution_clock::now() - inicio > std::chrono::minutes(1)) {
                tempoExcedido = true;
            }
        }

        if (tempoExcedido) {
            std::cout << "Tempo limite de 1 minuto excedido." << std::endl;
        }
    }

    int calcular_familas_protegidas(const std::vector<std::vector<int>>& mapa, const Estado& estado) {
        int protegidas = 0;
        std::vector<std::vector<bool>> protegida(mapa.size(), std::vector<bool>(mapa[0].size(), false));

        for (int i = 0; i < estado.x; i++) {
            for (int j = 0; j < estado.y; j++) {
                int deputados = estado.deputados; // Adaptar conforme a estrutura Estado
                int raio = raio_protecao(deputados);

            }
        }
        return protegidas;
    }

    std::vector<Estado> gerar_successores(const Estado& estado_atual, const std::vector<std::vector<int>>& mapa, int orcamento) {
        std::vector<Estado> sucessores;
        // Ajuste na lógica conforme necessário para adicionar os sucessores corretamente
        for (int x = 0; x < instancia.N; x++) {
            for (int y = 0; y < instancia.M; y++) {
                for (int deputados = 0; deputados <= instancia.Verba; deputados++) {
                    int custo = 4 + deputados; // Assumindo 4 como custo base + deputados
                    if (custo <= instancia.Verba) {
                        int estado;
                        Estado novoEstado = estado;
                        novoEstado.x = x;
                        novoEstado.y = y;
                        novoEstado.deputados = deputados;
                        novoEstado.custo += custo;
                        novoEstado.familiasProtegidas = calcularFamiliasProtegidas(novoEstado);
                        novoEstado.geracao = estado.geracao + 1;
                        sucessores.push_back(novoEstado);
                    }
                }
            }
        }
        return sucessores;

        void calcularEImprimir(const Estado &estado, int numExpansoes, double tempoGasto, int geracao) {
            int raio = calcularRaio(estado.deputados);
            int familiasProtegidas = 0;
            for (int i = std::max(0, estado.x - raio); i <= std::min(estado.x + raio, instancia.N - 1); i++) {
                for (int j = std::max(0, estado.y - raio); j <= std::min(estado.y + raio, instancia.M - 1); j++) {
                    familiasProtegidas += mapa[i][j];
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
                void calcularEImprimir(const Estado &estado, int numExpansoes, double tempoGasto, int geracao) {
                    int raio = calcularRaio(estado.deputados);
                    bool solucaoValida =
                            (estado.familiasProtegidas >= instancia.ProtegerA) && (estado.custo <= instancia.Verba);

                    // Reset da matriz de proteção
                    for (int i = 0; i < LARGURA_MAXIMA; ++i) {
                        std::fill_n(protecao[i], LARGURA_MAXIMA, false);
                    }

                    // Calcular famílias protegidas e marcar zonas
                    for (int i = std::max(0, estado.x - raio); i <= std::min(estado.x + raio, instancia.N - 1); ++i) {
                        for (int j = std::max(0, estado.y - raio);
                             j <= std::min(estado.y + raio, instancia.M - 1); ++j) {
                            protecao[i][j] = true;
                        }
                    }

                    // Imprimir o mapa e outras informações
                    std::cout << "Mapa com a delegacia e raio de proteção:\n";
                    for (int i = 0; i < instancia.N; ++i) {
                        for (int j = 0; j < instancia.M; ++j) {
                            // Iniciar a impressão com o número da célula
                            std::cout << std::setw(2) << mapa[i][j];

                            // Adicionar 'D' ou '!' se necessário, ou um espaço se não
                            if (i == estado.x && j == estado.y) {
                                std::cout << "D";
                            } else if (protecao[i][j]) {
                                std::cout << "!";
                            } else {
                                std::cout << " ";
                            }

                            // Espaço adicional após cada célula para separação
                            std::cout << " ";
                        }
                        std::cout << std::endl;
                    }

                    std::cout << "\nFamílias protegidas: " << estado.familiasProtegidas << "/" << instancia.ProtegerA
                              << "\n";
                    std::cout << "Custo total: " << estado.custo << " moedas de ouro\n";
                    std::cout << "Dinheiro disponível: " << instancia.Verba - estado.custo << "\n";
                    std::cout << "Número de deputados colocados: " << estado.deputados << "\n";

                    // Calculando o número de delegacias (neste caso, será sempre 1, pois cada Estado representa uma delegacia)
                    int numeroDelegacias = 1;
                    std::cout << "Número de delegacias: " << numeroDelegacias << "\n";
                    std::cout << "Número de expansões: " << numExpansoes << std::endl;
                    std::cout << "Geração: " << geracao << std::endl;
                    std::cout << "Tempo gasto: " << tempoGasto << " segundos" << std::endl;
                    // Imprimindo se a solução é válida ou não
                    if (estado.solucaoValida) {
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
                    mapaA.UCS(instancias[id - 1].ProtegerA);

                    std::cout << "\n------------------\n";

                    std::cout << "Instancia " << id << " - ProtegerB:\n";
                    CMapa mapaB(id);
                    mapaB.UCS(instancias[id - 1].ProtegerB);

                    std::cout << "\n==================\n";
                }
                return 0;

            }