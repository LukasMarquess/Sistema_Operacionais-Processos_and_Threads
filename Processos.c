#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <math.h>
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

// Libera Memória Alocada
void Liberar_Memoria(int **matriz, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

// Grava o resultado parcial da matriz em um arquivo 
void Gravar_Matriz(int colunas, int **matriz, const char *arquivo_txt, double tempo, int start, int end) {
    FILE *arquivo = fopen(arquivo_txt, "w");

    fprintf(arquivo, "%d %d\n", colunas, colunas);

    for (int idx = start; idx < end; idx++) {
                int i = idx / colunas;
                int j = idx % colunas;
                fprintf(arquivo, "C%d%d %d\n", i, j, matriz[i][j]);
            }

    fprintf(arquivo, "Tempo de execução: %f segundos\n", tempo);

    fclose(arquivo);
}

// Calcula o elemento da matriz resultado na posição (linha, coluna)
int Calcular_Elemento(int **matriz1, int **matriz2, int linha, int coluna, int colunas1) {
    int elemento = 0;
    for (int k = 0; k < colunas1; k++) {
        elemento += matriz1[linha][k] * matriz2[k][coluna];
    }
    return elemento;
}

int main(int argc, char *argv[]) {

    // Forma de uso
    if (argc != 4) {
        printf("Uso: %s <arquivo_matriz1> <arquivo_matriz2> <P>\n", argv[0]);
        return 1;
    }

    int linhas1, colunas1, linhas2, colunas2;

    int **matriz1 = Ler_Matriz(argv[1], &linhas1, &colunas1);
    int **matriz2 = Ler_Matriz(argv[2], &linhas2, &colunas2);

    // Verifica se as matrizes podem ser multiplicadas
    if (colunas1 != linhas2) {
        fprintf(stderr, "Erro: Matrizes não podem ser multiplicadas (colunas da primeira diferente de linhas da segunda).\n");
        exit(EXIT_FAILURE);
    }

    // Calcula o número de processos
    int P = atoi(argv[3]);
    int processos = (int) ceil((double) (colunas1*linhas2) / P);
    printf("Número de processos: %d\n", processos);


    clock_t inicio, fim;
    double tempo_gasto;
    
    // Alocação da matriz resultado
    int **resultado = malloc(linhas1 * sizeof(int *));
        for (int i = 0; i < linhas1; i++) {
            resultado[i] = calloc(colunas2, sizeof(int));
        }


   for(int p = 0; p < processos; p++){
        // Cria um novo processo
        pid_t pid = fork();

        // Verifica se houve erro na criação do processo
        if(pid < 0){
            perror("Erro ao criar processo");
            exit(EXIT_FAILURE);
        }

        else if(pid == 0){ // Processo filho
            
            inicio = clock();
            int start = p * P;
            int end = (p + 1) * P;
            if (end > linhas1*colunas2){
                end = linhas1*colunas2;
            } 

            for (int idx = start; idx < end; idx++) {
                int i = idx / colunas2;
                int j = idx % colunas2;
                resultado[i][j] = Calcular_Elemento(matriz1, matriz2, i, j, colunas1);
            }
            fim = clock();
            tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

            char nome_arquivo[256];
            sprintf(nome_arquivo, "Resultados/Processos/resultado_%d.txt", p);
            Gravar_Matriz(colunas2, resultado, nome_arquivo, tempo_gasto, start, end);
            printf("Processo %d: Salvo no arquivo %s. Tempo: %f segundos\n", p, nome_arquivo, tempo_gasto);

            // Termina o processo filho
            exit(0);

        }
   }

   // Processo pai espera todos os filhos
    for (int i = 0; i < processos; i++) {
        wait(NULL);
    }

    Liberar_Memoria(matriz1, linhas1);
    Liberar_Memoria(matriz2, linhas2);
    Liberar_Memoria(resultado, linhas1);

    return 0;
}
