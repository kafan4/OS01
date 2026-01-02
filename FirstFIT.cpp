#include <iostream>
#include <queue>

#include <cstring>
#include <ctime>
#include <cstdlib>
#include <cmath>

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

// Heuristique First-Fit
unsigned int first_fit(const unsigned *weights, int n, unsigned int bin_capacity, double &time) {
    unsigned int* bins = (unsigned int*)calloc(n, sizeof(unsigned int));
    unsigned int bins_ff = 0;
    clock_t start_ff = clock();
    for (int i = 0; i < n; i++) {
        int placed = 0;
        for (unsigned int j = 0; j < bins_ff; j++) {
            if (bins[j] + weights[i] <= bin_capacity) {
                bins[j] += weights[i];
                placed = 1;
                break;
            }
        }
        if (!placed) {
            bins[bins_ff] = weights[i];
            bins_ff++;
        }
    }
    clock_t end_ff = clock();
    time = (end_ff - start_ff) / (double)CLOCKS_PER_SEC;
    free(bins);
    return bins_ff;
}

// Heuristique First-Fit-Decreasing
unsigned int first_fit_decreasing(const unsigned *weights, int n, unsigned int bin_capacity, double &time) {
    unsigned *weights_sorted = (unsigned *)malloc(n * sizeof(unsigned));
    memcpy(weights_sorted, weights, n * sizeof(unsigned));
    // Tri décroissant
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (weights_sorted[j] < weights_sorted[j + 1]) {
                unsigned tmp = weights_sorted[j];
                weights_sorted[j] = weights_sorted[j + 1];
                weights_sorted[j + 1] = tmp;
            }
        }
    }
    unsigned int* bins_d = (unsigned int*)calloc(n, sizeof(unsigned int));
    unsigned int bins_ffd = 0;
    clock_t start_ffd = clock();
    for (int i = 0; i < n; i++) {
        int placed = 0;
        for (unsigned int j = 0; j < bins_ffd; j++) {
            if (bins_d[j] + weights_sorted[i] <= bin_capacity) {
                bins_d[j] += weights_sorted[i];
                placed = 1;
                break;
            }
        }
        if (!placed) {
            bins_d[bins_ffd] = weights_sorted[i];
            bins_ffd++;
        }
    }
    clock_t end_ffd = clock();
    time = (end_ffd - start_ffd) / (double)CLOCKS_PER_SEC;
    free(bins_d);
    free(weights_sorted);
    return bins_ffd;
}

int main(void) {
    for (int file_idx = 1; file_idx <= 6; file_idx++) {
        char filename[64];
        snprintf(filename, sizeof(filename), "knapsack_instance_%d.txt", file_idx);
        int n, capacity;
        Item *items;
        printf("\n==== %s ====\n", filename);
        if (!read_instance(filename, &n, &capacity, &items)) {
            fprintf(stderr, "Erreur lecture instance %s\n", filename);
            continue;
        }

        // Préparation des poids pour les heuristiques
        unsigned *weights = (unsigned *)malloc(n * sizeof(unsigned));
        for (int i = 0; i < n; i++) {
            weights[i] = (unsigned)items[i].weight;
        }
        unsigned int bin_capacity = (unsigned int)capacity;

        double time_ff = 0.0, time_ffd = 0.0;
        unsigned int bins_ff = first_fit(weights, n, bin_capacity, time_ff);
        unsigned int bins_ffd = first_fit_decreasing(weights, n, bin_capacity, time_ffd);
        printf("First-Fit : %u bins, temps = %.6f s\n", bins_ff, time_ff);
        printf("First-Fit-Decreasing : %u bins, temps = %.6f s\n", bins_ffd, time_ffd);

        free(weights);
        free(items);
    }
    return 0;
}
