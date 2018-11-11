#Runs all the experiments, takes the number of threads to use as the first parameter

nthread="$1"

#Compile
./compile.sh

for((n=2;n<=100;++n))
do
for((m=1;m<=5;++m))
do
	for((p=2;p<=5;++p))
	do
		for((en=1;en<=31;++en))
		do
				#Check the number of child processes
				childCnt=$(($(pgrep --parent $$ | wc -l) - 1))
				while((childCnt>=nthread))
				do
					echo "$m $p $en $n: There are $childCnt processes running; Waiting in queue..."
					wait -n
					childCnt=$(($(pgrep --parent $$ | wc -l) - 1))
				done
				echo "========================================"
				echo "Running Experiment $m $p $en $n"
				echo "========================================"
				#Check if the previous example has failed
				nprev=0
				let "nprev=n-1"
				preverror="Experiment-$m-$p-$en-$nprev/output/error.txt";
				thiserror="Experiment-$m-$p-$en-$n/output/error.txt";
				thisdir1="Experiment-$m-$p-$en-$n";
				if [ ! -f "$preverror" ]; then
					./experiment.sh $m $p $en $n &
				else
					echo "========================================"
					echo "Skipping Experiment $m $p $en $n"
					echo "Due to Error in $m $p $en $nprev"
					echo "========================================"
					#make the error file
					mkdir $thisdir1
					cd $thisdir1
					mkdir output
					cd ..
					echo "Skipped" > $thisdir1/output/error.txt
				fi
		done
	done
done 
done
