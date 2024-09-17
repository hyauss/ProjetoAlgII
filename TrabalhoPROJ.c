//Nomes
//Vinicius Alves Marques RA: 10417880
//Gabriel Barros Albertini RA: 10419482
//Rafael de Menezes Rossi RA: 10417954

#include <stdio.h>
#include <stdlib.h>

#define N 7
#define CENTRO 3

// Estrutura para armazenar os movimentos realizados
typedef struct {
    int from_x, from_y;  // Coordenadas de onde o pino se moveu
    int to_x, to_y;      // Coordenadas para onde o pino foi movido
} Movimento;

// Tabuleiro inicial (1 = pino, 0 = vazio, -1 = posição inválida)
int tabuleiro[N][N];

// Direções de movimento: cima, baixo, esquerda, direita
int dx[] = {-2, 2, 0, 0};
int dy[] = {0, 0, -2, 2};

// Vetor para armazenar os movimentos
Movimento movimentos[31];
int contador_movimentos = 0;

// Função para carregar o tabuleiro de um arquivo
void carregar_tabuleiro(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
        exit(1);
    }

    // Ler o estado do tabuleiro do arquivo
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fscanf(arquivo, "%d", &tabuleiro[i][j]);
        }
    }

    fclose(arquivo);
}

// Função para imprimir o tabuleiro (para debug)
void imprimir_tabuleiro() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (tabuleiro[i][j] == -1) {
                printf("  ");  // Espaço para posições inválidas
            } else {
                printf("%d ", tabuleiro[i][j]);
            }
        }
        printf("\n");
    }
}

// Função para verificar se um movimento é válido
int movimento_valido(int x, int y, int nx, int ny) {
    // Verifica se estamos dentro dos limites do tabuleiro
    if (nx < 0 || nx >= N || ny < 0 || ny >= N || tabuleiro[nx][ny] != 0) {
        return 0;
    }
    // Verifica se existe um pino entre a origem (x, y) e o destino (nx, ny)
    int mx = (x + nx) / 2;
    int my = (y + ny) / 2;
    if (tabuleiro[x][y] == 1 && tabuleiro[mx][my] == 1 && tabuleiro[nx][ny] == 0) {
        return 1;
    }
    return 0;
}

// Função para fazer um movimento
void faz_movimento(int x, int y, int nx, int ny) {
    int mx = (x + nx) / 2;  // Coordenadas do pino a ser removido (meio)
    int my = (y + ny) / 2;  // Coordenada Y intermediária
    tabuleiro[x][y] = 0;    // Remove pino de (x, y)
    tabuleiro[mx][my] = 0;  // Remove pino intermediário
    tabuleiro[nx][ny] = 1;  // Coloca o pino na nova posição
}

// Função para desfazer um movimento
void desfaz_movimento(int x, int y, int nx, int ny) {
    int mx = (x + nx) / 2;
    int my = (y + ny) / 2;  // Coordenada Y intermediária
    tabuleiro[x][y] = 1;    // Recoloca pino em (x, y)
    tabuleiro[mx][my] = 1;  // Recoloca pino intermediário
    tabuleiro[nx][ny] = 0;  // Remove pino da nova posição
}

// Função recursiva de backtracking
int resolver(int pinos_restantes) {
    // Se restar apenas um pino e ele estiver no centro, solução encontrada
    if (pinos_restantes == 1 && tabuleiro[CENTRO][CENTRO] == 1) {
        return 1;
    }

    // Tentar realizar movimentos
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            // Se houver um pino na posição (x, y)
            if (tabuleiro[x][y] == 1) {
                for (int d = 0; d < 4; d++) {
                    int nx = x + dx[d];
                    int ny = y + dy[d];
                    // Se o movimento é válido
                    if (movimento_valido(x, y, nx, ny)) {
                        // Executa o movimento
                        faz_movimento(x, y, nx, ny);

                        // Armazena o movimento
                        movimentos[contador_movimentos].from_x = x;
                        movimentos[contador_movimentos].from_y = y;
                        movimentos[contador_movimentos].to_x = nx;
                        movimentos[contador_movimentos].to_y = ny;
                        contador_movimentos++;

                        // Chama recursivamente para o próximo estado
                        if (resolver(pinos_restantes - 1)) {
                            return 1;
                        }

                        // Desfaz o movimento caso não encontre solução
                        contador_movimentos--;
                        desfaz_movimento(x, y, nx, ny);
                    }
                }
            }
        }
    }

    return 0;  // Se nenhum movimento levar à solução, retorna 0
}

// Função para salvar os movimentos em um arquivo
void salvar_movimentos() {
    FILE *arquivo = fopen("restaum_out.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar arquivo de saída.\n");
        return;
    }

    for (int i = 0; i < contador_movimentos; i++) {
        fprintf(arquivo, "Movimento %d: (%d, %d) -> (%d, %d)\n", i + 1,
                movimentos[i].from_x, movimentos[i].from_y,
                movimentos[i].to_x, movimentos[i].to_y);
    }

    fclose(arquivo);
}

int main() {
    // Carregar o tabuleiro de um arquivo
    carregar_tabuleiro("restaum_in.txt");

    // Imprimir o tabuleiro carregado
    printf("Tabuleiro inicial:\n");
    imprimir_tabuleiro();

    // Iniciar a resolução com 32 pinos no tabuleiro
    if (resolver(32)) {
        printf("Solução encontrada!\n");
        salvar_movimentos();
    } else {
        printf("Nenhuma solução encontrada.\n");
    }

    return 0;
}
