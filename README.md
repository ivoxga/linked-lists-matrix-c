# Linked Lists and Graphs in C: Matrix Manipulation

A C practical project that reads an integer matrix from a text file and manipulates it using **linked lists**, without storing the data in arrays. It is split into two phases:

1. **Phase 1:** the matrix as a linked list, with adding/removing rows and columns and editing values.
2. **Phase 2:** the matrix as a **graph** (list of vertices + edge lists), with depth-first search (DFS) and calculation of the highest-sum path.

> Project developed for the course **Advanced Data Structures**, BSc in **Electrical and Computer Engineering**, **Instituto Politécnico do Cávado e Ave (IPCA)**, **1st year**.
  
--- 

## Repository structure

```
.
├── TP_fase1/
│   ├── main.c
│   └── Matriz_Default.txt
├── TP_fase2/
│   ├── main.c
│   └── Matriz_Default.txt
└── README.md
```

---

## Input file format

Both phases read the file `Matriz_Default.txt`, which must be in the directory the program is run from.

- One matrix row per line in the file.
- Integer values separated by `;` (no trailing `;` at the end of a line).
- The file must end with a newline, since the number of rows is obtained by counting `\n` characters.

Example (3x3 matrix):

```
12;7;9
3;14;5
8;6;11
```

If the file does not exist or is empty, the program exits with an error message.

---

## Phase 1: Matrix as a linked list

The matrix values are stored in a single linked list (row by row). The position of each element is computed from the number of rows and columns.

### Features

| Option | Description |
|---|---|
| 1 | Add a row (at the beginning, middle or end) |
| 2 | Add a column (at the beginning, middle or end) |
| 3 | Remove a row |
| 4 | Remove a column |
| 5 | Change the value at a given position |
| 6 | Sum (highest possible sum) |
| 7 | Exit (frees all memory) |

- Rows and columns are counted **starting at 1**.
- At any position prompt, typing `99` returns to the main menu.
- Input validation: invalid characters and non-existent positions are rejected.

### Status of option 6

The sum option follows the **Hungarian algorithm** to find the highest possible sum (an assignment problem). The steps implemented and shown on screen are:

1. Negate all matrix values.
2. Add 900 to every value (to work with positive values and turn the maximization into a minimization).
3. Make the matrix square by adding a row or column of zeros.

**The function is unfinished**: the remaining steps of the algorithm (row and column reduction, zero covering and final assignment) were not implemented.

---

## Phase 2: Matrix as a graph

Each matrix value becomes a **vertex**. Each vertex has two edge lists (`aresta_lado` and `aresta_abaixo`), and the connections between vertices depend on the chosen rule.

### Connection rules

| Rule | Description |
|---|---|
| 1 | Each vertex connects to its **right** neighbour and its **bottom** neighbour |
| 2 | **Even** vertices connect to even ones and **odd** vertices connect to odd ones |

### Available operations (after choosing a rule)

| Option | Description |
|---|---|
| 1 | **Depth-first search (DFS)** from a chosen vertex: lists all possible paths and the sum of each one |
| 2 | **Highest possible sum**: runs DFS with backtracking from every vertex and shows the path with the highest sum |
| 3 | Back to the main menu |

### Concepts used

- Linked lists for vertices and edges
- Dynamic memory allocation and full deallocation (`libertarTodaMemoria`, `libertarTodasAsArestas`)
- Depth-first search (DFS) with **backtracking**
- Visited-vertex marking to avoid cycles

---

## Building and running

Requires a C compiler (for example, GCC/MinGW).

```bash
# Phase 1
cd phase1
gcc main.c -o phase1
./phase1

# Phase 2
cd ../phase2
gcc main.c -o phase2
./phase2
```

On Windows (CMD/PowerShell), run `phase1.exe` / `phase2.exe`.

---

## Notes and known limitations

- **Designed for Windows:** the program uses `system("cls")` to clear the screen and `setlocale(LC_ALL, "Portuguese")` for accented characters. On Linux/macOS the screen is not cleared and the locale may need adjusting.
- **Encoding:** comments and messages contain accented characters (Portuguese). If strange symbols appear, open the files using Windows-1252/ISO-8859-1 encoding.
- **Phase 1:** the highest-sum calculation (Hungarian algorithm) is incomplete.
- **Phase 2:** matrix values are used as vertex identifiers (including as indexes into the visited arrays), so values must be **unique** and within the range `0` to `n-1` for a matrix with `n` elements.
- Comments follow the **Doxygen** format, so HTML documentation can be generated with `doxygen`.

---

## Author

**Ivo Azevedo**
