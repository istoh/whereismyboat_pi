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

/** \file preferences.cpp Implement preferences.h. */

#include "preferences.h"

#include <wx/checkbox.h>
#include <wx/clrpicker.h>
#include <wx/fontpicker.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/spinctrl.h>
#include <wx/stattext.h>

PreferencesDialog::PreferencesDialog(wxWindow* parent, const Settings& settings)
    : wxDialog(parent, wxID_ANY, _("Where Is My Boat preferences"), wxDefaultPosition,
               wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER) {
  auto* grid = new wxFlexGridSizer(2, wxSize(12, 8));
  grid->AddGrowableCol(1);
  auto add = [&](const wxString& label, wxWindow* control) {
    grid->Add(new wxStaticText(this, wxID_ANY, label), 0, wxALIGN_CENTER_VERTICAL);
    grid->Add(control, 1, wxEXPAND);
  };

  m_arrow_size = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                                wxDefaultSize, wxSP_ARROW_KEYS, 12, 200, settings.arrow_size);
  add(_("Arrow size (pixels)"), m_arrow_size);
  m_font = new wxFontPickerCtrl(this, wxID_ANY, settings.font, wxDefaultPosition, wxDefaultSize,
                                wxFNTP_DEFAULT_STYLE);
  add(_("Text font"), m_font);
  m_arrow = new wxColourPickerCtrl(this, wxID_ANY, settings.arrow);
  add(_("Arrow colour"), m_arrow);
  m_outline = new wxColourPickerCtrl(this, wxID_ANY, settings.outline);
  add(_("Outline colour"), m_outline);
  m_text = new wxColourPickerCtrl(this, wxID_ANY, settings.text);
  add(_("Text colour"), m_text);
  m_background = new wxColourPickerCtrl(this, wxID_ANY, settings.background);
  add(_("Text background"), m_background);
  m_background_alpha = new wxSlider(this, wxID_ANY, settings.background_alpha, 0, 255,
                                    wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);
  add(_("Background opacity"), m_background_alpha);
  m_show_bearing = new wxCheckBox(this, wxID_ANY, _("Show bearing with the distance"));
  m_show_bearing->SetValue(settings.show_bearing);
  grid->AddSpacer(0);
  grid->Add(m_show_bearing);

  auto* top = new wxBoxSizer(wxVERTICAL);
  top->Add(grid, 1, wxEXPAND | wxALL, 12);
  top->Add(new wxStaticText(this, wxID_ANY,
                            _("The opacity applies in OpenGL mode.")),
           0, wxLEFT | wxRIGHT, 12);
  top->Add(CreateStdDialogButtonSizer(wxOK | wxCANCEL), 0, wxEXPAND | wxALL, 12);
  SetSizerAndFit(top);
  CentreOnParent();
}

Settings PreferencesDialog::GetSettings() const {
  Settings settings;
  settings.arrow_size = m_arrow_size->GetValue();
  settings.font = m_font->GetSelectedFont();
  settings.arrow = m_arrow->GetColour();
  settings.outline = m_outline->GetColour();
  settings.text = m_text->GetColour();
  settings.background = m_background->GetColour();
  settings.background_alpha = m_background_alpha->GetValue();
  settings.show_bearing = m_show_bearing->GetValue();
  return settings;
}
