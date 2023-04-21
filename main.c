#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct {
    int cor;
    int passou;
    int teste;
} posicao;

typedef struct {
    char canto;
    int cor;
} passo;


posicao ** lerArquivo(int *n, int *m, int *k, FILE* arquivo){
    
    if(arquivo == NULL){
        perror("Erro ao ler o arquivo");
        exit(1);
    }

    // Coletando a linha de parametros
    fscanf(arquivo, "%d %d %d", n, m, k);

    // Matriz com as cores
    posicao ** matriz = (posicao**)calloc(*n, sizeof(posicao*));
    if(matriz == NULL){
        perror("Erro ao alocar vetor de int");
        exit(1);
    }
    for(int i = 0; i < *n; i++){
        matriz[i] = (posicao*)calloc(*m, sizeof(posicao));
        if(matriz[i] == NULL){
            perror("Erro ao alocar vetor de int");
            exit(1);
        }
    }

    // Coletando a matriz
    for(int i = 0; i < *n; i++)
        for(int l = 0; l < *m; l++){
            fscanf(arquivo, "%d", &matriz[i][l].cor);
            matriz[i][l].teste = matriz[i][l].cor;
        }

    fclose(arquivo);

    return matriz;
}

// Verifica se a posição esta dentro da matriz e se a posição da 
// matriz tem uma cor igual a da primeira posição
bool is_valid(int i, int j, int val, posicao ** mat, int m, int n) {
    return i >= 0 && i < n && j >= 0 && j < m && mat[i][j].cor == val;
}

// Verifica se a posição esta dentro da matriz e se a posição da 
// matriz tem uma cor igual a da primeira posição
bool is_valid2(int i, int j, int val, posicao ** mat, int m, int n) {
    return i >= 0 && i < n && j >= 0 && j < m && mat[i][j].teste == val;
}

// Busca em profundidade, retorna a maior profundidade
void dfs(int i, int j, int val, posicao ** mat, int m, int n, int cor) {
    int *stack_i = (int *)calloc(m*n, sizeof(int));
    if(stack_i == NULL){
        perror("Erro ao alocar vetor de int");
        exit(1);
    }
    int *stack_j = (int *)calloc(m*n, sizeof(int));
    if(stack_j == NULL){
        perror("Erro ao alocar vetor de int");
        exit(1);
    }
    int stack_top = 0;

    stack_i[stack_top] = i;
    stack_j[stack_top] = j;

    while (stack_top >= 0) {
        int ci = stack_i[stack_top];
        int cj = stack_j[stack_top];
        stack_top--;

        if (is_valid(ci, cj, val, mat, m, n)) {
            mat[ci][cj].teste = -1;
            mat[ci][cj].cor = cor;

            int di[] = {-1, 0, 1, 0};
            int dj[] = {0, 1, 0, -1};
            for (int d = 0; d < 4; d++) {
                int ni = ci + di[d];
                int nj = cj + dj[d];
                if (is_valid(ni, nj, val, mat, m, n)) {
                    stack_top++;
                    stack_i[stack_top] = ni;
                    stack_j[stack_top] = nj;
                }
            }
        }
    }

    free(stack_i);
    free(stack_j);
}

// Busca em profundidade, retorna o numero de casas preenchidas
int dfs2(int i, int j, int val, posicao ** mat, int m, int n) {
    int *stack_i = (int *)calloc(m*n, sizeof(int));
    if(stack_i == NULL){
        perror("Erro ao alocar vetor de int");
        exit(1);
    }
    int *stack_j = (int *)calloc(m*n, sizeof(int));
    if(stack_j == NULL){
        perror("Erro ao alocar vetor de int");
        exit(1);
    }
    int stack_top = 0;
    int preenchidos = 0;

    stack_i[stack_top] = i;
    stack_j[stack_top] = j;

    while (stack_top >= 0) {
        int ci = stack_i[stack_top];
        int cj = stack_j[stack_top];
        stack_top--;

        if (is_valid(ci, cj, val, mat, m, n)) {

            if(mat[ci][cj].passou != -1){
                mat[ci][cj].passou = -1;
                preenchidos++;
            }
            
            int di[] = {-1, 0, 1, 0};
            int dj[] = {0, 1, 0, -1};
            for (int d = 0; d < 4; d++) {
                int ni = ci + di[d];
                int nj = cj + dj[d];
                if (is_valid(ni, nj, val, mat, m, n) && mat[ni][nj].passou != -1) {
                    stack_top++;
                    stack_i[stack_top] = ni;
                    stack_j[stack_top] = nj;
                    mat[ni][nj].passou = -1;
                    preenchidos++;
                }
            }
        }
    }

    free(stack_i);
    free(stack_j);

    return preenchidos;
}

passo * search_corners(posicao ** mat, int n, int m, int k, int *n_passos) {
    passo * vet_passos = (passo *)calloc(n*m, sizeof(passo));
    if(vet_passos == NULL){
        perror("Erro ao alocar vetor de passos");
        exit(1);
    }
    posicao ** matriz1 = (posicao**)calloc(n, sizeof(posicao*));
    // Matriz aux
    if(matriz1 == NULL){
        perror("Erro ao alocar vetor de int");
        exit(1);
    }
    for(int i = 0; i < n; i++){
        matriz1[i] = (posicao*)calloc(m, sizeof(posicao));
        if(matriz1[i] == NULL){
            perror("Erro ao alocar vetor de int");
            exit(1);
        }
    }
    int preenchidos = 0;
    int passos = 0;
    while(preenchidos < m*n){
        for(int i = 0; i < n; i++)
            for(int l = 0; l < m; l++)
                matriz1[i][l].cor = mat[i][l].cor;

        int max_depth2 = -1;
        int chosen_i = -1, chosen_j = -1, chosen_val = -1;
        int di[] = {0, 0, n-1, n-1};
        int dj[] = {0, m-1, 0, m-1};
        // Testando todas as cores
        for (int val = 1; val <= k; val++) {
            // Passando pelos 4 cantos
            for (int d = 0; d < 4; d++){
                // Verifica se a cor escolhida ja não esta no canto
                if (mat[di[d]][dj[d]].cor != val) {    
                    // Faz a busca 
                    dfs(di[d], dj[d], mat[di[d]][dj[d]].cor, mat, m, n, val);
                    int depth2 = dfs2(di[d], dj[d], val, mat, m, n);
                    for(int i = 0; i < n; i++)
                        for(int l = 0; l < m; l++){
                            mat[i][l].cor = matriz1[i][l].cor;
                            mat[i][l].passou = 0;
                            mat[i][l].teste = matriz1[i][l].cor;
                        }
                    // Se a busca encontrou um caminho mais fundo que o anterior, salva
                    if (depth2 > max_depth2) {
                        max_depth2 = depth2;
                        chosen_i = di[d];
                        chosen_j = dj[d];
                        chosen_val = val;
                    }
                }
                for(int i = 0; i < n; i++)
                    for(int l = 0; l < m; l++){
                        mat[i][l].cor = matriz1[i][l].cor;
                        mat[i][l].passou = 0;
                        mat[i][l].teste = matriz1[i][l].cor;
                    }
            }
        }

        dfs(chosen_i, chosen_j, mat[chosen_i][chosen_j].cor, mat, m, n, chosen_val);
        preenchidos = dfs2(chosen_i, chosen_j, chosen_val, mat, m, n);

        for(int i = 0; i < n; i++)
            for(int l = 0; l < m; l++)
                if(mat[i][l].passou == -1){
                    mat[i][l].cor = chosen_val;
                    mat[i][l].teste = chosen_val;
                }

        
        if(chosen_i == 0 && chosen_j == 0)
            vet_passos[passos].canto = 'a';
        else if(chosen_i == 0 && chosen_j == m-1)
            vet_passos[passos].canto = 'b';
        else if(chosen_i == n-1 &&  chosen_j == 0)
            vet_passos[passos].canto = 'd';
        else if(chosen_i == n-1 &&  chosen_j == m-1)
            vet_passos[passos].canto = 'c';
        vet_passos[passos].cor = chosen_val;
        passos++;
    }

    for (int i = 0; i < n; i++)
        free(matriz1[i]);
    free(matriz1);
    *n_passos = passos;
    return vet_passos;
}

int main(int argc, char **argv){
    int n = 0;
    int m = 0;
    int k = 0;
    int n_passos = 0;
    posicao ** arquivo;
    FILE * nome_arquivo;

    if(argc > 1)
        nome_arquivo = fopen(argv[1], "r");
    else 
        nome_arquivo = stdin;

    arquivo = lerArquivo(&n, &m, &k, nome_arquivo);

    passo * vet_passos = search_corners(arquivo, n, m, k, &n_passos);

    printf("%d\n", n_passos);
    for(int i = 0; i < n_passos; i++)
        printf("%c %d ", vet_passos[i].canto, vet_passos[i].cor);
    printf("\n");

    for (int i = 0; i < n; i++)
        free(arquivo[i]);
    free(arquivo);
    free(vet_passos);

    return 0;
}