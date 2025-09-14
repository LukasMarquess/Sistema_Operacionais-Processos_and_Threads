#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

//Estrutura para passar múltiplos argumentos para cada thread
typedef struct {
        int thread_id;
        int **Matriz1, **Matriz2, **Resultado;
        int colunas1, linhas2;
        int t, P;
    } ThreadData;

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

// Calcula o elemento da matriz resultado na posição (linha, coluna)
int Calcular_Elemento(int **matriz1, int **matriz2, int linha, int coluna, int colunas1) {
    int elemento = 0;
    for (int k = 0; k < colunas1; k++) {
        elemento += matriz1[linha][k] * matriz2[k][coluna];
    }
    return elemento;
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

// Libera Memória Alocada
void Liberar_Memoria(int **matriz, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

// Função executada por cada thread
void *Thread_Multiplica(void *arg){
    ThreadData *data = (ThreadData *) arg;

    int start = data->t * data->P;
    int end = (data->t + 1) * data->P;
    if (end > data->colunas1 * data->linhas2) {
        end = data->colunas1 * data->linhas2;
    }

    for (int idx = start; idx < end; idx++) {
        int i = idx / data->colunas1;
        int j = idx % data->colunas1;
        data->Resultado[i][j] = Calcular_Elemento(data->Matriz1, data->Matriz2, i, j, data->colunas1);
    }

    pthread_exit(NULL);

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

    // Calcula o número de threads
    int P = atoi(argv[3]);
    int Numero_Threads = (int) ceil((double) (colunas1*linhas2) / P);
    printf("Número de threads: %d\n", Numero_Threads);

    // Criação do Vetor de threads, uma para guardar os IDs das threads e outra para guardar os dados de cada thread
    pthread_t threads[Numero_Threads];
    ThreadData thread_data[Numero_Threads];

    // Alocação da matriz resultado
    int **resultado = malloc(linhas1 * sizeof(int *));
        for (int i = 0; i < linhas1; i++) {
            resultado[i] = calloc(colunas2, sizeof(int));
        }

    clock_t inicio, fim;
    double tempo_gasto;

    for (int i = 0; i < Numero_Threads; i++) {
        thread_data[i].thread_id = i;
        thread_data[i].Matriz1 = matriz1;
        thread_data[i].Matriz2 = matriz2;
        thread_data[i].Resultado = resultado;
        thread_data[i].colunas1 = colunas1;
        thread_data[i].linhas2 = linhas2;
        thread_data[i].t = i;
        thread_data[i].P = P;

        inicio = clock();
        pthread_create(&threads[i], NULL, Thread_Multiplica, (void *) &thread_data[i]);
        fim = clock();
        tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    }

    for (int i = 0; i < Numero_Threads; i++) {
        pthread_join(threads[i], NULL);

        int start = i * P;
        int end = (i + 1) * P;
        if (end > linhas1*colunas2){
            end = linhas1*colunas2;
        } 

        char nome_arquivo[256];
        sprintf(nome_arquivo, "Resultados/Threads/resultado_%d.txt", thread_data[i].t);
        Gravar_Matriz(thread_data[i].colunas1, thread_data[i].Resultado, nome_arquivo, tempo_gasto, start, end);
        printf("Thread %d: Salvo no arquivo %s.\n com tempo de execução: %f segundos\n", thread_data[i].t, nome_arquivo, tempo_gasto);
    }

    Liberar_Memoria(matriz1, linhas1);
    Liberar_Memoria(matriz2, linhas2);
    Liberar_Memoria(resultado, linhas1);

    return 0;
}