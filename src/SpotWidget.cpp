#include "SpotWidget.h"

SpotWidget::SpotWidget(wxWindow* parent, int spotId, const std::string& label, bool isOccupied, const std::string& vehicleReg, std::function<void(int)> onClick)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(120, 85), wxBORDER_NONE),
      m_spotId(spotId),
      m_label(label),
      m_isOccupied(isOccupied),
      m_vehicleReg(vehicleReg),
      m_onClick(onClick) {
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    Bind(wxEVT_PAINT, &SpotWidget::onPaint, this);
    Bind(wxEVT_LEFT_DOWN, &SpotWidget::onLeftDown, this);
}

void SpotWidget::updateState(bool isOccupied, const std::string& vehicleReg) {
    m_isOccupied = isOccupied;
    m_vehicleReg = vehicleReg;
    Refresh();
}

void SpotWidget::setOnClick(std::function<void(int)> onClick) {
    m_onClick = onClick;
}

int SpotWidget::getSpotId() const {
    return m_spotId;
}

bool SpotWidget::isOccupied() const {
    return m_isOccupied;
}

const std::string& SpotWidget::getLabel() const {
    return m_label;
}

void SpotWidget::onPaint(wxPaintEvent& event) {
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    wxSize sz = GetClientSize();
    wxRect rect(0, 0, sz.GetWidth(), sz.GetHeight());

    wxColour bgColour;
    wxColour borderColour;
    wxString statusStr;

    if (m_isOccupied) {
        bgColour = wxColour(220, 53, 69);
        borderColour = wxColour(167, 29, 42);
        statusStr = "OCCUPIED";
        SetCursor(wxCursor(wxCURSOR_NO_ENTRY));
    } else {
        bgColour = wxColour(33, 150, 243);
        borderColour = wxColour(21, 101, 192);
        statusStr = "free";
        SetCursor(wxCursor(wxCURSOR_HAND));
    }

    dc.SetBrush(wxBrush(bgColour));
    dc.SetPen(wxPen(borderColour, 2));
    dc.DrawRoundedRectangle(rect.Deflate(2), 6.0);

    dc.SetTextForeground(*wxWHITE);

    wxFont titleFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    dc.SetFont(titleFont);
    wxString title = wxString::FromUTF8(m_label.c_str());
    wxSize titleSz = dc.GetTextExtent(title);
    dc.DrawText(title, (sz.GetWidth() - titleSz.GetWidth()) / 2, 8);

    wxFont statusFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    dc.SetFont(statusFont);
    wxSize statusSz = dc.GetTextExtent(statusStr);
    dc.DrawText(statusStr, (sz.GetWidth() - statusSz.GetWidth()) / 2, 32);

    if (m_isOccupied && !m_vehicleReg.empty()) {
        wxFont regFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
        dc.SetFont(regFont);
        wxString regStr = wxString::FromUTF8(m_vehicleReg.c_str());
        wxSize regSz = dc.GetTextExtent(regStr);
        dc.DrawText(regStr, (sz.GetWidth() - regSz.GetWidth()) / 2, 55);
    }
}

void SpotWidget::onLeftDown(wxMouseEvent& event) {
    if (m_isOccupied) {
        return;
    }
    if (m_onClick) {
        m_onClick(m_spotId);
    }
}
