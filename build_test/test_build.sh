#!/bin/bash
# Simple test build without the full project
g++ -std=c++20 -I../inc -c ../tests/test_polygon_2d.cpp -o test_polygon_2d.o 2>&1 | head -50
