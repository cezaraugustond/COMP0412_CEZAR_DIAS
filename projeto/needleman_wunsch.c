#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *tabela_custo_minimo(char *X, int m, char *Y, int n, int gap, int mismatch, int match);
int backtracking(char *X, int m, char *Y, int n, int gap, int mismatch, int match, int *A, char *alin_X, char *alin_Y);
int custo(char a, char b, int mismatch, int match);
int minimo(int a, int b, int c);
int custo_total(char *alinh_x, char *alinh_y, int tamanho_alinh, int gap, int match, int mismatch);

int main() {
    char seq_X[] = "GGGCTTCAGGGAATAAAACAATTTATGTTAAGTAAAGTGCGGAGGAGAGGATGATGATCTGTTAAAGGCGTTCAATTGGTGTACCCTAGCGACCGCACTC";
    char seq_Y[] = "CGGTAAGAGGACTCCCTTCAAGTGCAATCGTTGAGCAAAATTGGAGAGACGTCATGCACGGAGTATGCCAGGAATAAGTGGGTGTCAGTCCTAGCACTCG";

    int gap = 2;
    int mismatch = 1;
    int match = 0;

    int m = strlen(seq_X);
    int n = strlen(seq_Y);

    char *alin_X = malloc((m + n + 1) * sizeof(char));
    char *alin_Y = malloc((m + n + 1) * sizeof(char));
    int tamanho_final = 0;

    int *matriz = tabela_custo_minimo(seq_X, m, seq_Y, n, gap, mismatch, match);

    tamanho_final = backtracking(seq_X, m, seq_Y, n, gap, mismatch, match, matriz, alin_X, alin_Y);

    int score = custo_total(alin_X, alin_Y, tamanho_final, gap, match, mismatch);

    printf("%d\n", score);
    printf("%s\n", alin_X);
    printf("%s\n", alin_Y);

    free(alin_X);
    free(alin_Y);

    return 0;
}

int *tabela_custo_minimo(char *X, int m, char *Y, int n, int gap, int mismatch, int match) {

    int *matriz = malloc((m + 1) * (n + 1) * sizeof(int));

    if (matriz == NULL) {
        printf("Erro ao alocar memoria para matriz de custos \n");
        return NULL;
    }

    // Caso Base: A[i][0]
    for(int i = 0; i < m; i++) {
        matriz[i * (n + 1)] = i * gap;
    }

    // Caso Base: A[0][j]
    for(int j = 0; j < n; j++) {
        matriz[j] = j * gap;
    }
 
    // Preenchimento do restante
    for(int i = 1; i <= m; i++) {
        for(int j = 1; j <= n; j++) {
            int custo_diagonal = matriz[(i-1) * (n+1) + (j-1)] + custo(X[i-1], Y[j-1], mismatch, match);
            int custo_cima = matriz[(i-1) * (n+1) + j] + gap;
            int custo_esquerda = matriz[i * (n+1) + (j-1)] + gap;
            matriz[i * (n + 1) + j] = minimo(custo_diagonal, custo_cima, custo_esquerda);
        }
    }

    return matriz;
}

int backtracking(char *X, int m, char *Y, int n, int gap, int mismatch, int match, int *A, char *alin_X, char *alin_Y) {
    
    int i = m;
    int j = n;
    int k = 0;
    int colunas = n + 1;

    while (i > 0 && j > 0) {

        int custo_atual = A[i * colunas + j];
        int score_diag = custo(X[i-1], Y[j-1], mismatch, match);

        int custo_diagonal = A[(i-1) * colunas + (j-1)] + score_diag;
        int custo_cima = A[(i-1) * colunas + j] + gap;

        if (custo_atual == custo_diagonal){
            alin_X[k] = X[i-1];
            alin_Y[k] = Y[j-1];
            i--; 
            j--;
        }
        else if (custo_atual == custo_cima) {
            alin_X[k] = X[i-1];
            alin_Y[k] = '-';
            i--;
        }
        else {
            alin_X[k] = '-';
            alin_Y[k] = Y[j-1];
            j--;
        }
        k++;
    }

    // Preenche gaps restantes
    while (i > 0) { 
        alin_X[k] = X[i-1]; 
        alin_Y[k] = '-'; 
        i--; 
        k++; 
    }

    while (j > 0) { 
        alin_X[k] = '-'; 
        alin_Y[k] = Y[j-1]; 
        j--; 
        k++; 
    }

    // Inversão das strings para ordem correta
    for (int s = 0; s < k / 2; s++) {
        char tx = alin_X[s]; 
        alin_X[s] = alin_X[k-1-s]; 
        alin_X[k-1-s] = tx;

        char ty = alin_Y[s]; 
        alin_Y[s] = alin_Y[k-1-s]; 
        alin_Y[k-1-s] = ty;
    }
    alin_X[k] = '\0'; alin_Y[k] = '\0';
    
    return k;
}

int custo(char a, char b, int mismatch, int match) {
    if (a == b)
        return match;
    else
        return mismatch;
}

int minimo(int a, int b, int c) {
    if (a <= b && a <= c) return a;
    if (b <= c) return b;
    return c;
}

int custo_total(char *alinh_x, char *alinh_y, int tamanho_alinh, int gap, int match, int mismatch) {
    int custo_acumulado = 0;
    for (int i = 0; i < tamanho_alinh; i++) {
        // Se houver um gap em qualquer uma das sequências
        if (alinh_x[i] == '-' || alinh_y[i] == '-') {
            custo_acumulado += gap;
        } 
        // Se as bases forem iguais (Match)
        else if (alinh_x[i] == alinh_y[i]) {
            custo_acumulado += match;
        } 
        // Se as bases forem diferentes (Mismatch)
        else {
            custo_acumulado += mismatch;
        }
    }
    return custo_acumulado;
}
