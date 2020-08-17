#!/bin/sh

# max=10
# for i in `eval "echo {2..$max}"`
# for i in `eval echo {2..$max}`
# for N in {1..50}
# for ((i=1; i<=20; i=i+1));
for i in 1 2 3 4 5
do
    # echo $i
    ./client.py $i &
done
wait