#include <cstdio>
#include <iostream>
#include <stack>
#include <climits>
#include <cmath>

using namespace std;

typedef long long ll;
typedef long double ld;
typedef pair<ll, ll> ii;

ll maxC, minC;
ll pondW, pondH;

ll depth[505][505];

ll dp[505][505][505];

ll calcRes(ll w, ll h, ll dep) {
  // cout << w << "...:::..." << h << endl;
  ll a = min(max(w, h), maxC);
  ll b = min(min(w, h), minC);


  ld res = ld(dep*pondW*pondH)/ld(pondW*pondH - a*b);

  // cout << a << " " << b << " " << dep << " " << res << endl;
  if(floor(res) == res) return a*b*(floor(res) - 1);
  return a*b*floor(res);
}

int main() {
  int a, b;
  cin >> a >> b >> pondH >> pondW;
  minC = min(a, b);
  maxC = max(a, b);

  for(int i = 0; i < pondH; ++i) {
    for(int j = 0; j < pondW; ++j) {
      cin >> depth[i][j];
    }
  }

  for(int k = 0; k < pondH; ++k) {
    for(int i = 0; i < pondW; ++i) {
      ll minD = depth[k][i];
      for(int j = i; j < pondW && j - i < maxC; ++j) {
        minD = min(minD, depth[k][j]);
        dp[k][i][j] = minD;
        // cout << k << ", " << i << ", " << j << ": " << minD << endl; 
      }
    }
    // cout << endl;
  }

  ll result = 0;
  for(int i = 0; i < pondW; ++i) {
    for(int j = i; j < pondW && j - i < maxC; ++j) {
      stack<ii> s;
      s.push(ii(-1, -1));
      for(int k = 0; k < pondH; ++k) {
        // cout << s.top().first << ":::" << dp[k][i][j] << endl;
        while(s.top().first > dp[k][i][j]) {
          ii top = s.top();
          s.pop();
          result = max(result, calcRes(j-i+1, k - s.top().second - 1, top.first));
        }
        s.push(ii(dp[k][i][j], k));
      }

      while(s.top().first != -1) {
        ii top = s.top();
        s.pop();
        result = max(result, calcRes(j-i+1, pondH - s.top().second - 1, top.first));
      }
    }
  }

  cout << result << endl;

  return 0;
}