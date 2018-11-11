tr -d '\r' < compile.sh > compile2.sh
mv compile2.sh compile.sh

tr -d '\r' < experiment.sh > experiment2.sh
mv experiment2.sh experiment.sh

tr -d '\r' < main.sh > main2.sh
mv main2.sh main.sh

tr -d '\r' < run.sh > run2.sh
mv run2.sh run.sh
