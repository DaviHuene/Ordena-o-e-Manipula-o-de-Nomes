#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#define MAX_NOMES 10000
#define MAX_TAMANHO_NOME 100

void executar_programa(const char* comando) {
    if (system(comando) == 0) {
        #ifdef _WIN32
            system("tempCodeRunnerFile.exe");
        #else
            system("./tempCodeRunnerFile");
        #endif
    } else {
        printf("Erro ao compilar o programa\n");
    }
}

// Função de troca
void swap(char *a, char *b) {
    char temp[MAX_TAMANHO_NOME];
    strcpy(temp, a);
    strcpy(a, b);
    strcpy(b, temp);
}

// Função Insertion Sort
void insertionSort(char nomes[][MAX_TAMANHO_NOME], int n) {
    for (int i = 1; i < n; i++) {
        char chave[MAX_TAMANHO_NOME];
        strcpy(chave, nomes[i]);
        int j = i - 1;
        while (j >= 0 && strcmp(nomes[j], chave) > 0) {
            strcpy(nomes[j + 1], nomes[j]);
            j--;
        }
        strcpy(nomes[j + 1], chave);
    }
}

// Função Binary Insertion Sort
void binaryInsertionSort(char nomes[][MAX_TAMANHO_NOME], int n) {
    for (int i = 1; i < n; i++) {
        char chave[MAX_TAMANHO_NOME];
        strcpy(chave, nomes[i]);
        
        int left = 0, right = i - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (strcmp(nomes[mid], chave) > 0) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }

        for (int j = i - 1; j >= left; j--) {
            strcpy(nomes[j + 1], nomes[j]);
        }
        strcpy(nomes[left], chave);
    }
}

// Função para ler os nomes de um arquivo
int lerNomes(const char *nomeArquivo, char nomes[][MAX_TAMANHO_NOME]) {
    FILE *file = fopen(nomeArquivo, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        return -1;
    }

    int i = 0;
    while (fgets(nomes[i], MAX_TAMANHO_NOME, file)) {
        nomes[i][strcspn(nomes[i], "\n")] = 0; // Remove newline
        i++;
    }
    fclose(file);
    return i;
}

// Função para registrar o tempo de execução em um arquivo de log
void registrarTempoExecucao(const char *metodo, const char *caso, double tempo) {
    FILE *file = fopen("output/relatorio_tempo.txt", "a"); // Modo append
    if (!file) {
        printf("Erro ao abrir o arquivo de relatorio de tempo.\n");
        return;
    }

    fprintf(file, "Método: %s, Caso: %s, Tempo de execução: %.5f segundos\n", metodo, caso, tempo);
    fclose(file);
}

// Função para salvar os nomes ordenados em um arquivo
void salvarNomes(const char *nomeArquivo, char nomes[][MAX_TAMANHO_NOME], int n) {
    FILE *file = fopen(nomeArquivo, "w");
    if (!file) {
        printf("Erro ao abrir o arquivo %s para escrita.\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        fprintf(file, "%s\n", nomes[i]);
    }
    fclose(file);
}

// Função para medir e executar a ordenação, e gerar relatório
void executarOrdenacao(char nomes[][MAX_TAMANHO_NOME], int n, const char *metodo, const char *caso) {
    double tempo;
    
    #ifdef _WIN32
    LARGE_INTEGER start, end, frequency;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    #else
    struct timeval start, end;
    gettimeofday(&start, NULL);
    #endif

    if (strcmp(metodo, "insertion") == 0) {
        insertionSort(nomes, n);
    } else if (strcmp(metodo, "binaryInsertion") == 0) {
        binaryInsertionSort(nomes, n);
    }

    #ifdef _WIN32
    QueryPerformanceCounter(&end);
    tempo = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
    #else
    gettimeofday(&end, NULL);
    tempo = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
    #endif

    printf("Tempo de execucao para %s Sort (%s caso): %.5f segundos\n", metodo, caso, tempo);
    registrarTempoExecucao(metodo, caso, tempo);

    // Gerar arquivo de saída com o método e caso utilizados
    char nomeArquivoSaida[50];
    sprintf(nomeArquivoSaida, "output/%s_%s.txt", metodo, caso); // Gera o nome do arquivo
    salvarNomes(nomeArquivoSaida, nomes, n); // Salva os nomes ordenados
}

// Função para exibir o menu e gerenciar as operações
void menu() {
    int opcao, n = 0;
    char nomes[MAX_NOMES][MAX_TAMANHO_NOME];
    char metodo[20];
    char *nomeArquivo;
    char *caso;

    while (1) {
        printf("<===============================>\n");
        printf("     ORDENACAO POR INSERCAO \n");
        printf("<===============================>\n");

        printf("\n1. Ordenar com Insertion Sort\n");
        printf("2. Ordenar com Binary Insertion Sort\n");
        printf("3. Adicionar nome\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        if (opcao == 4) break; // Ajustado para sair

        switch (opcao) {
            case 1:
                strcpy(metodo, "insertion");
                break;
            case 2:
                strcpy(metodo, "binaryInsertion");
                break;
            case 3:
                printf("Executando adicionar nome \n");
                executar_programa("gcc -o tempCodeRunnerFile nomes.c");
                continue;
                break;

            default:
                printf("Opcao invalida!\n");
                continue;
        }

        // Menu para selecionar o arquivo de entrada, que define o caso automaticamente
        printf("\nSelecione o arquivo de entrada:\n");
        printf("1. Melhor caso \n");
        printf("2. Pior caso \n");
        printf("3. Caso medio \n");
        printf("Escolha uma opcao: ");
        int arquivoOpcao;
        scanf("%d", &arquivoOpcao);

        switch (arquivoOpcao) {
            case 1:
                nomeArquivo = "nomes_crescente.txt";
                caso = "melhor";
                break;
            case 2:
                nomeArquivo = "nomes_decrescente.txt";
                caso = "pior";
                break;
            case 3:
                nomeArquivo = "nomes_aleatorio.txt";
                caso = "medio";
                break;
            default:
                printf("Opcao invalida!\n");
                continue;
        }

        n = lerNomes(nomeArquivo, nomes);
        if (n == -1) continue; // Se houve erro na leitura

        // Executar a ordenação escolhida
        executarOrdenacao(nomes, n, metodo, caso);
    }
}

int main() {
    menu();
    return 0;
}
