#include "Teuchos_ParameterList.hpp"
//#include <mpi.h>
#include "Teuchos_RCP.hpp"
//#include "Teuchos_Comm.hpp"
#include "Teuchos_DefaultMpiComm.hpp"
#include <Teuchos_XMLParameterListHelpers.hpp>

int main (){

  MPI_Init(NULL, NULL);
  int world_comm_rank, world_comm_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_comm_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_comm_size);

  auto globalComm = Teuchos::rcp(new Teuchos::MpiComm<int>(Teuchos::opaqueWrapper(MPI_COMM_WORLD)));

  Teuchos::RCP<Teuchos::ParameterList> verain = Teuchos::parameterList(std::string("VERA: ")+std::string("name"));

  Teuchos::updateParametersFromXmlFileAndBroadcast("coupled_1.5D.xml", verain.ptr(),*globalComm); 

  //verain->print(std::cout, Teuchos::ParameterList::PrintOptions().showDoc(true).indent(2).showTypes(true));
  if (world_comm_rank != 0){
    std::stringstream ss;
    ss << "Process " << world_comm_rank << ", Density: " << verain->sublist("CORE").sublist("Materials").sublist("Material_pyrex").get<double>("density") << std::endl;

    int messageLength = ss.str().length();
    char message[messageLength+1];
    strcpy(message, ss.str().c_str());

    MPI_Send(message, messageLength+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  }else {
    std::stringstream ss;
    ss << "Process " << world_comm_rank << ", Density: " << verain->sublist("CORE").sublist("Materials").sublist("Material_pyrex").get<double>("density") << std::endl;
    int messageLength = ss.str().length();
    char message[messageLength+1];
    strcpy(message, ss.str().c_str());
    std::cout << message << std::flush;

    for (int process = 1; process < world_comm_size; process++){
      MPI_Status status;
      MPI_Recv(message, messageLength+1, MPI_CHAR, process, 0, MPI_COMM_WORLD, &status);
      std::cout << message << std::flush;
    }
  }


  if (world_comm_rank != 0){
    std::stringstream ss;
    ss << "World CPU: " << world_comm_rank << "/" << world_comm_size << std::endl;

    int messageLength = ss.str().length();
    char message[messageLength+1];
    strcpy(message, ss.str().c_str());

    MPI_Send(message, messageLength+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  } else {
    std::stringstream ss;
    ss << "World CPU: " << world_comm_rank << "/" << world_comm_size << std::endl;
    int messageLength = ss.str().length();
    char message[messageLength+1];
    strcpy(message, ss.str().c_str());
    std::cout << message << std::flush;

    for (int process = 1; process < world_comm_size; process++){
      MPI_Status status;
      MPI_Recv(message, messageLength+1, MPI_CHAR, process, 0, MPI_COMM_WORLD, &status);
      std::cout << message << std::flush;
    }
  }

  MPI_Finalize();
  return 0;
}