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

/** \file pointer.h Where to put the pointer to a target outside the chart. */

#ifndef WHEREISMYBOAT_POINTER_H
#define WHEREISMYBOAT_POINTER_H

#include <optional>

/** Arrow tip position on the chart and the direction it points to. */
struct EdgePointer {
  double x;      ///< Tip x in chart pixels
  double y;      ///< Tip y in chart pixels, growing downwards
  double angle;  ///< Screen direction to the target in radians, 0 = up, clockwise
};

/**
 * Pointer at the chart edge in the screen direction of a target seen from
 * the chart centre.
 * @param width, height  Chart size in pixels
 * @param angle  Screen direction to the target: 0 = up, clockwise, radians
 * @param margin  Distance of the tip from the chart edge in pixels
 */
EdgePointer PointerAtEdge(double width, double height, double angle,
                          double margin);

/**
 * Whether a point in chart pixels is on the chart, allowing for a margin
 * so a boat just at the edge counts as visible.
 */
bool IsOnChart(double width, double height, double x, double y,
               double margin = 0);

/** Rhumb line bearing (true, degrees 0-360) and distance (nautical miles). */
struct Course {
  double bearing;
  double distance;
};
Course RhumbLine(double lat0, double lon0, double lat1, double lon1);

/**
 * A position the fraction `along` of the way from (lat0, lon0) to
 * (lat1, lon1) on a Mercator chart, i.e. on the straight on-screen line.
 */
void MercatorAlong(double lat0, double lon0, double lat1, double lon1,
                   double along, double* lat, double* lon);

/** Screen direction of a pixel offset: 0 = up, clockwise, radians. */
double ScreenAngle(double dx, double dy);

#endif
