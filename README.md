# BitcoinGraphAnalysis

## Introduction

This is the repo for CS6850 project at Cornell. The 3 folders (data1, data2, data3) are Bitcoin transaction graphs during 3 time periods.
**data1** represents 2017/11/16 - 2017/12/16, when Bitcoin price *rises* sharply.
**data2** represents 2018/1/5 - 2018/2/5, when Bitcoin price *falls* sharply.
**data3** represents 2017/8/11 - 2017/9/11, when Bitcoin price goes *plain*.

## Usage
First, build the analysis program:
```shell
./build.sh
```

There should be a binary file *stat1*. Sample usage:
```shell
# run analysis on data1 first day
./stat1 1 1
# run analysis on data3 12th day
./stat1 3 12
```

If you wish to run analysis on all datasets, type
```shell
./run_exps.py
```

The result will be printed to stdout and also to file *result.log*
