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

/** \file settings.h Appearance settings, stored in the OpenCPN config. */

#ifndef WHEREISMYBOAT_SETTINGS_H
#define WHEREISMYBOAT_SETTINGS_H

#include <wx/colour.h>
#include <wx/font.h>

class wxFileConfig;

struct Settings {
  int arrow_size = 44;  ///< Arrow length in pixels
  wxFont font = wxFont(wxFontInfo(15).Bold());
  wxColour arrow = wxColour(230, 81, 0);
  wxColour outline = wxColour(40, 40, 40);
  wxColour text = wxColour(20, 20, 20);
  wxColour background = wxColour(255, 255, 255);
  int background_alpha = 220;  ///< 0 transparent ... 255 opaque
  bool show_bearing = true;

  void Load(wxFileConfig* config);
  void Save(wxFileConfig* config) const;
};

#endif
