#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <numeric>

using namespace std;

struct TestCase {
  const std::vector<unsigned int> dimensions;
  const float *inp;
  const float *exp;
};


/*
 * The 'meanFilteredTensor' function .
 *
 * The function is returns a float*.
 * The function accepts const float* 
 * The function accepts const vector<unsigned int>& dimensions as parameter.
 */
float* meanFilteredTensor(const float* inputVector, const vector<unsigned int>& dimensions) {
  if (!inputVector || dimensions.size() == 0)
    return nullptr;
  int n = 1;
  for(auto const &v: dimensions) {n *= v;}  
  float *retval = new float [n];
  for(int i = 0 ; i < n ; i++) retval[i] = inputVector[i] ;
  int div = dimensions.size() * 2 + 1;
  vector<int> dimen;
  for(auto &v : dimensions) 
      dimen.push_back((int)v);
  dimen.push_back(1);
  reverse(dimen.begin(), dimen.end());
  vector<int> prefixmul(dimen.size());
  prefixmul[0] = 1;
  for(int i = 1; i < prefixmul.size(); i++) 
      prefixmul[i] = prefixmul[i-1] * dimen[i]; 
  for(int i = 1; i < dimen.size(); i++) {
      int cnt = 0;
      for(int j = 0 ; j < n ; j++) {
          if(cnt - prefixmul[i-1] >= 0) {
              retval[j] += inputVector[j-prefixmul[i-1]]; 
          }
          if(cnt + prefixmul[i-1] < prefixmul[i]) {
              retval[j] += inputVector[j+prefixmul[i-1]]; 
          }
          cnt++;
          if(cnt == prefixmul[i]) cnt = 0;
      }
  }
  for(int i = 0 ; i < n ; i++) {
      retval[i] /= div;
  }
  
  return retval;
}

bool checkOutput(std::unique_ptr<float[]>& output, TestCase& tc) {
  const unsigned int size{
    std::accumulate(tc.dimensions.begin(), tc.dimensions.end(), 1u, std::multiplies<unsigned int>())};

  for(int i = 0; i < size; ++i) {
    if(abs(output[i] - tc.exp[i]) > 0.001) {
      std::cout << "Mismatch for i = " << i << ", exp = " << tc.exp[i] << ", res = " << output[i] << std::endl;
      return false;
    }
  }
  return true;
}
  
int main() {
  const float inp1D[] = {17, 2, 33, 41, 3, 6};
  const float exp1D[] = {19.f / 3, 52.f / 3, 76.f / 3, 77.f / 3, 50.f / 3, 9.f / 3};
    
  const float inp2D[] = {0, 1, 2, 3, 4, 
	                 5, 6, 7, 8, 9, 
			 10, 11, 12, 13, 14};
  const float exp2D[] = {6.f / 5,
                         9.f / 5,
                         13.f / 5,
                         17.f / 5,
                         16.f / 5,
                         21.f / 5,
                         30.f / 5,
                         35.f / 5,
                         40.f / 5,
                         35.f / 5,
                         26.f / 5,
                         39.f / 5,
                         43.f / 5,
                         47.f / 5,
                         36.f / 5};

  const float inp3D[] = {47, 46, 45, 44,
                         43, 42, 41, 40,
                         39, 38, 37, 36,
                         35, 34, 33, 32,
                         31, 30, 29, 28,
                         27, 26, 25, 24,
                         23, 22, 21, 20,
                         19, 18, 17, 16,
                         15, 14, 13, 12,
                         11, 10, 9,  8,
                         7,  6,  5,  4,
                         3,  2,  1,  0};
  const float exp3D[] = {23.8571f,    30.f,        29.2857f,    22.4286f,
                         28.2857f,    33.7143f,    32.8571f,    26.4286f,
                         25.4286f,    30.2857f,    29.4286f,    23.5714f,
                         18.1429f,    22.5714f,    21.8571f,    16.7143f,
                         21.4286f,    25.1429f,    24.2857f,    19.5714f,
                         23.f,        26.f,        25.f,        20.7143f,
                         19.5714f,    22.f,        21.f,        17.2857f,
                         14.f,        16.f,        15.1429f,    12.1429f,
                         10.1429f,    11.7143f,    11.f,        8.71429f,
                         10.f ,       10.8571f,    10.f,        8.14286f,
                         7.14286f,    7.42857f,    6.57143f,    5.28571f,
                         4.42857f,    4.28571f,    3.57143f,    3.f};
                         
  std::vector<TestCase> testCases{{{6},       inp1D, exp1D},
                                  {{3, 5},    inp2D, exp2D},
                                  {{3, 4, 4}, inp3D, exp3D}};
                                  
  bool correct = true;
  for(TestCase &tc : testCases) {
    std::unique_ptr<float[]> result{meanFilteredTensor(tc.inp, tc.dimensions)};
    correct &= checkOutput(result, tc);
  }
  std::cout << (correct ? "PASS" : "FAIL") << std::endl;
  
  return 0;
}

