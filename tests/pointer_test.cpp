// Standalone checks for pointer.cpp: g++ -std=c++17 -Isrc tests/pointer_test.cpp src/pointer.cpp
#include <cassert>
#include <cmath>
#include <cstdio>

#include "pointer.h"

static bool near(double a, double b, double eps) { return std::abs(a - b) <= eps; }

int main() {
  const double pi = 3.14159265358979323846;
  // Straight up from the centre of an 800x600 chart: tip at the top margin.
  auto up = PointerAtEdge(800, 600, 0, 30);
  assert(near(up.x, 400, 1e-9) && near(up.y, 30, 1e-9));
  // Right: tip at the right margin, on the centre line.
  auto right = PointerAtEdge(800, 600, pi / 2, 30);
  assert(near(right.x, 770, 1e-9) && near(right.y, 300, 1e-9));
  // Towards the lower-right corner of a square chart: on the diagonal.
  auto corner = PointerAtEdge(600, 600, 3 * pi / 4, 0);
  assert(near(corner.x, 600, 1e-9) && near(corner.y, 600, 1e-9));

  assert(IsOnChart(800, 600, 0, 0) && IsOnChart(800, 600, 800, 600));
  assert(!IsOnChart(800, 600, -1, 300) && IsOnChart(800, 600, -5, 300, 10));

  assert(near(ScreenAngle(0, -1), 0, 1e-12));
  assert(near(ScreenAngle(1, 0), pi / 2, 1e-12));
  assert(near(ScreenAngle(0, 1), pi, 1e-12));
  assert(near(ScreenAngle(-1, 0), 3 * pi / 2, 1e-12));

  // One degree of latitude north is 60 NM at bearing 0.
  auto north = RhumbLine(60, 25, 61, 25);
  assert(near(north.bearing, 0, 1e-9) && near(north.distance, 60, 1e-6));
  // East along the equator: 60 NM per degree, bearing 90.
  auto east = RhumbLine(0, 10, 0, 11);
  assert(near(east.bearing, 90, 1e-9) && near(east.distance, 60, 1e-6));
  // East at 60 N: half as far.
  auto east60 = RhumbLine(60, 10, 60, 11);
  assert(near(east60.bearing, 90, 1e-9) && near(east60.distance, 30, 1e-6));
  // Across the date line the short way: 1 degree east, not 359 west.
  auto dateline = RhumbLine(0, 179.5, 0, -179.5);
  assert(near(dateline.bearing, 90, 1e-9) && near(dateline.distance, 60, 1e-6));

  double lat, lon;
  MercatorAlong(60, 25, 61, 26, 0, &lat, &lon);
  assert(near(lat, 60, 1e-9) && near(lon, 25, 1e-9));
  MercatorAlong(60, 25, 61, 26, 1, &lat, &lon);
  assert(near(lat, 61, 1e-9) && near(lon, 26, 1e-9));
  MercatorAlong(0, 179.5, 0, -179.5, 0.5, &lat, &lon);
  assert(near(std::abs(lon), 180, 1e-9));

  std::puts("pointer tests passed");
  return 0;
}
