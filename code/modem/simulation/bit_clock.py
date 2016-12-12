#!/bin/python
## Evan Widloski - 2016-11-27
## Test bit synchronization algorithm
from itertools import cycle
import matplotlib.pyplot as plt
import numpy as np

## inputs generated from simulation.m
## .5 noise input
input =  [1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0]
## .2 noise input
# input = [1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0]


bit_period = 8

last_change_time = 16
last_bit_time = 0
last_bit = 1
sync = []
output = []
bit_get = []
for bit in input:
    if last_change_time > 15 and bit != last_bit:
        last_bit_time = bit_period/2
        last_change_time = 0
        sync.append(1)
    else:
        sync.append(0)
    if last_bit_time == 8:
        if bit != last_bit:
            last_change_time = 0
        last_bit = bit
        last_bit_time = 0
        bit_get.append(1)
    else:
        bit_get.append(0)

    last_change_time += 1
    last_bit_time += 1
    output.append(last_bit)


input = np.array(input)
output = np.array(output)
bit_get = np.array(bit_get)
sync = np.array(sync)

plt.subplot(2,1,1)
plt.plot(input, 'b')
plt.plot(np.where(bit_get == 1),1, 'go')
plt.title('Thresholded input')
plt.axis([0, len(output), -.2, 1.2])
# plt.plot(sync, 'g')
plt.subplot(2,1,2)
plt.plot(output, 'r')
plt.title('Final output')
plt.axis([0, len(output), -.2, 1.2])
plt.show()

# import pdb
# pdb.set_trace()
