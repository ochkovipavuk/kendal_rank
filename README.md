# kendal_rank

## 1. Algorithm and Parallelization Method

### Algorithm
The algorithm implemented in this project is the **Kendall rank correlation coefficient, which is a measure of the ordinal association between two ranked lists. The Kendall Tau coefficient is calculated based on the number of concordant and discordant pairs in the two lists.

### Parallelization Method
The parallelization is achieved using **OpenMP** in the C++ code. Specifically, the nested loops responsible for counting concordant and discordant pairs are parallelized. The outer loop is parallelized using OpenMP's `#pragma omp parallel for` directive, and the reduction clause is used to safely accumulate the counts of concordant and discordant pairs across threads.

## 2. Instructions to Reproduce Results and Start the Algorithm

### Prerequisites
- **Python 3.X**
- **g++ compiler** with OpenMP support
- **Scipy** library (for check result)

### Reproduce
After you clone the repository and go to the directory:
```
git clone <repository_url>
cd kendal_rank
```
You can upload to directory you files with vectors (one vector - one file) in .txt format and compile .cpp files:
```
g++ -o vec_gen.o vec_gen.cpp -fopenmp -O0
g++ -o knd_tau.o knd_tau.cpp -fopenmp -O0
```
but there is a function in the code that does this.

**Command to run:**
```
python main.py [--lenght <int>] [--max <int>] [--min <int>] [--name <str>] [--generate] [--check] [--compare] [--compile]
```
for example:
```
python main.py --lenght 100000 --max 7 --min 0 --name v --generate --check --compare --compile
```
result on my machine:
```
### input:
 lenght: 100000
 max: 7
 min: 0
 name: v
 generate: True
 compare: True
 check: True
 compile: True

### compile
### generate vectors
### New vector in v_1.txt
### New vector in v_2.txt


### parallel tau: -0.00211319
### time: 31.087150812149048


### sequential tau: -0.00211319
### time: 98.06160616874695


### scipy tau: -0.0018488977975717857
### time: 0.07097792625427246
```
- **lenght** - size of vectors for generate
- **max** - maximum value of element in vectors
- **min** - minimum value of element in vectors
- **name** - name of files for vectors (in result name_{i}.txt where i = 1,2). If you dont generate vectors, you should point file name here in simular form.
- **generate** - force generate new vectors
- **check** - use scipy to check correctly result
- **compare** - use sequance code to compare results
- **complite** - force to compile .cpp files
We can run only:
```
python main.py
```
result:
```
### input:
 lenght: 100000
 max: 9
 min: 1
 name: vector
 generate: False
 compare: False
 check: False
 compile: False



### parallel tau: -0.000594388
### time: 28.02714467048645
```

## 3. Explanation of Parallelization

The parallelization is applied to the **Kendall Tau calculation** in the `knd_tau.cpp` file. The outer loop that iterates over the elements of the vectors is parallelized using OpenMP. The reduction clause is used to safely accumulate the counts of concordant and discordant pairs across multiple threads.

### Parallelized Code Snippet:
```cpp
#pragma omp parallel for reduction(+:concordant, discordant)
for (size_t i = 0; i < n; ++i) {
    for (size_t j = i + 1; j < n; ++j) {
        int a = (list1[i] - list1[j]) * (list2[i] - list2[j]);
        if (a > 0) {
            concordant += 1;
        } 
        else if (a < 0) {
            discordant += 1;
        }
    }
}
```

## 4. Speedup Calculation

### Speedup Formula
The speedup is calculated as **Sequential Time** / **Parallel Time**

### My Example Speedup Calculation
- **Sequential Time**: 98.06 seconds
- **Parallel Time**: 31.09 seconds
- **Speedup**: 3.15

### Number of Threads
By default, omp splits the process into as many threads as there are logical cores in the processor. In my case, by 4. We did not manage this value to make the code more multiplatform, as well as due to the fact that it is supposed to use code on vectors of 100,000 elements or more. Of course, I can't check whether it would make sense to split such a task into 8 threads, for example (I've never seen more cores in home processors), but I don't think the result will be worse. On 4 cores, I checked the calculations and they start to go worse in time only up to 1000 elements of the vector, in other cases, parallel calculations are 2 to 5 times faster. Therefore, I consider the choice of the number of cores by default to be justified.

---
