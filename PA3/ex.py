#!/usr/bin/env python
import subprocess

for i in [1, 2]:
    for j in [1]:
        qsub_command = """qsub -v rank={0},thread={1} mandelbrot.pbs""".format(i, j)

        print qsub_command

        exit_status = subprocess.call(qsub_command, shell=True)
        if exit_status is 1:
            print "Job R{0}T{1} failed".format(i, j)
print "Done."