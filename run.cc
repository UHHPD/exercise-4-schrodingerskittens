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

void runTests() {
  std::cout << "running tests...\n";
  std::vector<std::function<bool()> > tests(
      {testReadingSize, testReadingMeasurement, testReadingBinEdges,
       testReadingErrors, testCopyConstructor});
  for (auto test : tests)
    std::cout << (test() ? " ok" : " FAILED!") << std::endl;
}
//################################################################################
//################################################################################



int main() {
  using namespace std;

  cout << "******************************************************" << endl;
  runTests();
  cout << "******************************************************" << endl;

  //ignoring everything from above as it doesn't affect me....yet 
  cout << "******************************************************" << endl;


  /** 
  // create an object which holds data of experiment A
  Data datA("exp_A");
  //not so nice way to declare it
  Data datB("exp_B");
  Data datC("exp_C");
  Data datD("exp_D");

  // here is the data from experiment A
  cout << "bin 27: from " << datA.binLow(27) << " to " << datA.binHigh(27)
       << endl;
  cout << "measurement of experiment A in bin 27: " << datA.measurement(27)
       << endl;

*/

  vector<Data> exps;
  exps.push_back(Data("exp_A"));
  exps.push_back(Data("exp_B"));
  exps.push_back(Data("exp_C"));
  exps.push_back(Data("exp_D"));

  // Print measurement in bin 27 for each experiment
  for (int i = 0; i < exps.size(); ++i) {
    cout << "Experiment " << char(i+65) << " measurement in bin 27: (" 
         <<exps[i].binLow(27) <<", "<<exps[i].binHigh(27)<<") "
         << exps[i].measurement(27) <<" ± " <<exps[i].error(27)
         << endl;
  }

cout << endl<< "******************************************************" << endl;
  //i hate commenting it kills the fun in exploring code :)
  //its not i am lazy or anything, plus no one will ever read my comments :(
  //lets take first two exps A and B
  //y1 ± e1, y2 ± e2 and check if del(y) <1 sigma_(dely)
  double y1, y2, e1, e2, del_y, sigma_del_y;
  y1 = exps[0].measurement(27);
  e1 = exps[0].error(27);
  y2 = exps[1].measurement(27);
  e2 = exps[1].error(27);

  del_y = abs(y1-y2);
  sigma_del_y = sqrt(e1*e1 + e2*e2);

  if (del_y < 1 * sigma_del_y) {cout << "compatible"<<endl;}
  else {cout << "NOT compatible"<<endl;}

  //checking mismatch in ExpA and ExpB
  int mismatch = exps[0].checkCompatibility(exps[1], 2);
  if (mismatch!=0) {cout<<"mismatch in ExpA and ExpB "<< mismatch<<endl;}
  else {cout<< "fully compatible"<<endl;}

  cout << endl<< "******************************************************" << endl;
  //checking all combinations explicitly for n=2
  int n=1; 
  mismatch = -1;
  int flag = -1;

  while(flag!=0){
    flag = 0;
    for (int i=0; i<exps.size(); i++){
      for(int j= i+1; j<exps.size(); j++){
          mismatch = exps[i].checkCompatibility(exps[j], n);
          if (mismatch!=0){
              cout << "Exp" << char(i+65) << " and Exp" << char(j+65)
                << " are not compatible for "<< n <<"-sigma" << " with mismatches "
                <<mismatch <<" bins"<<endl;
              flag = mismatch;}
          else{
            cout << "Exp" << char(i+65) << " and Exp" << char(j+65)
                << " are compatible under "<< n <<"-sigma" <<endl;}
      }

    }
    cout << endl<< "******************************************************" << endl;
    n++;
    if(flag==0) {cout<<"All experiments are comatible under " << n-1<< "-sigma"<<endl; break;}
    
    
  }
  cout << endl<< "******************************************************" << endl;
  
  //checking all combinations for optimized n with compatability of more than 90% 
  //the function operator+ also prints the optimized n value
  Data exps_sum = exps[0];
  for (int i=0; i<exps.size(); i++){
    for(int j= i+1; j<exps.size(); j++){
      cout<< "Exp" << char(i+65) << " and Exp" << char(j+65)<<" can be more than 90% ";
      Data exps_sum = exps[i] + exps[j];
    }
  }

  cout << endl<< "******************************************************" << endl;
  //calculating chisq/ndf for each experiment
  vector<double> chisq_ndf;
  for(int i=0; i<exps.size(); i++){
    chisq_ndf.push_back(exps[i].chisquared_ndf());
    cout<< "Exp" << char(i+65) << " chisq/ndf: " << chisq_ndf[i];
    if (chisq_ndf[i]>0.8 && chisq_ndf[i]<1.2) cout<<" (good fit)"<<endl;
    else cout<<" (not so good fit)"<<endl;
    

  }
  
  cout << endl<< "******************************************************" << endl;
  exps_sum = exps[0];
  for(int i=1; i<exps.size(); i++){
    cout<< " Summing Exp with Exp" << char(i+65)<<" can be more than 90% ";
    exps_sum = exps_sum + exps[i];


  }
  cout << endl<< "******************************************************" << endl;
  cout<< "chisq/ndf of summed Exp: " << exps_sum.chisquared_ndf()<<endl;

  return 0;
}
