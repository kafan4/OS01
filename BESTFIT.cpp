#include <queue>
#include <time.h>

// Structure simple de heap pour la gestion des bins (min-heap sur la capacité restante)
typedef struct {
    unsigned int *tab;
    unsigned int taille;
    unsigned int capacite;
} h_simple;

void initialiser_h(h_simple *t, unsigned int max_bacs) {
    t->tab = (unsigned int *)calloc(max_bacs + 2, sizeof(unsigned int)); // indexation à partir de 1
    t->taille = 0;
    t->capacite = max_bacs + 2;
}

void liberer_h(h_simple *t) {
    free(t->tab);
}

void ajouter_h(h_simple *t, unsigned int val) {
    t->taille++;
    t->tab[t->taille] = val;
    unsigned int i = t->taille;
    while (i > 1 && t->tab[i] < t->tab[i/2]) {
        unsigned int tmp = t->tab[i];
        t->tab[i] = t->tab[i/2];
        t->tab[i/2] = tmp;
        i /= 2;
    }
}

void rehser(h_simple *t, unsigned int i) {
    while (2*i <= t->taille) {
        unsigned int j = 2*i;
        if (j+1 <= t->taille && t->tab[j+1] < t->tab[j]) j++;
        if (t->tab[i] <= t->tab[j]) break;
        unsigned int tmp = t->tab[i];
        t->tab[i] = t->tab[j];
        t->tab[j] = tmp;
        i = j;
    }
}

// Algorithme Best-Fit avec tableau (pas de heap)
unsigned int best_fit_h(unsigned int capacite_bac, const unsigned* objets, unsigned int n, double& temps) {
    unsigned int nb = 0;
    unsigned int bacs[1000]; // Limite arbitraire, style amateur
    for (unsigned int i = 0; i < 1000; i++) bacs[i] = 0;
    clock_t t1 = clock();
    for(unsigned int i = 0; i < n; i++) {
        unsigned int ind = 0;
        unsigned int cap = 0;
        for (unsigned int j = 0; j < nb; j++) {
            unsigned int tmp = bacs[j] + objets[i];
            if (tmp <= capacite_bac && tmp > cap) {
                ind = j+1;
                cap = tmp;
            }
        }
        if (ind > 0) {
            bacs[ind-1] += objets[i];
        } else {
            bacs[nb] = objets[i];
            nb++;
        }
    }
    clock_t t2 = clock();
    temps = (t2-t1)/(double)CLOCKS_PER_SEC;
    return nb;
}

// Algorithme Best-Fit-Decreasing avec tableau (utilise best_fit_h)
unsigned int best_fit_decreasing_heap(unsigned int capacite_bac, const unsigned* objets, unsigned int n, double& temps) {
    unsigned *tab2 = (unsigned *)malloc(n * sizeof(unsigned));
    for (unsigned int i = 0; i < n; i++) tab2[i] = objets[i];
    // tri à la main, pas optimisé
    for (unsigned int i = 0; i < n; i++) {
        for (unsigned int j = 0; j < n-1; j++) {
            if (tab2[j] < tab2[j+1]) {
                unsigned tmp = tab2[j];
                tab2[j] = tab2[j+1];
                tab2[j+1] = tmp;
            }
        }
    }
    unsigned int res = best_fit_h(capacite_bac, tab2, n, temps);
    free(tab2);
    return res;
}

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




int main() {
    int i;
    for (i = 1; i <= 6; i++) {
        char fname[64];
        snprintf(fname, sizeof(fname), "knapsack_instance_%d.txt", i);
        int n, c;
        Item *itms;
        printf("\n==== %s ====\n", fname);
        if (!read_instance(fname, &n, &c, &itms)) {
            printf("Erreur lecture instance %s\n", fname);
            continue;
        }
        unsigned *w = (unsigned *)malloc(n * sizeof(unsigned));
        for (int j = 0; j < n; j++) w[j] = (unsigned)itms[j].weight;
        double t = 0.0;
        unsigned int bins = best_fit_h((unsigned int)c, w, (unsigned int)n, t);
        printf("Best Fit Heap : %u bins, temps = %f\n", bins, t);
        double t2 = 0.0;
        unsigned int bins2 = best_fit_decreasing_heap((unsigned int)c, w, (unsigned int)n, t2);
        printf("Best Fit Decreasing Heap : %u bins, temps = %f\n", bins2, t2);
        free(w);
        free(itms);
    }
    return 0;
}


