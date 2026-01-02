typedef struct {
    int weight;
    int value;
    int original_index;
} Item;
#include <stdio.h>
#include <stdlib.h>

int read_instance(const char *filename, int *n, int *capacity, Item **items) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror("Erreur ouverture fichier");
        return 0;
    }

    /* Lecture de n et capacity */
    if (fscanf(f, "%d %d", n, capacity) != 2) {
        fclose(f);
        return 0;
    }

    /* Allocation du tableau d'items */
    *items = (Item *)malloc((*n) * sizeof(Item));
    if (!*items) {
        fclose(f);
        return 0;
    }

    /* Lecture des objets */
    for (int i = 0; i < *n; i++) {
        if (fscanf(f, "%d %d",
                   &(*items)[i].weight,
                   &(*items)[i].value) != 2) {
            free(*items);
            fclose(f);
            return 0;
        }
        (*items)[i].original_index = i;
    }

    fclose(f);
    return 1;  // succès
}

int sort_items_by_ratio(Item *items, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            double ratio_j = items[j].weight > 0 ? (double)items[j].value / items[j].weight : 0;
            double ratio_j1 = items[j + 1].weight > 0 ? (double)items[j + 1].value / items[j + 1].weight : 0;
            if (ratio_j < ratio_j1) {
                Item temp = items[j];
                items[j] = items[j + 1];
                items[j + 1] = temp;
            }
        }
    }
    return 0;
}

int knapsack_glouton(int n, int capacity, Item *items, int *taken) {
    int total_value = 0;
    int remaining_capacity = capacity;
    sort_items_by_ratio(items, n);
    for (int i = 0; i < n; i++) {
        taken[i] = 0;
    }
    for (int i = 0; i < n; i++) {
        if (items[i].weight <= 0) continue; // Ignore poids nul ou négatif
        if (items[i].weight <= remaining_capacity) {
            total_value += items[i].value;
            remaining_capacity -= items[i].weight;
            taken[items[i].original_index] = 1;
        }
    }
    return total_value;
}

int main(void) {
    for (int file_idx = 1; file_idx <= 6; file_idx++) {
        char filename[64];
        snprintf(filename, sizeof(filename), "knapsack_instance_%d.txt", file_idx);
        int n, capacity;
        Item *items;
        printf("\n==== %s ====\n", filename);
        if (!read_instance(filename, &n, &capacity, &items)) {
            printf("Erreur de lecture\n");
            continue;
        }

        printf("n = %d, capacity = %d\n", n, capacity);
        for (int i = 0; i < n; i++) {
            printf("Item %d : weight=%d value=%d\n",
                   i, items[i].weight, items[i].value);
        }
        int *taken = (int *)calloc(n, sizeof(int));
        int max_value = knapsack_glouton(n, capacity, items, taken);
        printf("Valeur maximale (glouton) : %d\n", max_value);
        printf("Objets pris (indices d'origine) : ");
        for (int i = 0; i < n; i++) {
            if (taken[i]) printf("%d ", i);
        }
        printf("\n");
        free(taken);
        free(items);
    }
    return 0;
}


