#include <stdio.h>

#define LINHAS 7
#define COLUNAS 7


// Backtracking

static void backTracking(int m[LINHAS][COLUNAS]){

//pos cursor 3.3
if (m[3][3]==1){
    return 1;
}

backTracking(m[LINHAS][COLUNAS]);







}










int main() {
    FILE *file;
    int matriz[LINHAS][COLUNAS];
    int i, j;

    // Abre o arquivo para leitura
    file = fopen("dados.txt", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }

    // Lê os dados do arquivo e armazena na matriz
    for (i = 0; i < LINHAS; i++) {
        for (j = 0; j < COLUNAS; j++) {
            fscanf(file, "%d", &matriz[i][j]);
        }
    }

    // Fecha o arquivo
    fclose(file);

    // Exibe a matriz lida
    printf("Matriz lida do arquivo:\n");
    for (i = 0; i < LINHAS; i++) {
        for (j = 0; j < COLUNAS; j++) {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
    return 0;
}
