#include "Data.hh"

#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>
#include <cmath>

using namespace std;

Data::Data(const std::string& filename) {
  ifstream file(filename);

  // check if file opened correctly
  if (!file.is_open()) {
    throw std::runtime_error("Error could not open file with name: " +
                             filename);
  }

  int size = -1;
  file >> size;

  // read in data from file: m_siz+1 bin edges
  for (int i = 0; i < size + 1; ++i) {
    double edge;
    file >> edge;
    m_bins.push_back(edge);
  }
  // read in data from file: m_siz bin contents
  for (int i = 0; i < size; ++i) {
    double entries;
    file >> entries;
    m_data.push_back(entries);
  }
  // read in data from file: m_siz bin contents
  for (int i = 0; i<size; ++i){
    double errors;
    file >> errors;
    m_errors.push_back(errors);
  }

  // done! close the file
  file.close();

  assertSizes();
};

void Data::assertSizes() { assert(m_data.size() + 1 == m_bins.size()); }

int Data::checkCompatibility(const Data& data2, int n) const {
    int count = 0;

    for (int i = 0; i < m_data.size(); ++i) {
        
        double y1, y2, e1, e2, del_y, sigma_del_y;
        y1 = this->measurement(i);
        e1 = this->error(i);

        y2 = data2.measurement(i);
        e2 = data2.error(i);
        
        del_y = abs(y1 - y2);
        sigma_del_y = sqrt(e1*e1 + e2*e2);

        if (del_y > n * sigma_del_y)
            count++;
    }

    return count;
}

Data Data::operator+(const Data& data2) const {
    int mismatch, n = 1, size = this->size();
    double uncompatibility = 100;

    //optimizes n for less than 10% uncompatibility
    while(n<4){
        mismatch = this->checkCompatibility(data2, n);
        uncompatibility = (double(mismatch)/size)*100;
        if (uncompatibility<10) break; 
        n++;
    }
    cout<<"compatible at n = " <<n<<endl;
    Data sum = *this;
    for (int i=0; i<size; i++){

        double y1, y2, w1, w2, y_sum, e_sum;
        y1 = this->measurement(i);
        w1 = 1/(this->error(i));
        
        y2 = data2.measurement(i);
        w2 = 1/(data2.error(i));

        y_sum = (y1*w1 + y2*w2)/(w1+w2);
        e_sum = sqrt(1/(w1+w2));

        sum.m_data[i] = y_sum;
        sum.m_errors[i] = e_sum;

    }

    return sum;
    
}

double f(double x, double alpha=0.005, double beta= -0.00001, double gamma=0.08, double delta=0.015){
  return (alpha + beta*x + gamma*exp(-delta*x));
}

double Data::chisquared_ndf() const {

    double sum = 0.0, ndf;
    ndf = this->size(); //number of degrees of freedom
    for (int i = 0; i<m_data.size(); i++){

      sum =  sum + pow((this->measurement(i) - f(this->binCenter(i)))/this->error(i), 2);

    }
    return (sum/ndf);
}