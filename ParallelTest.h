#ifndef PARALLEL_TEST_H
#define PARALLEL_TEST_H

#include "Teuchos_ParameterList.hpp"
#include "Teuchos_RCP.hpp"
#include "Teuchos_DefaultMpiComm.hpp"
#include <Teuchos_XMLParameterListHelpers.hpp>

class ParallelTest{
  public:
    void testPL();

    void sendMessage(const std::stringstream &messageStream, const MPI_Comm &io_comm);
    void recvMessage(const std::stringstream &messageStream, const MPI_Comm &io_comm);
};

#endif // PARALLEL_TEST_H 
