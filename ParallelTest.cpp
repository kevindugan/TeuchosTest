#include "ParallelTest.h"

void ParallelTest::testPL(){
  int world_comm_rank, world_comm_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_comm_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_comm_size);

  auto globalComm = Teuchos::rcp(new Teuchos::MpiComm<int>(Teuchos::opaqueWrapper(MPI_COMM_WORLD)));

  Teuchos::RCP<Teuchos::ParameterList> verain = Teuchos::parameterList(std::string("VERA: ")+std::string("name"));

  Teuchos::updateParametersFromXmlFileAndBroadcast("coupled_1.5D.xml", verain.ptr(),*globalComm);

  // Print value from parameter list from each process
  if (world_comm_rank != 0){
    std::stringstream ss;
    ss << "Process " << world_comm_rank << ", Density: " << verain->sublist("CORE").sublist("Materials").sublist("Material_pyrex").get<double>("density") << std::endl;
    sendMessage(ss, MPI_COMM_WORLD);
  }else {
    std::stringstream ss;
    ss << "Process " << world_comm_rank << ", Density: " << verain->sublist("CORE").sublist("Materials").sublist("Material_pyrex").get<double>("density") << std::endl;
    recvMessage(ss, MPI_COMM_WORLD);
  }
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

