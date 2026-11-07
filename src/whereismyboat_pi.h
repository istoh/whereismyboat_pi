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

/**
 * \file whereismyboat_pi.h
 * When the own ship is off the visible chart, draw an arrow at the chart
 * edge towards it with its distance and bearing. Clicking the arrow centres
 * the chart on the ship.
 */

#ifndef WHEREISMYBOAT_PI_H
#define WHEREISMYBOAT_PI_H

#include <ctime>
#include <map>

#include <wx/wx.h>

#include "ocpn_plugin.h"
#include "settings.h"

class piDC;

class WhereIsMyBoatPi : public opencpn_plugin_118 {
public:
  explicit WhereIsMyBoatPi(void* ppimgr);

  int Init() override;
  bool DeInit() override;

  int GetAPIVersionMajor() override;
  int GetAPIVersionMinor() override;
  int GetPlugInVersionMajor() override;
  int GetPlugInVersionMinor() override;
  int GetPlugInVersionPatch() override;
  int GetPlugInVersionPost() override;
  const char* GetPlugInVersionPre() override;
  const char* GetPlugInVersionBuild() override;
  wxBitmap* GetPlugInBitmap() override;
  wxString GetCommonName() override;
  wxString GetShortDescription() override;
  wxString GetLongDescription() override;

  void ShowPreferencesDialog(wxWindow* parent) override;
  void SetPositionFixEx(PlugIn_Position_Fix_Ex& fix) override;
  bool MouseEventHook(wxMouseEvent& event) override;
  bool RenderOverlayMultiCanvas(wxDC& dc, PlugIn_ViewPort* vp, int canvas,
                                int priority) override;
  bool RenderGLOverlayMultiCanvas(wxGLContext* context, PlugIn_ViewPort* vp,
                                  int canvas, int priority) override;

private:
  /** Where the arrow was last drawn on a canvas, for clicks. */
  struct Shown {
    wxRect area;
    double scale_ppm = 0;
    double pixel_scale = 1;  ///< Viewport pixels per logical pixel
  };

  bool HasPosition() const;
  bool Draw(piDC& dc, PlugIn_ViewPort* vp, int canvas, bool gl);

  wxBitmap m_panel_bitmap;
  Settings m_settings;
  bool m_has_position = false;
  double m_lat = 0;
  double m_lon = 0;
  time_t m_fix_time = 0;  ///< When the last real fix arrived, 0 if none yet
  std::map<int, Shown> m_shown;
};

#endif
