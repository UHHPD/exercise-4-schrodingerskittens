#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <cmath>

#include "Data.hh"

// generic function comparing two values of some type T used later for int and
// double
template <class T>
bool testEqual(const std::string& name, T expected, T real) {
  if (expected != real) {
    std::cout << "(" << name << ": <" << expected << ">"
              << ", " << real << ") ";
    return false;
  }
  return true;
}





bool testReadingSize() {
  std::cout << "testReadingSize...";
  Data datA("testA");
  return testEqual<int>("size", 1, datA.size());
}

bool testReadingMeasurement() {
  std::cout << "testReadingMeasurement...";
  Data datA("testA");
  return testEqual("measurement", 10., datA.measurement(0));
}

bool testReadingBinEdges() {
  std::cout << "testReadingBinEdges...";
  Data datA("testA");
  return testEqual("bin low", 0., datA.binLow(0)) &&
         testEqual("bin high", 1., datA.binHigh(0));
}

bool testReadingErrors() {
  std::cout << "testReadingErrors...";
  Data datA("testA");
  return testEqual("error", 2., datA.error(0));
}

bool testCopyConstructor() {
  std::cout << "testCopyConstructor...";
  Data a("testA");
  Data b(a);
  Data c = a;  // equivalent to Data c(a)
  return testEqual("measurement", 10., b.measurement(0)) &&
         testEqual("measurement", 10., c.measurement(0));
}

//insert a test for checkCompatibility
bool testCheckCompatibility(){
  std::cout << "testCheckCompatibility...";
  Data a("testA");
  Data b("testB");
  return testEqual("compatibility",1,  a.checkCompatibility(b,1)) &&
         testEqual("compatibility", 0, a.checkCompatibility(b,2));

}

void runTests() {
  std::cout << "running tests...\n";
  std::vector<std::function<bool()> > tests(
      {testReadingSize, testReadingMeasurement, testReadingBinEdges,
       testReadingErrors, testCopyConstructor, testCheckCompatibility});
  for (auto test : tests)
    std::cout << (test() ? " ok" : " FAILED!") << std::endl;
}

int main() {
  using namespace std;

  cout << "******************************************************" << endl;
  runTests();
  cout << "******************************************************" << endl;
  // create an object which holds data of experiment A, B, C, D
  Data datA("exp_A");
  Data datB("exp_B");
  Data datC("exp_C");
  Data datD("exp_D");

  //store them in a vector of data sets:
  std::vector<Data> data_vector {datA, datB, datC, datD};
  std::vector<char> p{ 'A', 'B', 'C', 'D'};

  

  // here is the data from all data sets:
  for(int i = 0; i < 4; i++){
  cout << "bin 27: from " << data_vector[i].binLow(27) << " to " << data_vector[i].binHigh(27)
       << endl;
  cout << "measurement of cross section of experiment " << p[i] << " in bin 27: " << data_vector[i].measurement(27)
       << endl;
  cout << "-------------------------------------" << endl;
  }

  
  //check if two samples agree within given uncertainty: 
  //check for sample A and B
  double y_a = datA.measurement(27);
  double y_b = datB.measurement(27);
  double err_y_a = datA.error(27);
  double err_y_b = datB.error(27);
  double difsig_AB  = abs( y_a - y_b);
  
  //using error propagation formula to find error of this difference
  double err_difsig = sqrt(pow(err_y_a, 2) + pow(err_y_b, 2));

  if ( difsig_AB < err_difsig){
    cout << "For bin 27, measurement A and B lie within 1 standard deviation" << endl;
  }
  else if ( err_difsig < difsig_AB < 2 * err_difsig )
  {
    cout << "For bin 27, measurement A and B lie within 2 standard deviations" << endl;
  }
  else if ( 2 * err_difsig < difsig_AB < 3 * err_difsig){
    cout << "For bin 27, measurement A and B lie within 3 standard deviations" << endl;
  }
  else if (difsig_AB >  3 * err_difsig){
    cout << "For bin 27, the measurements A and B do not lie within 3 standard deviations of each other" << endl;
  }
  

  cout << "******************************************************" << endl;
  cout << " Exercise 1e): testing, if implementation of getAverage worked:" << endl;

  Data average = datA.getAverage(datB, 3);
  cout << "Measurement of average in bin 1: " << average.measurement(1) << endl;
  cout << "Errors of average in bin 1: " << average.error(1) << endl;
  cout << "Implementation of getAverage worked." << endl;




//checking if it plays a role how we combine the data sets for task 2)
/*
  cout << "******************************************************" << endl;
  Data ab = datA.getAverage(datB, 3);
  Data cd = datC.getAverage(datD, 3);
  Data ac = datA.getAverage(datC, 3);
  Data bd = datB.getAverage(datD, 3);

  Data abcd = ab.getAverage(cd, 3);
  Data acbd = ac.getAverage(bd, 3);

  cout << "abcd: " << abcd.measurement(1) << endl;
  cout << "acbd: " << acbd.measurement(1) << endl;
*/



//2a) checking, how many data points differ by more than 2 or 3 standard deviations

for (int i = 0; i < 4; i++){

  for (int j = 0; j < 4 ; j++){

    if(j > i){
      cout << "--------------------" << endl;
      cout << "--------------------" << endl;
      cout << "comparing Data sets " << p[i] << " and " << p[j] << "..." << endl;
      cout << "amount of data points outside 2 sigma: " << data_vector[i].checkCompatibility(data_vector[j], 2) << endl;
      cout << "amount of data points outside 3 sigma: " << data_vector[i].checkCompatibility(data_vector[j], 3) << endl;
    }

  }
}

//for 2 sigma, values should be compatible 96% of the time, meaning out of 56 measurements, I would expect 56*4%= 2.26
//measurements not to be compatible


//2b) checking, if each data set is compatible with background model by using chi2 

cout << "------------------exercise 2b)------------------ " << endl;

for (int i = 0; i < 4; i++){

  cout << " Chi2/ndf value for data set " << p[i] << ": " << data_vector[i].norm_chi_square() << endl;
  //if theoretical model is compatible with measurements, these chi2/ndf values should be close to 1

}


//2c) combine all 4 measurements and compute chi2/ndf
//all values lie within 4 sigma 

Data ab = datA.getAverage(datB, 4);
Data cd = datC.getAverage(datD, 4);
Data combination = ab.getAverage(cd, 4);

cout << "------------exercise 2c)------------------" << endl;
cout << "Combined data set gives chi2/ndf: " << combination.norm_chi_square() << endl; 


  return 0;
}

