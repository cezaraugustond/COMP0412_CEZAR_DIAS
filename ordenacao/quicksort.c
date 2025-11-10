#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void quicksort(int *A, int n);
void quicksort2(int *A, int inicio, int fim);
int particiona(int *A, int inicio, int fim);

int main() {
    clock_t inicio, fim;
    double tempo_gasto;
    srand(time(NULL));

    int vezes = 100;
    int tamanhos[7] = {1000, 5000, 10000, 50000, 100000, 500000, 1000000};
    int quantidade = sizeof(tamanhos) / sizeof(tamanhos[0]);

    FILE *arquivo = fopen("quickSort.csv", "w");

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
            quicksort(A, tamanho);
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

void quicksort(int *A, int n) {
    quicksort2(A, 0, n - 1);
}

void quicksort2(int *A, int inicio, int fim) {
    int indice;
    if (inicio < fim){
        indice = particiona(A, inicio, fim);
        quicksort2(A, inicio, indice - 1);
        quicksort2(A, indice + 1, fim);
    }
}

int particiona(int *A, int inicio, int fim) {

    //Pega o pivô sendo o último elemento da lista
    int pivot = A[fim];

    /*i serve para marcar o índice entre os elemento que são menores ou igual que o pivô.
    Porém, quando nenhum elemento foi comparado, o índice deve começar antes do primeiro elemento.
    Ou seja, o "inicio - 1" garante que o i só será incrementado se o elemento inicial for menor ou igual ao pivô. */
    int i = inicio - 1; 

    int aux, j;
    for (j = inicio; j < fim; j++){
        if (A[j] <= pivot) {
            i++;
            /*Troca o elemento correspondente do índice j com o elemente correspondente do índice i
            para que os elementos menores ou iguais ao pivô fiquem a esquerda.*/ 
            aux = A[j];
            A[j] = A[i];
            A[i] = aux;
        }
    }
    /*Troca o elemento do A[fim] com o elemento do A[i+1], pois como os elementos até i são menores que o pivô, 
    o elemento do pivô irá para o índice i+1, e elemento que estava nessa posição irá para última para se tornar 
    o novo pivô*/
    aux = A[i+1];
    A[i+1] = A[fim];
    A[fim] = aux;
    return i + 1;
}
