#include <stdio.h>
#include <stdlib.h>

#define LINHAS 7
#define COLUNAS 7

// Representação do tabuleiro inicial (será preenchida a partir de um arquivo)
int tabuleiro[LINHAS][COLUNAS];

// Estrutura para armazenar movimentos
typedef struct {
    int x1, y1; // Posição de origem
    int x2, y2; // Posição de destino
} Movimento;

// Função para ler o tabuleiro a partir de um arquivo
int lerTabuleiro(const char *nomeArquivo) {
    FILE *file = fopen(nomeArquivo, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s!\n", nomeArquivo);
        return 0;
    }

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            if (fscanf(file, "%d", &tabuleiro[i][j]) != 1) {
                printf("Erro ao ler o arquivo %s!\n", nomeArquivo);
                fclose(file);
                return 0;
            }
        }
    }

    fclose(file);
    return 1; // Sucesso
}

// Função para imprimir o tabuleiro atual
void imprimirTabuleiro() {
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            printf("%d ", tabuleiro[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Verifica se há apenas um pino no tabuleiro
int umPinoRestante() {
    int count = 0;
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            if (tabuleiro[i][j] == 1)
                count++;
        }
    }
    return count == 1 && tabuleiro[3][3] == 1; // O pino deve estar na posição central (3,3)
}

// Função para verificar se um movimento é válido
int movimentoValido(int x1, int y1, int x2, int y2) {
    if (x2 < 0 || x2 >= LINHAS || y2 < 0 || y2 >= COLUNAS)
        return 0; // Movimento fora do tabuleiro
    if (tabuleiro[x2][y2] != 0 || tabuleiro[x1][y1] != 1)
        return 0; // Destino não está vazio ou origem não tem um pino
    int xm = (x1 + x2) / 2, ym = (y1 + y2) / 2; // Coordenadas do pino a ser pulado
    return tabuleiro[xm][ym] == 1; // Deve haver um pino a ser pulado
}

// Faz o movimento no tabuleiro
void mover(int x1, int y1, int x2, int y2) {
    int xm = (x1 + x2) / 2, ym = (y1 + y2) / 2;
    tabuleiro[x1][y1] = 0; // Remove o pino de origem
    tabuleiro[xm][ym] = 0; // Remove o pino pulado
    tabuleiro[x2][y2] = 1; // Coloca o pino na nova posição
}

// Desfaz o movimento no tabuleiro
void desfazer(int x1, int y1, int x2, int y2) {
    int xm = (x1 + x2) / 2, ym = (y1 + y2) / 2;
    tabuleiro[x1][y1] = 1; // Restaura o pino de origem
    tabuleiro[xm][ym] = 1; // Restaura o pino pulado
    tabuleiro[x2][y2] = 0; // Remove o pino da nova posição
}

// Função de backtracking para resolver o jogo
int resolver(Movimento movimentos[], int passo, int *tentativas) {
    // Exibe progresso a cada 1000 tentativas
    if (*tentativas % 1000000 == 0) {
        printf("Tentativas: %d\n", *tentativas);
    }
    
    if (umPinoRestante()) {
        printf("Solução encontrada com %d movimentos:\n", passo);
        for (int i = 0; i < passo; i++) {
            printf("(%d, %d) -> (%d, %d)\n", movimentos[i].x1, movimentos[i].y1, movimentos[i].x2, movimentos[i].y2);
        }
        return 1; // Solução encontrada
    }

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            if (tabuleiro[i][j] == 1) { // Para cada pino no tabuleiro
                // Testa movimentos em 4 direções (cima, baixo, esquerda, direita)
                int direcoes[4][2] = {{-2, 0}, {2, 0}, {0, -2}, {0, 2}};
                for (int d = 0; d < 4; d++) {
                    int x2 = i + direcoes[d][0], y2 = j + direcoes[d][1];
                    if (movimentoValido(i, j, x2, y2)) {
                        mover(i, j, x2, y2); // Faz o movimento
                        movimentos[passo] = (Movimento){i, j, x2, y2}; // Armazena o movimento
                        (*tentativas)++; // Incrementa o contador de tentativas
                        if (resolver(movimentos, passo + 1, tentativas)) { // Chamada recursiva
                            return 1; // Solução encontrada
                        }
                        desfazer(i, j, x2, y2); // Desfaz o movimento
                    }
                }
            }
        }
    }
    return 0; // Sem solução encontrada neste ramo
}

int main() {
    Movimento movimentos[31]; // No máximo 31 movimentos
    int tentativas = 0; // Contador de tentativas
    
    // Lê o tabuleiro a partir do arquivo "tabuleiro.txt"
    if (!lerTabuleiro("dados.txt")) {
        return 1; // Sai se houver erro ao ler o arquivo
    }
    
    printf("Tabuleiro inicial:\n");
    imprimirTabuleiro();
    
    if (resolver(movimentos, 0, &tentativas)) {
        printf("Solução completa encontrada e salva em arquivo.\n");
        FILE *file = fopen("solucao.txt", "w");
        for (int i = 0; i < 31; i++) {
            fprintf(file, "(%d, %d) -> (%d, %d)\n", movimentos[i].x1, movimentos[i].y1, movimentos[i].x2, movimentos[i].y2);
        }
        fclose(file);
    } else {
        printf("Nenhuma solução encontrada.\n");
    }
    
    return 0;
}
