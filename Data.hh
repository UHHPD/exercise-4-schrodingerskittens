#ifndef DATA_HH
#define DATA_HH

#include <vector>
#include <string>

class Data {
 public:
  Data(const std::string& filename);

  unsigned int size() const { return m_data.size(); }
  double measurement(int i) const { return m_data[i]; }
  double binCenter(int i) const { return ((binHigh(i)+binLow(i))/2); }
  double binLow(int i) const { return m_bins[i]; }
  double binHigh(int i) const { return m_bins[i+1]; }
  double error(int i) const { return m_errors[i]; }
  int checkCompatibility(const Data& data2, int n) const;
  Data operator+(const Data& data2) const;
  double chisquared_ndf() const;

 private:
  Data() {}  // disallow empty data set
  void assertSizes();
  
  std::vector<double> m_data;
  std::vector<double> m_bins;
  std::vector<double> m_errors;
};

#endif
