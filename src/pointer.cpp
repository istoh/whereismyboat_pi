/***************************************************************************
 *   Copyright (C) 2026 Isto Hunttila                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/

/** \file pointer.cpp Implement pointer.h. */

#include "pointer.h"

#include <algorithm>
#include <cmath>
#include <limits>

EdgePointer PointerAtEdge(double width, double height, double angle,
                          double margin) {
  const double dx = std::sin(angle);
  const double dy = -std::cos(angle);
  const double half_w = std::max(0.0, width / 2 - margin);
  const double half_h = std::max(0.0, height / 2 - margin);
  // Walk from the centre along the direction until the inset rectangle.
  const double inf = std::numeric_limits<double>::infinity();
  const double along = std::min(std::abs(dx) > 1e-9 ? half_w / std::abs(dx) : inf,
                                std::abs(dy) > 1e-9 ? half_h / std::abs(dy) : inf);
  return {width / 2 + dx * along, height / 2 + dy * along, angle};
}

bool IsOnChart(double width, double height, double x, double y,
               double margin) {
  return x >= -margin && y >= -margin && x <= width + margin &&
         y <= height + margin;
}

namespace {
constexpr double kPi = 3.14159265358979323846;
constexpr double kRad = kPi / 180;

double MercatorY(double lat) { return std::log(std::tan(kPi / 4 + lat * kRad / 2)); }

/** Longitude difference the short way round, in degrees. */
double LonDelta(double lon0, double lon1) {
  double delta = std::fmod(lon1 - lon0 + 540.0, 360.0) - 180.0;
  return delta == -180.0 ? 180.0 : delta;
}
}  // namespace

Course RhumbLine(double lat0, double lon0, double lat1, double lon1) {
  const double dlon = LonDelta(lon0, lon1) * kRad;
  const double dlat = (lat1 - lat0) * kRad;
  const double dy = MercatorY(lat1) - MercatorY(lat0);
  double bearing = std::atan2(dlon, dy) / kRad;
  if (bearing < 0) bearing += 360;
  // East-west lines: the stretched latitude ratio is 0/0, use cos(lat).
  const double q = std::abs(dy) > 1e-12 ? dlat / dy : std::cos(lat0 * kRad);
  const double distance = std::hypot(dlat, q * dlon) / kRad * 60;
  return {bearing, distance};
}

void MercatorAlong(double lat0, double lon0, double lat1, double lon1,
                   double along, double* lat, double* lon) {
  const double y = MercatorY(lat0) + along * (MercatorY(lat1) - MercatorY(lat0));
  *lat = (2 * std::atan(std::exp(y)) - kPi / 2) / kRad;
  *lon = lon0 + along * LonDelta(lon0, lon1);
  if (*lon > 180) *lon -= 360;
  if (*lon < -180) *lon += 360;
}

double ScreenAngle(double dx, double dy) {
  double angle = std::atan2(dx, -dy);
  return angle < 0 ? angle + 2 * kPi : angle;
}
