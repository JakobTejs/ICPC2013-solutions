#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

string preStr, inStr, postStr;

bool preIn[30][30][30];
bool prePost[30][30][30];

bool dp[30][30][30][30];

int type[6];

int length;

bool first = true;


struct Node {
  int pre;
  int in;
  int post;
  int length;
  int type;
  int sol;

  bool solution;

  Node() : solution(false), left(NULL), right(NULL), sol(-1) {}
  Node(int pre, int in, int post, int length) :
    pre(pre), in(in), post(post), length(length), 
    solution(false), left(NULL), right(NULL), sol(-1) {}

  Node *left;
  Node *right;

  string PRE() {
    stringstream ss;
    ss << preStr[this->sol];
    if(this->left != NULL)
      ss << this->left->PRE();
    if(this->right != NULL)
      ss << this->right->PRE();

    return ss.str();
  }

  string IN() {
    stringstream ss;
    if(this->left != NULL)
      ss << this->left->IN();
    ss << preStr[this->sol];
    if(this->right != NULL)
      ss << this->right->IN();

    return ss.str();
  }

  bool compare(Node *other) {
    // cout << this->PRE() << " " << other->PRE() << endl;
    // cout << this->length << " " << other->length << endl;


    if(this->PRE().compare(other->PRE()) == 0) {
      return this->IN().compare(other->IN()) < 0;
    }
    return this->PRE().compare(other->PRE()) < 0;
  }

  Node* copy() {
    Node *n = new Node;
    n->left = left;
    n->right = right;
    n->solution = solution;
    n->sol = sol;
    return n;
  }
};

Node* dp2[3][3][3][26][26][26][26];

void solve(Node *node, int pre, int in, int post) {
  // if(node->pre+node->length >= length || node->in+node->length >= length || node->post+node->length >= length) {
  //   cout << node->pre << " " << node->in << " " << node->post << " " << node->length << endl;
  //   cout << length << endl;  // }
  int preType = 3*type[1]+type[0];
  int inType = 3*type[3]+type[2];
  int postType = 3*type[5]+type[4];
  if(node->solution) cout << "WTF!?!?!" << endl;
  if(!dp[node->pre][node->in][node->post][node->length]) return;
  if(dp2[pre][in][post][node->pre][node->in][node->post][node->length] != NULL) {
    // cout << dp2[preType][inType][postType][node->pre][node->in][node->post][node->length]->solution << endl;
    node->left = dp2[pre][in][post][node->pre][node->in][node->post][node->length]->left;
    node->right = dp2[pre][in][post][node->pre][node->in][node->post][node->length]->right;
    node->solution = dp2[pre][in][post][node->pre][node->in][node->post][node->length]->solution;
    node->sol = dp2[pre][in][post][node->pre][node->in][node->post][node->length]->sol;
    return;
  }
  // cout << node->length << endl;

  if(node->length == 0) {
    // if(debug)
    //   cout << node->pre << " " << node->in << " " << node->post << " " << node->length << endl;
    node->sol = node->pre;
    node->solution = true;
    node->left = node->right = NULL;
    return;
  }

  // if(debug) {
  //   cout << node->pre << " " << node->in << " " << node->post << " " << node->length << endl;
  //   cout << pre << in << post << endl << endl;
  // }

  Node *left = new Node();
  left->length = node->length-1;
  left->pre = node->pre + (pre == 0 ? 1 : 0);
  left->in = node->in + (in == 0 ? 1 : 0);
  left->post = node->post + (post == 0 ? 1 : 0);
  solve(left, type[2*pre], type[2*in], type[2*post]);

  Node *right = new Node();
  right->length = node->length-1;
  right->pre = node->pre + (pre == 2 ? 0 : 1);
  right->in = node->in + (in == 2 ? 0 : 1);
  right->post = node->post + (post == 2 ? 0 : 1);
  solve(right, type[2*pre+1], type[2*in+1], type[2*post+1]);

  if(left->solution) {
    node->solution = true;
    node->left = left->copy();
    node->right = NULL;
    node->sol = node->pre + (pre == 0 ? 0 : node->length);
  }

  if(right->solution) {
    // cout << "0" << endl;
    Node *tmp = new Node;
    tmp->solution = true;
    tmp->right = right;
    tmp->sol = node->pre + (pre == 2 ? node->length : 0);
    tmp->length = node->length;

    if(!node->solution || tmp->compare(node)) {

      node->solution = true;
      node->right = right->copy();
      node->left = NULL;
      node->sol = tmp->sol;
    }
  }


  for(int i = 0; i < node->length-1; ++i) {
    left->solution = right->solution = false;

    left->length = i;

    --right->length;

    ++right->pre;
    ++right->in;
    ++right->post;

    solve(left, type[2*pre], type[2*in], type[2*post]);
    solve(right, type[2*pre+1], type[2*in+1], type[2*post+1]);
    if(left->solution && right->solution) {
      // cout << "1" << endl;
      Node *tmp = new Node;
      tmp->solution = true;
      tmp->left = left;
      tmp->right = right;
      tmp->length = node->length;

      switch(pre) {
        case 0:
          tmp->sol = node->pre;
          break;
        case 1:
          tmp->sol = node->pre + i + 1;
          break;
        case 2:
          tmp->sol = node->pre + node->length;
          break;
      }

      if(!node->solution || tmp->compare(node)) {
        node->solution = true;
        node->left = left->copy();
        node->right = right->copy();
        node->sol = tmp->sol;
      }
      // return;
    }
  }

  dp2[pre][in][post][node->pre][node->in][node->post][node->length] = node->copy();
}


void prePrint(Node *t)  {                     
  cout << preStr[t->sol];
  if (t->left != NULL)
    prePrint(t->left);
  if (t->right != NULL) 
    prePrint(t->right);
}

void inPrint(Node *t)  {                     
  if (t->left != NULL)
    inPrint(t->left);
  cout << preStr[t->sol];
  if (t->right != NULL) 
    inPrint(t->right);
}

void postPrint(Node *t)  {                     
  if (t->left != NULL)
    postPrint(t->left);
  if (t->right != NULL) 
    postPrint(t->right);
  cout << preStr[t->sol];  
}

void print(Node *node) {
  // if(!first) cout << endl;
  for(int i = 0; i < 6; ++i) {
    switch(type[i]) {
      case 0:
        cout << "Pre";
        break;
      case 1:
        cout << "In";
        break;
      case 2:
        cout << "Post";
        break;
    }
    if(i!=5) cout << " ";
  }
  cout << endl;

  prePrint(node);
  cout << endl;
  inPrint(node);
  cout << endl;
  postPrint(node);
  cout << endl;
  first = false;
}

int main() {
  cin >> preStr >> inStr >> postStr;
  length = preStr.size();


  for(int i = 0; i < length; ++i) {
    for(int j = 0; j < length; ++j) {
      bool preL[26] = {false};
      bool inL[26] = {false};;
      bool postL[26] = {false};;
      for(int k = 0; i+k < length && j+k < length; ++k) {
        // Do some clever checking
        preL[ preStr[i+k]  - 'A'] = true;
        inL[  inStr[j+k]   - 'A'] = true;
        postL[postStr[j+k] - 'A'] = true;


        bool samePI = true;
        bool samePP = true;
        for(int l = 0; l < 26; l++) {
          samePI = samePI && (preL[l] == inL[l]);
          samePP = samePP && (preL[l] == postL[l]); 
        }

        preIn[i][j][k] = samePI;
        prePost[i][j][k] = samePP;
      }
    }
  }

  for(int i = 0; i < length; ++i) {
    for(int ii = 0; ii < length; ++ii) {
      for(int iii = 0; iii < length; ++iii) {
        for(int k = 0; i+k < length && ii+k < length && iii+k < length; ++k) {
          dp[i][ii][iii][k] = preIn[i][ii][k] && prePost[i][iii][k];
        }
      }
    }
  }


  for(int i = 0; i < 9; ++i) {
    for(int ii = 0; ii < 9; ++ii) {
      for(int iii = 0; iii < 9; ++iii) {
        int freq[3] = {0};
        ++freq[type[1] = i%3];
        ++freq[type[0] = i/3];
        ++freq[type[3] = ii%3];
        ++freq[type[2] = ii/3];
        ++freq[type[5] = iii%3];
        ++freq[type[4] = iii/3];


        // Pre Post In Post In Pre
        // if(type[0] == 0 && type[1] == 2 && type[2] == 1 && type[3] == 2 && type[4] == 1 && type[5] == 0) debug = true;
        // else debug = false;

        if(freq[0] == 2 && freq[1] == 2) {
          for(int j = 0; j < 3; ++j)for(int jj = 0; jj < 3; ++jj)for(int jjj = 0; jjj < 3; ++jjj)for(int i=0; i < 26;++i)for(int ii=0; ii < 26;++ii)for(int iii=0; iii < 26;++iii)for(int iiii=0; iiii < 26;++iiii)
            dp2[j][jj][jjj][i][ii][iii][iiii] = NULL;
          Node *top = new Node(0, 0, 0, length-1);
          solve(top, 0, 1, 2);
          if(top->solution) {
            print(top);
          }
        }
      }
    }
  }

  // 0,0,1,1,2,2
  // 0,0,1,2,1,2
  // 0,0,1,2,2,1
  // 0,0,2,1,1,2
  // 0,0,2,1,2,1
  // 0,0,2,2,1,1

  return 0;
}