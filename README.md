# LPR for the Bandwidth Coloring Problem

This repository starts from the official source released with:

> X. Lai, J.-K. Hao, Z. Lü, and F. Glover, “A learning-based path relinking
> algorithm for the bandwidth coloring problem,” *Engineering Applications of
> Artificial Intelligence*, 52 (2016), 81–91.

The immutable import is tagged `upstream-2017-03-10`; see [UPSTREAM.md](UPSTREAM.md)
for URLs and checksums. No official Git repository was found, so the Git history was
created locally from the authors' ZIP before making any changes.

## Revision changes

The search procedure and the published parameter values are retained. The revised driver:

- validates command-line arguments and every reported coloring;
- fixes the one-based node-weight index used by the benchmark format;
- removes fixed-size output-path buffers and the division by zero on zero successes;
- accepts a reproducible base seed and records the seed of every run;
- records per-run CPU and wall-clock times in CSV form;
- releases all allocated graph and search memory; and
- builds warning-free as C++17 with Clang or GCC.

The upstream driver seeded the random generator once from the current time. The revised
driver uses `BASE_SEED + run - 1` for independently reproducible runs. This changes only
the random-stream control and reporting, not the LPR neighborhood, evaluation function,
path-relinking operators, learning mechanism, stopping conditions, or parameter values.

## Build and test

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

## Run

```sh
./build/lpr-bcp INSTANCE K RUNS [BASE_SEED] [RUN_CSV]
```

For example:

```sh
./build/lpr-bcp ../bcp-cpp/dataset/GEOM20.col 21 20 1001 geom20-k21.runs.csv
```

The CSV contains one row per stochastic run. Feasible colorings are written to
`RUN_CSV.solutions.txt`. The program also prints one `LPR_SUMMARY` line. CPU time is
retained because it is the measure reported by the original program; wall-clock time is
added for the revised experimental protocol.
