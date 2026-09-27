#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(1000, 750)),
      m_parkingSystem(std::make_shared<ParkingSystem>(20)) {

    SetMinSize(wxSize(850, 650));
    CreateStatusBar(2);
    SetStatusText("Parking System Ready", 0);

    m_notebook = new wxNotebook(this, wxID_ANY);

    buildCheckInTab();
    buildCheckOutTab();
    buildOverviewTab();

    m_notebook->AddPage(m_checkInTab, "Vehicle Check-In");
    m_notebook->AddPage(m_checkOutTab, "Check-Out & Billing");
    m_notebook->AddPage(m_overviewTab, "Parking Lot Overview");

    refreshAllLayouts();
    Centre();
}

void MainFrame::buildCheckInTab() {
    m_checkInTab = new wxPanel(m_notebook);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    m_detailsPanel = new wxPanel(m_checkInTab);
    wxBoxSizer* detailsSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* headerLabel = new wxStaticText(m_detailsPanel, wxID_ANY, "Step 1: Enter Customer & Vehicle Details");
    wxFont headerFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    headerLabel->SetFont(headerFont);
    detailsSizer->Add(headerLabel, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 15);

    wxStaticText* subLabel = new wxStaticText(m_detailsPanel, wxID_ANY, "Please provide the required details below. Next, you will be guided to select an available parking spot.");
    detailsSizer->Add(subLabel, 0, wxBOTTOM | wxALIGN_CENTER_HORIZONTAL, 20);

    wxFlexGridSizer* formSizer = new wxFlexGridSizer(4, 2, 14, 15);
    formSizer->AddGrowableCol(1, 1);

    wxFont fieldFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

    wxStaticText* nameLbl = new wxStaticText(m_detailsPanel, wxID_ANY, "Whole Name:");
    nameLbl->SetFont(fieldFont);
    m_nameInput = new wxTextCtrl(m_detailsPanel, wxID_ANY, "", wxDefaultPosition, wxSize(320, 32));

    wxStaticText* phoneLbl = new wxStaticText(m_detailsPanel, wxID_ANY, "Phone Number:");
    phoneLbl->SetFont(fieldFont);
    m_phoneInput = new wxTextCtrl(m_detailsPanel, wxID_ANY, "", wxDefaultPosition, wxSize(320, 32));

    wxStaticText* idLbl = new wxStaticText(m_detailsPanel, wxID_ANY, "National ID Number:");
    idLbl->SetFont(fieldFont);
    m_idInput = new wxTextCtrl(m_detailsPanel, wxID_ANY, "", wxDefaultPosition, wxSize(320, 32));

    wxStaticText* regLbl = new wxStaticText(m_detailsPanel, wxID_ANY, "Vehicle Registration:");
    regLbl->SetFont(fieldFont);
    m_regInput = new wxTextCtrl(m_detailsPanel, wxID_ANY, "", wxDefaultPosition, wxSize(320, 32));

    formSizer->Add(nameLbl, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
    formSizer->Add(m_nameInput, 0, wxEXPAND);

    formSizer->Add(phoneLbl, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
    formSizer->Add(m_phoneInput, 0, wxEXPAND);

    formSizer->Add(idLbl, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
    formSizer->Add(m_idInput, 0, wxEXPAND);

    formSizer->Add(regLbl, 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
    formSizer->Add(m_regInput, 0, wxEXPAND);

    detailsSizer->Add(formSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxLEFT | wxRIGHT, 40);

    wxButton* proceedBtn = new wxButton(m_detailsPanel, wxID_ANY, "Proceed to Choose Parking Spot ->", wxDefaultPosition, wxSize(300, 42));
    proceedBtn->SetFont(fieldFont);
    proceedBtn->SetBackgroundColour(wxColour(33, 150, 243));
    proceedBtn->SetForegroundColour(*wxWHITE);
    proceedBtn->Bind(wxEVT_BUTTON, &MainFrame::onProceedToLayout, this);
    detailsSizer->Add(proceedBtn, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 30);

    m_detailsPanel->SetSizer(detailsSizer);
    mainSizer->Add(m_detailsPanel, 1, wxEXPAND);

    m_layoutSelectionPanel = new wxPanel(m_checkInTab);
    wxBoxSizer* layoutSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* step2Label = new wxStaticText(m_layoutSelectionPanel, wxID_ANY, "Step 2: Choose Your Parking Spot");
    step2Label->SetFont(headerFont);
    layoutSizer->Add(step2Label, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10);

    m_selectionBanner = new wxStaticText(m_layoutSelectionPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    wxFont bannerFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL);
    m_selectionBanner->SetFont(bannerFont);
    m_selectionBanner->SetForegroundColour(wxColour(25, 118, 210));
    layoutSizer->Add(m_selectionBanner, 0, wxBOTTOM | wxALIGN_CENTER_HORIZONTAL, 12);

    wxGridSizer* gridSizer = new wxGridSizer(4, 5, 12, 12);
    const auto& spots = m_parkingSystem->getAllSpots();
    for (const auto& spot : spots) {
        SpotWidget* sw = new SpotWidget(m_layoutSelectionPanel, spot.id, spot.label, spot.isOccupied, spot.occupant.vehicleRegistration, [this](int id) {
            this->onSpotSelectedForCheckIn(id);
        });
        m_selectionSpotWidgets.push_back(sw);
        gridSizer->Add(sw, 0, wxALIGN_CENTER);
    }
    layoutSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

    wxBoxSizer* navSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* backBtn = new wxButton(m_layoutSelectionPanel, wxID_ANY, "<- Back to Edit Details", wxDefaultPosition, wxSize(200, 36));
    backBtn->Bind(wxEVT_BUTTON, &MainFrame::onBackToDetails, this);
    navSizer->Add(backBtn, 0, wxALL, 10);
    layoutSizer->Add(navSizer, 0, wxALIGN_CENTER_HORIZONTAL);

    m_layoutSelectionPanel->SetSizer(layoutSizer);
    m_layoutSelectionPanel->Hide();
    mainSizer->Add(m_layoutSelectionPanel, 1, wxEXPAND);

    m_checkInTab->SetSizer(mainSizer);
}

void MainFrame::buildCheckOutTab() {
    m_checkOutTab = new wxPanel(m_notebook);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* headerLabel = new wxStaticText(m_checkOutTab, wxID_ANY, "Check-Out & Vacate Parking Spot");
    wxFont headerFont(13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    headerLabel->SetFont(headerFont);
    leftSizer->Add(headerLabel, 0, wxALL, 12);

    wxStaticText* selectLbl = new wxStaticText(m_checkOutTab, wxID_ANY, "Select Occupied Spot / Vehicle:");
    wxFont boldFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    selectLbl->SetFont(boldFont);
    leftSizer->Add(selectLbl, 0, wxLEFT | wxRIGHT | wxTOP, 12);

    m_occupiedChoice = new wxChoice(m_checkOutTab, wxID_ANY, wxDefaultPosition, wxSize(360, 32));
    m_occupiedChoice->Bind(wxEVT_CHOICE, &MainFrame::onOccupiedChoiceSelected, this);
    leftSizer->Add(m_occupiedChoice, 0, wxALL, 12);

    m_spotDetailsLabel = new wxStaticText(m_checkOutTab, wxID_ANY, "No spot selected.");
    leftSizer->Add(m_spotDetailsLabel, 0, wxALL, 12);

    leftSizer->Add(new wxStaticLine(m_checkOutTab, wxID_ANY), 0, wxEXPAND | wxALL, 10);

    wxStaticText* timingLbl = new wxStaticText(m_checkOutTab, wxID_ANY, "Check-Out Time Calculation Mode:");
    timingLbl->SetFont(boldFont);
    leftSizer->Add(timingLbl, 0, wxLEFT | wxRIGHT | wxTOP, 12);

    wxArrayString timingChoices;
    timingChoices.Add("Actual Current System Time");
    timingChoices.Add("Simulate: 20 Minutes (Free tier)");
    timingChoices.Add("Simulate: 1 Hour 30 Minutes (KES 50 tier)");
    timingChoices.Add("Simulate: 3 Hours 30 Minutes (KES 100 tier)");
    timingChoices.Add("Simulate: 5 Hours 00 Minutes (KES 300 tier)");
    timingChoices.Add("Simulate: 7 Hours 30 Minutes (KES 500 tier)");
    timingChoices.Add("Simulate: Custom Minutes Below");

    m_durationSimulationChoice = new wxChoice(m_checkOutTab, wxID_ANY, wxDefaultPosition, wxSize(360, 32), timingChoices);
    m_durationSimulationChoice->SetSelection(0);
    leftSizer->Add(m_durationSimulationChoice, 0, wxALL, 12);

    wxBoxSizer* customMinSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* spinLbl = new wxStaticText(m_checkOutTab, wxID_ANY, "Custom Duration (Minutes):");
    m_customMinutesSpin = new wxSpinCtrl(m_checkOutTab, wxID_ANY, "45", wxDefaultPosition, wxSize(100, -1), wxSP_ARROW_KEYS, 1, 10080, 45);
    customMinSizer->Add(spinLbl, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
    customMinSizer->Add(m_customMinutesSpin, 0, wxALIGN_CENTER_VERTICAL);
    leftSizer->Add(customMinSizer, 0, wxALL, 12);

    wxButton* checkoutBtn = new wxButton(m_checkOutTab, wxID_ANY, "Calculate Bill & Vacate Spot", wxDefaultPosition, wxSize(360, 42));
    checkoutBtn->SetFont(boldFont);
    checkoutBtn->SetBackgroundColour(wxColour(220, 53, 69));
    checkoutBtn->SetForegroundColour(*wxWHITE);
    checkoutBtn->Bind(wxEVT_BUTTON, &MainFrame::onProcessCheckOut, this);
    leftSizer->Add(checkoutBtn, 0, wxALL, 15);

    mainSizer->Add(leftSizer, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(new wxStaticLine(m_checkOutTab, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL), 0, wxEXPAND | wxTOP | wxBOTTOM, 10);

    wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* receiptLbl = new wxStaticText(m_checkOutTab, wxID_ANY, "Printed Receipt Output:");
    receiptLbl->SetFont(headerFont);
    rightSizer->Add(receiptLbl, 0, wxALL, 12);

    m_receiptDisplay = new wxTextCtrl(m_checkOutTab, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL);
    wxFont monoFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    m_receiptDisplay->SetFont(monoFont);
    rightSizer->Add(m_receiptDisplay, 1, wxEXPAND | wxALL, 10);

    mainSizer->Add(rightSizer, 1, wxEXPAND | wxALL, 10);

    m_checkOutTab->SetSizer(mainSizer);
}

void MainFrame::buildOverviewTab() {
    m_overviewTab = new wxPanel(m_notebook);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* headerLabel = new wxStaticText(m_overviewTab, wxID_ANY, "Parking Lot Real-Time Status");
    wxFont headerFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    headerLabel->SetFont(headerFont);
    mainSizer->Add(headerLabel, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 12);

    m_overviewStatsLabel = new wxStaticText(m_overviewTab, wxID_ANY, "Total Spots: 20 | Free: 20 | Occupied: 0");
    wxFont statsFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    m_overviewStatsLabel->SetFont(statsFont);
    mainSizer->Add(m_overviewStatsLabel, 0, wxBOTTOM | wxALIGN_CENTER_HORIZONTAL, 12);

    wxGridSizer* gridSizer = new wxGridSizer(4, 5, 12, 12);
    const auto& spots = m_parkingSystem->getAllSpots();
    for (const auto& spot : spots) {
        SpotWidget* sw = new SpotWidget(m_overviewTab, spot.id, spot.label, spot.isOccupied, spot.occupant.vehicleRegistration, nullptr);
        m_overviewSpotWidgets.push_back(sw);
        gridSizer->Add(sw, 0, wxALIGN_CENTER);
    }
    mainSizer->Add(gridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

    wxPanel* legendPanel = new wxPanel(m_overviewTab);
    wxBoxSizer* legendSizer = new wxBoxSizer(wxHORIZONTAL);

    wxPanel* blueBox = new wxPanel(legendPanel, wxID_ANY, wxDefaultPosition, wxSize(24, 24));
    blueBox->SetBackgroundColour(wxColour(33, 150, 243));
    wxStaticText* freeDesc = new wxStaticText(legendPanel, wxID_ANY, " Blue: Free (Clickable for Check-In)   ");

    wxPanel* redBox = new wxPanel(legendPanel, wxID_ANY, wxDefaultPosition, wxSize(24, 24));
    redBox->SetBackgroundColour(wxColour(220, 53, 69));
    wxStaticText* occDesc = new wxStaticText(legendPanel, wxID_ANY, " Red: Occupied (Unclickable)");

    legendSizer->Add(blueBox, 0, wxALIGN_CENTER_VERTICAL);
    legendSizer->Add(freeDesc, 0, wxALIGN_CENTER_VERTICAL);
    legendSizer->Add(redBox, 0, wxALIGN_CENTER_VERTICAL);
    legendSizer->Add(occDesc, 0, wxALIGN_CENTER_VERTICAL);
    legendPanel->SetSizer(legendSizer);

    mainSizer->Add(legendPanel, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

    wxStaticText* ratesInfo = new wxStaticText(m_overviewTab, wxID_ANY,
        "Tariff Rates: Upto 30 mins: Free | Upto 2 hrs: KES 50 | Upto 4 hrs: KES 100 | Upto 6 hrs: KES 300 | Over 6 hrs: KES 500");
    wxFont ratesFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL);
    ratesInfo->SetFont(ratesFont);
    mainSizer->Add(ratesInfo, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 10);

    m_overviewTab->SetSizer(mainSizer);
}

void MainFrame::onProceedToLayout(wxCommandEvent& event) {
    wxString name = m_nameInput->GetValue().Trim();
    wxString phone = m_phoneInput->GetValue().Trim();
    wxString idNum = m_idInput->GetValue().Trim();
    wxString reg = m_regInput->GetValue().Trim();

    if (name.IsEmpty() || phone.IsEmpty() || idNum.IsEmpty() || reg.IsEmpty()) {
        wxMessageBox("Please fill in all details:\n- Whole Name\n- Phone Number\n- ID Number\n- Vehicle Registration", "Missing Information", wxOK | wxICON_WARNING, this);
        return;
    }

    if (m_parkingSystem->getFreeCount() == 0) {
        wxMessageBox("Sorry, the parking lot is currently full!", "No Vacancy", wxOK | wxICON_INFORMATION, this);
        return;
    }

    m_pendingUser.wholeName = name.ToStdString();
    m_pendingUser.phoneNumber = phone.ToStdString();
    m_pendingUser.idNumber = idNum.ToStdString();
    m_pendingUser.vehicleRegistration = reg.ToStdString();

    wxString bannerText = wxString::Format(
        "Driver: %s  |  ID: %s  |  Phone: %s  |  Vehicle: %s\nClick on any open BLUE spot marked 'free' below to complete assignment.",
        name, idNum, phone, reg
    );
    m_selectionBanner->SetLabel(bannerText);

    m_detailsPanel->Hide();
    m_layoutSelectionPanel->Show();
    m_checkInTab->Layout();
}

void MainFrame::onBackToDetails(wxCommandEvent& event) {
    m_layoutSelectionPanel->Hide();
    m_detailsPanel->Show();
    m_checkInTab->Layout();
}

void MainFrame::onSpotSelectedForCheckIn(int spotId) {
    const ParkingSpot* spot = m_parkingSystem->getSpot(spotId);
    if (!spot || spot->isOccupied) {
        return;
    }

    wxString confirmPrompt = wxString::Format(
        "Confirm assigning Parking Spot %s to:\n\nDriver: %s\nID Number: %s\nPhone: %s\nVehicle Reg: %s",
        spot->label, m_pendingUser.wholeName, m_pendingUser.idNumber, m_pendingUser.phoneNumber, m_pendingUser.vehicleRegistration
    );

    int answer = wxMessageBox(confirmPrompt, "Confirm Assignment", wxYES_NO | wxICON_QUESTION, this);
    if (answer != wxYES) {
        return;
    }

    auto checkInTime = std::chrono::system_clock::now();
    bool success = m_parkingSystem->assignSpot(spotId, m_pendingUser, checkInTime);

    if (success) {
        wxString timeStr = wxString::FromUTF8(ParkingSystem::formatTime(checkInTime).c_str());
        wxString successMsg = wxString::Format(
            "Spot %s assigned successfully!\n\nDriver: %s\nVehicle: %s\nCheck-In Time: %s",
            spot->label, m_pendingUser.wholeName, m_pendingUser.vehicleRegistration, timeStr
        );
        wxMessageBox(successMsg, "Check-In Confirmed", wxOK | wxICON_INFORMATION, this);

        m_nameInput->Clear();
        m_phoneInput->Clear();
        m_idInput->Clear();
        m_regInput->Clear();

        m_layoutSelectionPanel->Hide();
        m_detailsPanel->Show();
        m_checkInTab->Layout();

        refreshAllLayouts();
    }
}

void MainFrame::onOccupiedChoiceSelected(wxCommandEvent& event) {
    int sel = m_occupiedChoice->GetSelection();
    if (sel == wxNOT_FOUND) {
        m_spotDetailsLabel->SetLabel("No spot selected.");
        return;
    }

    void* clientData = m_occupiedChoice->GetClientData(sel);
    if (!clientData) {
        m_spotDetailsLabel->SetLabel("No spot selected.");
        return;
    }

    int spotId = reinterpret_cast<intptr_t>(clientData);
    const ParkingSpot* spot = m_parkingSystem->getSpot(spotId);
    if (spot && spot->isOccupied) {
        wxString details = wxString::Format(
            "Spot: %s\nDriver: %s\nPhone: %s\nID: %s\nVehicle Reg: %s\nCheck-In Time: %s",
            spot->label,
            spot->occupant.wholeName,
            spot->occupant.phoneNumber,
            spot->occupant.idNumber,
            spot->occupant.vehicleRegistration,
            ParkingSystem::formatTime(spot->checkInTime)
        );
        m_spotDetailsLabel->SetLabel(details);
    } else {
        m_spotDetailsLabel->SetLabel("No spot selected.");
    }
}

void MainFrame::onProcessCheckOut(wxCommandEvent& event) {
    int sel = m_occupiedChoice->GetSelection();
    if (sel == wxNOT_FOUND) {
        wxMessageBox("Please select an occupied spot to check out.", "Selection Required", wxOK | wxICON_WARNING, this);
        return;
    }

    void* clientData = m_occupiedChoice->GetClientData(sel);
    if (!clientData) {
        wxMessageBox("Invalid spot selected.", "Error", wxOK | wxICON_ERROR, this);
        return;
    }

    int spotId = reinterpret_cast<intptr_t>(clientData);
    const ParkingSpot* spot = m_parkingSystem->getSpot(spotId);
    if (!spot || !spot->isOccupied) {
        wxMessageBox("Selected spot is no longer occupied.", "Error", wxOK | wxICON_ERROR, this);
        refreshAllLayouts();
        return;
    }

    std::chrono::system_clock::time_point checkOutTime;
    int mode = m_durationSimulationChoice->GetSelection();

    if (mode == 0) {
        checkOutTime = std::chrono::system_clock::now();
    } else if (mode == 1) {
        checkOutTime = spot->checkInTime + std::chrono::minutes(20);
    } else if (mode == 2) {
        checkOutTime = spot->checkInTime + std::chrono::minutes(90);
    } else if (mode == 3) {
        checkOutTime = spot->checkInTime + std::chrono::minutes(210);
    } else if (mode == 4) {
        checkOutTime = spot->checkInTime + std::chrono::minutes(300);
    } else if (mode == 5) {
        checkOutTime = spot->checkInTime + std::chrono::minutes(450);
    } else {
        int customMins = m_customMinutesSpin->GetValue();
        checkOutTime = spot->checkInTime + std::chrono::minutes(customMins);
    }

    BillReceipt receipt;
    bool vacated = m_parkingSystem->vacateSpot(spotId, checkOutTime, receipt);

    if (vacated) {
        m_receiptDisplay->SetValue(wxString::FromUTF8(receipt.receiptText.c_str()));
        wxMessageBox(wxString::FromUTF8(receipt.receiptText.c_str()), "Spot Vacated - Bill Receipt", wxOK | wxICON_INFORMATION, this);
        m_spotDetailsLabel->SetLabel("No spot selected.");
        refreshAllLayouts();
    }
}

void MainFrame::refreshAllLayouts() {
    const auto& spots = m_parkingSystem->getAllSpots();

    for (size_t i = 0; i < spots.size() && i < m_selectionSpotWidgets.size(); ++i) {
        m_selectionSpotWidgets[i]->updateState(spots[i].isOccupied, spots[i].occupant.vehicleRegistration);
    }

    for (size_t i = 0; i < spots.size() && i < m_overviewSpotWidgets.size(); ++i) {
        m_overviewSpotWidgets[i]->updateState(spots[i].isOccupied, spots[i].occupant.vehicleRegistration);
    }

    m_occupiedChoice->Clear();
    for (const auto& spot : spots) {
        if (spot.isOccupied) {
            wxString item = wxString::Format("Spot %s - %s (%s)", spot.label, spot.occupant.vehicleRegistration, spot.occupant.wholeName);
            m_occupiedChoice->Append(item, reinterpret_cast<void*>(static_cast<intptr_t>(spot.id)));
        }
    }
    if (m_occupiedChoice->GetCount() > 0) {
        m_occupiedChoice->SetSelection(0);
        wxCommandEvent dummy;
        onOccupiedChoiceSelected(dummy);
    } else {
        m_spotDetailsLabel->SetLabel("No occupied spots currently.");
    }

    wxString statsText = wxString::Format("Total Spots: %d | Free: %d | Occupied: %d",
        m_parkingSystem->getTotalSpots(),
        m_parkingSystem->getFreeCount(),
        m_parkingSystem->getOccupiedCount()
    );
    m_overviewStatsLabel->SetLabel(statsText);

    wxString statusMsg = wxString::Format("Available: %d | Occupied: %d", m_parkingSystem->getFreeCount(), m_parkingSystem->getOccupiedCount());
    SetStatusText(statusMsg, 1);
}
