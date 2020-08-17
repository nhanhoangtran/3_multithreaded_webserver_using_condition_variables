#!/bin/bash

for N in {1..150}
do

    # ./client.py $N &
    # ./client.rb $N &
    ./client.rb $((($N % 5) + 1)) $N &

done
wait