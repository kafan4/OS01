#include <queue>
#include <time.h>

// Structure simple de heap pour la gestion des bins (min-heap sur la capacité restante)
typedef struct {
    unsigned int *elements;
    unsigned int size;
    unsigned int capacity;
} simple_heap;

void simple_heap_init(simple_heap *h, unsigned int max_bins) {
    h->elements = (unsigned int *)calloc(max_bins + 2, sizeof(unsigned int)); // indexation à partir de 1
    h->size = 0;
    h->capacity = max_bins + 2;
}

void simple_heap_free(simple_heap *h) {
    free(h->elements);
}

void simple_heap_push(simple_heap *h, unsigned int value) {
    h->size++;
    h->elements[h->size] = value;
    // Remonter
    unsigned int i = h->size;
    while (i > 1 && h->elements[i] < h->elements[i/2]) {
        unsigned int tmp = h->elements[i];
        h->elements[i] = h->elements[i/2];
        h->elements[i/2] = tmp;
        i /= 2;
    }
}

void simple_heap_reheap_down(simple_heap *h, unsigned int i) {
    while (2*i <= h->size) {
        unsigned int j = 2*i;
        if (j+1 <= h->size && h->elements[j+1] < h->elements[j]) j++;
        if (h->elements[i] <= h->elements[j]) break;
        unsigned int tmp = h->elements[i];
        h->elements[i] = h->elements[j];
        h->elements[j] = tmp;
        i = j;
    }
}

// Algorithme Best-Fit avec heap
unsigned int best_fit_heap(unsigned int bin_capacity, const unsigned* objects, unsigned int n, double& time) {
    unsigned int num_bins = 0;
    simple_heap bins;
    simple_heap_init(&bins, n);
    std::queue<unsigned int> heap_queue;

    clock_t start = clock();
    for(unsigned int i = 0; i < n; i++) {
        unsigned int best_bin = 0; // index du meilleur bin (1-based)
        unsigned int best_cap = 0; // capacité après ajout

        // Chercher le meilleur bin (le plus rempli possible sans dépasser)
        for (unsigned int j = 1; j <= bins.size; j++) {
            unsigned int temp_cap = bins.elements[j] + objects[i];
            if (temp_cap <= bin_capacity && temp_cap > best_cap) {
                best_bin = j;
                best_cap = temp_cap;
            }
        }

        if (best_bin > 0) {
            bins.elements[best_bin] += objects[i];
            simple_heap_reheap_down(&bins, best_bin);
        } else {
            simple_heap_push(&bins, objects[i]);
            num_bins++;
        }
    }
    clock_t end = clock();
    time = (end-start)/static_cast<double>(CLOCKS_PER_SEC);
    simple_heap_free(&bins);
    return num_bins;
}

// Algorithme Best-Fit-Decreasing avec heap
unsigned int best_fit_decreasing_heap(unsigned int bin_capacity, const unsigned* objects, unsigned int n, double& time) {
    // Copier et trier les objets par poids décroissant
    unsigned *sorted = (unsigned *)malloc(n * sizeof(unsigned));
    memcpy(sorted, objects, n * sizeof(unsigned));
    for (unsigned int i = 0; i < n - 1; i++) {
        for (unsigned int j = 0; j < n - i - 1; j++) {
            if (sorted[j] < sorted[j + 1]) {
                unsigned tmp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = tmp;
            }
        }
    }
    unsigned int num_bins = best_fit_heap(bin_capacity, sorted, n, time);
    free(sorted);
    return num_bins;
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

        // Préparation des poids pour best_fit_heap
        unsigned *weights = (unsigned *)malloc(n * sizeof(unsigned));
        for (int i = 0; i < n; i++) {
            weights[i] = (unsigned)items[i].weight;
        }
        double time = 0.0;
        unsigned int num_bins = best_fit_heap((unsigned int)capacity, weights, (unsigned int)n, time);
        printf("Best Fit Heap : %u bins, temps = %.6f s\n", num_bins, time);

        double time_decr = 0.0;
        unsigned int num_bins_decr = best_fit_decreasing_heap((unsigned int)capacity, weights, (unsigned int)n, time_decr);
        printf("Best Fit Decreasing Heap : %u bins, temps = %.6f s\n", num_bins_decr, time_decr);

        free(weights);
        free(items);
    }
    return 0;
}


