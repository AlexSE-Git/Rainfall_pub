#! /bin/bash

echo "Using sequential simulator"
./rainfall_seq 1 30 0.25 128 artifacts/sample_128x128.in
python3 ./check.py 128 artifacts/sample_128x128.out output.txt
rm output.txt
sleep 1
./rainfall_seq 1 20 0.5 16 artifacts/sample_16x16.in
python3 ./check.py 16 artifacts/sample_16x16.out output.txt
rm output.txt
sleep 1
./rainfall_seq 1 10 0.25 4 artifacts/sample_4x4.in
python3 ./check.py 4 artifacts/sample_4x4.out output.txt
echo "-------"