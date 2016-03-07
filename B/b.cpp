#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef double ld;

ld p;
ld x;
vector<ld> rates;

void calcRates(int i) {
  for(int j = rates.size(); j <= i+1; ++j) {
    rates.push_back(rates[j - 1]*(1.0-p)/p);
  }
}

ld getRate(int i) {
  calcRates(abs(i));

  return (i < 0 ? 1/rates[-i] : rates[i]);
}

ld calcBest(int L, int W) {
  if(L == W) return 0;
  ld rl = getRate(L);
  ld rw = getRate(W);
  ld p0 = (1 - rl)/(rw - rl);
  // cout << "p0: " <<  p0;
  // cout << " L: " <<  L;
  // cout << " W: " <<  W;
  // cout << " result: " << p0*W + (1 - p0)*(1 - x)*L << endl;

  return p0*W + (1 - p0)*(1 - x)*L;
}

int main() {
  cin >> x >> p;
  p /= 100;
  x /= 100;
  if(p == 0) {
    cout << 0 << endl;
    return 0;
  }
  // cout << "p " << p << endl;

  rates.push_back(1);

  ld best = 0;
  for(int i = 0; true && i < 3000; ++i) {
    ld lround = 0;
    int j = 0;
    for(j = 0; true && j > -25000; --j) {
      ld tmp = calcBest(j, i);
      // cout << j << " " << i << " " << tmp << endl;
      if(tmp < lround) break;
      lround = max(lround, tmp);
    }

    if(lround < best) { 
      // cout << i << " " << j << endl;
      break;
    }
    best = max(best, lround);
  }

  cout << best;

  return 0;
}