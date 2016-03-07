#include <cstdio>
#include <iostream>
#include <complex>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

typedef long double ld;
typedef long long ll;
typedef complex<ld> com;

ld pi = acos(-1);

ld turn(com a, com b, com c) {
  return imag((a-b)*conj(c-b));
}

ld angle(com a, com b, com c) {
  ld cosVal = (pow(abs(a-b), 2) + pow(abs(a-c), 2) - pow(abs(b-c), 2))/(ld(2.0)*abs(a-b)*abs(a-c));
  if(cosVal < -1) return pi;
  if(cosVal > 1) return 0;
  return acos(cosVal);
}

ld triangleArea(com a, com b) {
  return turn(0, a, b)/2;
}

ld circleArea(com a, com b, ld r) {
  ld phi = angle(0, a, b);

  if(real(a) < real(b)) {
    return -r*r*phi/ld(2);
  }
  else {
    return r*r*phi/ld(2);
  }
}

com omega(com x, com y) {
  return conj(x-y)/(x-y);
}

com rho(com x, com y) {
  return (conj(x) * y - x * conj(y))/(x-y);
}

com proj(com a, com b, com p) {
  return ld(0.5) * conj(p * omega(a,b) + conj(p) - rho(a,b));
}

pair<com, com> lineCircleIntersection(com a, com b, com c, ld r) {
    com p = proj(a, b, c);
    ld dist = abs(p-c);
    if(dist > r) return make_pair(0, 0);
    if(dist == 0) return make_pair(c + com(0, r), c + com(0, -r));
    com ex = sqrt(pow(r/dist, 2) - 1) * (p-c) * com(0,1);
    return make_pair(p + ex, p - ex);
}

bool isBetween(com a, com b, com c) {
  return (
      (real(a) <= real(c) && real(c) <= real(b))
   || (real(a) >= real(c) && real(c) >= real(b))
  )
  && (
      (imag(a) <= imag(c) && imag(c) <= imag(b))
   || (imag(a) >= imag(c) && imag(c) >= imag(b))
  );
}

vector<com> linesegmentCircleIntersection(com a, com b, com c, ld r) {
  com p = proj(a, b, c);
  ld dist = abs(p-c);

  vector<com> intersections;
  if(dist > r) return intersections;

  com ex;
  if(dist == 0) {
    if(conj(a) == conj(c)) {
      ex = com(0, r);
    }
    else {
      ex = com(r, 0);
    }
  } 
  else {
    ex = sqrt(pow(r/dist, 2) - 1) * (p-c) * com(0,1);
  }
  
  if(isBetween(a, b, p + ex)) intersections.push_back(p + ex);
  if(isBetween(a, b, p - ex)) intersections.push_back(p - ex);
  
  if(intersections.size() == 2 && abs(intersections[1] - a) < abs(intersections[0] - a))
    swap(intersections[0], intersections[1]);

  return intersections;
}

int main() {
  int n;
  ld r;
  cin >> n >> r;
  vector<com> points;

  for(int i = 0; i < n; ++i) {
    int x, y;
    cin >> x >> y;
    points.push_back(com(x, y));
  }

  ld sum(0);
  for(int i = 0; i < n; ++i) {
    vector<com> intersections = linesegmentCircleIntersection(points[i], points[(i+1)%n], 0, r);

    com p, p1, p2;
    switch(intersections.size()) {
      case 2:
        p1 = points[i]*r/abs(points[i]);
        p2 = points[(i+1)%n]*r/abs(points[(i+1)%n]);

        // cout << "2:" << endl;
        sum += circleArea(p1, intersections[0], r);
        // cout << sum << endl;
        sum += triangleArea(intersections[0], intersections[1]);
        // cout << sum << endl;
        sum += circleArea(intersections[1], p2, r);
        // cout << sum << endl;

        break;
      case 1:
        if(abs(points[i]) < r) {
          // cout << "1,1" << endl;
          p = points[(i+1)%n]*r/abs(points[(i+1)%n]);

          // cout << points[i] << intersections[0] << p << endl;

          sum += triangleArea(points[i], intersections[0]);
          sum += circleArea(intersections[0], p, r);
        }
        else {
          // cout << "1,2" << endl;
          p = points[i]*r/abs(points[i]);;


          // cout << p << intersections[0] << points[(i+1)%n] << endl;

          sum += circleArea(p, intersections[0], r);
          sum += triangleArea(intersections[0], points[(i+1)%n]);
        }
        break;
      case 0:
        if(abs(points[i]) < r) {
          // cout << "0,1" << endl;

          // cout << points[i] << points[(i+1)%n] << endl;
          sum += triangleArea(points[i], points[(i+1)%n]);
        }
        else {
          // cout << "0,2" << endl;
          p1 = points[i]*r/abs(points[i]);;
          p2 = points[(i+1)%n]*r/abs(points[(i+1)%n]);


          // cout << p1 << p2 << endl;

          sum += circleArea(p1, p2, r);
        }
        break;
    }
    // cout << sum << endl;
  }
  cout << fixed << setprecision(9);
  cout << abs(sum) << endl;


  return 0;
}