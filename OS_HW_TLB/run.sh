#!/bin/bash

echo '# x y' > time.data
NUMPAGES=1
sudo make
while [ $NUMPAGES -le 8192 ] 
do
	echo NUMPAGES: $NUMPAGES
	./tlb $NUMPAGES 500000 >> time.data
	NUMPAGES=$[ $NUMPAGES * 2]
done
sudo make clean
python3 ./make_graph_pyplot.py 
#rm time.data
#wslview ./TLB_Measurement_graph.png
