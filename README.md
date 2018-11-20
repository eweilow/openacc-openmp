# OpenACC / OpenMP experimentation

This repository contains experimental C code for testing the potential speedup attainable when parallelizing code using [https://www.openacc.org/](OpenAcc) and [https://www.openmp.org/](OpenMP)

## Current testing algorithm

```pseudo
N <- 10000

# create a 2D array of ints
results = Int2D[columns = N, rows = N]

for a from 1 to N {
  for b from 1 to N {
    sum <- 0
    for n from 1 to (a+b) {
      sum = sum + n
    }

    # set results at row a and column b to sum
    results[a, b] = sum
  }
}
```

## Current experimentally shown speedup

| Method         | Total time (ms) | Time per (a,b) sum (μs) | Relative speed |
| -------------- | --------------- | ----------------------- | -------------- |
| Unparallelized | 57750           | 0.5775                  | 1              |
| Unparallelized | 15019           | 0.15019                 | 3.8            |
| Unparallelized | 1060            | 0.0106                  | 54.5           |

## How to use

Please note:
_THIS GUIDE IS CURRENTLY WINDOWS-ONLY_

### Step 1 - install PGI

https://developer.nvidia.com/openacc-toolkit

### Step 2 - make sure PGI is in your path

When running `pgcc` in a terminal, the command should output something like `pgcc-Warning-No files to process`.

If it doesn't - try restarting your computer.

### Step 3 - clone this repository

Either clone it with a git client, or simply download it as a zip and unpack it into an appropriate location.

### Step 4 - run the code

There's currently 3 commands in this repository:

`build.bat` - builds and start the code running without parallelization

`build_mp.bat` - builds and start the code running, using parallelization via OpenMP on all CPU cores

`build_acc.bat` - builds and start the code running, using parallelization via OpenACC on a GPU
