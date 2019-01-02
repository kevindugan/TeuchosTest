#include "ParallelTest.h"
#include "libmesh/libmesh.h"

int main (int argc, char ** argv){

  libMesh::LibMeshInit init (argc, argv);

  // Test Parameter List
  ParallelTest test(init.comm());
  test.testPL();

  // Print ranking from each process
  int world_comm_rank, world_comm_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_comm_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_comm_size);
  if (world_comm_rank != 0){
    std::stringstream ss;
    ss << "World CPU: " << world_comm_rank << "/" << world_comm_size << std::endl;
    test.sendMessage(ss, MPI_COMM_WORLD);
  } else {
    std::stringstream ss;
    ss << "World CPU: " << world_comm_rank << "/" << world_comm_size << std::endl;
    test.recvMessage(ss, MPI_COMM_WORLD);
  }

  return 0;
}

