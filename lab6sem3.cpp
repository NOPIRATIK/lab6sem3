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
        printf("%2d ", j + 1);
    }
    printf("\n");

    for (int i = 0; i < g->n; i++) {
        printf("%2d ", i + 1);
        for (int j = 0; j < g->n; j++) {
            printf("%2d ", g->mat[i][j]);
        }
        printf("\n");
    }
}

void printAdjList(Graph* g) {
    for (int i = 0; i < g->n; i++) {
        printf("Вершина %d: ", i + 1);
        for (int j = 0; j < g->n; j++) {
            if (g->mat[i][j] == 1) {
                printf("%d ", j + 1);
            }
        }
        printf("\n");
    }
}

void identifyMatrix(Graph* g, int u, int v) {
    u = u - 1;
    v = v - 1;

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

    printf("Отождествление вершин %d и %d:\n", u + 1, v + 1);
    printMatrix(newG);
    freeGraph(newG);
}

void contractMatrix(Graph* g, int u, int v) {
    u = u - 1;
    v = v - 1;

    if (u < 0 || v < 0 || u >= g->n || v >= g->n) {
        printf("Ошибка: неверные вершины\n");
        return;
    }

    if (g->mat[u][v] == 0) {
        printf("Ребро %d-%d не существует!\n", u + 1, v + 1);
        return;
    }

    printf("Стягивание ребра %d-%d:\n", u + 1, v + 1);
    identifyMatrix(g, u + 1, v + 1);
}

void splitMatrix(Graph* g, int v) {
    int original_v = v;
    v = v - 1;

    if (v < 0 || v >= g->n) {
        printf("Ошибка: неверная вершина\n");
        return;
    }

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

    printf("Расщепление вершины %d:\n", original_v);
    printf("Новая вершина: %d\n", newV + 1);
    printMatrix(newG);
    freeGraph(newG);
}

void unionMatrix(Graph* g1, Graph* g2) {
    int max_n = (g1->n > g2->n) ? g1->n : g2->n;
    Graph* newG = createGraph(max_n);

    for (int i = 0; i < max_n; i++) {
        for (int j = 0; j < max_n; j++) {
            int edge1 = 0, edge2 = 0;

            if (i < g1->n && j < g1->n) {
                edge1 = g1->mat[i][j];
            }

            if (i < g2->n && j < g2->n) {
                edge2 = g2->mat[i][j];
            }

            newG->mat[i][j] = edge1 || edge2;
        }
    }

    printf("Объединение графов G1 и G2:\n");
    printf("Множество вершин: {");
    for (int i = 0; i < max_n; i++) {
        printf("%d", i + 1);
        if (i < max_n - 1) printf(", ");
    }
    printf("}\n");
    printMatrix(newG);
    freeGraph(newG);
}

void intersectMatrix(Graph* g1, Graph* g2) {
    int min_n = (g1->n < g2->n) ? g1->n : g2->n;
    Graph* newG = createGraph(min_n);

    for (int i = 0; i < min_n; i++) {
        for (int j = 0; j < min_n; j++) {
            newG->mat[i][j] = g1->mat[i][j] && g2->mat[i][j];
        }
    }

    printf("Пересечение графов G1 и G2:\n");
    printf("Общие вершины: 1-%d\n", min_n);
    printMatrix(newG);
    freeGraph(newG);
}

void ringSumMatrix(Graph* g1, Graph* g2) {
    int max_n = (g1->n > g2->n) ? g1->n : g2->n;
    Graph* newG = createGraph(max_n);

    for (int i = 0; i < max_n; i++) {
        for (int j = 0; j < max_n; j++) {
            int val1 = (i < g1->n && j < g1->n) ? g1->mat[i][j] : 0;
            int val2 = (i < g2->n && j < g2->n) ? g2->mat[i][j] : 0;
            newG->mat[i][j] = val1 ^ val2;
        }
    }

    printf("Кольцевая сумма G1 и G2:\n");
    printf("Вершины: 1-%d\n", max_n);
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

    printf("Декартово произведение G1 и G2:\n");
    printf("Всего вершин: %d (пары вида (i,j))\n", newN);
    printMatrix(newG);
    freeGraph(newG);
}

void identifyList(Graph* g, int u, int v) {
    printf("Отождествление вершин %d и %d (списки):\n", u, v);
    identifyMatrix(g, u, v);
    printf("Списки смежности:\n");
    printAdjList(g);
}

void contractList(Graph* g, int u, int v) {
    printf("Стягивание ребра %d-%d (списки):\n", u, v);
    contractMatrix(g, u, v);
    printf("Списки смежности:\n");
    printAdjList(g);
}

void splitList(Graph* g, int v) {
    printf("Расщепление вершины %d (списки):\n", v);
    splitMatrix(g, v);
    printf("Списки смежности:\n");
    printAdjList(g);
}

void showAvailableVertices(Graph* g) {
    printf("Доступные вершины: ");
    for (int i = 0; i < g->n; i++) {
        printf("%d ", i + 1);
    }
    printf("\n");
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(NULL));

    int n1, n2;
    printf("Количество вершин для G1: ");
    scanf("%d", &n1);
    printf("Количество вершин для G2: ");
    scanf("%d", &n2);

    Graph* g1 = createGraph(n1);
    Graph* g2 = createGraph(n2);

    randomGraph(g1);
    randomGraph(g2);

    printf("\nМатрица смежности G1:\n");
    printMatrix(g1);
    printf("\nМатрица смежности G2:\n");
    printMatrix(g2);

    printf("\nСписки смежности G1:\n");
    printAdjList(g1);
    printf("\nСписки смежности G2:\n");
    printAdjList(g2);

    printf("\n=== Унарные операции над G1 (матрицы) ===\n");

    int u, v;
    printf("Операция отождествления вершин:\n");
    showAvailableVertices(g1);
    printf("Введите номера вершин (u v): ");
    scanf("%d %d", &u, &v);
    identifyMatrix(g1, u, v);

    printf("\nОперация стягивания ребра:\n");
    showAvailableVertices(g1);
    printf("Введите номера вершин (u v): ");
    scanf("%d %d", &u, &v);
    contractMatrix(g1, u, v);

    printf("\nОперация расщепления вершины:\n");
    showAvailableVertices(g1);
    printf("Введите номер вершины: ");
    scanf("%d", &v);
    splitMatrix(g1, v);

    printf("\n=== Унарные операции над G1 (списки) ===\n");

    printf("Операция отождествления вершин:\n");
    showAvailableVertices(g1);
    printf("Введите номера вершин (u v): ");
    scanf("%d %d", &u, &v);
    identifyList(g1, u, v);

    printf("\nОперация стягивания ребра:\n");
    showAvailableVertices(g1);
    printf("Введите номера вершин (u v): ");
    scanf("%d %d", &u, &v);
    contractList(g1, u, v);

    printf("\nОперация расщепления вершины:\n");
    showAvailableVertices(g1);
    printf("Введите номер вершины: ");
    scanf("%d", &v);
    splitList(g1, v);

    printf("\n=== Бинарные операции над G1 и G2 ===\n");
    unionMatrix(g1, g2);
    intersectMatrix(g1, g2);
    ringSumMatrix(g1, g2);
    cartesianMatrix(g1, g2);

    freeGraph(g1);
    freeGraph(g2);

    return 0;
}