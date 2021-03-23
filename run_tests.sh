#!/bin/sh

#numSets = $1, blocksPerSet = $2, blockSize = $3

make clean
make 

echo "GCC.TRACE  #############################" > Results_$1_$2_$3.txt

echo "" >> Results_$1_$2_$3.txt
echo "write-allocate write-back fifo" >> Results_$1_$2_$3.txt
./csim $1 $2 $3 write-allocate write-back fifo < gcc.trace >> Results_$1_$2_$3.txt

echo "" >> Results_$1_$2_$3.txt
echo "write-allocate write-through fifo" >> Results_$1_$2_$3.txt
./csim $1 $2 $3 write-allocate write-through fifo < gcc.trace >> Results_$1_$2_$3.txt

echo "" >> Results_$1_$2_$3.txt
echo "no-write-allocate write-through fifo" >> Results_$1_$2_$3.txt
./csim $1 $2 $3 no-write-allocate write-through fifo < gcc.trace >> Results_$1_$2_$3.txt

echo "" >> Results_$1_$2_$3.txt
echo "write-allocate write-back lru" >> Results_$1_$2_$3.txt
./csim $1 $2 $3 write-allocate write-back lru < gcc.trace >> Results_$1_$2_$3.txt

echo "" >> Results_$1_$2_$3.txt
echo "write-allocate write-through lru" >> Results_$1_$2_$3.txt
./csim $1 $2 $3 write-allocate write-through lru < gcc.trace >> Results_$1_$2_$3.txt

echo "" >> Results_$1_$2_$3.txt
echo "no-write-allocate write-through lru" >> Results_$1_$2_$3.txt
./csim $1 $2 $3 no-write-allocate write-through lru < gcc.trace >> Results_$1_$2_$3.txt



#swim.trace commands
echo "" >> Results_$1_$2_$3.txt
echo "SWIM.TRACE   #############################" >> Results_$1_$2_$3.txt

echo "" >> Results_$1_$2_$3.txt
echo "write-allocate write-back fifo" >> Results_$1_$2_$3.txt
./csim $1 $2 $3 write-allocate write-back fifo < swim.trace >> Results_$1_$2_$3.txt

echo "" >> Results_$1_$2_$3.txt
echo "write-allocate write-through fifo" >> Results_$1_$2_$3.txt
./csim $1 $2 $3 write-allocate write-through fifo < swim.trace >> Results_$1_$2_$3.txt

echo "" >> Results_$1_$2_$3.txt
echo "no-write-allocate write-through fifo" >> Results_$1_$2_$3.txt
./csim $1 $2 $3 no-write-allocate write-through fifo < swim.trace >> Results_$1_$2_$3.txt

echo "" >> Results_$1_$2_$3.txt
echo "write-allocate write-back lru" >> Results_$1_$2_$3.txt
./csim $1 $2 $3 write-allocate write-back lru < swim.trace >> Results_$1_$2_$3.txt

echo "" >> Results_$1_$2_$3.txt
echo "write-allocate write-through lru" >> Results_$1_$2_$3.txt
./csim $1 $2 $3 write-allocate write-through lru < swim.trace >> Results_$1_$2_$3.txt

echo "" >> Results_$1_$2_$3.txt
echo "no-write-allocate write-through lru" >> Results_$1_$2_$3.txt
./csim $1 $2 $3 no-write-allocate write-through lru < swim.trace >> Results_$1_$2_$3.txt



