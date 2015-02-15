#!/bin/sh

# g++ >= 4.9 was used
g++ -std=c++1y -I../include *test*.cpp -lgtest -lpthread
