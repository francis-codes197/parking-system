#ifndef SPOT_WIDGET_H
#define SPOT_WIDGET_H

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/dcbuffer.h>
#include <wx/colour.h>
#include <wx/font.h>
#include <wx/cursor.h>
#include <functional>
#include <string>
#include "Models.h"

class SpotWidget : public wxPanel {
private:
    int m_spotId;
    std::string m_label;
    bool m_isOccupied;
    std::string m_vehicleReg;
    std::function<void(int)> m_onClick;

public:
    SpotWidget(wxWindow* parent, int spotId, const std::string& label, bool isOccupied, const std::string& vehicleReg = "", std::function<void(int)> onClick = nullptr);
    virtual ~SpotWidget() override = default;

    void updateState(bool isOccupied, const std::string& vehicleReg = "");
    void setOnClick(std::function<void(int)> onClick);
    int getSpotId() const;
    bool isOccupied() const;
    const std::string& getLabel() const;

    void onPaint(wxPaintEvent& event);
    void onLeftDown(wxMouseEvent& event);
};

#endif
