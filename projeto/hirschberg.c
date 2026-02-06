#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SEQ 10005

int *tabela_custo_minimo(char *X, int m, char *Y, int n, int gap, int mismatch, int match);
int backtracking(char *X, int m, char *Y, int n, int gap, int mismatch, int match, int *A, char *alin_X, char *alin_Y);
int *forward_space_efficient_alignment(char *X, int m, char *Y, int n, int gap, int mismatch, int match);
int *backward_space_efficient_alignment(char *X, int m, char *Y, int n, int gap, int mismatch, int match);
void divide_and_conquer_alignment(char *X, int m, char *Y, int n, int gap, int mismatch, int match, char **alin_x, char **alin_y, int *tam);
void caso_base(char *X, int m, char *Y, int n, int gap, int mismatch, int match, char **alin_x, char **alin_y, int *tam);
int encontrar_q(int *f, int *g, int m);
int custo(char a, char b, int mismatch, int match);
int minimo(int a, int b, int c);
int custo_total(char *alinh_x, char *alinh_y, int tamanho_alinh, int gap, int match, int mismatch);

int main() {

    static char seq_X[MAX_SEQ];
    static char seq_Y[MAX_SEQ];

    if (scanf("%s", seq_X) == EOF) return 0;
    if (scanf("%s", seq_Y) == EOF) return 0;

    int gap = 2;
    int mismatch = 1;
    int match = 0;

    char *alin_X = NULL, *alin_Y = NULL;
    int tamanho_final = 0;

    int m = strlen(seq_X);
    int n = strlen(seq_Y);

    divide_and_conquer_alignment(seq_X, m, seq_Y, n, gap, mismatch, match, &alin_X, &alin_Y, &tamanho_final);

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
    for(int i = 0; i <= m; i++) {
        matriz[i * (n + 1)] = i * gap;
    }

    // Caso Base: A[0][j]
    for(int j = 0; j <= n; j++) {
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
        char tx = alin_X[s]; alin_X[s] = alin_X[k-1-s]; alin_X[k-1-s] = tx;
        char ty = alin_Y[s]; alin_Y[s] = alin_Y[k-1-s]; alin_Y[k-1-s] = ty;
    }
    alin_X[k] = '\0'; alin_Y[k] = '\0';
    
    return k;
}

int *forward_space_efficient_alignment(char *X, int m, char *Y, int n, int gap, int mismatch, int match) {
    
    int *anterior = malloc((m + 1) * sizeof(int));
    int *atual = malloc((m + 1) * sizeof(int));

    if (anterior == NULL || atual == NULL) {
        printf("Erro ao alocar memoria \n");
        free(anterior);
        free(atual);
        return NULL;
    }

    // Inicializacao: coluna j=0 (Y vazio)
    for (int i = 0; i <= m; i++) {
        anterior[i] = i * gap;  //f(i, 0)
    }

    // Loop sobre colunas j = 1 ate n
    for (int j = 1; j <= n; j++) {
        // Caso base: linha i = 0 (X vazio)
        atual[0] = j * gap;// f(0, j)

        // Linhas i = 1 ate m
        for (int i = 1; i <= m; i++) {
            atual[i] = minimo(anterior[i-1] + custo(X[i-1], Y[j-1], mismatch, match), anterior[i] + gap, atual[i-1] + gap);
        }
        memcpy(anterior, atual, (m + 1) * sizeof(int)); //Copia a coluna atual para anterior
    }
    free(atual);
    return anterior; // Retorna a ultima coluna calculada
}

int *backward_space_efficient_alignment(char *X, int m, char *Y, int n, int gap, int mismatch, int match) {
    int *anterior = malloc((m + 1) * sizeof(int));
    int *atual = malloc((m + 1) * sizeof(int));
    if (anterior == NULL || atual == NULL) {
        printf("Erro ao alocar memoria \n");
        free(anterior);
        free(atual);
        return NULL;
    }

    // Inicializacao: coluna j = n (Y acabou - ultima coluna)
    for (int i = 0; i <= m; i++) {
        anterior[i] = (m - i) * gap;
    }

    // Loop: colunas j = n - 1 ate 0
    for (int j = n - 1; j >= 0; j--) {
        // Caso base: i = m (X acabou)
        // g(m, j) = (y - j) * gap;
        atual[m] = (n - j) * gap;

        // Linhas i = (m - 1) ate 0
        for (int i = m - 1; i >= 0; i--) {
            atual[i] = minimo(anterior[i+1] + custo(X[i], Y[j], mismatch, match), anterior[i] + gap, atual[i+1] + gap);
        }
        memcpy(anterior, atual, (m + 1) * sizeof(int));//Copia a coluna atual para anterior
    }
    free(atual);
    return anterior;
}

void divide_and_conquer_alignment(char *X, int m, char *Y, int n, int gap, int mismatch, int match, char **alin_x, char **alin_y, int *tam) {
    
    // Caso base: resolver com Needleman-Wusch
    if (n <= 1 || m == 0) { 
        caso_base(X, m, Y, n, gap, mismatch, match, alin_x, alin_y, tam);
        return;
    }

    //1. Encontrar coluna do meio
    int mid = n / 2;

    //2. Calcular forward até coluna do meio
    int *f = forward_space_efficient_alignment(X, m, Y, mid, gap, mismatch, match);
    if (f == NULL) return;

    //3. Calcular backward até coluna do meio
    int *g = backward_space_efficient_alignment(X, m, Y + mid, n - mid, gap, mismatch, match);
    if (g == NULL) {
        free(f);
        return;
    }

    //4. Encontrar q que minimiza f[q] + g[q]
    int q = encontrar_q(f, g, m);
    free(f); free(g);

    char *ax_esq, *ay_esq, *ax_dir, *ay_dir;
    int t_esq, t_dir;

    //5. Dividir e conquistar recursivamente

    // Parte superior esquerda: X[0 ... q] com Y[0 ... coluna_meio]
    divide_and_conquer_alignment(X, q, Y, mid, gap, mismatch, match, &ax_esq, &ay_esq, &t_esq);
    
    // Parte inferior direita: X[q+1 ... m] com Y[coluna_meio+1 ... n]
    divide_and_conquer_alignment(X + q, m - q, Y + mid, n - mid, gap, mismatch, match, &ax_dir, &ay_dir, &t_dir);

    //6. Combinar resultados
    *tam = t_esq + t_dir;
    *alin_x = malloc((*tam + 1) * sizeof(char));
    *alin_y = malloc((*tam + 1) * sizeof(char));
    
    memcpy(*alin_x, ax_esq, t_esq);
    memcpy(*alin_x + t_esq, ax_dir, t_dir + 1);
    memcpy(*alin_y, ay_esq, t_esq);
    memcpy(*alin_y + t_esq, ay_dir, t_dir + 1);

    free(ax_esq); free(ay_esq); free(ax_dir); free(ay_dir);
}

void caso_base(char *X, int m, char *Y, int n, int gap, int mismatch, int match, char **alin_x, char **alin_y, int *tam) {
    int *matriz = tabela_custo_minimo(X, m, Y, n, gap, mismatch, match);
    *alin_x = malloc((m + n + 1) * sizeof(char));
    *alin_y = malloc((m + n + 1) * sizeof(char));
    *tam = backtracking(X, m, Y, n, gap, mismatch, match, matriz, *alin_x, *alin_y);
    free(matriz);
}

int encontrar_q(int *f, int *g, int m) {
    int q = 0;
    int menor_soma = f[0] + g[0];
    for (int i = 1; i <= m; i++) {
        int soma = f[i] + g[i];
        if (soma < menor_soma) {
            menor_soma = soma;
            q = i;
        }
    }
    return q;
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
