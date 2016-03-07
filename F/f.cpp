#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;

int n, k;
vector<int> batteries;

int main() {
  cin >> n >> k;

  for(int i = 0; i < 2*k*n; ++i) {
    int power;
    cin >> power;
    batteries.push_back(power);
  }

  sort(batteries.begin(), batteries.end());

  int max = 1e9;
  int min = 0;

  while(min < max) {
    int mid = (min + max)/2;
    bool possible = true;
    int j = 0;
    for(int i = 0; i < n && possible; ++i) {
      bool found = false;
      for(; j <= 2*k*i && !found; ++j) {
        if(batteries[j+1] - batteries[j] <= mid) {
          found = true;
          ++j;
        }
      }
      possible = found;
    }

    if(possible) {
      max = mid;
    }
    else {
      min = mid + 1;
    }
  }

  cout << max << endl;
  return 0;
}