#!/bin/sh

# clang++ >= 3.5 was used
clang++ -std=c++1y -I../include *test*.cpp -lgtest -lpthread
