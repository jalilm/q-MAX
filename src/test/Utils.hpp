#ifndef UTILS_HPP
#define UTILS_HPP

#include <fstream>
#include <iostream>
#include <cstring>
#include <stdexcept>

using namespace std;

void setupOutputFile(string result_file, ofstream& result_stream, bool sliding, bool ed=false) {
  bool addCSVHeader = false;
  ifstream check_result_stream;
  check_result_stream.open(result_file, fstream::in | fstream::out | fstream::app);
  if (!check_result_stream) {
    throw invalid_argument("Could not open " + result_file + " for reading.");
  }
  if (check_result_stream.peek() == EOF) {
    addCSVHeader = true;
  }
  check_result_stream.close();

  result_stream.open(result_file, fstream::in | fstream::out | fstream::app);
  if (!result_stream) {
    throw invalid_argument("Could not open " + result_file + " for writing.");
  }
  if (addCSVHeader) {
    if (sliding) {
      result_stream << "dataset,type,insertions,size,gamma,W,tau,duration" << endl;
    } else {
	if(ed) {
		result_stream << "dataset,type,insertions,size,gamma,c,duration" << endl;
	} else {
      		result_stream << "dataset,type,insertions,size,gamma,duration" << endl;
	}
    }
  }
}

#endif //UTILS_HPP

