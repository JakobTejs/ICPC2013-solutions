#include <cstdio>
#include <iostream>
#include <climits>
#include <set>

using namespace std;

int n;
int dolls[505];
int S[505][505];
int M[505];

int minRange[505][505];
int cumFreq[505][505];

int C(int start, int split, int end) {
  int minLeft = minRange[start][split-1];
  int minRight = minRange[split][end-1];

  // Use the cumulative frequencies to determine how many dolls are smaller on one
  // side than the smallest on the other side.
  int cnt = 0;
  if (minLeft < minRight) {
    cnt = cumFreq[start][minRight-1] - cumFreq[split][minRight-1];
  }
  else {
    if (end < n)
      cnt = cumFreq[split][minLeft-1] - cumFreq[end][minLeft-1];
    else
      cnt = cumFreq[split][minLeft-1];
  }

  //  This is how many moves we need here...
  return end - start - cnt;
}

int main() {
  cin >> n;
  for(int i = 0; i < n; ++i) {
    cin >> dolls[i];
    S[i][i+1] = 0;
  }

  // Store all mins.
  for (int i=0; i<n; i++) {
    int tempMin = dolls[i];
    for (int j=i; j<n; j++) {
      if (dolls[j] < tempMin) tempMin = dolls[j];
      minRange[i][j] = tempMin;
    }
  }

  // First we store regular frequencies in here.
  for (int i=0; i<n; i++)
    for (int j=i; j<n; j++)
      cumFreq[i][dolls[j]]++;

  // Now, calculate cumulative frequencies.
  for (int i=0; i<n; i++)
    for (int j=1; j<501; j++)
      cumFreq[i][j] += cumFreq[i][j-1];

  for(int i = n-1; i >= 0; --i) {
    // set<int> numbers;
    // numbers.insert(dolls[i]);
    for(int j = i+2; j <= n; ++j) {
      // if(numbers.count(dolls[j-1])) break;
      // numbers.insert(dolls[j-1]);
      S[i][j] = INT_MAX;

      for(int k = i+1; k < j; ++k) {
        if(S[i][k] != INT_MAX && S[k][j] != INT_MAX) {
          // cout << i << " " << k << " " << j << " " << S[i][k] << " " << S[k][j] << " " << c << endl;
          S[i][j] = min(S[i][j], S[i][k]+S[k][j]+C(i,k,j));
        }
      }

      // cout << i << " " << j << " " << S[i][j] << endl;
    }
  }
  // cout << endl;

  M[n] = 0;
  for(int i = n-1; i >= 0; --i) {
    bool size[505] = {false};
    M[i] = INT_MAX;
    for(int j = i+1; j <= n; ++j) {
      size[dolls[j-1]] = true;
      bool ok = true;
      for(int k = 1; k <= j-i && ok; ++k) {
        ok = size[k];
        // cout << size[k] << endl;
      }


      if(ok && S[i][j] != INT_MAX && M[j] != INT_MAX) {
        // cout << S[i][j] << endl;
        M[i] = min(M[i], S[i][j] + M[j]);
      }
    }

    // cout << i << " " << M[i] << endl;
  }

  if(M[0] == INT_MAX) {
    cout << "impossible" << endl;
  }
  else {
    cout << M[0] << endl;  
  }

  return 0;
}