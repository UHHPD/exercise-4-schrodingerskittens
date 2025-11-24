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
  for (int i = 0; i < size; i++) {//implement another for loop to read in the errors
    double errors;
    file >> errors;
    m_errors.push_back(errors);
  }
  

  // done! close the file
  file.close();

  assertSizes();

};

void Data::assertSizes() { assert(m_data.size() + 1 == m_bins.size()); };

//write int method to check if data points are compatible to another object of type Data
int Data::checkCompatibility(const Data& in, int n) const {
  double difference{};
  double error{};
  int counter{};

  for (int i = 0; i < m_data.size(); i++){

  difference = abs(m_data[i]- in.m_data[i]);
  error = sqrt(pow(m_errors[i], 2) + pow(in.m_errors[i], 2));

  if (difference / error > n){
    counter++;
  }
  }
  return counter;
}

//e) write function that averages two data sets and returns the result
Data Data::getAverage(const Data& other, int n) const{
  double y = {};
  double sigma_y {};
  std::vector<double> measurements_y;
  std::vector<double> err_y;

  if((this->checkCompatibility(other, n)) != 0){
    cout << "WARNING: The two Data sets are incompatible. Some values of the two sets lie outside of " << n << " sigma of each other." << endl;
  }

    Data result; //creates empty Data object that we will fill up now

    for(int i = 0; i < m_data.size(); i++){
      y = ((m_data[i]/pow(m_errors[i], 2)) + (other.m_data[i]/pow(other.m_errors[i], 2))) / (1/pow(m_errors[i],2) + 1/pow(other.m_errors[i], 2));
      sigma_y = sqrt(1/((1/pow(m_errors[i], 2)) + 1/pow(other.m_errors[i], 2)));

      measurements_y.push_back(y);
      err_y.push_back(sigma_y);
    }

    result.m_bins = this->m_bins;
    result.m_data = measurements_y;
    result.m_errors = err_y;

    


    

    return result;

  }

  vector<double> Data::prediction() const{

    vector<double> pred_values{};
    double f_value = 0;

    for(int i = 0; i < m_data.size(); i++){

      f_value = 0.005 - 0.00001 * binCenter(i) + 0.08 * exp(-0.015 * binCenter(i));

      pred_values.push_back(f_value);
    
    }

    return pred_values;

  }

  double Data::norm_chi_square() const {

    double sum {0};
    

    for (int i = 0; i < m_data.size(); i++){

      sum += pow(m_data[i] - prediction()[i], 2) / pow(error(i),2);

    }

    return  sum / 56; // 56 is degrees of freedom
    
  }
  



