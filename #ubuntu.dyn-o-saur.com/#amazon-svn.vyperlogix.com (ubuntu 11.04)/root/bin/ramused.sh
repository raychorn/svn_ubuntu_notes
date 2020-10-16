#!/bin/bash

free -t -m
ps -auxf | sort -nr -k 4 | head -10 