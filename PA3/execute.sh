#!/bin/bash

for i in {1..2}
    do
        for j in 1
        do
            cat > mandelbrot${i}R${j}T.pbs 
        done
    done
