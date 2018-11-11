## This script runs our treewidht-based approach, together with 8 different heuristics and compares the number of cache misses in each case
## The script gets a sequence of references in input/ref.txt, it also gets the parameters m p  where m is the size of the cache, p is the packing factor

m="$1";
p="$2";
tl=300; #time limit for our algorithms
tltwo=600; #time limit for finding tree decompositions
tlh=300; #time limit for heuristics
echo "time limit is:"
echo $tl
#move to a temporary directory
cp ../Algorithm1/Algorithm1 temp
cp ../Algorithm1/Algorithm2 temp
cp ../heuristics/1/h1 temp
cp ../heuristics/2/h2 temp
cp ../heuristics/3/h3 temp
cp ../heuristics/4/h4 temp
cp ../heuristics/5/h5 temp
cp ../heuristics/6/h6 temp
cp ../heuristics/util/cache_miss_count temp
cp ../nice-tree-decomposition/MakeNiceTreeDecomposition.jar temp
cp input/ref.txt temp

cd temp

################################
echo "Running our Approach..."
echo "Getting the Nice Tree Decomposition"
time timeout $tltwo java -jar MakeNiceTreeDecomposition.jar $m $p
rc=$?
if(($rc!=0))
then
echo "Error (perhaps TL exceeded?) in MakeNiceTreeDecomposition" >> ../output/error.txt
exit $rc
fi

if(($m>1))
then
	echo "Running the second algorithm, because m>1"
	time timeout $tl ./Algorithm2 $m $p
	rc=$?
	if(($rc!=0))
	then
	echo "Error (perhaps TL exceeded?) in Algorithm 2" >> ../output/error.txt
	exit $rc
	fi
else
	echo "Running the first algorithm, m = 1"
	time timeout $tl ./Algorithm1 $m $p
	rc=$?
	if(($rc!=0))
	then
	echo "Error (perhaps TL exceeded?) in Algorithm 1" >> ../output/error.txt
	exit $rc
	fi
fi
echo "End of our approach"
################################


echo "Running the heuristics"


################################
echo "Running Pipeline A"
time timeout $tlh ./h1 $m $p > scheme.txt
cat scheme.txt
echo "Cache misses:"
./cache_miss_count $m $p
################################


################################
echo "Running Pipeline B"
time timeout $tlh ./h2 $m $p > scheme.txt
cat scheme.txt
echo "Cache misses:"
./cache_miss_count $m $p
################################


################################
echo "Running Pipeline C"
time timeout $tlh ./h3 $m $p > scheme.txt
cat scheme.txt
echo "Cache misses:"
./cache_miss_count $m $p
################################


################################
echo "Running Pipline D"
time timeout $tlh ./h2 $m $p > scheme.txt
time timeout $tlh ./h4 $m $p > scheme.txt
cat scheme.txt
echo "Cache misses:"
./cache_miss_count $m $p
################################


################################
echo "Running Pipline E"
time timeout $tlh ./h3 $m $p > scheme.txt
time timeout $tlh ./h4 $m $p > scheme.txt
cat scheme.txt
echo "Cache misses:"
./cache_miss_count $m $p
################################


################################
echo "Running Pipline F"
time timeout $tlh ./h2 1 1 > scheme.txt
time timeout $tlh ./h4 $m $p > scheme.txt
cat scheme.txt
echo "Cache misses:"
./cache_miss_count $m $p
################################


################################
echo "Running Pipeline G"
time timeout $tlh ./h5 $m $p > scheme.txt
cat scheme.txt
echo "Cache misses:"
./cache_miss_count $m $p
################################

################################
echo "Running Pipeline H"
time timeout $tlh ./h6 $m $p > scheme.txt
cat scheme.txt
echo "Cache misses:"
./cache_miss_count $m $p
################################


echo "Run finished"

#clean up
#cd ..
#rm -r -f temp
