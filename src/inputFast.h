/*Raul P. Pelaez 2017. Contains utility classes/functions to read snapshots from a file
  This file differs from input.h in the way a line is processed, this uses strtod instead of stringstream, it speeds up the reading process by 300%, which results in an overall speed up of 600% when using GPU (when the computations is highly IO bound, small rcut, high number of particles).
 */
#ifndef INPUT_H
#define INPUT_H

#include"vector_algebra.cuh"
#include<fstream>
#include<sstream>
#include<string>
#include"superIO.h"
#include<memory>
namespace gdr{
    //Reads numbers from a file or standard input line by line
  class InputParse{
    //shared_ptr<istream> input;
    std::shared_ptr<superIO::superFile> in;
    //FILE *input = nullptr;
    char * currentLine = nullptr;
    size_t linesize = 0;
  public:
    //Read from stdin by default
    InputParse(){ }
    //Take input from cin
    bool open(){
      // input = stdin;
      // if(feof(input)){
      // 	cerr<<"ERROR: Unable to read from stdin!"<<endl;
      // 	return false;
      // }
      // return true;
      in = std::make_shared<superIO::superFile>();
      return true;
    }
    //Open and read from a file
    bool open(std::string fileName){
      // input = fopen(fileName.c_str(), "r");
      // if(!input){
      // 	cerr<<"ERROR: Unable to open file!"<<endl;
      // 	return false;
      // }
      in = std::make_shared<superIO::superFile>(fileName);
      return true;
    }
    //Is input a readable stream?
    bool good(){
      return in->good();
    }
    //Reads a line from input
    char* goToNextLine(){
      // int nr = getline(&currentLine, &linesize, input);
      //This is wc -l fast
      linesize = in->getNextLine(currentLine);
      return currentLine;
    }
    //Reads a line and
    template<class Iterator>
    void parseNextLine(Iterator numbersInLine, int numberColumnsToRead){
      this->goToNextLine();
      //This uses boost if available
      superIO::string2numbers(currentLine, linesize, numberColumnsToRead, numbersInLine);
      
      // //This could be faster using boost::spirit::qi
      // char *l1 = currentLine;
      // char *l2;
      // for(int i=0; i<numberColumnsToRead; i++){
      //   numbersInLine[i] = strtod(l1, &l2);
      // 	l1 = l2;
      // }
    }

  };


}
#endif

namespace gdr{
  //Reads an entire frame to the second argument. Meant to be used with real4,real3 or real2
  template<class vecType>
  void readFrame(InputParse &input, vecType *pos, int numberParticles, int nCoordinatesToRead){
    //Check for input file validity
    if(!input.good()){
      std::cerr<<"ERROR: No open file!"<<std::endl;
      exit(1);
    }
    //Ignore frame separator
    input.goToNextLine();
    
    for(int i= 0; i<numberParticles; i++){

      input.parseNextLine((real*)&pos[i], nCoordinatesToRead);

    }

  }
}
