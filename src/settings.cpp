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

/** \file settings.cpp Implement settings.h. */

#include "settings.h"

#include <algorithm>

#include <wx/fileconf.h>

namespace {

const char* const kGroup = "/PlugIns/WhereIsMyBoat";

wxColour ReadColour(wxFileConfig* config, const char* key, const wxColour& fallback) {
  wxString value;
  wxColour colour;
  if (config->Read(key, &value) && colour.Set(value)) return colour;
  return fallback;
}

}  // namespace

void Settings::Load(wxFileConfig* config) {
  if (!config) return;
  config->SetPath(kGroup);
  arrow_size = std::clamp(static_cast<int>(config->ReadLong("ArrowSize", arrow_size)), 12, 200);
  wxString font_desc;
  if (config->Read("Font", &font_desc)) {
    wxFont loaded;
    if (loaded.SetNativeFontInfo(font_desc) && loaded.IsOk()) font = loaded;
  }
  arrow = ReadColour(config, "ArrowColour", arrow);
  outline = ReadColour(config, "OutlineColour", outline);
  text = ReadColour(config, "TextColour", text);
  background = ReadColour(config, "BackgroundColour", background);
  background_alpha =
      std::clamp(static_cast<int>(config->ReadLong("BackgroundAlpha", background_alpha)), 0, 255);
  show_bearing = config->ReadBool("ShowBearing", show_bearing);
}

void Settings::Save(wxFileConfig* config) const {
  if (!config) return;
  config->SetPath(kGroup);
  config->Write("ArrowSize", arrow_size);
  config->Write("Font", font.GetNativeFontInfoDesc());
  config->Write("ArrowColour", arrow.GetAsString(wxC2S_HTML_SYNTAX));
  config->Write("OutlineColour", outline.GetAsString(wxC2S_HTML_SYNTAX));
  config->Write("TextColour", text.GetAsString(wxC2S_HTML_SYNTAX));
  config->Write("BackgroundColour", background.GetAsString(wxC2S_HTML_SYNTAX));
  config->Write("BackgroundAlpha", background_alpha);
  config->Write("ShowBearing", show_bearing);
  config->Flush();
}
