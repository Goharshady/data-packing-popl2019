
Efficient Parameterized Algorithms for Data Packing
Guide to the Artifact

Note: The webpage for this artifact is http://pub.ist.ac.at/~akafshda/popl19ae/. Please check it out as it contains a virtual machine and might also contain bug fixes.

=================
Overview
=================
We are considering the problem of data packing over a two-level memory system and providing an algorithm for solving it by exploiting the treewidth of the underlying access graphs. Our algorithm is exact and optimal, i.e. it outputs the minimal number of cache misses possible.

We have implemented our algorithm in Java. Our implementation relies on tree decompositions of the underlying access hypergraphs. These are obtained using the LibTW tool.

We use several common programs as benchmarks. These are all implemented in C++. We compare the results of our algorithm (i.e. the optimal number of cache misses) with those of several previous benchmarks (also implemented in C++). Moreover, we have implemented a cache simulator to count the number of cache misses caused by each of these heuristics.


=================
File Structure, Prerequisites and Compilation
=================
Many of our scripts are in Bash. We suggest that you execute them on a 64-bit Linux machine. Our codes are in C++ and Java. Hence, you will need g++ for compilation and a JRE. 

File Structure
Experiment: the source codes and scripts for the main experiment
Algorithm1: the source codes of our approach in C++
nice-tree-decomposition: contains a Java program that creates a nice tree decomposition of the access (hyper)graph of the input sequence using LibTW
examples: The benchmark programs in C++
heuristics: C++ implementations of heuristics / there is a readme file explaining what each heuristic implements

Compilation
To compile all the C++ programs, open a terminal and run

./compile.sh

You will need to have g++ installed. Please make sure that you compile everything before running any experiments.

=================
Running the Main Experiment
=================
The main experiment (from which all of our experimental results are obtained) runs all possible instances for all benchmarks and 2 <= n <= 100, 1 <= m <= 5 and 2 <=p <= 5. [See the “custom input” section below for the definition of n.] We did not include p = 1, because data packing is trivial in this case (i.e. each data element should be in its own singleton block).

Note that this amounts to roughly 550k runs, each of which with a time limit of 5 minutes. However, many of these runs terminate much faster. On our machine, running with 12 threads, the process finished in ~10 hours.

Instructions
To run the main experiment, simply execute the following command:

./main.sh k >> full_log.txt 2>>full_log.txt

where k is the number of threads you want to allow for the execution of the experiment. In our case, k was 12.

Note: Running the complete experiment takes a lot of time, memory and disk space. Please ensure that the machine has at least k CPU cores, 4k GB of RAM and 12 GB disk space. A complete log of the experiment will be written to “full_log.txt”, which you can check to see the progress. 

Note: You can customize which parts of the experiment you want to run by setting the loop bounds in “main.sh”.

Note: To save time, if the experiment times out on an input, larger inputs of the same benchmark, with the same parameters, will be skipped.

Obtaining the Report
After the experiment execution concludes, you can obtain a csv file containing a report of all the results by simply running:

./make_csv > data.csv

Note: If you are customizing the ranges for parameters in the experiment, make sure you also apply it in the loops in the main function of “make_csv.cpp” and compile again.

=================
Testing the Artifact with Custom Input
=================
Generating an Input Access Sequence
The benchmark programs (examples) are numbered from 1 to 31. Each benchmark program takes a size value n as input parameter and runs the respective algorithm on random inputs of size n (the randomization seed is fixed, so as to make the process deterministic and reproducible). It then prints the resulting memory access sequence.

For example, if you open a terminal in the “examples” folder and run

./ex12 3

it prints an access sequence obtained from running bubble-sort on a random array of size 3.

Running a Single Instance
To run a single instance of our experiment, simply execute

./experiment.sh m p en n

Here, m is the cache size (as in the paper), p is the packing factor (as in the paper), en is the example number and n is the size parameter passed to the example. For example, running

./experiment.sh 1 2 12 3

first generates an access sequence obtained from bubble-sorting an array of size 3 (as in the example above) and then runs our algorithm and all the heuristic algorithms on it to obtain data packing schemes for a cache of size 1 with a packing factor of 2. It then simulates the cache and counts the number of misses caused by each method. Finally, it outputs the number of cache misses caused by each method (starting with ours) and the time consumed by each.

Note that in some instances computing tree decompositions and dynamic programming values takes a lot of memory. So, make sure that the machine has access to at least 8 GB of RAM.

Detailed Results
After running an instance as above, you can find detailed information about it in the newly created folder “Experiment-m-p-en-n”. For example, after running the experiment above, the folder “Experiment-1-2-12-3” will be created which contains the input access sequence R, the tree decomposition, and logs. The complete input/output file contains these detailed results for all the instances of the main experiment.

Changing Time Limits
By default, the time given to each algorithm for producing a data placement scheme is limited to 5 minutes. To change this limit, you can edit lines 6-8 of “run.sh”.

Testing with Your Own Input
If you wish to test our code with your own input, follow these steps:

Create a text file named “ref.txt” and write the access sequence R in it (one data item per line). Each data item should be numbered by a single 32-bit integer.
Choose the cache size m and the packing factor p
Put “ref.txt” in the folder “nice-tree-decomposition” and run 
java -jar MakeNiceTreeDecomposition.jar m p
This will create a nice tree decomposition for the access hypergraph of order q* and write it in NiceTreeDecomposition.txt
Copy NiceTreeDecomposition.txt and refs.txt to the folder “Algorithm1” and run either
./Algorithm1 m p
if m = 1or
./Algorithm2 m p
if m > 1. Note that, in the paper, we present two different algorithms for these cases. This is why there are different files here.
The optimal number of cache misses will be output by the algorithm
Note: The program will crash if you change the value of m and p without generating a new tree decomposition or if the wrong algorithm is run in step 4 above. If something did not work as expected, please write to us and include the input. We will look into it and answer ASAP.