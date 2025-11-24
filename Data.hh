#ifndef DATA_HH
#define DATA_HH

#include <vector>
#include <string>

class Data {
 public:
  Data(const std::string& filename);

  unsigned int size() const { return m_data.size(); }
  double measurement(int i) const { return m_data[i]; }
  double binCenter(int i) const {return  0.5 * (m_bins[i] + m_bins[i+1] ); }
  double binLow(int i) const { return  m_bins[i];}
  double binHigh(int i) const {return m_bins[i+1]; }
  double error(int i) const { return m_errors[i]; }
  int checkCompatibility(const Data& in, int n) const;
  Data getAverage(const Data& other, int n) const;
  std::vector<double> prediction() const;
  double norm_chi_square() const;
  

 private:
  Data() {}  // disallow empty data set
  void assertSizes();
  std::vector<double> m_data;
  std::vector<double> m_bins;
  //add vector for errors
  std::vector<double> m_errors;
};

#endif
