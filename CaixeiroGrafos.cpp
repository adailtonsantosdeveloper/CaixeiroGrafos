#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <fstream>

using namespace std;
using namespace std::chrono;

// Constantes globais
const int INF = 1e9;         // Representa um valor "infinito" para inicializar as distâncias mínimas
const int N = 14;            // Número total de cidades (de A até N)

// Matriz de distâncias entre as 14 cidades
int dist_matrix[N][N] = {
    {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26},  // A
    {2, 0, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25},  // B
    {4, 3, 0, 1, 2, 3, 5, 7, 11, 13, 17, 19, 23, 27},  // C
    {6, 5, 1, 0, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55},  // D
    {8, 7, 2, 10, 0, 5, 14, 16, 20, 15, 13, 1, 3, 4},  // E
    {10, 9, 3, 15, 5, 0, 7, 12, 15, 20, 11, 14, 13, 8},  // F
    {12, 11, 5, 20, 14, 7, 0, 1, 9, 2, 8, 3, 7, 4},  // G
    {14, 13, 7, 25, 16, 12, 1, 0, 2, 4, 8, 16, 32, 64},  // H
    {16, 15, 11, 30, 20, 15, 9, 2, 0, 3, 5, 7, 11, 13},  // I
    {18, 17, 13, 35, 15, 20, 2, 4, 3, 0, 1, 2, 4, 8},  // J
    {20, 19, 17, 40, 13, 11, 8, 8, 5, 1, 0, 15, 25, 35},  // K
    {22, 21, 19, 45, 1, 14, 3, 16, 7, 2, 15, 0, 4, 10},  // L
    {24, 23, 23, 50, 3, 13, 7, 32, 11, 4, 25, 4, 0, 8},  // M
    {26, 25, 27, 55, 4, 8, 4, 64, 13, 8, 35, 10, 8, 0}   // N
};

// Nomes das cidades correspondentes aos índices
char cities[N] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N' };

// Tabelas de Programação Dinâmica (DP)
// memo[mask][pos]: guarda o custo mínimo a partir da cidade 'pos', tendo visitado o conjunto de cidades representado por 'mask'.
// parent[mask][pos]: guarda qual foi a próxima cidade escolhida, usado para reconstruir o caminho ótimo no final.
int memo[1 << N][N];
int parent[1 << N][N];

// Função recursiva principal para o Caixeiro Viajante com Bitmask
int tsp(int mask, int pos) {
    // Caso base: Se todas as cidades foram visitadas (máscara cheia de 1s)
    if (mask == (1 << N) - 1) {
        return dist_matrix[pos][0]; // Retorna o custo de voltar da cidade atual para a origem (A)
    }

    // Se o subproblema já foi resolvido anteriormente, retorna o valor salvo (Memoization)
    if (memo[mask][pos] != -1) {
        return memo[mask][pos];
    }

    int ans = INF;
    int best_next = -1;

    // Tenta visitar todas as outras cidades que ainda não foram visitadas
    for (int next = 0; next < N; next++) {
        // Verifica se o bit da cidade 'next' é 0 (ou seja, ainda não foi visitada)
        if ((mask & (1 << next)) == 0) {
            // Custo atual + chamada recursiva marcando a cidade 'next' como visitada (ligando o bit com OR bitwise)
            int new_ans = dist_matrix[pos][next] + tsp(mask | (1 << next), next);

            // Encontra a rota de menor custo
            if (new_ans < ans) {
                ans = new_ans;
                best_next = next;
            }
        }
    }

    // Salva a melhor escolha de próximo passo e armazena o resultado na tabela de memoização
    parent[mask][pos] = best_next;
    return memo[mask][pos] = ans;
}

int main() {
    // Configura o console para aceitar caracteres especiais/acentos
    setlocale(LC_ALL, "Portuguese");

    // Inicia a contagem do tempo de execução
    auto start_time = high_resolution_clock::now();

    // Inicializa a tabela de memoização com -1 (indicando que nenhum estado foi calculado ainda)
    for (int i = 0; i < (1 << N); i++) {
        for (int j = 0; j < N; j++) {
            memo[i][j] = -1;
        }
    }

    // Executa o algoritmo TSP:
    // '1' em binário (00000000000001) significa que apenas a cidade 0 (A) foi visitada inicialmente.
    // '0' indica que estamos posicionados na cidade 0 (A).
    int min_distance = tsp(1, 0);

    // Reconstrução do caminho ótimo usando a tabela 'parent' preenchida durante a recursão
    vector<int> best_route;
    int mask = 1;
    int pos = 0;
    best_route.push_back(0); // Começa na cidade A

    for (int i = 0; i < N - 1; i++) {
        pos = parent[mask][pos];  // Descobre qual é a próxima cidade ótima
        best_route.push_back(pos); // Adiciona na rota
        mask |= (1 << pos);       // Marca a cidade como visitada na máscara
    }
    best_route.push_back(0); // Retorna para a cidade de origem (A)

    // Para a contagem do tempo
    auto end_time = high_resolution_clock::now();
    duration<double, milli> execution_time = end_time - start_time;

    // Exibe os resultados no console
    cout << "=== RESULTADO DO CAIXEIRO VIAJANTE (C++) ===" << endl;
    cout << "Rota Ótima: ";
    for (size_t i = 0; i < best_route.size(); i++) {
        cout << cities[best_route[i]];
        if (i < best_route.size() - 1) cout << " → ";
    }
    cout << "\nDistância Total: " << min_distance << "\n";
    cout << "Tempo de Execução: " << execution_time.count() / 1000.0 << " segundos\n\n";

    // --- GERAÇÃO DO ARQUIVO VISUAL (Formato DOT para Graphviz) ---
    // Cria um arquivo de texto contendo as instruções gráficas do grafo
    ofstream outfile("rota_grafo.dot");
    outfile << "graph TSP {\n";
    outfile << "    node [shape=circle, style=filled, fillcolor=lightblue];\n";

    // Mapeia quais arestas fazem parte da rota ótima para destacá-las em vermelho
    vector<pair<char, char>> optimal_edges;
    for (size_t i = 0; i < best_route.size() - 1; i++) {
        optimal_edges.push_back({ cities[best_route[i]], cities[best_route[i + 1]] });
    }

    // Insere todas as conexões (arestas) possíveis entre as cidades no arquivo DOT
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            bool is_optimal = false;
            // Verifica se a aresta atual faz parte da melhor rota encontrada
            for (auto& edge : optimal_edges) {
                if ((edge.first == cities[i] && edge.second == cities[j]) ||
                    (edge.first == cities[j] && edge.second == cities[i])) {
                    is_optimal = true;
                    break;
                }
            }
            // Se for da rota ótima, pinta de vermelho e engrossa a linha; senão, deixa em cinza tracejado
            if (is_optimal) {
                outfile << "    " << cities[i] << " -- " << cities[j]
                    << " [label=\"" << dist_matrix[i][j] << "\", color=red, penwidth=2.0];\n";
            }
            else {
                outfile << "    " << cities[i] << " -- " << cities[j]
                    << " [label=\"" << dist_matrix[i][j] << "\", color=gray, style=dashed];\n";
            }
        }
    }
    outfile << "}\n";
    outfile.close();

    cout << "Arquivo 'rota_grafo.dot' gerado com sucesso!" << endl;
    cout << "Instrução: Cole o conteúdo desse arquivo em 'www.graphvizonline.com' para visualizar o desenho." << endl;

    return 0;
}