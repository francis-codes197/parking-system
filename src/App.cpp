#include "App.h"

bool ParkingApp::OnInit() {
    MainFrame* frame = new MainFrame("Smart Parking Management System");
    frame->Show(true);
    return true;
}
