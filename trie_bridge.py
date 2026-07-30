import ctypes
import os
import sys

class TrieBridge:
    def __init__(self):
        if not os.path.exists("./libtrievec.so") and not os.path.exists("./libtrievec.dll"):
            if sys.platform.startswith("win"):
                os.system("gcc -O3 -shared -o libtrievec.dll trie_vector.c")
                lib_path = "./libtrievec.dll"
            else:
                os.system("gcc -O3 -shared -fPIC -o libtrievec.so trie_vector.c")
                lib_path = "./libtrievec.so"
        else:
            lib_path = "./libtrievec.dll" if sys.platform.startswith("win") else "./libtrievec.so"

        self.lib = ctypes.CDLL(lib_path)
        self.lib.init_trie_vector_node.restype = ctypes.c_void_p
        self.lib.insert_vector_entry.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_int, ctypes.POINTER(ctypes.c_float)]
        self.lib.search_prefix_best_match.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.POINTER(ctypes.c_float)]
        self.lib.search_prefix_best_match.restype = ctypes.c_int
        
        self.root = self.lib.init_trie_vector_node()

    def insert(self, prefix: str, doc_id: int, vector: list):
        c_vec = (ctypes.c_float * 4)(*vector)
        self.lib.insert_vector_entry(self.root, prefix.encode('utf-8'), doc_id, c_vec)

    def search(self, prefix: str, query_vector: list) -> int:
        c_qvec = (ctypes.c_float * 4)(*query_vector)
        return self.lib.search_prefix_best_match(self.root, prefix.encode('utf-8'), c_qvec)
