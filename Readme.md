# Trabalho 1 - Multiplicação de Matrizes com Avaliação de Desempenho

## Sobre o Projeto

Este projeto tem como objetivo realizar a multiplicação de duas matrizes de mesmo tamanho, utilizando três abordagens distintas: sequencial, processos e threads. O foco principal é comparar o desempenho entre essas abordagens, analisando o tempo de execução e a eficiência de cada método. O projeto foi feito na Linguagem C usando um terminal linux, portanto caso seu sistema seja Windows, basta baixar o WSL(Windows Subsystem for Linux). 

O projeto foi desenvolvido como parte da disciplina de Sistemas Operacionais(IMD0036) ministrada pelo professor Kayo Gonçalves e Silva do Instituto Metrópole Digital da Universidade Federal do Rio Grande do Norte (UFRN).

## Estrutura do Projeto

- `CriarMatriz.c`: Gera matrizes de inteiros e salva em arquivos texto.
- `Sequencial.c`: Multiplica as matrizes de forma sequencial.
- `Processos.c`: Multiplica as matrizes utilizando múltiplos processos.
- `Threads.c`: Multiplica as matrizes utilizando múltiplas threads.
- `Resultados/`: Diretório onde os resultados das multiplicações são salvos.

## Como Compilar

Abra o terminal linux na pasta do projeto e utilize os comandos abaixo para compilar cada arquivo:

### Compilar o gerador de matrizes
```bash
gcc -o CriarMatriz CriarMatriz.c
```

### Compilar o código sequencial
```bash
gcc -o Sequencial Sequencial.c
```

### Compilar o código com processos
```bash
gcc -o Processos Processos.c -lm
```

### Compilar o código com threads
```bash
gcc -o Threads Threads.c -lpthread -lm
```

## Como Executar

Cada executável espera argumentos específicos. Por exemplo, para multiplicar matrizes:

```bash
./Sequencial Matriz1.txt Matriz2.txt
./Processos Matriz1.txt Matriz2.txt <P>
./Threads Matriz1.txt Matriz2.txt <P>
```

Onde `<P>` representa um número inteiro que diz respeito a quantos processos/threads vamos ter a cada P elementos da Matriz.

## Observações
- Certifique-se de que as matrizes de entrada tenham dimensões compatíveis para multiplicação.
- Os resultados são salvos no diretório `Resultados/`.
- O projeto permite avaliar o impacto do paralelismo no desempenho da multiplicação de matrizes.

---

Desenvolvido para fins acadêmicos na UFRN/IMD.

## Desenvolvedor

Lucas Marques  
Departamento de Engenharia da Computação e Automação (DCA)  