util: 
Input: gets m and p (command line parameters), R in ref.txt, data placement scheme in scheme.txt (one block per line)
Output: Writes number of cache misses with LRU to stdout

1: CCDP Algorithm ("Cache-conscious data placement" by Calder et al)
Input: m and p (command line), R in ref.txt
Output: A scheme in stdout (one block per line)

2: CPACK algorithm (as explained by the "CApRI" paper, reference [10] of CApRI)

3: GPART algorithm (as explained by the "CApRI" paper, reference [15] of CApRI)

4: CApRI algorithm for optimizing an existing data placement scheme

5: A hierarchical model of data locality (by Zhang et al)

6: k-distance algorithm (as explained by "Array regrouping and structure splitting using whole-program reference affinity", by Zhong et al)

Pipelines for comparison:
A) 1
B) 2
C) 3
D) 2->4
E) 3->4
F) 4 [using unary blocks]
G) 5
H) 6
