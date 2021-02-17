import random
import os
from sinkhorn_knopp import sinkhorn_knopp as skp
from copy import copy, deepcopy
import time
sk = skp.SinkhornKnopp()

n, m = map(int, input().split())


total = 0
rounds = 0

while True:
    correct = 0

    deck = [i for j in range(m) for i in range(n)]
    random.shuffle(deck)

    A = [[1 for j in range(n * m)] for i in range(n * m)]

    for row in range(n * m):
        dist = []
        for column in range(n):
            last = sum(A[row][column * m: (column + 1) * m])
            if last > 0:

                AA = deepcopy(A)
                for i in range(n * m):
                    AA[row][i] = 0
                    AA[i][column * m + last - 1] = 0
                AA[row][column * m + last - 1] = 1

                edges = []
                for i in range(n * m):
                    for j in range(n * m):
                        if AA[i][j] == 1:
                            edges.append((i, j))

                f = open("input.txt", "w")
                f.write('{0} {1} {2} {3}\n'.format(n * m, n * m, len(edges), 100))
                for u, v in edges:
                    f.write('{0} {1}\n'.format(u + 1, v + 1))
                Q = sk.fit(AA)
                for i in range(n * m):
                    for j in range(n * m):
                        f.write('{0}'.format(Q[i][j]))
                        if j + 1 == n * m:
                            f.write('\n')
                        else:
                            f.write(' ')


                f.close()

                os.system("./code.out < input.txt > output.txt")

                cnt = 0
                matches = 0
                with open('output.txt') as file:
                    for line in file:
                        cnt += 1
                        matches += float(line)
                matches /= cnt
                dist.append(matches)

            else:
               dist.append(0)
        index = dist.index(max(dist))
        if deck[row] == index:
            correct += 1
            last = sum(A[row][index * m: (index + 1) * m])
            for i in range(n * m):
                A[row][i] = 0
                A[i][index * m + last - 1] = 0
            A[row][index * m + last - 1] = 1
        else:
            for column in range(index * m, (index + 1) * m):
                A[row][column] = 0

    total += correct
    rounds += 1

    print("rounds ", rounds, ":", total / rounds)