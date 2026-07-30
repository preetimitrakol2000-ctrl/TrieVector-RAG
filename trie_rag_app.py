from trie_bridge import TrieBridge

if __name__ == "__main__":
    trie = TrieBridge()

    # Index doc chunks restricted by code namespace
    trie.insert("std::vector::push", 101, [0.1, 0.9, 0.2, 0.4])
    trie.insert("std::vector::pop", 102, [0.2, 0.8, 0.1, 0.5])
    trie.insert("std::string::concat", 201, [0.9, 0.1, 0.8, 0.1])

    # Perform similarity search restricted solely under 'std::vector' namespace
    query_vector = [0.15, 0.85, 0.18, 0.42]
    matched = trie.search("std::vector", query_vector)

    print("=== TRIEVECTOR-RAG PREFIX CONSTRAINED SEARCH ===")
    print(f"[*] Searched prefix 'std::vector' with cosine fallback. Best Matched Chunk ID: {matched}")
