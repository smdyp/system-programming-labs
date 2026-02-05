#!/bin/bash
gcc -o task task.c 2>/dev/null && echo "${1:-test}" | strace ./task 2>&1 | 
awk '/read\(0,/ {r++} /write\(1,/ {w++} END {print "read:", r, "\nwrite:", w}' && rm -f task 2>/dev/null