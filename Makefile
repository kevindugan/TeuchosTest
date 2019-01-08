

#mpic++ -g -O0 -std=c++11 -o test        -I ${TRILINOS_DIR}/include        -I $MOOSE_DIR/libmesh/installed/include ParallelTest.cpp testWithLibmesh.cpp        -L ${TRILINOS_DIR}/lib -lteuchoscomm -lteuchosparameterlist -lteuchoscore        -L $MOOSE_DIR/libmesh/installed/lib -lmesh_dbg

CXX = mpic++
CXX_FLAGS = -g -O0 -std=c++11
CXX_INCLUDES = -I ${TRILINOS_DIR}/include -I ${MOOSE_DIR}/libmesh/installed/include
LD_FLAGS = -L ${TRILINOS_DIR}/lib -lteuchoscomm -lteuchosparameterlist -lteuchoscore -L ${MOOSE_DIR}/libmesh/installed/lib -lmesh_dbg

test: ParallelTest.cpp testWithLibmesh.cpp
	@$(CXX) $(CXX_FLAGS) -o test $(CXX_INCLUDES) $^ $(LD_FLAGS)

all: test

.PHONY: clean
clean:
	@rm -f test

