#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <string>
#include <iostream>
#include <list>
#include <stdio.h>
// include your other headers here
//#include "QueryPreprocessor.h"
#include "PKB.h"
#include "QueryEvaluator.h"
#include "Parser.h"
#include "AbstractWrapper.h"

class TestWrapper : public AbstractWrapper {
 public:
  // default constructor
  TestWrapper();
  
  // destructor
  ~TestWrapper();
  
  // method for parsing the SIMPLE source
  virtual void parse(std::string filename);
  
  // method for evaluating a query
  virtual void evaluate(std::string query, std::list<std::string>& results);
};

#endif
