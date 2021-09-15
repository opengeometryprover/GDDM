/*  
 * Copyright 2017, Pedro Quaresma, pedro@mat.uc.pt
 *
 * Server TGTP 
 * --> { "Query":"<query>", "Filters":"<filters>" }
 * <-- { "<teoId1>":
 *       { "Name":"<name>","Description":"<description>","Code":"<code>"}
 *       "<teoId2>
 *       { "Name":"<name>","Description":"<description>","Code":"<code>"}
 *       ...
 *       "<teoIdN>
 *       { "Name":"<name>","Description":"<description>","Code":"<code>"}
 *     }
 */
#include <stdio.h>                                                              
#include <unistd.h>                                                             
#include <stdlib.h>                                                             
#include <string.h>                                                             
#include <sys/types.h>                                                          
#include <sys/socket.h>                                                         
#include <netinet/in.h>                                                         
#include <signal.h>
#include <arpa/inet.h>

#include <string>
#include <iostream>
#include <sstream>

// To make the connection with the MySQL satabase
#include "mysql_connection.h"  

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

// BOOST PTree and JSON_PARSER
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

// Connection information
#include "connectionData.hpp"

//using namespace std;

void error(const char *msg) {
  perror(msg); 
  exit(1);
}
                                                                            
/******** DOSTUFF() ********************************* 
 * There is a separate instance of this function for each connection.
 * It handles all communication once a connnection has been
 * established.
*****************************************************/
                                     
void dostuff (int sock) {
  int n;                                                                       
  char buffer[16384];
  
  std::ostringstream tampaoSaida;
  std::string msg;
  std::ostringstream memTemp;
  
  // sql connector variables
  sql::Driver *driver;
  sql::Connection *con;
  sql::Statement *stmt;
  sql::ResultSet *res;

  using boost::property_tree::ptree;
  using boost::property_tree::read_json;
  using boost::property_tree::write_json;
  
  std::string query,filter,mysqlQuery;
  int tamMsg;
  char tamMsgStr[10];
  
  // Read JSON.
  ptree ptR,ptW,ptAux;

  // Read the handshake information
  memset(buffer, '\0', 129);
  n = read(sock,buffer,128);     

  std::string handshake(buffer,128);

  // check the handshake information
  std::ifstream handshakeFile(".handshake.txt");
  std::string handshakes[100];
  int pos,aux;
  
  if (!handshakeFile) {
    error("Handshake file do not exist\n");
  }

  std::streampos current = handshakeFile.tellg();
  handshakeFile.seekg (0, handshakeFile.end);
  bool empty = !handshakeFile.tellg(); // true if empty file
  handshakeFile.seekg (current, handshakeFile.beg); //restore stream position
  
  if (empty) {
    error("Handshake file is empty\n");
  }
  pos=0;
  while (!handshakeFile.eof()) {
    getline(handshakeFile,handshakes[pos]);
    pos++;
  }

  pos--; // last line is always eof

  bool erro;

  erro = true;
  aux = 0;
  while (erro && aux<=pos) {
    if (handshake == handshakes[aux]) {
      erro = false;
    }
    aux++;    
  }

  if (erro) {
    error("not authorised client\n");
  }
  else {
    // send OK to client
    send(sock,"OK", 3, 0);
  }

  // Read the Query (JSON object)
  memset(buffer, '\0', 16384);
  n = read(sock,buffer,16383);                                                   
  if (n < 0) {
    error("ERROR reading from socket");
  }

  std::istringstream is(buffer);
  read_json(is, ptR);

  query = ptR.get<std::string>("Query");
  filter = ptR.get<std::string>("Filters");

   /*
   * Query the database
   */
  // Connect to the DBMS
  driver = get_driver_instance();
  con = driver->connect(DBMS,USERNAME,PASSWORD);
  // Connect to the MySQL database
  con->setSchema(DATABASE);

  // creates a statement 
  stmt = con->createStatement();

  /*
   * Set the query
   * query should be a "WHERE" clause, e.g.  teoName LIKE "%ceva%"
   * filter should be a conjunction, e.g. AND drawerId=3
   */
  mysqlQuery = "SELECT teoId, teoName, shortDescription, code FROM theorems INNER JOIN demonstrations USING (teoId) WHERE "+query+filter;

  // send the query to the DBMS
  res = stmt->executeQuery(mysqlQuery);

  /*
   * Write it as a JSON object
   */
  while (res->next()) { 
    ptAux.put ("Name", res->getString("teoName"));
    ptAux.put ("Description", res->getString("shortDescription"));
    ptAux.put ("Code", res->getString("code"));
    ptW.push_back(ptree::value_type(res->getString("teoId"),ptAux));
  }

  // convert the ptree into a JSON object
  std::ostringstream jsonBuffer;
  write_json (jsonBuffer, ptW, false);

  tamMsg = jsonBuffer.str().size(); // size of the answer
  sprintf(tamMsgStr,"%09d",tamMsg); // transform it to a string of fixed length

  send(sock,tamMsgStr, 11, 0);
  
  send(sock,jsonBuffer.str().c_str(), jsonBuffer.str().size() + 1, 0);
  
  delete res;
  delete stmt;
  delete con;
}                                                                               
                            
int main(int argc, char *argv[]) {
  int sockfd, newsockfd, portno, pid;                                        
  socklen_t clilen;                                                          
  struct sockaddr_in serv_addr, cli_addr;                                    

  // writes the logging time
  time_t timer;
  char buffer[26];
  struct tm* tm_info;
  
  std::ofstream fsaid("/tmp/serverTGTP.log");
  if (!fsaid) {
    std::cerr << "\nIts impossible to open the log file\n\n"; 
    exit(1);
  }
    
  signal(SIGCHLD,SIG_IGN); // avoid zombies
  
  if (argc < 2) {                                                            
    std::cerr << "ERROR, no port provided\n";
    exit(1);                                                               
  }                                                                          
  sockfd = socket(AF_INET, SOCK_STREAM, 0);                                  
  if (sockfd < 0)                                                            
    error("ERROR opening socket");                                          
  bzero((char *) &serv_addr, sizeof(serv_addr));                             
  portno = atoi(argv[1]);                                                    
  serv_addr.sin_family = AF_INET;                                            
  serv_addr.sin_addr.s_addr = INADDR_ANY;                                    
  serv_addr.sin_port = htons(portno);                                        
  if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
    error("ERROR on binding");
  }
  listen(sockfd,5);                                                          
  clilen = sizeof(cli_addr);                                                 
  while (1) { // infinite cycle
    newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);

    fsaid << "Socket: " << inet_ntoa(cli_addr.sin_addr) << ":";
    fsaid << (int) ntohs(cli_addr.sin_port) << "  ";

    // time of connection
    time(&timer);
    tm_info = localtime(&timer);
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    std::string connectionTime(buffer);

    fsaid << connectionTime << std::endl;

    
    if (newsockfd < 0)  {                                                   
      error("ERROR on accept");
    }
    pid = fork();                                                          
    if (pid < 0)                                                           
      error("ERROR on fork");                                            
    if (pid == 0)  {                                                       
      close(sockfd);                                                     
      dostuff(newsockfd);                                                
      exit(0);                                                           
    }                                                                      
    else close(newsockfd);                                                 
  } /* end of while */                                                       
  close(sockfd);
  fsaid.close();
  return 0; /* we never get here */                                          
}
