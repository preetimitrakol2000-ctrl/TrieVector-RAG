#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ALPHABET_SIZE 128
#define VECTOR_DIM 4

typedef struct TrieVectorNode {
    struct TrieVectorNode* children[ALPHABET_SIZE];
    float* embedding;
    int doc_id;
    int is_terminal;
} TrieVectorNode;

#ifdef _WIN32
    __declspec(dllexport) TrieVectorNode* init_trie_vector_node();
    __declspec(dllexport) void insert_vector_entry(TrieVectorNode* root, const char* prefix, int doc_id, float* vec);
    __declspec(dllexport) int search_prefix_best_match(TrieVectorNode* root, const char* prefix, float* query_vec);
#endif

TrieVectorNode* init_trie_vector_node() {
    TrieVectorNode* node = (TrieVectorNode*)malloc(sizeof(TrieVectorNode));
    node->is_terminal = 0;
    node->doc_id = -1;
    node->embedding = NULL;
    for (int i = 0; i < ALPHABET_SIZE; i++) node->children[i] = NULL;
    return node;
}

static float cosine_similarity(float* v1, float* v2, int dim) {
    float dot = 0.0f, norm1 = 0.0f, norm2 = 0.0f;
    for (int i = 0; i < dim; i++) {
        dot += v1[i] * v2[i];
        norm1 += v1[i] * v1[i];
        norm2 += v2[i] * v2[i];
    }
    if (norm1 == 0.0f || norm2 == 0.0f) return 0.0f;
    return dot / (sqrtf(norm1) * sqrtf(norm2));
}

void insert_vector_entry(TrieVectorNode* root, const char* prefix, int doc_id, float* vec) {
    TrieVectorNode* curr = root;
    int len = strlen(prefix);
    for (int i = 0; i < len; i++) {
        unsigned char index = (unsigned char)prefix[i];
        if (index >= ALPHABET_SIZE) continue;
        if (!curr->children[index]) {
            curr->children[index] = init_trie_vector_node();
        }
        curr = curr->children[index];
    }
    curr->is_terminal = 1;
    curr->doc_id = doc_id;
    curr->embedding = (float*)malloc(sizeof(float) * VECTOR_DIM);
    memcpy(curr->embedding, vec, sizeof(float) * VECTOR_DIM);
}

static void collect_best_in_subtree(TrieVectorNode* curr, float* query_vec, int* best_doc, float* max_sim) {
    if (!curr) return;
    if (curr->is_terminal && curr->embedding) {
        float sim = cosine_similarity(curr->embedding, query_vec, VECTOR_DIM);
        if (sim > *max_sim) {
            *max_sim = sim;
            *best_doc = curr->doc_id;
        }
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (curr->children[i]) {
            collect_best_in_subtree(curr->children[i], query_vec, best_doc, max_sim);
        }
    }
}

int search_prefix_best_match(TrieVectorNode* root, const char* prefix, float* query_vec) {
    TrieVectorNode* curr = root;
    int len = strlen(prefix);
    for (int i = 0; i < len; i++) {
        unsigned char index = (unsigned char)prefix[i];
        if (!curr->children[index]) return -1;
        curr = curr->children[index];
    }
    int best_doc = -1;
    float max_sim = -1.0f;
    collect_best_in_subtree(curr, query_vec, &best_doc, &max_sim);
    return best_doc;
}
