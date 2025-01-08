#!/bin/bash

if [ "$1" == "hello" ]; then
    echo "You said hello"
elif [ "$1" == "goodbye" ]; then
    echo "You said goodbye"
else
    echo "Unrecognized input"
fi

for i in 1 2 3; do
    echo "Iteration $i"
done

counter=0
while [ $counter -lt 3 ]; do
    echo "Counter: $counter"
    counter=$((counter + 1))
done

counter=5
until [ $counter -le 0 ]; do
    echo "Counting down: $counter"
    counter=$((counter - 1))
done
