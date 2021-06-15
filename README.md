# Wise Red-Black Tree
Wise Red-Black Trees are a data structure designed to efficiently manage blocks of consecutive elements in an ordered set ([Learn more](https://arxiv.org/abs/2106.06950)).
## Usage
To use a Wise Red-Black Tree in your project, first make sure to include the header file at the top of the code:
```c
#include "wrbt.h"
```
### Search
To access the block of size ```m (int)``` located in position ```k (int)```:
```c
node_t *node = kthSmallest(TRoot, k); //first node of the block
puts(node->data); //shows node on screen

for (int i=1; i<m; i++){
  node = inOrderSuccessor(node); //remaining nodes
  puts(node->data); //shows node on screen
}
```
**Note: ```k``` must be greater than 0, ```k + m``` less than or equal to the cardinality of the set**.
### Insertion
To append a block of size ```m (int)``` made by elements ```elem (str)```:
```c
for (int i=0; i<m; i++){
  append(elem[i]);
}
```
If you want to insert in a generic position, you also need information on each ```key (int)```:
```c
node_t *node = insertFirst(key[0], elem[0], m); //first node of the block
for (int i=1; i<m; i++){
  node = insertSubsq(node, key[i], elem[i]); //remaining nodes
}
```
**Note: By definition, keys must be positive and unique. As a block, they must also be increasing**.
### Deletion
To delete the block of size ```m (int)``` in position ```k (int)```:
```c
delete(k,m);
```
**Note: ```k``` must be greater than 0, ```k + m``` less than or equal to the cardinality of the set**.
## Compilation and execution
You can compile your file using GCC.

For a basic compilation on UNIX-like systems (e.g. Linux or Mac OS X):
```bash
/usr/bin/gcc -std=gnu11 -O2 -o your_project.out -I wise-red-black-tree/include your_project.c wise-red-black-tree/src/wrbt.c
```
To run:
```bash
./your_project.out
```

> If you don't have permission to execute the output file, please check out [Manage file permissions on Unix-like systems](https://kb.iu.edu/d/abdb).

## License
This project falls under the **GNU LGPLv3** license.
