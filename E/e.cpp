#include <cstdio>
#include <iostream>
#include <vector>
#include <climits>

using namespace std;

typedef long long ll;

int b, s;

bool firstBlock[13];
int nrInBlock[13];
int block[13];
ll C[13][13];
ll startCost;
ll best = 1e15;

bool inProgram[13];


struct Stmt {
  bool repeat;
  int nr;
  ll times;
  vector<Stmt> stmts;

  int last() {
    if(repeat) {
      for(int i = stmts.size()-1; i >= 0; --i) {
        int tmp = stmts[i].last();
        if(tmp != -1) return tmp;
      }
      return -1;
    }
    else {
      if(firstBlock[nr]) return -1;
      return nr;
    }
  }

  int first() {
    if(repeat) {
      for(int i = 0; i < stmts.size(); ++i) {
        int tmp = stmts[i].first();
        if(tmp != -1) return tmp;
      }
      return -1;
    }
    else {
      if(firstBlock[nr]) return -1;
      return nr;

    }
  }

  void updateCost(ll t) {
    if(repeat) {
      int l = -1;
      for(int i = 0; i < stmts.size(); ++i) {
        int next = stmts[i].first();
        if(next != -1 && l != -1 && next != l) C[l][next] += times*t;
        stmts[i].updateCost(times*t);
        if(stmts[i].last() != -1) l = stmts[i].last();
      }

      if(first() != last() && first() != -1) {
        C[last()][first()] += (times-1)*t;
      }
    }
  }
} program;


ll readRepeat(Stmt *repeat) {
  ll cost = 0;
  while(true) {
    char tmp;
    if(!(cin >> tmp)) return cost;
    Stmt stmt;
    switch(tmp) {
    case 'V':
      stmt.repeat = false;
      cin >> stmt.nr;
      --stmt.nr;
      inProgram[stmt.nr] = true;
      ++cost;
      repeat->stmts.push_back(stmt);
      break;
    case 'R':
      stmt.repeat = true;
      cin >> stmt.times;
      cost += stmt.times*readRepeat(&stmt);
      repeat->stmts.push_back(stmt);
      break;
    case 'E':
      return cost;
    }
  }
}

ll checkResult() {
  ll cost = 0;
  for(int i = 0; i < min(13, s*b); ++i) {
    for(int j = 0; j < min(13, s*b); ++j) {
      if(block[i] != block[j]) {
        cost += C[i][j];
      }
    }
  }
  return cost;
}

bool isValid() {
  bool valid = true;
  int total = 0;
  for(int i = 1; i < b; ++i) {
    total += nrInBlock[i];
    for(int j = i+1; j < b; ++j) {
      if(nrInBlock[i] + nrInBlock[j] <= s) valid = nrInBlock[j] == 0;
    }
  }
  return valid;
}

void assignBlock(int nr) {
  if(nr == min(13, s*b)) {
    if(isValid()) {
      ll cost = checkResult();
      bool allFirstBlock = true;
      for(int i = 0; i < min(13, s*b); ++i) allFirstBlock = allFirstBlock && (!inProgram[i] || firstBlock[i]);

      best = min(best, startCost+cost+!allFirstBlock);
    }
  }
  else if(block[nr] == 0) {
    assignBlock(nr+1);
  }
  else {
    for(int i = 1; i < b; ++i) {
      if(nrInBlock[i] >= s) continue;
      block[nr] = i;
      ++nrInBlock[i];
      assignBlock(nr+1);
      --nrInBlock[i];
      block[nr] = -1;
      if(nrInBlock[i] == 0) return;
    }
  }
}

void setFirstBlock(int nr, int left) {
  if(left == 0) {
    for(int i = 0; i < min(13, s*b); ++i) {
      for(int j = 0; j < min(13, s*b); ++j) {
        C[i][j] = 0;
      }
    }
    program.updateCost(1);
    assignBlock(0);
  }
  else {
    for(int i = nr; i <= min(13, s*b) - left; ++i) {
      firstBlock[i] = true;
      block[i] = 0;
      setFirstBlock(i+1, left-1);
      block[i] = -1;
      firstBlock[i] = false;
    }
  }
}

int main() {
  cin >> b >> s;
  
  program.repeat = true;
  program.times = 1;
  startCost = readRepeat(&program);
  for(int i = 0; i < min(13, s*b); ++i) block[i] = -1;
  setFirstBlock(0, s);
  cout << best << endl;
  return 0;
}