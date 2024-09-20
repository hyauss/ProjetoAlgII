//Vinicius Alves Marques RA: 10417880
//Gabriel Barros Albertini RA: 10419482
//Rafael de Menezes Rossi RA: 10417954

#include <stdio.h>
#include <stdlib.h>

#define N 7
#define CENTRO 3

// Estrutura para armazenar os movimentos realizados
typedef struct {
    int tabuleiro[N][N];  // Armazena o estado do tabuleiro
} EstadoTabuleiro;

// Tabuleiro inicial (1 = pino, 0 = vazio, -1 = posição inválida)
int tabuleiro[N][N];

// Direções de movimento: cima, baixo, esquerda, direita
int dx[] = {-2, 2, 0, 0};
int dy[] = {0, 0, -2, 2};

// Vetor para armazenar os estados do tabuleiro
EstadoTabuleiro estados[31];
int contador_movimentos = 0;
int tentativas = 0;  // Variável global para contar tentativas

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

// Função para imprimir um tabuleiro no formato especificado
void imprimir_tabuleiro_arquivo(FILE *arquivo, int estado[N][N]) {
    fprintf(arquivo, "#########\n");  // Imprime a borda superior
    for (int i = 0; i < N; i++) {
        // Imprime a borda lateral
        if (i == 2 || i == 3 || i == 4) {
            fprintf(arquivo, "#");  // Borda lateral única para linhas do meio
        } else {
            fprintf(arquivo, "###");  // Borda dupla para as outras linhas
        }

        for (int j = 0; j < N; j++) {
            // Imprime os elementos do tabuleiro
            if (estado[i][j] == 1) {
                fprintf(arquivo, "o");  // Pino
            } else if (estado[i][j] == -1) {
                continue;  // Não imprime nada para células inválidas
            } else {
                fprintf(arquivo, " ");  // Espaço vazio (sem pino)
            }
        }

        // Finaliza a linha com a borda lateral
        if (i == 2 || i == 3 || i == 4) {
            fprintf(arquivo, "#\n");  // Borda lateral única
        } else {
            fprintf(arquivo, "###\n");  // Borda dupla
        }
    }
    fprintf(arquivo, "#########\n\n");  // Imprime a borda inferior
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

// Função para salvar o estado atual do tabuleiro
void salvar_estado_atual() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            estados[contador_movimentos].tabuleiro[i][j] = tabuleiro[i][j];
        }
    }
}

// Função recursiva de backtracking
int resolver(int pinos_restantes) {
    // Exibe uma mensagem a cada 1 milhão de tentativas para indicar progresso
    tentativas++;
    if (tentativas % 1000000 == 0) {
        printf("O programa está calculando! Tentativa: %d\n", tentativas);  // Indica que o programa não está travado
        fflush(stdout);  // Garante que a mensagem seja impressa imediatamente
    }

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

                        // Salva o estado atual
                        salvar_estado_atual();
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

// Função para imprimir todos os estados armazenados no arquivo
void imprimir_estados_arquivo(FILE *arquivo) {
    // Imprime todos os estados da solução
    for (int i = 0; i < contador_movimentos; i++) {
        imprimir_tabuleiro_arquivo(arquivo, estados[i].tabuleiro);
    }
}

int main() {
    // Carregar o tabuleiro de um arquivo
    carregar_tabuleiro("restaum_in.txt");

    // Salvar o estado inicial antes de começar a resolver
    salvar_estado_atual();

    // Abrir o arquivo para salvar os estados do tabuleiro
    FILE *arquivo_saida = fopen("restaum_out.txt", "w");
    if (arquivo_saida == NULL) {
        printf("Erro ao criar arquivo de saída.\n");
        return 1;
    }

    // Imprimir o estado inicial no arquivo
    imprimir_tabuleiro_arquivo(arquivo_saida, tabuleiro);

    // Iniciar a resolução com 32 pinos no tabuleiro
    if (resolver(32)) {
        printf("\nSolução encontrada!\n");
    } else {
        printf("\nNenhuma solução encontrada.\n");
        fclose(arquivo_saida);
        return 1;  // Sai do programa se não houver solução
    }

    // Imprimir os movimentos subsequentes
    imprimir_estados_arquivo(arquivo_saida);

    // Fechar o arquivo de saída
    fclose(arquivo_saida);

    return 0;
}
