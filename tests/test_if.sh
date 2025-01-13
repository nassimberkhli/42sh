#!/bin/bash

if [ 3 -gt 2 ]; then
    echo "Test 1: Great work"
fi

if [ 5 -lt 3 ]; then
    echo "Test 2: Not well"
else
    echo "Test 2: Great work"
fi

if [ 4 -lt 3 ]; then
    echo "Test 3: Not well"
elif [ 4 -eq 4 ]; then
    echo "Test 3: Great work"
else
    echo "Test 3: Not well"
fi

if [ 2 -eq 2 ]; then
    if [ 5 -ne 5 ]; then
        echo "Test 4: Not well"
    else
        echo "Test 4: Great work"
    fi
fi

if [ 4 -gt 3 ] && [ 2 -eq 2 ]; then
    echo "Test 5: Great work"
else
    echo "Test 5: Not well"
fi

if [ 5 -eq 6 ] || [ 1 -lt 2 ]; then
    echo "Test 6: Great work"
else
    echo "Test 6: Not well"
fi

if [ "hello" = "hello" ]; then
    echo "Test 7: Great work"
else
    echo "Test 7: Not well"
fi
