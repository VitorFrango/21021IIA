//
// Created by Vitor Frango on 06/04/2024.
//

#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <SFML/Graphics.hpp>


using namespace std;

// Estrutura para representar o estado
struct Estado {
    vector<tuple<int, int, int>> delegacias; // Posição X, Posição Y, Deputados
    int custoTotal;
    int familiasProtegidas;

    Estado(vector<tuple<int, int, int>> delegacias = {}, int custoTotal = 0, int familiasProtegidas = 0)
            : delegacias(delegacias), custoTotal(custoTotal), familiasProtegidas(familiasProtegidas) {}

    // Comparador para a fila de prioridade
    bool operator>(const Estado& e) const {
        return this->custoTotal > e.custoTotal;
    }
};

// Calcula o raio de proteção com base no número de deputados
int raioDeProtecao(int deputados) {
    if (deputados == 0) return 1;
    if (deputados == 1) return 2;
    if (deputados == 5) return 3;
    if (deputados == 13) return 4;
    return 0;
}

// Função para calcular famílias protegidas
int calcularFamiliasProtegidas(const vector<vector<int>>& mapa, const Estado& estado) {
    int protegidas = 0;
    vector<vector<bool>> protegidaPorDelegacia(mapa.size(), vector<bool>(mapa[0].size(), false));

    for (const auto& delegacia : estado.delegacias) {
        int x = get<0>(delegacia);
        int y = get<1>(delegacia);
        int deputados = get<2>(delegacia);
        int raio = raioDeProtecao(deputados);

        for (int i = max(0, x - raio); i <= min((int)mapa.size() - 1, x + raio); ++i) {
            for (int j = max(0, y - raio); j <= min((int)mapa[0].size() - 1, y + raio); ++j) {
                if (!protegidaPorDelegacia[i][j]) {
                    protegidas += mapa[i][j];
                    protegidaPorDelegacia[i][j] = true;
                }
            }
        }
    }
    return protegidas;
}

// Função para gerar estados sucessores
vector<Estado> gerarSucessores(Estado& estadoAtual, const vector<vector<int>>& mapa, int moedas) {
    vector<Estado> sucessores;
    static const vector<int> deputadosPermitidos = {0, 1, 5, 13};

    for (int deputado : deputadosPermitidos) {
        int custoDelegacia = 4 + deputado; // 4 para a delegacia e 1 para cada deputado
        if (estadoAtual.custoTotal + custoDelegacia <= moedas) {
            for (int i = 0; i < mapa.size(); ++i) {
                for (int j = 0; j < mapa[i].size(); ++j) {
                    Estado novoEstado = estadoAtual;
                    novoEstado.delegacias.push_back(make_tuple(i, j, deputado));
                    novoEstado.custoTotal += custoDelegacia;
                    novoEstado.familiasProtegidas = calcularFamiliasProtegidas(mapa, novoEstado);
                    if (novoEstado.familiasProtegidas >= estadoAtual.familiasProtegidas) {
                        sucessores.push_back(novoEstado);
                    }
                }
            }
        }
    }

    return sucessores;
}

// Algoritmo de custo uniforme para encontrar a solução
void custoUniforme(vector<vector<int>>& mapa, int moedas, int familiasMinimas) {
    priority_queue<Estado, vector<Estado>, greater<Estado>> fronteira;

    // Estado inicial
    Estado inicial;
    fronteira.push(inicial);

    while (!fronteira.empty()) {
        Estado estadoAtual = fronteira.top();
        fronteira.pop();

        if (estadoAtual.familiasProtegidas >= familiasMinimas && estadoAtual.custoTotal <= moedas) {
            cout << "Solução encontrada com custo: " << estadoAtual.custoTotal << endl;
            // Mostrar detalhes da solução
            return;
        }

        vector<Estado> sucessores = gerarSucessores(estadoAtual, mapa, moedas);
        for (auto& sucessor : sucessores) {
            fronteira.push(sucessor);
        }
    }

    cout << "Não foi possível encontrar uma solução." << endl;
}

// Função para desenhar o mapa, as delegacias e as áreas de proteção
void desenharMapa(sf::RenderWindow& window, const vector<vector<int>>& mapa, const Estado& estado) {
    window.clear();

    int altura = mapa.size();
    int largura = mapa[0].size();

    float tamanhoCelulaX = window.getSize().x / static_cast<float>(largura);
    float tamanhoCelulaY = window.getSize().y / static_cast<float>(altura);

    // Desenhar zonas
    for (int i = 0; i < altura; ++i) {
        for (int j = 0; j < largura; ++j) {
            sf::RectangleShape celula(sf::Vector2f(tamanhoCelulaX - 2, tamanhoCelulaY - 2));
            celula.setPosition(j * tamanhoCelulaX + 1, i * tamanhoCelulaY + 1);
            celula.setFillColor(sf::Color::White);
            window.draw(celula);

            // Se a zona tem famílias, alterar a cor de fundo
            if (mapa[i][j] > 0) {
                celula.setFillColor(sf::Color(100, 250, 50));
                window.draw(celula);
            }
        }
    }

    // Desenhar delegacias e áreas de influência
    for (const auto& delegacia : estado.delegacias) {
        int x = get<0>(delegacia);
        int y = get<1>(delegacia);
        int deputados = get<2>(delegacia);
        int raio = raioDeProtecao(deputados);

        // Área de influência
        for (int dx = max(0, x - raio); dx <= min(altura - 1, x + raio); ++dx) {
            for (int dy = max(0, y - raio); dy <= min(largura - 1, y + raio); ++dy) {
                sf::RectangleShape area(sf::Vector2f(tamanhoCelulaX - 2, tamanhoCelulaY - 2));
                area.setPosition(dy * tamanhoCelulaX + 1, dx * tamanhoCelulaY + 1);
                area.setFillColor(sf::Color(250, 150, 100));
                window.draw(area);
            }
        }

        // Delegacia
        sf::RectangleShape deleg(sf::Vector2f(tamanhoCelulaX - 2, tamanhoCelulaY - 2));
        deleg.setPosition(y * tamanhoCelulaX + 1, x * tamanhoCelulaY + 1);
        deleg.setFillColor(sf::Color::Red);
        window.draw(deleg);
    }

    window.display();
}

int main() {
    // Definir o mapa e inicializá-lo com os dados fornecidos
    vector<vector<int>> mapa = {
            {1, 2, 3, 4, 5, 6, 7, 8, 9},
            {2, 3, 4, 5, 6, 7, 8, 9, 1},
            // Continue preenchendo as linhas restantes com os dados reais
            // Certifique-se de que cada linha tem exatamente 9 elementos
    };

    sf::RenderWindow window(sf::VideoMode(800, 600), "Território Protegido");

    // Simulação de estado final para fins de demonstração (deve ser substituído pela lógica real)
    vector<tuple<int, int, int>> delegacias = {make_tuple(3, 3, 5), make_tuple(5, 10, 1)};
    Estado estadoFinal(delegacias, 9, 100);  // Estado fictício para demonstração

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        desenharMapa(window, mapa, estadoFinal);
        sf::sleep(sf::seconds(0.1)); // Reduz a velocidade de atualização para visualização
    }

    return 0;
}