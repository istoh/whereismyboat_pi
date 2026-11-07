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

/** \file preferences.h The plugin's preferences dialog. */

#ifndef WHEREISMYBOAT_PREFERENCES_H
#define WHEREISMYBOAT_PREFERENCES_H

#include <wx/dialog.h>

#include "settings.h"

class wxCheckBox;
class wxColourPickerCtrl;
class wxFontPickerCtrl;
class wxSlider;
class wxSpinCtrl;

class PreferencesDialog : public wxDialog {
public:
  PreferencesDialog(wxWindow* parent, const Settings& settings);
  Settings GetSettings() const;

private:
  wxSpinCtrl* m_arrow_size;
  wxFontPickerCtrl* m_font;
  wxColourPickerCtrl* m_arrow;
  wxColourPickerCtrl* m_outline;
  wxColourPickerCtrl* m_text;
  wxColourPickerCtrl* m_background;
  wxSlider* m_background_alpha;
  wxCheckBox* m_show_bearing;
};

#endif
