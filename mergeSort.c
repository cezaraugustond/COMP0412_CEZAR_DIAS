#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void mergesort(int *A, int inicio, int fim);
void mergesort2(int *A, int n);
void merge(int *A, int inicio, int meio, int fim);

int main() {
    clock_t inicio, fim;
    double tempo_gasto;
    srand(time(NULL));

    int vezes = 100;
    int tamanhos[7] = {1000, 5000, 10000, 50000, 100000, 500000, 1000000};
    int quantidade = sizeof(tamanhos) / sizeof(tamanhos[0]);

    FILE *arquivo = fopen("mergeSort.csv", "w");

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
            mergesort2(A, tamanho);
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

void mergesort(int *A, int inicio, int fim) {
    int meio;
    if (inicio < fim) {
        meio = floor((inicio + fim) / 2);
        mergesort(A, inicio, meio);
        mergesort(A, meio + 1, fim);
        merge(A, inicio, meio, fim);
    }
}

void mergesort2(int *A, int n) {
    mergesort(A, 0, n - 1);
}

void merge(int *A, int inicio, int meio, int fim) {

    // Calculo do tamanho dos vetores auxiliares
    int tamanho1 = meio - inicio + 1;
    int tamanho2 = fim - meio;

    //Alocação dinâmica dos vetores auxiliares
    int *lista_esquerda = (int *) malloc(tamanho1 * sizeof(int));
    int *lista_direita = (int *) malloc(tamanho2 * sizeof(int));

    int j, i, k;

    if (lista_esquerda == NULL && lista_direita == NULL) {
        printf("Erro: falha na alocacao de memoria! \n");
        free(lista_direita);
        free(lista_esquerda);
        return;
    } 

    //Cópia da Divisão do vetor original nos dois vetores auxiliares
    for (i = 0; i < tamanho1; i++) {
        lista_esquerda[i] = A[inicio + i];
    }

    for (j = 0, k = meio + 1; j < tamanho2; j++, k++) {
        lista_direita[j] = A[k];
    }
 
    i = 0;
    j = 0;

    //Ordenação do vetor original com base nos vetores auxiliares
    for (k = inicio; i < tamanho1 && j < tamanho2; k++){
        if (lista_esquerda[i] <= lista_direita[j]) {
            A[k] = lista_esquerda[i];
            i++;
        } else {
            A[k] = lista_direita[j];
            j++;
        }    
    }
    // Faz a cópia do restante de um vetor caso o outro já tenha sido copiado por completo.
    while (i < tamanho1) {
        A[k] = lista_esquerda[i];
        i++;
        k++;
    }

    while (j < tamanho2) {
        A[k] = lista_direita[j];
        j++;
        k++;
    }

    free(lista_direita);
    free(lista_esquerda);
}
