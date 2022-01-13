#! /bin/bash

make clean
if make ; then
    echo "Using sequential simulator"
    ./rainfall_seq 1 30 0.25 128 artifacts/sample_128x128.in
    python3 ./check.py 128 artifacts/sample_128x128.out output.txt
    ./rainfall_seq 1 20 0.5 16 artifacts/sample_16x16.in
    python3 ./check.py 16 artifacts/sample_16x16.out output.txt
    ./rainfall_seq 1 10 0.25 4 artifacts/sample_4x4.in
    python3 ./check.py 4 artifacts/sample_4x4.out output.txt
    echo "-------"

    echo "Using parallel"
    for i in 1 2 4 8
    do
    echo "THREAD = $i"
    ./rainfall_pt $i 30 0.25 128 artifacts/sample_128x128.in
    python3 ./check.py 128 artifacts/sample_128x128.out output.txt
    ./rainfall_pt $i 20 0.5 16 artifacts/sample_16x16.in
    python3 ./check.py 16 artifacts/sample_16x16.out output.txt
    ./rainfall_pt $i 10 0.25 4 artifacts/sample_4x4.in
    python3 ./check.py 4 artifacts/sample_4x4.out output.txt
    done
    echo "-------"
else
    echo "-------\nBuild failed, stop demo early"
fi




