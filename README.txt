To compile this test, Teuchos needs to be installed in ${TRILINOS_DIR}. For this example, Trilinos is installed in
the VERA installation directory

```sh
export TRILINOS_DIR=~/Software/VERA/build-debug/INSTALL
```

Compile using the MPI wrappers for C++

```sh
mpic++ -std=c++11 -o test -I ${TRILINOS_DIR}/include ParallelTest.cpp testWithLibmesh.cpp -L ${TRILINOS_DIR}/lib -lteuchoscomm -lteuchosparameterlist -lteuchoscore
```

Run using mpiexec and exporting Trilinos to the Library Path.

```sh
LD_LIBRARY_PATH=${TRILINOS_DIR}/lib:$LD_LIBRARY_PATH mpiexec -n 8 ./test
```

