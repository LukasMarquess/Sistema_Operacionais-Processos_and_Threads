#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Lê uma matriz a partir de um arquivo
int **Ler_Matriz(const char *arquivo_txt, int *linhas, int *colunas) {
    FILE *arquivo = fopen(arquivo_txt, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    // Lê dimensões da matriz
    fscanf(arquivo, "%d %d", linhas, colunas);

    // Alocação da matriz
    int **matriz = malloc(*linhas * sizeof(int *));
    for (int i = 0; i < *linhas; i++) {
        matriz[i] = calloc(*colunas, sizeof(int));
    }

    // Lê os valores da matriz do arquivo
    for (int i = 0; i < *linhas; i++) {
        for (int j = 0; j < *colunas; j++) {
            char temp[10];
            int valor;
            fscanf(arquivo, "%s %d", temp, &valor);
            matriz[i][j] = valor;
        }
    }


    fclose(arquivo);
    return matriz;
}

// Multiplica duas Matrizes
int **Multiplicar_Matrizes(int **matriz1, int linhas1, int colunas1, int **matriz2, int linhas2, int colunas2) {

    // A condição para que ocorra a multiplicação de matrizes é que o número de colunas da primeira matriz seja igual ao número de linhas da segunda matriz.
    if (colunas1 != linhas2) {
        fprintf(stderr, "Erro: Matrizes não podem ser multiplicadas, número de colunas da matriz 1 é diferente do número de linhas da matriz 2.\n");
        exit(EXIT_FAILURE);
    }

    // Alocação da matriz
    int **resultado = malloc(linhas1 * sizeof(int *));
    for (int i = 0; i < linhas1; i++) {
        resultado[i] = calloc(colunas2, sizeof(int));
    }

    // Multiplica as matrizes
    for (int i = 0; i < linhas1; i++) {
        for (int j = 0; j < colunas2; j++) {
            for (int k = 0; k < colunas1; k++) {
                resultado[i][j] += matriz1[i][k] * matriz2[k][j];
            }
        }
    }

    return resultado;
}

// Libera Memória Alocada
void Liberar_Memoria(int **matriz, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

// Grava a matriz em um arquivo
void Gravar_Matriz(int linhas, int colunas, int **matriz, const char *arquivo_txt, double tempo) {
    FILE *arquivo = fopen(arquivo_txt, "w");

    fprintf(arquivo, "%d %d\n", linhas, colunas);

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fprintf(arquivo, "C%d%d %d\n", i, j, matriz[i][j]);
        }
        //fprintf(arquivo, "\n");
    }

    fprintf(arquivo, "Tempo de execução: %f segundos\n", tempo);


    fclose(arquivo);
}

int main(int argc, char *argv[]) {

    // Forma de uso
    if (argc != 3) {
        printf("Uso: %s <arquivo_matriz1> <arquivo_matriz2>\n", argv[0]);
        return 1;
    }

    int linhas1, colunas1, linhas2, colunas2;

    int **matriz1 = Ler_Matriz(argv[1], &linhas1, &colunas1);
    int **matriz2 = Ler_Matriz(argv[2], &linhas2, &colunas2);

    clock_t inicio, fim;
    double tempo_gasto;
    int **resultado = NULL;
 
    inicio = clock();
    resultado = Multiplicar_Matrizes(matriz1, linhas1, colunas1, matriz2, linhas2, colunas2);
    fim = clock();

    tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;

    Gravar_Matriz(colunas1, linhas2, resultado, "Resultados/Sequencial/MatrizResultado.txt", tempo_gasto);
    printf("Tempo de execução: %f segundos\n", tempo_gasto);


    Liberar_Memoria(matriz1, linhas1);
    Liberar_Memoria(matriz2, linhas2);
    Liberar_Memoria(resultado, linhas1);

    return 0;
}
