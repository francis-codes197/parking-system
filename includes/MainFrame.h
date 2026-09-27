#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/notebook.h>
#include <wx/statline.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/scrolwin.h>
#include <wx/msgdlg.h>
#include <vector>
#include <memory>
#include <string>
#include <sstream>
#include <iomanip>
#include "ParkingSystem.h"
#include "SpotWidget.h"

class MainFrame : public wxFrame {
private:
    std::shared_ptr<ParkingSystem> m_parkingSystem;
    UserInfo m_pendingUser;

    wxNotebook* m_notebook;

    wxPanel* m_checkInTab;
    wxPanel* m_detailsPanel;
    wxPanel* m_layoutSelectionPanel;

    wxTextCtrl* m_nameInput;
    wxTextCtrl* m_phoneInput;
    wxTextCtrl* m_idInput;
    wxTextCtrl* m_regInput;
    wxStaticText* m_selectionBanner;

    std::vector<SpotWidget*> m_selectionSpotWidgets;
    std::vector<SpotWidget*> m_overviewSpotWidgets;

    wxPanel* m_checkOutTab;
    wxChoice* m_occupiedChoice;
    wxChoice* m_durationSimulationChoice;
    wxSpinCtrl* m_customMinutesSpin;
    wxTextCtrl* m_receiptDisplay;
    wxStaticText* m_spotDetailsLabel;

    wxPanel* m_overviewTab;
    wxStaticText* m_overviewStatsLabel;

    void buildCheckInTab();
    void buildCheckOutTab();
    void buildOverviewTab();

    void onProceedToLayout(wxCommandEvent& event);
    void onBackToDetails(wxCommandEvent& event);
    void onSpotSelectedForCheckIn(int spotId);

    void onRefreshCheckOutList();
    void onOccupiedChoiceSelected(wxCommandEvent& event);
    void onProcessCheckOut(wxCommandEvent& event);

    void refreshAllLayouts();

public:
    MainFrame(const wxString& title);
    virtual ~MainFrame() override = default;
};

#endif
