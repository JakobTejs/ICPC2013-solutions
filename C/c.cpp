#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <set>
#include <climits>

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef double ld;
typedef pair<int, int> ii;
typedef pair<int, bool> ib;

struct Edge {
  int from;
  int to;
  int cap;
  int flow;
  int index;

  Edge(int from, int to, int cap, int flow, int index) :
    from(from), to(to), cap(cap), flow(flow), index(index) {}
};

vector<int> cars;
vector<ii> distEdges[25005];
vector<Edge> edges[25005];
int nrCars[25005];
int dist[25005];

int n, m, c;

void dijkstra() {
  for(int i = 1; i <= n; ++i) dist[i] = -1;
  priority_queue<ii> q;
  q.push(ii(0, 1));

  while(!q.empty()) {
    ii next = q.top();
    q.pop();

    // cout << next.first << ", " << next.second << endl;
    if(dist[next.second] != -1) continue;
    dist[next.second] = -next.first;

    for(auto edge : distEdges[next.second]) {
      if(dist[edge.first] == -1) {
        q.push(ii(-edge.second + next.first, edge.first));
      }
    }
  }
}

bool visited[25005];
bool debug;

int findPath(int s, int minn) {
  if(s == 1) return minn;
  
  visited[s] = true;

  for(int i = 0; i < edges[s].size(); ++i) {
    Edge* e = &edges[s][i];
    int capacity = e->cap - e->flow;
    if(debug)
      cout << s << " "  << e->to << " " << capacity << endl;
    if(!visited[e->to] && capacity > 0) {
      if(int flow = findPath(e->to, min(minn, capacity))) {
        e->flow += flow;
        edges[e->to][e->index].flow -= flow;
        // visited[s] = false;
        return flow;
      }
    }
  }

  // visited[s] = false;
  return 0;
}

bool cared[25005];

int calcFlow(int carDist) {
  // cout << "  " << carDist << endl;
  for(auto car : cars) {
    if(carDist == dist[car] && !cared[car]) {
      cared[car] = true;
      // cout << nrCars[car] << endl;
      edges[0].push_back(Edge(0, car, nrCars[car], 0, edges[car].size()));
      edges[car].push_back(Edge(car, 0, 0, 0, edges[0].size() - 1));
    }
  }
  int result = 0;
  int flow;
  while(flow = findPath(0, INT_MAX)) {
    result += flow;
    // if(result == 198) debug = true;
    // cout << "flow: " << flow << ", " << result << endl;
    for(int i = 0; i <= n; ++i) visited[i] = false;
  }
  // cout << "cleaning up: " << endl;

  for(int i = 1; i <= n; ++i) {
    visited[i] = false;
    cared[i] = false;
    for(int j = 0; j < edges[i].size(); ++j) {
      edges[i][j].flow = 0;
      if(edges[i][j].to == 0) {
        edges[i].resize(j, Edge(0,0,0,0,0));
      }
    }
  }
  edges[0].clear();
  // cout << "finished cleaning up!" << endl;

  // cout << "dist: " << carDist << ", " << result << endl;

  return result;
}

int main() {
  cin >> n >> m >> c;

  for(int i = 0; i < m; ++i) {
    int x, y, t;
    cin >> x >> y >> t;

    distEdges[x].push_back(ii(y,t));
    distEdges[y].push_back(ii(x,t));
  }

  for(int i = 0; i < c; ++i) {
    int car;
    cin >> car;
    cars.push_back(car);
    ++nrCars[car];
  }

  dijkstra();

  for(int i = 1; i <= n; ++i) {
    // cout << i << " " << dist[i] << endl;
    for(auto edge : distEdges[i]) {
      if(dist[i] - edge.second == dist[edge.first]) {
        edges[i].push_back(Edge(i, edge.first, 1, 0, edges[edge.first].size()));
        edges[edge.first].push_back(Edge(edge.first, i, 0, 0, edges[i].size() - 1));
      }
    }
  }

  set<int> calced;
  int result = 0;
  for(auto car : cars) {
    if(calced.count(dist[car])) continue;
    // cout << dist[car] << endl;

    result += calcFlow(dist[car]);

    calced.insert(dist[car]);
  }

  cout << result << endl;

  return 0;
}