#!/bin/bash

echo '# x y' > time.data
NUMPAGES=1
sudo make
while [ $NUMPAGES -le 2048 ] 
do
	echo NUMPAGES: $NUMPAGES
	./tlb $NUMPAGES 100000 >> time.data
	NUMPAGES=$[ $NUMPAGES * 2]
done
sudo make clean
python3 ./make_graph.py
#rm time.data
#wslview ./TLB_Measurement_Graph.pdf
