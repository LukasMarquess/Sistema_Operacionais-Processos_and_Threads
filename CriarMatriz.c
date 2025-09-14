#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Gera um número inteiro aleatório
int Num_Aleatorio(int min, int max) {
    return (rand() % (max - min + 1)) + min; 
}

// Preenche uma matriz com números aleatórios
int **Preencher_Matriz(int linhas, int colunas) {

    // Alocação da matriz
    int **matriz = malloc(linhas * sizeof(int *));
    for (int i = 0; i < linhas; i++) {
        matriz[i] = calloc(colunas, sizeof(int));
    }

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            matriz[i][j] = Num_Aleatorio(1, 100);
        }
    }
    return matriz;
}

// Grava a matriz em um arquivo
void Gravar_Matriz(int linhas, int colunas, int **matriz, const char *arquivo_txt) {
    FILE *arquivo = fopen(arquivo_txt, "w");

    fprintf(arquivo, "%d %d\n", linhas, colunas);

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fprintf(arquivo, "C%d%d %d\n", i, j, matriz[i][j]);
        }
    }
    fprintf(arquivo, "\n");
    fclose(arquivo);
}

// Libera Memória Alocada
void Liberar_Memoria(int **matriz, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

int main(int argc, char *argv[]) {

    // Forma de uso
    if (argc != 5) {
        printf("Uso: %s <Linhas_M1> <Colunas_M1> <Linhas_M2> <Colunas_M2>\n", argv[0]);
        return 1;
    }

    srand(time(NULL));  // Reseta a semente com o tempo atual


    int **matriz1 = Preencher_Matriz(atoi(argv[1]), atoi(argv[2]));
    int **matriz2 = Preencher_Matriz(atoi(argv[3]), atoi(argv[4]));

    Gravar_Matriz(atoi(argv[1]), atoi(argv[2]), matriz1, "Matriz1.txt");
    Gravar_Matriz(atoi(argv[3]), atoi(argv[4]), matriz2, "Matriz2.txt");
    Liberar_Memoria(matriz1, atoi(argv[1]));
    Liberar_Memoria(matriz2, atoi(argv[3]));

    return 0;
}


