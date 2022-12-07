## Bplus-Hash Map and Bplus-tree index structures implementation in C

                
----

This software package provides an implementation of the Bplus-Hash Map and Bplus-tree index structures as a structural and functional simulation of a RDBMS in-memory file system indexing sub-system structures. There are 2 versions of these index structures that store record references based on primary key fields of both integer and string type.

### Quality Assurance

                
----

This package includes a set of unit testing function tools, integration tests and memory testing tools in order to be provided a qualitative dynamic, refactorable and maintainable software package.

#### Memory testing tools

- [Valgrind software package](https://valgrind.org/).

- [Valgrind Memcheck](https://valgrind.org/docs/manual/mc-manual.html#mc-manual.errormsgs) tool for problematic inefficient memory management and memory error detection in C.

- [Valgrind Massif](https://valgrind.org/docs/manual/ms-manual.html) & [Massif-Visualizer](https://apps.kde.org/massif-visualizer/) tools for heap profiling in C.

### Documentation

                
----

A complete analysis of the implemented Bplus-Hash Map and Bplus-tree index structures functional and structural parts theory, design, architecture and efficiency in terms of time performance is provided in the theoretical computational study entitled Analysis and Comparison of Data Structures for Database Indexing that was held and conducted as an academic research of the UOWM ECE department.

The complete study is published and provided via the [ResearchGate](https://www.researchgate.net/profile/Sotirios-Salakos).

### Dataset

                
----

The initial dataset that was reconstructed and used on the computational processes of the conducted studies is provided on [retail banking demo data](https://data.world/lpetrocelli/retail-banking-demo-data).

&nbsp;

&nbsp;

&nbsp;
### Using Library
----

#### Build Dynamic C Library on Linux
----

&nbsp;
To create the dynamic (shared) C library of Bplus-Hash Map and Bplus-tree index structures using GCC:
```
$ gcc -c BplusHashMap.c BplusTree.c DoubleLinkedBplusList.c -fpic
$ gcc *.o -shared -o Lib-BplusHashMap_Bplus-tree_indexes.so
```

#### Using Dynamic C Library
----

&nbsp;
To set up the Bplus-tree index structure integration test:
```
$ gcc -c Integration-Test_Bplus-tree_Insertion_Selection.c -o Integration-Test_Bplus-tree_Insertion_Selection.o
$ gcc -o Integration-Test_Bplus-tree_Insertion_Selection Integration-Test_Bplus-tree_Insertion_Selection.o -L. Lib-BplusHashMap_Bplus-tree_indexes.so
$
$ gcc -c Integration-Test_Bplus-tree_Deletion.c -o Integration-Test_Bplus-tree_Deletion.o
$ gcc -o Integration-Test_Bplus-tree_Deletion Integration-Test_Bplus-tree_Deletion.o -L. Lib-BplusHashMap_Bplus-tree_indexes.so
```

To set up the Bplus-Hash Map index structure integration test:
```
$ gcc -c Integration-Test_BplusHashMap.c -o Integration-Test_BplusHashMap.o
$ gcc -o Integration-Test_BplusHashMap Integration-Test_BplusHashMap.o -L. Lib-BplusHashMap_Bplus-tree_indexes.so
```

To utilize the shared C library use the following command that allows program loader to locate the library by adding it to the LD_LIBRARY_PATH environment variable or add the Lib-BplusHashMap_Bplus-tree_indexes.so file to the /usr/local/lib directory that program loader searches automatically for libraries at runtime: 
```
$ export LD_LIBRARY_PATH=:/full/path/to/library/directory
```

#### Utilizing integration tests
----

&nbsp;
Running tests:
```
$ nohup ./Integration-Test_Bplus-tree_Insertion_Selection > TestingReport-Integration-Test_Bplus-tree_Insertion_Selection.out &
$ nohup ./Integration-Test_Bplus-tree_Deletion > TestingReport-Integration-Test_Bplus-tree_Deletion.out &
$ nohup ./Integration-Test_BplusHashMap > TestingReport-Integration-Test_BplusHashMap.out &
```

#### Utilizing integration tests & Valgrind Memcheck
----

&nbsp;
Run integration tests using valgrind memcheck tool:
```
$ nohup valgrind --tool=memcheck --leak-check=yes ./Integration-Test_Bplus-tree_Insertion_Selection > TestingReport-Integration-Test_Bplus-tree_Insertion_Selection.out &
$ nohup valgrind --tool=memcheck --leak-check=yes ./Integration-Test_Bplus-tree_Deletion > TestingReport-Integration-Test_Bplus-tree_Deletion.out &
$ nohup valgrind --tool=memcheck --leak-check=yes ./Integration-Test_BplusHashMap > TestingReport-Integration-Test_BplusHashMap.out &
```

