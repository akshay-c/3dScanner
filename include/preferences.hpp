#ifndef __PREFERENCES_H
#define __PREFERENCES_H

#define LABEL_START_X                       10
#define LABEL_START_Y                       10

#define BTN_OUT_FILE                        1                                    //Not used because of compatability issues
#define BTN_CALIBRATE                       2
#define BTN_SAVE                            3
#define BTN_CANCEL                          4

extern int          TextBoxHeight;
extern int          TextBoxWidth;
extern int          LabelHeight;
extern int          MaxLabelWidth;
extern int          CheckBoxStartY;
extern int          LabelAdj;

extern GtkWidget    *SettingsContainerControls;
extern GtkWidget    *BoardWidthLabel;
extern GtkWidget    *BoardHeightLabel;
extern GtkWidget    *SelectedPatternLabel;
extern GtkWidget    *SquareSizeLabel;
extern GtkWidget    *CalibrationImagesLabel;
extern GtkWidget    *AspectRatioLabel;
extern GtkWidget    *CalibrationLabel;
extern GtkWidget    *OutputFileLabel;

extern GtkWidget    *BoardWidthText;
extern GtkWidget    *BoardHeightText;
extern GtkWidget    *SelectedPatternText;
extern GtkWidget    *SquareSizeText;
extern GtkWidget    *CalibrationImagesText;
extern GtkWidget    *AspectRatioText;
extern GtkWidget    *CalibrationText;
extern GtkWidget    *OutputFileText;

extern GtkWidget    *WriteFeautrePointsCheck;
extern GtkWidget    *WriteExtrinsicParametersCheck;
extern GtkWidget    *TangentialDistortionCheck;
extern GtkWidget    *CentralPrincipalPointCheck;
extern GtkWidget    *FlipVerticalCheck;
extern GtkWidget    *ShowUndistortedCheck;

extern GtkWidget    *OutputFileButton;

extern GtkWidget    *OutputFileSelect;

extern GtkWidget    *PreferencesCalibrateButton;
extern GtkWidget    *PreferencesSaveButton;
extern GtkWidget    *PreferencesCancelButton;

void ShowPreferences(void);

#endif
