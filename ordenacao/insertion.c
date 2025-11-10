#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void insertion(int *A, int n);

int main() {
    clock_t inicio, fim;
    double tempo_gasto;
    srand(time(NULL));

    int vezes = 100;
    int tamanhos[5] = {1000, 5000, 10000, 50000, 100000};
    int quantidade = sizeof(tamanhos) / sizeof(tamanhos[0]);

    FILE *arquivo = fopen("insertion.csv", "w");

    if (arquivo == NULL) {
        printf("Erro ao criar arquivo! \n");
        return 1;
    }

    fprintf(arquivo, "Vez,Quantidade,Tempo(segundos)\n");

    for(int i = 0; i < quantidade; i++){
        int tamanho = tamanhos[i];
        for(int k = 0; k < vezes; k++){
            int *A = (int *) malloc(tamanho * sizeof(int));
            if (A != NULL) {
                for(int j = 0; j < tamanho; j++){
                    A[j] = rand() % 100000;
                }
            } else {
                printf("Erro: alocar memoria! \n");
                fclose(arquivo);
                return 1;
            }
            inicio = clock();
            insertion(A, tamanho);
            fim = clock();
            tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
            free(A);
            fprintf(arquivo, "%d,%d,%.5f\n", k+1, tamanho, tempo_gasto);
        }
    }

    printf("Arquivo gerado! \n");

    fclose(arquivo);

    return 0;
}

void insertion(int *A, int n) {
    int chave, i, j;
    for(i = 1; i < n; i++){
        chave = A[i];
        j = i - 1;
        // Move todos os elementos maiores que a chave para direita
        while(j >= 0 && A[j] > chave){
            A[j + 1] = A[j];
            j--;
        }
        // Insere a chave no lugar "vago".
        A[j+1] = chave;
    }
}
