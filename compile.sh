#Compiles everything
echo "Compiling heuristics..."
cd heuristics/1/
g++ make_scheme.cpp -std=c++11 -oh1
cd ../2/
g++ cpack.cpp -std=c++11 -oh2
cd ../3/
g++ gpart.cpp -std=c++11 -oh3
cd ../4/
g++ capri.cpp -std=c++11 -oh4
cd ../5/
g++ hierarchical.cpp -std=c++11 -oh5
cd ../6/
g++ kdistance.cpp -std=c++11 -oh6

####################################
echo "Compiling utilities..."
cd ../util/
g++ cache_miss_count.cpp -std=c++11 -ocache_miss_count
g++ summarize_log.cpp -std=c++11 -osummarize_log
cd ..
cd ..
g++ -std=c++11 make_csv.cpp -omake_csv

####################################
echo "Compiling our algorithms..."
cd Algorithm1
g++ -std=c++11 Algorithm1.cpp -oAlgorithm1
g++ -std=c++11 Algorithm2.cpp -oAlgorithm2
cd ..

####################################
echo "Compiling example generators..."
cd examples
g++ -std=c++11 01-scalar_vector_multiplication.cpp -oex1
g++ -std=c++11 02-vetor_vector_multiplication.cpp -oex2
g++ -std=c++11 03-vector_matrix_multiplication.cpp -oex3
g++ -std=c++11 04-scalar_matrix_multiplication.cpp -oex4
g++ -std=c++11 05-matrix_matrix_multiplication.cpp -oex5
g++ -std=c++11 06-Gram_Schmidt.cpp -oex6
g++ -std=c++11 07-Fibonacci.cpp -oex7
g++ -std=c++11 08-RecursiveFibonacci.cpp -oex8
g++ -std=c++11 09-Binomial.cpp -oex9
g++ -std=c++11 10-RecursiveBinomial.cpp -oex10
g++ -std=c++11 11-longest_common_subsequence.cpp -oex11
g++ -std=c++11 12-bubble_sort.cpp -oex12
g++ -std=c++11 13-insertion_sort.cpp -oex13
g++ -std=c++11 14-merge_sort.cpp -oex14
g++ -std=c++11 15-merge.cpp -oex15
g++ -std=c++11 16-quick_sort.cpp -oex16
g++ -std=c++11 17-heap.cpp -oex17
g++ -std=c++11 18-heap-sort.cpp -oex18
g++ -std=c++11 19-binary_search_tree_insertion.cpp -oex19
g++ -std=c++11 20-binary_search_tree_insertion_and_search.cpp -oex20
g++ -std=c++11 21-array_binary_search.cpp -oex21
g++ -std=c++11 22-DSU.cpp -oex22
g++ -std=c++11 23-string_naive_pattern_search.cpp -oex23
g++ -std=c++11 24-KMP.cpp -oex24
g++ -std=c++11 25-Rabin-Karp.cpp -oex25
g++ -std=c++11 26-postorder-traversal.cpp -oex26
g++ -std=c++11 27-Inorder-traversal.cpp -oex27
g++ -std=c++11 28-preorder-traversal.cpp -oex28
g++ -std=c++11 29-closest-pair.cpp -oex29
g++ -std=c++11 30-knapsack.cpp -oex30
g++ -std=c++11 31-convex-hull.cpp -oex31

echo "Compilation Finished"
