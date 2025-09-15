#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXTENSAO ".txt"
#define LINHA_MAX 256

// Função que lê a última linha de um arquivo
int ler_ultima_linha(const char *caminho, char *linha_final) {
    FILE *fp = fopen(caminho, "r");
    if (!fp) {
        perror(caminho);
        return 0;
    }

    char linha[LINHA_MAX];
    linha_final[0] = '\0';

    while (fgets(linha, LINHA_MAX, fp)) {
        strcpy(linha_final, linha);  // guarda a última linha lida
    }

    fclose(fp);
    return 1;
}

// Função principal
int main() {
    int max_arquivo;
    char subpasta[30];

    printf("Digite a subpasta (Processos ou Threads): ");
    scanf("%s", subpasta);

    // Verifica se a subpasta é válida
    if (strcmp(subpasta, "Processos") != 0 &&
        strcmp(subpasta, "Threads") != 0) {
        printf("Subpasta inválida. Use: Processos ou Threads\n");
        return 1;
    }

    printf("Digite até qual número de arquivo deseja ler: ");
    scanf("%d", &max_arquivo);

    double menor_tempo = -1;
    int arquivo_menor = -1;

    for (int i = 0; i <= max_arquivo; i++) {
        char caminho[150];
        // Monta o caminho com base na subpasta escolhida
        sprintf(caminho, "Resultados/%s/resultado_%d%s", subpasta, i, EXTENSAO);

        char ultima_linha[LINHA_MAX];
        if (!ler_ultima_linha(caminho, ultima_linha)) continue;

        double tempo = 0.0;
        if (sscanf(ultima_linha, "Tempo de execução: %lf", &tempo) == 1) {
            if (menor_tempo < 0 || tempo > menor_tempo) {
                menor_tempo = tempo;
                arquivo_menor = i;
            }
        }
    }

    if (arquivo_menor >= 0)
        printf("Maior tempo encontrado: %.6f segundos (arquivo resultado_%d.txt em %s)\n",
               menor_tempo, arquivo_menor, subpasta);
    else
        printf("Nenhum tempo válido foi encontrado nos arquivos.\n");

    return 0;
}
