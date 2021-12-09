from numpy.random import choice
import numpy as np
draw = choice(range(5), 100,
              p=[0.1, 0.1, 0.2, 0.3, 0.3])
print(draw)

a= np.array([1, 2, 3, 4, 5])
a = a / np.sum(a)

print(a)