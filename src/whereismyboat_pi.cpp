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

/** \file whereismyboat_pi.cpp Implement whereismyboat_pi.h. */

#include "whereismyboat_pi.h"

#include <algorithm>
#include <cmath>
#include <string>

#include <wx/filename.h>

#include "config.h"
#include "pidc.h"
#include "pointer.h"

extern "C" DECL_EXP opencpn_plugin* create_pi(void* ppimgr) {
  return new WhereIsMyBoatPi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p) { delete p; }

namespace {

/** Seconds without a new fix before the arrow turns grey. */
constexpr time_t kStaleFix = 60;
/** Beyond this many pixels, GetCanvasPixLL could overflow its int result. */
constexpr double kMaxPixels = 1e6;

wxString FormatDistance(double nm) {
  return nm < 10 ? wxString::Format("%.1f NM", nm) : wxString::Format("%.0f NM", nm);
}

wxPoint At(double x, double y) {
  return wxPoint(static_cast<int>(std::lround(x)), static_cast<int>(std::lround(y)));
}

}  // namespace

WhereIsMyBoatPi::WhereIsMyBoatPi(void* ppimgr) : opencpn_plugin_118(ppimgr) {
  wxFileName icon(GetPluginDataDir(PKG_NAME), "whereismyboat_panel_icon.svg");
  icon.AppendDir("data");
  if (icon.FileExists())
    m_panel_bitmap = GetBitmapFromSVGFile(icon.GetFullPath(), 32, 32);
}

int WhereIsMyBoatPi::Init() {
  return WANTS_OVERLAY_CALLBACK | WANTS_OPENGL_OVERLAY_CALLBACK |
         WANTS_NMEA_EVENTS | WANTS_MOUSE_EVENTS;
}

bool WhereIsMyBoatPi::DeInit() { return true; }

int WhereIsMyBoatPi::GetAPIVersionMajor() { return std::stoi(API_VERSION); }

int WhereIsMyBoatPi::GetAPIVersionMinor() {
  std::string version(API_VERSION);
  return std::stoi(version.substr(version.find('.') + 1));
}

int WhereIsMyBoatPi::GetPlugInVersionMajor() { return PLUGIN_VERSION_MAJOR; }
int WhereIsMyBoatPi::GetPlugInVersionMinor() { return PLUGIN_VERSION_MINOR; }
int WhereIsMyBoatPi::GetPlugInVersionPatch() { return PLUGIN_VERSION_PATCH; }
int WhereIsMyBoatPi::GetPlugInVersionPost() { return PLUGIN_VERSION_TWEAK; }
const char* WhereIsMyBoatPi::GetPlugInVersionPre() { return PKG_PRERELEASE; }
const char* WhereIsMyBoatPi::GetPlugInVersionBuild() { return PKG_BUILD_INFO; }
wxBitmap* WhereIsMyBoatPi::GetPlugInBitmap() { return &m_panel_bitmap; }
wxString WhereIsMyBoatPi::GetCommonName() { return PLUGIN_API_NAME; }
wxString WhereIsMyBoatPi::GetShortDescription() { return PKG_SUMMARY; }
wxString WhereIsMyBoatPi::GetLongDescription() { return PKG_DESCRIPTION; }

void WhereIsMyBoatPi::SetPositionFixEx(PlugIn_Position_Fix_Ex& fix) {
  if (std::isnan(fix.Lat) || std::isnan(fix.Lon) || fix.FixTime == 0) return;
  m_lat = fix.Lat;
  m_lon = fix.Lon;
  m_fix_time = wxDateTime::Now().GetTicks();
}

bool WhereIsMyBoatPi::HasFix() const { return m_fix_time != 0; }

bool WhereIsMyBoatPi::RenderOverlayMultiCanvas(wxDC& dc, PlugIn_ViewPort* vp,
                                               int canvas, int priority) {
  if (priority != OVERLAY_OVER_UI) return false;
  piDC pidc(dc);
  return Draw(pidc, vp, canvas);
}

bool WhereIsMyBoatPi::RenderGLOverlayMultiCanvas(wxGLContext* context,
                                                 PlugIn_ViewPort* vp,
                                                 int canvas, int priority) {
  if (priority != OVERLAY_OVER_UI) return false;
  piDC pidc(context);
  pidc.SetVP(vp);
  return Draw(pidc, vp, canvas);
}

bool WhereIsMyBoatPi::Draw(piDC& dc, PlugIn_ViewPort* vp, int canvas) {
  m_shown.erase(canvas);
  if (!HasFix() || !vp || !vp->bValid) return false;

  const double width = vp->pix_width;
  const double height = vp->pix_height;
  const Course course = RhumbLine(vp->clat, vp->clon, m_lat, m_lon);
  const double pixels = course.distance * 1852 * vp->view_scale_ppm;
  if (pixels < kMaxPixels) {
    wxPoint boat;
    GetCanvasPixLL(vp, &boat, m_lat, m_lon);
    if (IsOnChart(width, height, boat.x, boat.y)) return false;
  }

  // The on-screen direction, through OpenCPN's own projection so chart
  // rotation is handled: project a point about 100 px towards the boat.
  double probe_lat, probe_lon;
  MercatorAlong(vp->clat, vp->clon, m_lat, m_lon, std::min(1.0, 100 / pixels),
                &probe_lat, &probe_lon);
  wxPoint centre, probe;
  GetCanvasPixLL(vp, &centre, vp->clat, vp->clon);
  GetCanvasPixLL(vp, &probe, probe_lat, probe_lon);
  const double angle = ScreenAngle(probe.x - centre.x, probe.y - centre.y);

  const double size = 22 * GetOCPNGUIToolScaleFactor_PlugIn();
  const EdgePointer tip = PointerAtEdge(width, height, angle, size);
  const double ux = std::sin(angle), uy = -std::cos(angle);  // towards the boat
  const double nx = -uy, ny = ux;                            // across the arrow

  const bool stale = wxDateTime::Now().GetTicks() - m_fix_time > kStaleFix;
  const wxColour fill = stale ? wxColour(140, 140, 140) : wxColour(230, 81, 0);
  wxPoint arrow[4] = {
      At(tip.x + ux * size * 0.6, tip.y + uy * size * 0.6),
      At(tip.x - ux * size * 0.6 + nx * size * 0.55, tip.y - uy * size * 0.6 + ny * size * 0.55),
      At(tip.x - ux * size * 0.25, tip.y - uy * size * 0.25),
      At(tip.x - ux * size * 0.6 - nx * size * 0.55, tip.y - uy * size * 0.6 - ny * size * 0.55),
  };
  dc.SetPen(wxPen(wxColour(40, 40, 40), std::max(1, static_cast<int>(size / 12))));
  dc.SetBrush(wxBrush(fill));
  dc.DrawPolygon(4, arrow);

  // Distance and bearing from the chart centre, inside the arrow.
  wxString label = FormatDistance(course.distance) +
                   wxString::Format("  %03.0f°", course.bearing) +
                   (stale ? _(" (old fix)") : wxString());
  wxFont* font = GetOCPNScaledFont_PlugIn(_("Dialog"));
  dc.SetFont(*font);
  wxCoord text_w, text_h;
  dc.GetTextExtent(label, &text_w, &text_h);
  const int pad = static_cast<int>(size / 4);
  const int box_w = text_w + 2 * pad;
  const int box_h = text_h + 2 * pad;
  // Centre the box behind the arrow, kept inside the chart.
  const int box_x = std::clamp(
      static_cast<int>(std::lround(tip.x - ux * (size + box_w / 2.0) - box_w / 2.0)), 2,
      std::max(2, static_cast<int>(width) - box_w - 2));
  const int box_y = std::clamp(
      static_cast<int>(std::lround(tip.y - uy * (size + box_h / 2.0) - box_h / 2.0)), 2,
      std::max(2, static_cast<int>(height) - box_h - 2));
  dc.SetPen(wxPen(fill, 2));
  dc.SetBrush(wxBrush(wxColour(255, 255, 255, 220)));
  dc.DrawRoundedRectangle(box_x, box_y, box_w, box_h, pad);
  dc.SetTextForeground(wxColour(20, 20, 20));
  dc.DrawText(label, box_x + pad, box_y + pad);

  wxRect area(box_x, box_y, box_w, box_h);
  for (const auto& point : arrow) area.Union(wxRect(point, wxSize(1, 1)));
  m_shown[canvas] = {area.Inflate(pad), vp->view_scale_ppm};
  return true;
}

bool WhereIsMyBoatPi::MouseEventHook(wxMouseEvent& event) {
  if (!event.LeftDown() && !event.LeftUp()) return false;
  const int canvas = GetCanvasIndexUnderMouse();
  const auto shown = m_shown.find(canvas);
  if (shown == m_shown.end()) return false;
  // The viewport is in physical pixels, mouse events in logical ones.
  wxWindow* window = GetCanvasByIndex(canvas);
  const double scale = window ? window->GetContentScaleFactor() : 1.0;
  const wxPoint position(static_cast<int>(event.GetX() * scale),
                         static_cast<int>(event.GetY() * scale));
  if (!shown->second.area.Contains(position)) return false;
  // Take both the press and the release, so the chart does not start a drag.
  if (event.LeftUp() && window)
    CanvasJumpToPosition(window, m_lat, m_lon, shown->second.scale_ppm);
  return true;
}
