#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;

bool edges[55][55];
bool visited[55];

void connectEdges(int a, int b, int c, int d) {
  if(a == -1) return;

  if(b != -1) edges[a][(b + 27) % 54] = true;
  if(c != -1) edges[a][(c + 27) % 54] = true;
  if(d != -1) edges[a][(d + 27) % 54] = true;
}

bool df(int i) {
  if(visited[i]) return true;
  visited[i] = true;

  for(int j = 0; j < 54; ++j) {
    if(edges[i][j]) {
      if(df(j)) return true;
    }
  }
  visited[i] = false;
  return false;
}

int main() {
  int n;
  cin >> n;

  for(int i = 0; i < n; ++i) {
    string tmp;
    cin >> tmp;
    int a[4];
    for(int i = 0; i < 4; ++i) {
      if(tmp[i*2] == '0') {
        a[i] = -1;
        continue;
      }
      a[i] = (tmp[i*2] - 'A');
      if(tmp[i*2 + 1] == '-') a[i] += 27;
    }
    connectEdges(a[0], a[1], a[2], a[3]);
    connectEdges(a[1], a[0], a[2], a[3]);
    connectEdges(a[2], a[1], a[0], a[3]);
    connectEdges(a[3], a[1], a[2], a[0]);
  }

  for(int i = 0; i < 54; ++i) {
    if(df(i)) {
      cout << "unbounded" << endl;
      return 0;
    }
  }

  cout << "bounded" << endl;
  return 0;
}