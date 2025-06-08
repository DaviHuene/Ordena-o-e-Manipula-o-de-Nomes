#include <stdio.h>
#include <stdlib.h>

void menu() {
    printf("<===================>\n");
    printf("  Menu Principal\n");
    printf("<===================>\n");
    printf("1. Ordenacao por Troca\n");
    printf("2. Ordenacao por Selecao\n");
    printf("3. Ordenacao por Insercao\n");
    printf("4. Outros Metodos\n");
    printf("5. Adicionar nome\n");
    printf("6. Sair\n");
    printf("Escolha uma opcao: ");
}

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

int main() {
    int opcao;

    do {
        menu();
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                printf("Executando Ordenacao por Troca\n");
                executar_programa("gcc -o tempCodeRunnerFile ordenacao-por-troca/ordenacao_por_troca.c");
                continue;
                break;

            case 2:
                printf("Executando Ordenacao por Selecao\n");
                executar_programa("gcc -o tempCodeRunnerFile ordenacao-por-selecao/ordenacao_por_selecao.c");
                continue;
                break;

            case 3:
                printf("Executando Ordenacao por Insercao\n");
                executar_programa("gcc -o tempCodeRunnerFile ordenacao-por-insercao/ordenacao_por_insercao.c");
                continue;
                break;

            case 4:
                printf("Executando outros metodos de ordenacao\n");
                executar_programa("gcc -o tempCodeRunnerFile outros-metodos/outro_metodos.c");
                continue;
                break;

            case 5:
                printf("Executando adicionar nome \n");
                executar_programa("gcc -o tempCodeRunnerFile nomes.c");
                continue;
                break;

            case 6:
                printf("Saindo do programa\n");
                break;

            default:
                printf("Opcao invalida! Tente novamente\n");
                break;
        }
    } while (opcao != 6);

    return 0;
}
