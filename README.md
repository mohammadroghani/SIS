# SIS

In this repository, we implement sequential importance sampling for estimating expectations over the space of perfect matchings. Moreover, we implement two applications of estimating the number of perfect matchings, counting Latin rectangles, and card guessing with yes/no feedback.

Also, we implement the greedy strategy for the card guessing in polynomial time using dynamic programming.

## Compile & Run

You can clone the repository using the following command:
```
git clone https://github.com/mohammadroghani/SIS.git
```

Please use C++11 to compile files. You can use the following commands to compile files:
```
g++ SIS.cpp -o SIS.out -O2 -std=c++11
g++ card_guessing_SIS.cpp -o card_guessing_SIS.out -O2 -std=c++11
g++ card_guessing_greedy.cpp -o card_guessing_greedy.out -O2 -std=c++11
g++ latin_rect.cpp -o latin_rect.out -O2 -std=c++11
```


Then you can run each of the codes using one of the following commands:
```
./SIS.out
./card_guessing_SIS.out
./card_guessing_greedy.out
./latin_rect.out
```

### SIS.cpp
This file contains the general implementation of the sequential importance sampling for counting the expected number of perfect matchings in the bipartite graph. The input of this file is
```
n m iterations
```
where \\(n\\) is the number of vertices in each part, $m$ is the number of edges, and $iterations$ is the number of iterations used in the sequential importance sampling. The output is equal to the expected number of the perfect matchings that the algorithm finds after this number of iterations.

### card_guessing_SIS.cpp
This file contains the implementation of the application of the sequential importance sampling in card guessing with yes/no feedback. The input of this file is
```
n m samples iterations
```
where $n$ is the number of different types of cards, each type repeated $m$ times, $iterations$ is the number of iterations used in the sequential importance sampling, and $samples$ is the number of the card guessing games that the algorithm simulates. The output contains $samples$ line where each line shows the number of correct guesses on the corresponding game.

### card_guessing_greedy.cpp
This file contains the implementation of greedy strategy in card guessing with yes/no feedback. The input of this file is
```
n m samples iterations
```
where $n$ is the number of different types of cards, each type repeated $m$ times, and $samples$ is the number of the card guessing games that the algorithm simulates. The output contains $samples$ line where each line shows the number of correct guesses on the corresponding game.

### latin_rect.cpp
This file contains the implementation of the application of the sequential importance sampling in counting Latin rectangles. The input of this file is
```
n k samples iterations
```
where $n$ is the number of columns, $k$ is the number of rows such that $k \leq n$, $iterations$ is the number of iterations used in the sequential importance sampling, and $samples$ is the number of the samples that the algorithm simulates. The output contains $samples$ line where each line shows the number of Latin rectangles in one sample.



