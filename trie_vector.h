#ifndef TRIE_VECTOR_H
#define TRIE_VECTOR_H

typedef struct TrieVectorNode TrieVectorNode;

TrieVectorNode* init_trie_vector_node();
void insert_vector_entry(TrieVectorNode* root, const char* prefix, int doc_id, float* vec);
int search_prefix_best_match(TrieVectorNode* root, const char* prefix, float* query_vec);

#endif
