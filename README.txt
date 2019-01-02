To compile this test, Teuchos needs to be installed in ${TRILINOS_DIR}. For this example, Trilinos is installed in
the VERA installation directory

```sh
export TRILINOS_DIR=~/Software/VERA/build-debug/INSTALL
```

Compile using the MPI wrappers for C++

Test With Libmesh:

```sh
mpic++ -g -O0 -std=c++11 -o test \
       -I ~/Software/VERA/build-debug/INSTALL/include \
       -I $MOOSE_DIR/libmesh/installed/include ParallelTest.cpp testWithLibmesh.cpp \
       -L ~/Software/VERA/build-debug/INSTALL/lib -lteuchoscomm -lteuchosparameterlist -lteuchoscore \
       -L $MOOSE_DIR/libmesh/installed/lib -lmesh_dbg
```

Test Without Libmesh:

```sh
mpic++ -std=c++11 -o test -I ${TRILINOS_DIR}/include test.cpp -L ${TRILINOS_DIR}/lib -lteuchoscomm -lteuchosparameterlist -lteuchoscore
```

Run using mpiexec and exporting Trilinos to the Library Path.

```sh
LD_LIBRARY_PATH=~/Software/VERA/build-debug/INSTALL/lib:$MOOSE_DIR/libmesh/installed/lib:$LD_LIBRARY_PATH mpiexec -n 8 ./test
```

