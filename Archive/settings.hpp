#ifndef __SETTINGS_H
#define __SETTINGS_H

#define SW_WIDTH        1024
#define SW_HEIGHT       600
#define CA_WIDTH        640
#define CA_HEIGHT       480
#define CONTROLS_WIDTH  384
#define CONTROLS_HEIGHT 300

#define PATTERN_CHESSBOARD                  0
#define PATTERN_CIRCLES_GRID                1
#define PATTERN_ASYMMETRIC_CIRCLES_GRID     2


static GtkWidget    *SettingsWindow;
static GtkWidget	*SettingsContainerMain;
static GtkWidget    *SettingsContainerCam;
static GtkWidget	*ComboCamSelect;
static GtkWidget 	*CamArea;
static int          CamAreaWidth 	= 500;
static int			CamAreaHeight	= 480;
static bool         CalibrationComplete = false;

extern bool SettingsWindowClosed;
extern int  RefreshRate;                                                        //Set Refresh Rate in fps

void PopulateCamList(void);
int ValidateForm(void);

#endif
