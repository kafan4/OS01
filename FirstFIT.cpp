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
unsigned int first_fit(const unsigned *wieght, int nb_objets, unsigned int bin_capacity, double &temps) {
    unsigned int* bins = (unsigned int*)calloc(nb_objets, sizeof(unsigned int));
    unsigned int nb_bins = 0;
    clock_t debut = clock();
    for (int objet = 0; objet < nb_objets; objet++) {
        int place = 0;
        for (unsigned int b = 0; b < nb_bins; b++) {
            if (bins[b] + wieght[objet] <= bin_capacity) {
                bins[b] += wieght[objet];
                place = 1;
                break;
            }
        }
        if (!place) {
            bins[nb_bins] = wieght[objet];
            nb_bins++;
        }
    }
    clock_t fin = clock();
    temps = (fin - debut) / (double)CLOCKS_PER_SEC;
    free(bins);
    return nb_bins;
}

// Heuristique First-Fit-Decreasing
unsigned int first_fit_decreasing(const unsigned *wieght, int nb_objets, unsigned int bin_capacity, double &temps) {
    unsigned *wieght_tries = (unsigned *)malloc(nb_objets * sizeof(unsigned));
    memcpy(wieght_tries, wieght, nb_objets * sizeof(unsigned));
    // Tri décroissant
    for (int i = 0; i < nb_objets - 1; i++) {
        for (int j = 0; j < nb_objets - i - 1; j++) {
            if (wieght_tries[j] < wieght_tries[j + 1]) {
                unsigned tmp = wieght_tries[j];
                wieght_tries[j] = wieght_tries[j + 1];
                wieght_tries[j + 1] = tmp;
            }
        }
    }
    unsigned int* bins_d = (unsigned int*)calloc(nb_objets, sizeof(unsigned int));
    unsigned int nb_bins_d = 0;
    clock_t debut = clock();
    for (int objet = 0; objet < nb_objets; objet++) {
        int place = 0;
        for (unsigned int b = 0; b < nb_bins_d; b++) {
            if (bins_d[b] + wieght_tries[objet] <= bin_capacity) {
                bins_d[b] += wieght_tries[objet];
                place = 1;
                break;
            }
        }
        if (!place) {
            bins_d[nb_bins_d] = wieght_tries[objet];
            nb_bins_d++;
        }
    }
    clock_t fin = clock();
    temps = (fin - debut) / (double)CLOCKS_PER_SEC;
    free(bins_d);
    free(wieght_tries);
    return nb_bins_d;
}

int main(void) {
    for (int indice_fichier = 1; indice_fichier <= 6; indice_fichier++) {
        char nom_fichier[64];
        snprintf(nom_fichier, sizeof(nom_fichier), "knapsack_instance_%d.txt", indice_fichier);
        int nb_objets, capacite;
        Item *objets;
        printf("\n==== %s ====\n", nom_fichier);
        if (!read_instance(nom_fichier, &nb_objets, &capacite, &objets)) {
            fprintf(stderr, "Erreur lecture instance %s\n", nom_fichier);
            continue;
        }
        unsigned *wieght = (unsigned *)malloc(nb_objets * sizeof(unsigned));
        for (int i = 0; i < nb_objets; i++) {
            wieght[i] = (unsigned)objets[i].weight;
        }
        unsigned int bin_capacity = (unsigned int)capacite;
        double temps_ff = 0.0, temps_ffd = 0.0;
        unsigned int bacs_ff = first_fit(wieght, nb_objets, bin_capacity, temps_ff);
        unsigned int bacs_ffd = first_fit_decreasing(wieght, nb_objets, bin_capacity, temps_ffd);
        printf("First-Fit : %u bins, temps = %.6f s\n", bacs_ff, temps_ff);
        printf("First-Fit-Decreasing : %u bins, temps = %.6f s\n", bacs_ffd, temps_ffd);
        free(wieght);
        free(objets);
    }
    return 0;
}
