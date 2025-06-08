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

// Função Merge Sort
void merge(char nomes[][MAX_TAMANHO_NOME], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge(nomes, left, mid);
        merge(nomes, mid + 1, right);
        
        // Merging
        int n1 = mid - left + 1;
        int n2 = right - mid;
        char leftArr[n1][MAX_TAMANHO_NOME], rightArr[n2][MAX_TAMANHO_NOME];
        
        for (int i = 0; i < n1; i++) strcpy(leftArr[i], nomes[left + i]);
        for (int j = 0; j < n2; j++) strcpy(rightArr[j], nomes[mid + 1 + j]);
        
        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (strcmp(leftArr[i], rightArr[j]) <= 0) {
                strcpy(nomes[k], leftArr[i]);
                i++;
            } else {
                strcpy(nomes[k], rightArr[j]);
                j++;
            }
            k++;
        }

        while (i < n1) {
            strcpy(nomes[k], leftArr[i]);
            i++;
            k++;
        }

        while (j < n2) {
            strcpy(nomes[k], rightArr[j]);
            j++;
            k++;
        }
    }
}

// Função Bucket Sort
void bucketSort(char nomes[][MAX_TAMANHO_NOME], int n) {
    int i, j;
    char **buckets = malloc(n * sizeof(char *));
    for (i = 0; i < n; i++) {
        buckets[i] = malloc(MAX_TAMANHO_NOME * sizeof(char));
    }

    // Sort nomes using bucket sort logic (simplified here)
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (strcmp(nomes[i], nomes[j]) > 0) {
                swap(nomes[i], nomes[j]);
            }
        }
    }

    free(buckets);
}

// Função para realizar o Radix Sort
void countingSort(char nomes[][MAX_TAMANHO_NOME], int n, int exp) {
    char output[n][MAX_TAMANHO_NOME];
    int count[256] = {0};

    for (int i = 0; i < n; i++) {
        count[(nomes[i][exp] & 0xFF)]++;
    }

    for (int i = 1; i < 256; i++) {
        count[i] += count[i - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        int idx = (nomes[i][exp] & 0xFF);
        strcpy(output[count[idx] - 1], nomes[i]);
        count[idx]--;
    }

    for (int i = 0; i < n; i++) {
        strcpy(nomes[i], output[i]);
    }
}

void radixSort(char nomes[][MAX_TAMANHO_NOME], int n) {
    int maxLen = 0;
    for (int i = 0; i < n; i++) {
        if (strlen(nomes[i]) > maxLen) {
            maxLen = strlen(nomes[i]);
        }
    }

    for (int exp = maxLen - 1; exp >= 0; exp--) {
        countingSort(nomes, n, exp);
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
        printf("Erro ao abrir o arquivo %s para escrita.\n", nomeArquivo);
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

    if (strcmp(metodo, "merge") == 0) {
        merge(nomes, 0, n - 1);
    } else if (strcmp(metodo, "bucket") == 0) {
        bucketSort(nomes, n);
    } else if (strcmp(metodo, "radix") == 0) {
        radixSort(nomes, n);
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
    char metodo[10];
    char *nomeArquivo;
    char *caso;

    while (1) {
        printf("<===============================>\n");
        printf("          OUTROS METODOS\n");
        printf("<===============================>\n");

        printf("\n1. Ordenar com Merge Sort\n");
        printf("2. Ordenar com Bucket Sort\n");
        printf("3. Ordenar com Radix Sort\n");
        printf("4. Adicionar nome\n");
        printf("5. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        if (opcao == 5) break;

        switch (opcao) {
            case 1:
                strcpy(metodo, "merge");
                break;
            case 2:
                strcpy(metodo, "bucket");
                break;
            case 3:
                strcpy(metodo, "radix");
                break;
            case 4:
                printf("Executando adicionar nome \n");
                executar_programa("gcc -o tempCodeRunnerFile nomes.c");
                continue;
                break;
            default:
                printf("Opcao invalida!\n");
                continue;
        }

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
        if (n == -1) continue;

        executarOrdenacao(nomes, n, metodo, caso);
    }
}

int main() {
    menu();
    return 0;
}
