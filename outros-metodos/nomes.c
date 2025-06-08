#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LENGTH 100000

// ignorando diferenças de maiúsculas e minúsculas
int caseInsensitiveCompare(const char *str1, const char *str2) {
    while (*str1 && *str2) {
        if (tolower((unsigned char)*str1) != tolower((unsigned char)*str2)) {
            return tolower((unsigned char)*str1) - tolower((unsigned char)*str2);
        }
        str1++;
        str2++;
    }
    return tolower((unsigned char)*str1) - tolower((unsigned char)*str2);
}

// Ordenação

void bubbleSort(char **names, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (caseInsensitiveCompare(names[j], names[j + 1]) > 0) {
                char *temp = names[j];
                names[j] = names[j + 1];
                names[j + 1] = temp;
            }
        }
    }
}

void reverseBubbleSort(char **names, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (caseInsensitiveCompare(names[j], names[j + 1]) < 0) {
                char *temp = names[j];
                names[j] = names[j + 1];
                names[j + 1] = temp;
            }
        }
    }
}

// embaralhar lista de nomes (modo aleatório)
void shuffleNames(char **names, size_t n) {
    srand(time(NULL));
    for (size_t i = n - 1; i > 0; i--) {
        size_t j = rand() % (i + 1);
        char *temp = names[i];
        names[i] = names[j];
        names[j] = temp;
    }
}

// Leitura e Escrita de Arquivos

void readNames(const char *filename, char ***names, size_t *n) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    *n = 0;
    size_t capacity = 10;
    *names = malloc(capacity * sizeof(char *));
    if (*names == NULL) {
        perror("Unable to allocate memory");
        exit(EXIT_FAILURE);
    }

    while (1) {
        if (*n >= capacity) {
            capacity *= 2;
            *names = realloc(*names, capacity * sizeof(char *));
            if (*names == NULL) {
                perror("Unable to allocate memory");
                exit(EXIT_FAILURE);
            }
        }
        (*names)[*n] = malloc(MAX_LENGTH * sizeof(char));
        if (fgets((*names)[*n], MAX_LENGTH, file) == NULL) {
            free((*names)[*n]);
            break;
        }
        (*names)[*n][strcspn((*names)[*n], "\n")] = '\0';
        (*n)++;
    }
    fclose(file);
}

void writeNamesToFile(const char *filename, char **names, size_t n) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Unable to open names file");
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < n; i++) {
        fprintf(file, "%s\n", names[i]);
    }
    fclose(file);
}

// adicionar um nome e manter a ordem 
void addNameKeepingOrder(const char *filename, int orderType) {
    char **names;
    size_t n;

    readNames(filename, &names, &n);

    char newName[MAX_LENGTH];
    printf("Digite o nome que deseja adicionar ao arquivo %s: ", filename);
    getchar();
    fgets(newName, MAX_LENGTH, stdin);
    newName[strcspn(newName, "\n")] = '\0';

    // Verifica se o nome já existe
    for (size_t i = 0; i < n; i++) {
        if (caseInsensitiveCompare(names[i], newName) == 0) {
            printf("Nome '%s' ja existe no arquivo.\n", newName);
            return;
        }
    }

    // Adiciona o novo nome e atualiza a lista
    names = realloc(names, (n + 1) * sizeof(char *));
    names[n] = malloc(MAX_LENGTH * sizeof(char));
    strcpy(names[n], newName);
    n++;

    // Aplica a ordenação ou embaralha conforme a ordem solicitada
    if (orderType == 1) { // Crescente
        bubbleSort(names, n);
    } else if (orderType == 2) { // Decrescente
        reverseBubbleSort(names, n);
    } else if (orderType == 3) { // Aleatório
        shuffleNames(names, n);
    }

    writeNamesToFile(filename, names, n);

    for (size_t i = 0; i < n; i++) {
        free(names[i]);
    }
    free(names);

    printf("Nome '%s' adicionado ao arquivo %s e os dados estão agora na ordem apropriada!\n", newName, filename);
}

// Função de menu principal
void printMenu() {
   printf("<-----------------------------------> \n");
    printf("          ADICIONAR NOME  \n");
   printf("<-----------------------------------> \n");
    printf("\n1. Adicionar Nome a um Arquivo\n");
    printf("2. Sair\n");
    
       
}

// Função principal
// Função principal
int main() {
    int running = 1;

    while (running) {
        printMenu();
        int choice;
        printf("Escolha uma opcao: ");
        while (scanf("%d", &choice) != 1) {
            printf("Entrada invalida! Por favor, insira um numero.\n");
            while (getchar() != '\n'); 
            printf("Escolha uma opcao: ");
        }

        if (choice == 2) {
            running = 0; // Isso encerra o loop e fecha o programa
            printf("Saindo do programa...\n");
            break;
        }

        if (choice == 1) {
            printf("Escolha o arquivo para adicionar um nome:\n");
            printf("1. Aleatorio\n");
            printf("2. Crescente\n");
            printf("3. Decrescente\n");
            int file_choice;
            printf("Escolha uma opcao: ");
            while (scanf("%d", &file_choice) != 1) {
                printf("Entrada invalida! Por favor, insira um numero.\n");
                while (getchar() != '\n');
                printf("Escolha uma opcao: ");
            }

            switch (file_choice) {
                case 1:
                    addNameKeepingOrder("nomes_aleatorio.txt", 3); // Aleatório
                    break;
                case 2:
                    addNameKeepingOrder("nomes_crescente.txt", 1); // Crescente
                    break;
                case 3:
                    addNameKeepingOrder("nomes_decrescente.txt", 2); // Decrescente
                    break;
                default:
                    printf("Opcao invalida!\n");
                    break;
            }
            continue;
        }

        printf("Opcao invalida!\n");
    }

    return 0; // Isso garante que o programa saia corretamente
}
