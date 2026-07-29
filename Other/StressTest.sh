#!/usr/bin/env bash

# Compile once:
# g++ -std=c++17 -O2 A.cpp -o A
# g++ -std=c++17 -O2 A-gen.cpp -o A-gen
# g++ -std=c++17 -O2 A-brute.cpp -o A-brute
# Usage: bash /path/to/Other/StressTest.sh [prefix]  (default: A)
# The generator should read argv[1] as its random seed for reproducibility.

set -u

name=${1:-A}
input="${name}.in"
output="${name}.out"
answer="${name}.ans"

for ((seed = 1; ; ++seed)); do
    "./${name}-gen" "$seed" > "$input" || exit 1

    if ! timeout 2s "./${name}" < "$input" > "$output"; then
        echo "RE/TLE: seed=$seed, input=$input"
        exit 1
    fi

    if ! timeout 5s "./${name}-brute" < "$input" > "$answer"; then
        echo "Brute failed: seed=$seed, input=$input"
        exit 1
    fi

    if ! diff -wB "$output" "$answer" > /dev/null; then
        echo "WA: seed=$seed, input=$input"
        diff -wB "$output" "$answer"
        exit 1
    fi

    printf '\rPassed: %d' "$seed"
done
