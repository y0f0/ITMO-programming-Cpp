#ifndef HOMEWORK1_TRIANGLE_H
#define HOMEWORK1_TRIANGLE_H

#include "Polygon.h"

class Triangle : public Polygon {
public:
  Triangle();
  Triangle(int n, Point *points);

  Triangle(const Triangle& other);
  using PolygonalChain::operator=;
  ~Triangle();

  bool hasRightAngle() const;
};


#endif //HOMEWORK1_TRIANGLE_H
