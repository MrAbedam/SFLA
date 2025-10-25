# Final System Design Project
### Mohamadreza Abedin , Mohammadhhosein Shabaniraad
#### Dr.Athena Abdi , K.N Toosi University

# SystemC Implementation of the Shuffled Frog Leaping Algorithm (SFLA)
(Implementation of the following paper:[2202.03477v1.pdf](https://github.com/user-attachments/files/23145261/2202.03477v1.pdf))
 
## 📘 Overview
This project provides a **SystemC-based implementation** of the **Shuffled Frog Leaping Algorithm (SFLA)** — a population-based metaheuristic inspired by memetic evolution and social learning among individuals.  
Originally proposed by Eusuff and Lansey (2006), SFLA combines the strengths of **Particle Swarm Optimization (PSO)** and **memetic algorithms**, making it suitable for solving complex combinatorial and continuous optimization problems.

This implementation models SFLA’s distributed and parallel nature through **SystemC** modules and processes, enabling simulation of population evolution at a hardware-description level.

---

## ⚙️ Algorithm Description
The SFLA consists of a population of solutions (“frogs”) divided into subgroups known as **memeplexes**. Each memeplex performs a local search to improve its frogs, while periodic shuffling enables global information sharing.

### Algorithm Phases
1. **Initialization** – Randomly generate the population of frogs (solutions).  
2. **Memeplex Division** – Partition frogs into memeplexes based on fitness.  
3. **Local Search** – Improve the worst frog’s position using best frog influence.  
4. **Global Shuffle** – Exchange information between memeplexes.  
5. **Termination** – Stop when convergence or iteration limits are met.

---

## 🧩 SystemC Architecture
The algorithm is implemented using **SystemC processes and channels**, representing each frog and memeplex as concurrent modules:

- `Frog` module – stores position and fitness  
- `Memeplex` module – handles local search and memetic evolution  
- `Controller` module – manages synchronization and global shuffling  
- `Monitor` module – tracks global best solution and convergence

The architecture supports modular scalability and concurrent evaluation of frogs, reflecting real-world parallel optimization.

---

## 🧠 Features
- Fully modular **SystemC simulation of SFLA**
- Hardware-level modeling of population-based optimization
- Adjustable hyperparameters (population size, memeplex count, step size)
- Supports real-time convergence visualization and logging

---

## 🧮 Example Parameters
| Parameter | Description |
|------------|--------------|
| `m` | Number of memeplexes |
| `n` | Frogs per memeplex |
| `Smax` | Maximum leaping step |
| `N` | Number of local iterations |
| `Fitness()` | User-defined objective function |


## Example 


                           PHASE 0 _ PARAMETERS
NUMBER_OF_FROGS 12

NUMBER_OF_MEMPLEX 2

NUMBER_OF_ITEMS 9

Q_SELECTION 3

L_MAX_ITERATION 2

G_MAX_ITERATION 2

STEP_SIZE_MAX 2

EPSILON_CHANGE_UG 1
 _______________________________________________________________

                 PHASE1 _ INIT FROGS, SET FITNESS AND SORT
_______________________________________________________________
                   PHASE2 _ MEMPLEX PARTITION

    
    Fitness:27
    1 1 0 1 0 0 0 0 1
    
    Fitness:25
    0 1 0 1 1 0 0 0 1
    
    Fitness:23
    0 1 1 0 0 1 0 1 0
    
    Fitness:23
    0 1 1 0 0 1 0 1 0
    
    Fitness:-1
    1 1 0 0 1 1 1 0 1
    
    Fitness:-1
    0 0 0 1 0 1 1 1 0
    
    Fitness:-1
    0 0 1 1 0 1 0 1 0
    
    Fitness:-1
    1 0 0 1 1 1 1 0 0
    
    Fitness:-1
    1 1 0 0 1 0 1 1 0
    
    Fitness:-1
    0 0 1 1 1 0 0 1 1
    
    Fitness:-1
    1 1 1 1 0 0 0 0 1
    
    Fitness:-1
    1 0 0 0 1 1 1 0 0

    
    [27, 0, 0] Solution: 100001011
    
    [23, 0, 1] Solution: 010100110
    
    [-1, 0, 2] Solution: 101110011
    
    [-1, 0, 3] Solution: 010101100
    
    [-1, 0, 4] Solution: 011010011
    
    [-1, 0, 5] Solution: 100001111
    
    [25, 1, 0] Solution: 100011010
    
    [23, 1, 1] Solution: 010100110
    
    [-1, 1, 2] Solution: 011101000
    
    [-1, 1, 3] Solution: 001111001
    
    [-1, 1, 4] Solution: 110011100
    
    [-1, 1, 5] Solution: 001110001
_______________________________________________________________
                   PHASE3 _ PROBABILITY CALCULATION
Shared Probabilities: 0.285714 0.238095 0.190476 0.142857 0.0952381 0.047619
_______________________________________________________________
                   PHASE4 _ SELECT Q FROGS
Frog 0 sent successfully

Frog 1 sent successfully

Frog 2 sent successfully

Frog 3 sent successfully

Frog 4 sent successfully

Frog 5 sent successfully

Frog 6 sent successfully

Frog 7 sent successfully

Frog 8 sent successfully


Frog 9 sent successfully

Frog 10 sent successfully

Frog 11 sent successfully
    
    Frog 0received 27
    
    Frog 1received 25
    
    Frog 2received 23
    
    Frog 3received 23
    
    Frog 4received -1
    
    Frog 5received -1
    
    Frog 6received -1
    
    Frog 7received -1
    
    Frog 8received -1
    
    Frog 9received -1
    
    Frog 10received -1
    
    
    Frog 11received -1

0.285714 0.238095 0.190476 0.142857 0.0952381 0.047619

Selected frogs from Memplex 0: [Fitness: 27, Offset: 0] [Fitness: -1, Offset: 3] [Fitness: 23, Offset: 1]

old least is:-1  010101100

//-----------------------------------------//

0 1

MAX STEPS USED!

Ub fitness and solution: 27  -  100001011

Uw fitness and solution: -1  -  010101100

UWprime    and solution: -1  -  010101111

0
1

MAX STEPS USED!!

Ug fitness and solution: 27  -  100001011

Uw fitness and solution: -1  -  010101100

UWprime    and solution: -1  -  010101111

RANDOM USED BY MEMPLEX: 0

NEW SOLUTION 100001011 NEW FITNESS 27

//-----------------------------------------//

UG FITNESS27

old least is:23  010100110

//-----------------------------------------//

0 2

MAX STEPS USED!

Ub fitness and solution: 27  -  100001011

Uw fitness and solution: 23  -  010100110

UWprime    and solution: 28  -  010100011

NEW SOLUTION 010100011 NEW FITNESS 28

UB USED BY MEMPLEX 0
//-----------------------------------------//

//-----------------------------------------//

UG FITNESS27
M: 0
Memplex 0 Evolution Complete.
    
    Fitness:27
    1 1 0 1 0 0 0 0 1
    
    Fitness:25
    
    Fitness:28
    1 1 0 0 0 1 0 1 0
    
    Fitness:23
    
    Fitness:-1
    1 1 0 0 1 1 1 0 1
    
    Fitness:-1
    
    Fitness:27
    1 1 0 1 0 0 0 0 1
    
    Fitness:-1
    
    Fitness:-1
    1 1 0 0 1 0 1 1 0
    
    Fitness:-1
    
    Fitness:-1
    1 1 1 1 0 0 0 0 1
    
    Fitness:-1

0.285714 0.238095 0.190476 0.142857 0.0952381 0.047619

Selected frogs from Memplex 1: [Fitness: 25, Offset: 0] [Fitness: -1, Offset: 2] [Fitness: 23, Offset: 1]

old least is:-1  011101000

//-----------------------------------------//

1 4

MAX STEPS USED!

Ub fitness and solution: 25  -  100011010

Uw fitness and solution: -1  -  011101000

UWprime    and solution: -1  -  011111010

0
1

MAX STEPS USED!!

Ug fitness and solution: 27  -  100001011

Uw fitness and solution: -1  -  011101000

UWprime    and solution: -1  -  011101011

RANDOM USED BY MEMPLEX: 1

NEW SOLUTION 100001011 NEW FITNESS 27

//-----------------------------------------//

UG FITNESS27

old least is:23  010100110

//-----------------------------------------//

0 2

MAX STEPS USED!

Ub fitness and solution: 27  -  100001011

Uw fitness and solution: 23  -  010100110

UWprime    and solution: 28  -  010100011

NEW SOLUTION 010100011 NEW FITNESS 28

UB USED BY MEMPLEX 1

//-----------------------------------------//

//-----------------------------------------//

UG FITNESS27

M: 1
Memplex 1 Evolution Complete.
    
    Fitness:27
    
    Fitness:25
    0 1 0 1 1 0 0 0 1
    
    Fitness:28
    
    Fitness:28
    1 1 0 0 0 1 0 1 0
    
    Fitness:-1
    
    Fitness:27
    1 1 0 1 0 0 0 0 1
    
    Fitness:27
    
    Fitness:-1
    1 0 0 1 1 1 1 0 0
    
    Fitness:-1
    
    Fitness:-1
    0 0 1 1 1 0 0 1 1
    
    Fitness:-1
    
    Fitness:-1
    1 0 0 0 1 1 1 0 0

Byeee data now!
    
    Fitness:27
    1 1 0 1 0 0 0 0 1
    
    Fitness:25
    0 1 0 1 1 0 0 0 1
    
    Fitness:28
    1 1 0 0 0 1 0 1 0
    
    Fitness:28
    1 1 0 0 0 1 0 1 0
    
    Fitness:-1
    1 1 0 0 1 1 1 0 1
    
    Fitness:27
    1 1 0 1 0 0 0 0 1
    
    Fitness:27
    1 1 0 1 0 0 0 0 1
    
    Fitness:-1
    1 0 0 1 1 1 1 0 0
    
    Fitness:-1
    1 1 0 0 1 0 1 1 0
    
    Fitness:-1
    0 0 1 1 1 0 0 1 1
    
    Fitness:-1
    1 1 1 1 0 0 0 0 1
    
    Fitness:-1
    
    1 0 0 0 1 1 1 0 0

Frog 0received from ev 27

Frog 1received from ev 25

Frog 2received from ev 28

Frog 3received from ev 28

Frog 4received from ev -1

Frog 5received from ev 27

Frog 6received from ev 27

Frog 7received from ev -1

Frog 8received from ev -1

Frog 9received from ev -1

Frog 10received from ev -1

Frog 11received from ev -1

    Fitness:28
    1 1 0 0 0 1 0 1 0
    
    Fitness:28
    1 1 0 0 0 1 0 1 0
    
    Fitness:27
    1 1 0 1 0 0 0 0 1
    
    Fitness:27
    1 1 0 1 0 0 0 0 1
    
    Fitness:27
    1 1 0 1 0 0 0 0 1
    
    Fitness:25
    0 1 0 1 1 0 0 0 1
    
    Fitness:-1
    1 1 0 0 1 1 1 0 1
    
    Fitness:-1
    1 0 0 1 1 1 1 0 0
    
    Fitness:-1
    1 1 0 0 1 0 1 1 0
    
    Fitness:-1
    0 0 1 1 1 0 0 1 1
    
    Fitness:-1
    1 1 1 1 0 0 0 0 1
    
    Fitness:-1
    1 0 0 0 1 1 1 0 0

||||||||| iteration 0 |||||||||
    
    Fitness:28
    1 1 0 0 0 1 0 1 0
    
    Fitness:28
    1 1 0 0 0 1 0 1 0
    
    Fitness:27
    1 1 0 1 0 0 0 0 1
    
    Fitness:27
    1 1 0 1 0 0 0 0 1
    
    Fitness:27
    1 1 0 1 0 0 0 0 1
    
    Fitness:25
    0 1 0 1 1 0 0 0 1
    
    Fitness:-1
    1 1 0 0 1 1 1 0 1
    
    Fitness:-1
    1 0 0 1 1 1 1 0 0
    
    Fitness:-1
    1 1 0 0 1 0 1 1 0
    
    Fitness:-1
    0 0 1 1 1 0 0 1 1
    
    Fitness:-1
    1 1 1 1 0 0 0 0 1
    
    Fitness:-1
    1 0 0 0 1 1 1 0 0
    

    [28, 0, 0] Solution: 010100011
    
    [27, 0, 1] Solution: 100001011
    
    [27, 0, 2] Solution: 100001011
    
    [-1, 0, 3] Solution: 101110011
    
    [-1, 0, 4] Solution: 011010011
    
    [-1, 0, 5] Solution: 100001111
    
    [28, 1, 0] Solution: 010100011
    
    [27, 1, 1] Solution: 100001011
    
    [25, 1, 2] Solution: 100011010
    
    [-1, 1, 3] Solution: 001111001
    
    [-1, 1, 4] Solution: 110011100
    
    [-1, 1, 5] Solution: 001110001
_______________________________________________________________
                   PHASE3 _ PROBABILITY CALCULATION
Shared Probabilities: 0.285714 0.238095 0.190476 0.142857 0.0952381 0.047619
_______________________________________________________________
                   PHASE4 _ SELECT Q FROGS
    Frog 0 sent successfully
    
    Frog 1 sent successfully
    
    Frog 2 sent successfully
    
    Frog 3 sent successfully
    
    Frog 4 sent successfully
    
    Frog 5 sent successfully
    
    Frog 6 sent successfully
    
    Frog 7 sent successfully
    
    Frog 8 sent successfully
    
    Frog 9 sent successfully
    
    Frog 10 sent successfully
    
    Frog 11 sent successfully
    
    Frog 0received 28
    
    Frog 1received 28
    
    Frog 2received 27
    
    Frog 3received 27
    
    Frog 4received 27
    
    Frog 5received 25
    
    Frog 6received -1
    
    Frog 7received -1
    
    Frog 8received -1
    
    Frog 9received -1
    
    Frog 10received -1
    
    Frog 11received -1

0.285714 0.238095 0.190476 0.142857 0.0952381 0.047619

Selected frogs from Memplex 0: [Fitness: 27, Offset: 1] [Fitness: -1, Offset: 4] [Fitness: 28, Offset: 0]

old least is:-1  011010011

//-----------------------------------------//

4 5

MAX STEPS USED!

Ub fitness and solution: 28  -  010100011

Uw fitness and solution: -1  -  011010011

UWprime    and solution: -1  -  011100011

4
5

MAX STEPS USED!!

Ug fitness and solution: 28  -  010100011

Uw fitness and solution: -1  -  011010011

UWprime    and solution: -1  -  011100011

RANDOM USED BY MEMPLEX: 0

NEW SOLUTION 100001011 NEW FITNESS 27

//-----------------------------------------//


UG FITNESS28

old least is:27  100001011

//-----------------------------------------//

3 5

MAX STEPS USED!

Ub fitness and solution: 28  -  010100011

Uw fitness and solution: 27  -  100001011

UWprime    and solution: 29  -  100100011

NEW SOLUTION 100100011 NEW FITNESS 29

UB USED BY MEMPLEX 0

//-----------------------------------------//

//-----------------------------------------//


UG FITNESS28

M: 0

Memplex 0 Evolution Complete.
    
    Fitness:28
    1 1 0 0 0 1 0 1 0
    
    Fitness:28
    
    Fitness:27
    1 1 0 1 0 0 0 0 1
    
    Fitness:27
    
    Fitness:27
    1 1 0 1 0 0 0 0 1
    
    Fitness:25
    
    Fitness:-1
    1 1 0 0 1 1 1 0 1
    
    Fitness:-1
    
    Fitness:29
    1 1 0 0 0 1 0 0 1
    
    Fitness:-1
    
    Fitness:-1
    1 1 1 1 0 0 0 0 1
    
    Fitness:-1


0.285714 0.238095 0.190476 0.142857 0.0952381 0.047619

Selected frogs from Memplex 1: [Fitness: 27, Offset: 1] [Fitness: 28, Offset: 0] [Fitness: -1, Offset: 4]


old least is:-1  110011100

//-----------------------------------------//

0 1

MAX STEPS USED!

Ub fitness and solution: 28  -  010100011
Uw fitness and solution: -1  -  110011100

UWprime    and solution: -1  -  110011111

0
1

MAX STEPS USED!!

Ug fitness and solution: 28  -  010100011

Uw fitness and solution: -1  -  110011100

UWprime    and solution: -1  -  110011111

RANDOM USED BY MEMPLEX: 1

NEW SOLUTION 100001011 NEW FITNESS 27

//-----------------------------------------//

UG FITNESS28

old least is:27  100001011

//-----------------------------------------//

3 5

MAX STEPS USED!

Ub fitness and solution: 28  -  010100011

Uw fitness and solution: 27  -  100001011

UWprime    and solution: 29  -  100100011

NEW SOLUTION 100100011 NEW FITNESS 29

UB USED BY MEMPLEX 1

//-----------------------------------------//

//-----------------------------------------//

UG FITNESS28

M: 1

Memplex 1 Evolution Complete.
    
    Fitness:28
    
    Fitness:28
    1 1 0 0 0 1 0 1 0
    
    Fitness:27
    
    Fitness:27
    1 1 0 1 0 0 0 0 1
    
    Fitness:27
    
    Fitness:25
    0 1 0 1 1 0 0 0 1
    
    Fitness:-1
    
    Fitness:-1
    1 0 0 1 1 1 1 0 0
    
    Fitness:29
    
    Fitness:29
    1 1 0 0 0 1 0 0 1
    
    Fitness:-1
    
    Fitness:-1
    1 0 0 0 1 1 1 0 0

Byeee data now!

    Fitness:28
    1 1 0 0 0 1 0 1 0
    
    Fitness:28
    1 1 0 0 0 1 0 1 0
    
    Fitness:27
    1 1 0 1 0 0 0 0 1
    
    Fitness:27
    1 1 0 1 0 0 0 0 1
    
    Fitness:27
    1 1 0 1 0 0 0 0 1
    
    Fitness:25
    0 1 0 1 1 0 0 0 1
    
    Fitness:-1
    1 1 0 0 1 1 1 0 1
    
    Fitness:-1
    1 0 0 1 1 1 1 0 0
    
    Fitness:29
    1 1 0 0 0 1 0 0 1
    
    Fitness:29
    1 1 0 0 0 1 0 0 1
    
    Fitness:-1
    1 1 1 1 0 0 0 0 1
    
    Fitness:-1
    
    1 0 0 0 1 1 1 0 0

Frog 0received from ev 28

Frog 1received from ev 28

Frog 2received from ev 27

Frog 3received from ev 27

Frog 4received from ev 27

Frog 5received from ev 25

Frog 6received from ev -1

Frog 7received from ev -1


Frog 8received from ev 29

Frog 9received from ev 29

Frog 10received from ev -1

Frog 11received from ev -1

    Fitness:29
    1 1 0 0 0 1 0 0 1
    
    Fitness:29
    1 1 0 0 0 1 0 0 1
    
    Fitness:28
    1 1 0 0 0 1 0 1 0
    
    Fitness:28
    1 1 0 0 0 1 0 1 0
    
    Fitness:27
    1 1 0 1 0 0 0 0 1
    
    Fitness:27
    1 1 0 1 0 0 0 0 1
    
    Fitness:27
    1 1 0 1 0 0 0 0 1
    
    Fitness:25
    0 1 0 1 1 0 0 0 1
    
    Fitness:-1
    1 1 0 0 1 1 1 0 1
    
    Fitness:-1
    1 0 0 1 1 1 1 0 0
    
    Fitness:-1
    1 1 1 1 0 0 0 0 1
    
    Fitness:-1
    1 0 0 0 1 1 1 0 0

||||||||| iteration 1 |||||||||

||||||||||||||||||||||||||||||||||||

||||||||| Final Solution |||||||||||

    28 010100011

||||||||||||||||||||||||||||||||||||

        SystemC 3.0.1-Accellera --- Oct 29 2024 08:34:19
        Copyright (c) 1996-2024 by all Contributors,
        ALL RIGHTS RESERVED

Process finished with exit code 0
