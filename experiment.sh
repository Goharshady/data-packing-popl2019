#Runs one of our experiments -- gets m p example_number n  as parameters
m="$1";
p="$2";
en="$3";
n="$4";
prefdir="Experiment-$m-$p-$en-$n"
echo $m
echo $p
echo $en
echo $n

#Create a directory for this experiment
mkdir $prefdir

#Go in that directory and make input and output directories
cd $prefdir
mkdir input
mkdir output
mkdir temp
cd ..

#Copy all the necessary files to the relevant directory
cp run.sh $prefdir
cp examples/ex$en $prefdir
cp heuristics/util/summarize_log $prefdir/temp

cd $prefdir
#Generate the input
./ex$en $n  > ref.txt
mv ref.txt input


#Run the experiment
./run.sh $m $p >> log.txt 2>>log.txt
cp log.txt temp
mv log.txt output

#Summarize the log
cd temp
./summarize_log < log.txt > short.txt
cd ..

#Show the report
cat temp/short.txt >> output/short_report.txt
echo "========================================"
echo "Results of Experiment $m $p $en $n"
echo "========================================"
cat temp/short.txt
echo "========================================"



