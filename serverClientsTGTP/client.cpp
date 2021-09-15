/* 
 * Copyright 2017, Pedro Quaresma, pedro@mat.uc.pt
 *
 * Client WGL (to be called by a WGL's Web-page)
 *
 * --> reads a string (query) from the command line
 * Send handshake information and the query to the server 
 * <-- HANDSHAKE
 * <-- { "Query":"<query>", "Filters":"<filters>" } to the server
 * Gets the answer from the server
 * --> { "<teoId1>":
 *       { "Name":"<name>","Description":"<description>","Code":"<code>"}
 *       "<teoId2>
 *       { "Name":"<name>","Description":"<description>","Code":"<code>"}
 *       ...
 *       "<teoIdN>
 *       { "Name":"<name>","Description":"<description>","Code":"<code>"}
 *     }
 * <-- send the information back to the caller XXX falta saber como ???
 *
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>    // hostent

// C++ libraries
#include <string>
#include <iostream>

// ptree & JSON libraries
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

// handshake information
#include "handshake.hpp"

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

using namespace std;

void error(const char *msg) {
  perror(msg);
  exit(0);
}

int main(int argc, char *argv[]) {
  int sockfd, portno, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  
  char buffer[16384];
  string auxBuffer = ""; // buffer to read the 1 char stream (msg)
  string rBuffer = "";  // read buffer
  string wBuffer = "";  // write buffer
  ostringstream jsonBuffer;
  
  int sizeBuffer; // the value of the income msg
  int numCharReads = 1; // counts the number of chars already read

  ptree pt,ptAnswer,ptAux;
  
  if (argc < 4) {
    fprintf(stderr,"usage %s hostname port 'query'\n", argv[0]);
    exit(0);
  }

  portno = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket");
  server = gethostbyname(argv[1]);
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(portno);
  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) { 
    error("ERROR connecting");
  }

  rBuffer = argv[3];
  
  
  /*
   * Send the handshake information
   */
  n = write(sockfd,HANDSHAKE,128);
  if (n < 0) {
    error("ERROR writing to socket");
  }

  // check if the server sends the OK
  memset(buffer, '\0', 4);
  n = read(sockfd,buffer,3);

  std::string strBuffer(buffer);
  if (strBuffer != "OK") {
    // stops imediatly
    error("The server refused the authentication\n");
  }
    
  /*
   * Ask the query to send to the server
   */

  // Write it as a JSON object

  // Put the elements in a ptree
  pt.put("Query", rBuffer);
  pt.put("Filters","drawerId=4"); // select GeoGebra constructions
  // convert it to a JSON object
  write_json (jsonBuffer, pt, false);
  
  // Send it to the server
  n = write(sockfd,jsonBuffer.str().c_str(),jsonBuffer.str().size());
  if (n < 0) {
    error("ERROR writing to socket");
  }

  /* 
   * Reads the answer
   */
  // reads the size of the msg (as a string of size 10)
  memset(buffer, '\0', 12);
  n = read(sockfd,buffer,11);
  sizeBuffer = atoi(buffer);

  // clean the buffer for a new series of 1 char long
  memset(buffer, '\0',2);
  while (numCharReads <= sizeBuffer) {
    n = read(sockfd,buffer,1);
    if (n < 0) {
      error("ERROR reading from socket");
    }
    auxBuffer += buffer;
    numCharReads++;
  }

  istringstream is (auxBuffer);
  read_json (is, ptAnswer);

  //  printTree (ptAnswer,0);
  string teoId,teoName,teoDescription,teoCode;
  
  cout << auxBuffer;
  
  close(sockfd);
  return 0;
}
