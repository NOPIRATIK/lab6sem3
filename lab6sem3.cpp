#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

typedef struct {
    int n;
    int** mat;
} Graph;

Graph* createGraph(int n) {
    Graph* g = (Graph*)malloc(sizeof(Graph));
    g->n = n;
    g->mat = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        g->mat[i] = (int*)calloc(n, sizeof(int));
    }
    return g;
}

void freeGraph(Graph* g) {
    for (int i = 0; i < g->n; i++) {
        free(g->mat[i]);
    }
    free(g->mat);
    free(g);
}

void randomGraph(Graph* g) {
    for (int i = 0; i < g->n; i++) {
        for (int j = i + 1; j < g->n; j++) {
            g->mat[i][j] = rand() % 2;
            g->mat[j][i] = g->mat[i][j];
        }
    }
}

void printMatrix(Graph* g) {
    printf("   ");
    for (int j = 0; j < g->n; j++) {
        printf("%2d ", j);
    }
    printf("\n");

    for (int i = 0; i < g->n; i++) {
        printf("%2d ", i);
        for (int j = 0; j < g->n; j++) {
            printf("%2d ", g->mat[i][j]);
        }
        printf("\n");
    }
}

void printAdjList(Graph* g) {
    for (int i = 0; i < g->n; i++) {
        printf("Вершина %d: ", i);
        for (int j = 0; j < g->n; j++) {
            if (g->mat[i][j] == 1) {
                printf("%d ", j);
            }
        }
        printf("\n");
    }
}

void identifyMatrix(Graph* g, int u, int v) {
    if (u == v || u < 0 || v < 0 || u >= g->n || v >= g->n) {
        printf("Ошибка: неверные вершины\n");
        return;
    }

    Graph* newG = createGraph(g->n - 1);

    int newI = 0;
    for (int i = 0; i < g->n; i++) {
        if (i == v) continue;

        int newJ = 0;
        for (int j = 0; j < g->n; j++) {
            if (j == v) continue;

            if (i == u || j == u) {
                newG->mat[newI][newJ] = g->mat[u][j] || g->mat[v][j] ||
                    g->mat[i][u] || g->mat[i][v];
            }
            else {
                newG->mat[newI][newJ] = g->mat[i][j];
            }
            newJ++;
        }
        newI++;
    }

    printf("Отождествление %d и %d:\n", u, v);
    printMatrix(newG);
    freeGraph(newG);
}

void contractMatrix(Graph* g, int u, int v) {
    if (g->mat[u][v] == 0) {
        printf("Ребро %d-%d не существует!\n", u, v);
        return;
    }
    printf("Стягивание ребра %d-%d:\n", u, v);
    identifyMatrix(g, u, v);
}

void splitMatrix(Graph* g, int v) {
    Graph* newG = createGraph(g->n + 1);
    int newV = newG->n - 1;

    for (int i = 0; i < g->n; i++) {
        for (int j = 0; j < g->n; j++) {
            newG->mat[i][j] = g->mat[i][j];
        }
    }

    for (int i = 0; i < g->n; i++) {
        if (g->mat[v][i] == 1) {
            newG->mat[newV][i] = 1;
            newG->mat[i][newV] = 1;
        }
    }

    newG->mat[v][newV] = 1;
    newG->mat[newV][v] = 1;

    printf("Расщепление вершины %d:\n", v);
    printf("Новая вершина: %d\n", newV);
    printMatrix(newG);
    freeGraph(newG);
}

void unionMatrix(Graph* g1, Graph* g2) {
    int newN = g1->n + g2->n;
    Graph* newG = createGraph(newN);

    for (int i = 0; i < g1->n; i++) {
        for (int j = 0; j < g1->n; j++) {
            newG->mat[i][j] = g1->mat[i][j];
        }
    }

    for (int i = 0; i < g2->n; i++) {
        for (int j = 0; j < g2->n; j++) {
            newG->mat[i + g1->n][j + g1->n] = g2->mat[i][j];
        }
    }

    printf("Объединение графов:\n");
    printMatrix(newG);
    freeGraph(newG);
}

void intersectMatrix(Graph* g1, Graph* g2) {
    int newN = (g1->n < g2->n) ? g1->n : g2->n;
    Graph* newG = createGraph(newN);

    for (int i = 0; i < newN; i++) {
        for (int j = 0; j < newN; j++) {
            newG->mat[i][j] = g1->mat[i][j] && g2->mat[i][j];
        }
    }

    printf("Пересечение графов:\n");
    printMatrix(newG);
    freeGraph(newG);
}

void ringSumMatrix(Graph* g1, Graph* g2) {
    int newN = (g1->n > g2->n) ? g1->n : g2->n;
    Graph* newG = createGraph(newN);

    for (int i = 0; i < newN; i++) {
        for (int j = 0; j < newN; j++) {
            int val1 = (i < g1->n && j < g1->n) ? g1->mat[i][j] : 0;
            int val2 = (i < g2->n && j < g2->n) ? g2->mat[i][j] : 0;
            newG->mat[i][j] = val1 ^ val2;
        }
    }

    printf("Кольцевая сумма:\n");
    printMatrix(newG);
    freeGraph(newG);
}

void cartesianMatrix(Graph* g1, Graph* g2) {
    int newN = g1->n * g2->n;
    Graph* newG = createGraph(newN);

    for (int i1 = 0; i1 < g1->n; i1++) {
        for (int j1 = 0; j1 < g1->n; j1++) {
            for (int i2 = 0; i2 < g2->n; i2++) {
                for (int j2 = 0; j2 < g2->n; j2++) {
                    int idx1 = i1 * g2->n + i2;
                    int idx2 = j1 * g2->n + j2;

                    if (i1 == j1 && g2->mat[i2][j2] == 1) {
                        newG->mat[idx1][idx2] = 1;
                    }
                    if (i2 == j2 && g1->mat[i1][j1] == 1) {
                        newG->mat[idx1][idx2] = 1;
                    }
                }
            }
        }
    }

    printf("Декартово произведение:\n");
    printMatrix(newG);
    freeGraph(newG);
}

void identifyList(Graph* g, int u, int v) {
    printf("Отождествление %d и %d (списки):\n", u, v);
    identifyMatrix(g, u, v);
    printf("Списки смежности:\n");
    printAdjList(g);
}

void contractList(Graph* g, int u, int v) {
    printf("Стягивание %d-%d (списки):\n", u, v);
    contractMatrix(g, u, v);
    printf("Списки смежности:\n");
    printAdjList(g);
}

void splitList(Graph* g, int v) {
    printf("Расщепление %d (списки):\n", v);
    splitMatrix(g, v);
    printf("Списки смежности:\n");
    printAdjList(g);
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(NULL));

    int n1, n2;
    printf("Вершин для G1: ");
    scanf("%d", &n1);
    printf("Вершин для G2: ");
    scanf("%d", &n2);

    Graph* g1 = createGraph(n1);
    Graph* g2 = createGraph(n2);

    randomGraph(g1);
    randomGraph(g2);

    printf("\nМатрица G1:\n");
    printMatrix(g1);
    printf("\nМатрица G2:\n");
    printMatrix(g2);

    printf("\nСписки G1:\n");
    printAdjList(g1);
    printf("\nСписки G2:\n");
    printAdjList(g2);

    printf("\n=== Унарные операции (матрицы) ===\n");

    int u, v;
    printf("Отождествление вершин (u v): ");
    scanf("%d %d", &u, &v);
    identifyMatrix(g1, u, v);

    printf("Стягивание ребра (u v): ");
    scanf("%d %d", &u, &v);
    contractMatrix(g1, u, v);

    printf("Расщепление вершины (v): ");
    scanf("%d", &v);
    splitMatrix(g1, v);
    printf("\n=== Унарные операции (списки) ===\n");

    printf("Отождествление вершин (u v): ");
    scanf("%d %d", &u, &v);
    identifyList(g1, u, v);

    printf("Стягивание ребра (u v): ");
    scanf("%d %d", &u, &v);
    contractList(g1, u, v);

    printf("Расщепление вершины (v): ");
    scanf("%d", &v);
    splitList(g1, v);

    printf("\n=== Бинарные операции ===\n");
    unionMatrix(g1, g2);
    intersectMatrix(g1, g2);
    ringSumMatrix(g1, g2);
    cartesianMatrix(g1, g2);

    freeGraph(g1);
    freeGraph(g2);

    return 0;
}