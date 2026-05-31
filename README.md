# C Microshell

A simple Unix-like shell written in C.

## Features

- custom shell prompt displaying the current working directory
- built-in `cd` command
- built-in `help` command
- built-in `exit` command
- custom implementation of `head`
- custom implementation of `grep`
- execution of external programs using `fork()` and `execvp()`
- basic error handling

## Compilation

```bash
gcc microshell.c -o microshell
```

## Running

```bash
./microshell
```

## Example commands

```text
help
cd ..
head test.txt
head -n5 test.txt
grep word test.txt
ls -la
pwd
exit
```

## Purpose

This project was developed as part of a university programming course. The goal was to implement a simple Linux shell capable of executing built-in commands and external programs using process management mechanisms available in Unix systems.
