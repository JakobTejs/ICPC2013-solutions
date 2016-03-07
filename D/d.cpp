#include <cstdio>
#include <iostream>
#include <cmath>
#include <climits>
#include <vector>
#include <algorithm>


using namespace std;

/*
(n | a1, a2, a3, ..., am) = n!/(a1!a2!a3!...am!) hvor a1+a2+a3+...+am=n
*/

typedef long long ll;

ll maxK = ll(1) << 63 - 1;
ll primes[16] = {2,  3,  5,  7,  11, 
                 13, 17, 19, 23, 29, 
                 31, 37, 41, 43, 47, 
                 53};
int currK[16];
vector<pair<ll,ll> > results;

ll calcResult() {
  ll result = 1;
  for(int i = 0; i < 16; ++i) {

  }
  return 0;
}

ll nrResults = 0;

void calcK(int nrPrime, int maxE, ll currVal, ll currResult, int total) {
  if(nrPrime > 15) return;
  ll prime = primes[nrPrime];
  for(int i = 1; i <= maxE && (prime*currVal)/prime == currVal; ++i) {
    currVal *= prime;
    ll result;

    if(((total+i)*(currResult/i))/(total+i) != (currResult/i) ||
       (currResult % i != 0 && ((currResult % i)*(total+i)/i)/(currResult % i) != (total+i)/i) ||
       (currResult % i)*(total+i)/i + (currResult/i)*(total+i) < 0) break;
    result = (currResult % i)*(total+i)/i + (currResult/i)*(total+i);
    results.push_back(make_pair(result, currVal));
    currResult = result;
    calcK(nrPrime+1, i, currVal, currResult, total+i);
  }
}

int main() {
  //possibleK.push_back(2);
  results.push_back(make_pair(1, 2));
  for(int i = 1; i < 63; ++i) {
    calcK(1, i, ll(1) << i, 1, i);
  }
  sort(results.begin(), results.end());

  ll n;
  while(cin >> n) {
    bool found = false;
    for(auto res : results) {
      if(res.first == n) {
        found = true;
        cout << n << " " << res.second << " " << endl;
        break;
      }
      // else if(found) break;
    }
    if(!found) {
      cout << "ERROR: " << n << endl;
      break;
    }
  }

  //sort(possibleK.begin(), possibleK.end());
  //cout << possibleK.size() << endl;

  return 0;
}