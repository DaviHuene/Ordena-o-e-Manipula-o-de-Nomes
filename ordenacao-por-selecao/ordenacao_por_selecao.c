#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h> // Para QueryPerformanceCounter

#define MAX_NOMES 10000
#define MAX_TAMANHO_NOME 100

// Funções auxiliares e de ordenação
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

void swap(char *a, char *b) {
    char temp[MAX_TAMANHO_NOME];
    strcpy(temp, a);
    strcpy(a, b);
    strcpy(b, temp);
}

void selectionSort(char nomes[][MAX_TAMANHO_NOME], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (strcmp(nomes[j], nomes[minIdx]) < 0) {
                minIdx = j;
            }
        }
        swap(nomes[i], nomes[minIdx]);
    }
}

void heapify(char nomes[][MAX_TAMANHO_NOME], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && strcmp(nomes[left], nomes[largest]) > 0) {
        largest = left;
    }
    if (right < n && strcmp(nomes[right], nomes[largest]) > 0) {
        largest = right;
    }
    if (largest != i) {
        swap(nomes[i], nomes[largest]);
        heapify(nomes, n, largest);
    }
}

void heapSort(char nomes[][MAX_TAMANHO_NOME], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(nomes, n, i);
    }
    for (int i = n - 1; i > 0; i--) {
        swap(nomes[0], nomes[i]);
        heapify(nomes, i, 0);
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
        nomes[i][strcspn(nomes[i], "\n")] = 0;
        i++;
    }
    fclose(file);
    return i;
}

void registrarTempoExecucao(const char *metodo, const char *caso, double tempo) {
    FILE *file = fopen("output/relatorio_tempo.txt", "a");
    if (!file) {
        printf("Erro ao abrir o arquivo de relatorio de tempo.\n");
        return;
    }

    fprintf(file, "Metodo: %s, Caso: %s, Tempo de execucao: %.5f segundos\n", metodo, caso, tempo);
    fclose(file);
}

void salvarNomes(const char *nomeArquivo, char nomes[][MAX_TAMANHO_NOME], int n) {
    FILE *file = fopen(nomeArquivo, "w");
    if (!file) {
        printf("Erro ao abrir o arquivo %s para escrita.\n", nomeArquivo);
        return;
    }

    for (int i = 0; i < n; i++) {
        fprintf(file, "%s\n", nomes[i]);
    }
    fclose(file);
}

void executarOrdenacao(char nomes[][MAX_TAMANHO_NOME], int n, const char *metodo, const char *caso) {
    LARGE_INTEGER start, end, frequency;
    QueryPerformanceFrequency(&frequency); // Frequência do contador
    QueryPerformanceCounter(&start); // Início

    if (strcmp(metodo, "selection") == 0) {
        selectionSort(nomes, n);
    } else if (strcmp(metodo, "heap") == 0) {
        heapSort(nomes, n);
    }

    QueryPerformanceCounter(&end); // Fim

    double tempo = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart; // Tempo em segundos
    printf("Tempo de execucao para %s Sort (%s caso): %.5f segundos\n", metodo, caso, tempo);
    registrarTempoExecucao(metodo, caso, tempo);

    char nomeArquivoSaida[50];
    sprintf(nomeArquivoSaida, "output/%s_%s.txt", metodo, caso);
    salvarNomes(nomeArquivoSaida, nomes, n);
}

// Função para exibir o menu
void menu() {
    int opcao, n = 0;
    char nomes[MAX_NOMES][MAX_TAMANHO_NOME];
    char metodo[10];
    char *nomeArquivo;
    char *caso;

    while (1) {
        printf("<===============================>\n");
        printf("     ORDENACAO POR Selecao\n");
        printf("<===============================>\n");

        printf("\n1. Ordenar com Selection Sort\n");
        printf("2. Ordenar com Heap Sort\n");
        printf("3. Adicionar nome\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        if (opcao == 4) break;

        switch (opcao) {
            case 1:
                strcpy(metodo, "selection");
                break;
            case 2:
                strcpy(metodo, "heap");
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

        printf("\nSelecione o arquivo de entrada:\n");
        printf("1. Melhor caso\n");
        printf("2. Pior caso\n");
        printf("3. Caso medio\n");
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
        if (n == -1) continue;

        executarOrdenacao(nomes, n, metodo, caso);
    }
}

int main() {
    menu();
    return 0;
}
