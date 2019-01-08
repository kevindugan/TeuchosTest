#include "ParallelTest.h"
#include <assert.h>

ParallelTest::ParallelTest(const libMesh::Parallel::Communicator &comm) : libMesh::ParallelObject(comm){}

void ParallelTest::testPL(){
  int world_comm_rank, world_comm_size;
  MPI_Comm_rank(_communicator.get(), &world_comm_rank);
  MPI_Comm_size(_communicator.get(), &world_comm_size);

  auto globalComm = Teuchos::rcp(new Teuchos::MpiComm<int>(Teuchos::opaqueWrapper(_communicator.get())));

  Teuchos::RCP<Teuchos::ParameterList> verain = Teuchos::parameterList(std::string("VERA: ")+std::string("name"));

  Teuchos::updateParametersFromXmlFileAndBroadcast("coupled_1.5D.xml", verain.ptr(),*globalComm);

  verain->print();
  std::cout << std::flush;

  // Print value from parameter list from each process
  if (world_comm_rank != 0){
    std::stringstream ss;
    double density = verain->sublist("CORE").sublist("Materials").sublist("Material_pyrex").get<double>("density");
    assert(std::fabs(density-2.23) < 1E-13);
    ss << "Process " << world_comm_rank << ", Density: " << density << std::endl;
    sendMessage(ss, _communicator.get());
  }else {
    std::stringstream ss;
    double density = verain->sublist("CORE").sublist("Materials").sublist("Material_pyrex").get<double>("density");
    assert(std::fabs(density-2.23) < 1E-13);
    ss << "Process " << world_comm_rank << ", Density: " << density << std::endl;
    recvMessage(ss, _communicator.get());
  }

  verain->print();
  std::cout << std::flush;

}

void ParallelTest::sendMessage(const std::stringstream &messageStream, const MPI_Comm &io_comm){
  int messageLength = messageStream.str().length();
  char message[messageLength+1];
  strcpy(message, messageStream.str().c_str());

  MPI_Send(message, messageLength+1, MPI_CHAR, 0, 0, io_comm);
}

void ParallelTest::recvMessage(const std::stringstream &messageStream, const MPI_Comm &io_comm){
  int comm_size;
  MPI_Comm_size(io_comm, &comm_size);

  int messageLength = messageStream.str().length();
  char message[messageLength+1];
  strcpy(message, messageStream.str().c_str());
  std::cout << message << std::flush;

  for (int process = 1; process < comm_size; process++){
    MPI_Status status;
    MPI_Recv(message, messageLength+1, MPI_CHAR, process, 0, io_comm, &status);
    std::cout << message << std::flush;
  }
}

