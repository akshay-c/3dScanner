#include <gtk/gtk.h>
#include <ctype.h>
#include <stdlib.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cctype>
#include <string.h>
#include <time.h>

#include "preferences.hpp"
#include "main.hpp"
#include "settings.hpp"

int         BoardWidth;
int         BoardHeight;
int         SelectedPattern;
float       SquareSize;
int         CalibrationImages;
bool        FixAspectRatio;
float       AspectRatio = 1.0f;
int         CalibrationDelay = 100;
bool        WriteFeautrePoints;
bool        WriteExtrinsicParameters;
bool        TangentialDistortion;
bool        CentralPrincipalPoint;
bool        FlipVertical;
bool        UseVideoFile = false;
char        *OutputFileName;
bool        ShowUndistorted;

//
int         TextBoxHeight;
int         TextBoxWidth;
int         LabelHeight = 35;
int         MaxLabelWidth;
int         LabelAdj;
int         CheckBoxStartY;

//
GtkWidget   *BoardWidthLabel;
GtkWidget   *BoardHeightLabel;
GtkWidget   *SelectedPatternLabel;
GtkWidget   *SquareSizeLabel;
GtkWidget   *CalibrationImagesLabel;
GtkWidget   *CalibrationDelayLabel;
GtkWidget   *OutputFileLabel;

GtkWidget   *BoardWidthText;
GtkWidget   *BoardHeightText;
GtkWidget   *SelectedPatternText;
GtkWidget   *SquareSizeText;
GtkWidget   *CalibrationImagesText;
GtkWidget   *CalibrationDelayText;
GtkWidget   *OutputFileText;

GtkWidget   *WriteFeautrePointsCheck;
GtkWidget   *WriteExtrinsicParametersCheck;
GtkWidget   *TangentialDistortionCheck;
GtkWidget   *CentralPrincipalPointCheck;
GtkWidget   *AspectRatioCheck;
GtkWidget   *FlipVerticalCheck;
GtkWidget   *ShowUndistortedCheck;

GtkWidget   *OutputFileButton;
GtkWidget   *OutputFileSelect;
GtkWidget   *PreferencesCalibrateButton;
GtkWidget   *PreferencesSaveButton;
GtkWidget   *PreferencesCancelButton;

GtkRequisition req;

void InitComponents(void);
void PlaceComponents(void);
void SetValidation(void);
void LoadDefaults(void);

void ValidateInt(GtkEntry *entry, const gchar *text, gint length,
    gint *position, gpointer data);
void ValidateFloat(GtkEntry *entry, const gchar *text, gint length,
    gint *position, gpointer data);
static void ValidateFileName(GtkWidget *w, GtkFileSelection *fs);
static void ButtonClicked(GtkWidget *w, gpointer data);

//
void ShowPreferences()
{
    InitComponents();
    PlaceComponents();
    gtk_widget_show_all (SettingsContainerControls);
    SetValidation();
    LoadDefaults();
}

void InitComponents()
{
    BoardWidthLabel = gtk_label_new((gchar*)"Board Width*");
    BoardHeightLabel = gtk_label_new((gchar*)"Board Height*");
    SelectedPatternLabel = gtk_label_new((gchar*)"Pattern");
    SquareSizeLabel = gtk_label_new((gchar*)"Square Size*");
    CalibrationImagesLabel = gtk_label_new((gchar*)"Image Count*");
    CalibrationDelayLabel = gtk_label_new((gchar*)"Frame Rate");
    OutputFileLabel = gtk_label_new((gchar*)"Output File*");

    BoardWidthText = gtk_entry_new();
    BoardHeightText = gtk_entry_new();
    SelectedPatternText = gtk_combo_box_text_new();
    SquareSizeText = gtk_entry_new();
    CalibrationImagesText = gtk_entry_new();
    CalibrationDelayText = gtk_entry_new();
    OutputFileText = gtk_entry_new();

    gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(SelectedPatternText),
        PATTERN_CIRCLES_GRID, "CIRCLES");
    gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(SelectedPatternText),
        PATTERN_ASYMMETRIC_CIRCLES_GRID, "ASYMMETRIC CIRCLES");
    gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(SelectedPatternText),
        PATTERN_CHESSBOARD, "CHESSBOARD");
    WriteFeautrePointsCheck = gtk_check_button_new_with_label(
        "Write Feature Points");
    WriteExtrinsicParametersCheck = gtk_check_button_new_with_label(
        "Write Extrinsic Parameters");
    TangentialDistortionCheck = gtk_check_button_new_with_label(
        "Tangential Distortion");
    CentralPrincipalPointCheck = gtk_check_button_new_with_label(
        "Central Principal Point");
    FlipVerticalCheck = gtk_check_button_new_with_label("Flip Vertical");
    ShowUndistortedCheck = gtk_check_button_new_with_label("Show Undistorted");
    AspectRatioCheck = gtk_check_button_new_with_label("Fix Aspect Ratio");

    OutputFileButton = gtk_button_new_with_label("...");

    PreferencesCalibrateButton = gtk_button_new_with_mnemonic("Cali_brate");
    PreferencesSaveButton = gtk_button_new_with_mnemonic("_Save");
    PreferencesCancelButton = gtk_button_new_with_mnemonic("_Cancel");
}

void PlaceComponents()
{
    gtk_layout_put(GTK_LAYOUT(SettingsContainerControls),
        CalibrationDelayLabel, LABEL_START_X, 10);
    gtk_widget_size_request(BoardWidthLabel, &req);
    LabelHeight = req.height + 10;
    MaxLabelWidth = req.width + 10;
    // g_print("Width: %d\n",MaxLabelWidth);

    gtk_layout_put(GTK_LAYOUT(SettingsContainerControls),
        BoardWidthText, LABEL_START_X + MaxLabelWidth, LABEL_START_Y);
    gtk_widget_size_request(BoardWidthText, &req);
    TextBoxHeight = req.height + 10;
    TextBoxWidth =req.width;
    LabelAdj = ((TextBoxHeight - LabelHeight)/2);
    gtk_layout_put(GTK_LAYOUT(SettingsContainerControls), BoardWidthLabel,
        LABEL_START_X, LABEL_START_Y + LabelAdj);

    gtk_layout_put(GTK_LAYOUT(SettingsContainerControls), BoardHeightLabel,
        LABEL_START_X, 10 + TextBoxHeight + LabelAdj);
    gtk_layout_put(GTK_LAYOUT(SettingsContainerControls), SelectedPatternLabel,
        LABEL_START_X, 10 + (TextBoxHeight * 2) + LabelAdj);
    gtk_layout_put(GTK_LAYOUT(SettingsContainerControls), SquareSizeLabel,
        LABEL_START_X, 10 + (TextBoxHeight * 3) + LabelAdj);
    gtk_layout_put(GTK_LAYOUT(SettingsContainerControls),
        CalibrationImagesLabel, LABEL_START_X, 10 + (TextBoxHeight*4)+LabelAdj);
    gtk_layout_move(GTK_LAYOUT(SettingsContainerControls),
        CalibrationDelayLabel, LABEL_START_X,
        10 + (TextBoxHeight * 5)+ LabelAdj);
    gtk_layout_put(GTK_LAYOUT(SettingsContainerControls), OutputFileLabel,
        LABEL_START_X, 10 + (TextBoxHeight * 6) + LabelAdj);

    gtk_layout_put(GTK_LAYOUT(SettingsContainerControls), BoardHeightText,
        LABEL_START_X + MaxLabelWidth, 10 + TextBoxHeight);
    gtk_layout_put(GTK_LAYOUT(SettingsContainerControls), SelectedPatternText,
        LABEL_START_X + MaxLabelWidth, 10 + TextBoxHeight*2);
    gtk_layout_put(GTK_LAYOUT(SettingsContainerControls), SquareSizeText,
        LABEL_START_X + MaxLabelWidth, 10 + TextBoxHeight*3);
    gtk_layout_put(GTK_LAYOUT(SettingsContainerControls),
        CalibrationImagesText, LABEL_START_X + MaxLabelWidth,
        10 + TextBoxHeight*4);
    gtk_layout_put(GTK_LAYOUT(SettingsContainerControls),
        CalibrationDelayText, LABEL_START_X + MaxLabelWidth,
        10 + TextBoxHeight * 5);
    gtk_widget_set_size_request(OutputFileText, (TextBoxWidth)*2 / 3,
        TextBoxHeight - 10);
    gtk_layout_put(GTK_LAYOUT(SettingsContainerControls), OutputFileText,
        LABEL_START_X + MaxLabelWidth, 10 + TextBoxHeight * 6);
    gtk_widget_set_size_request(OutputFileButton, (TextBoxWidth)/ 3,
        TextBoxHeight - 10);
    gtk_layout_put(GTK_LAYOUT(SettingsContainerControls), OutputFileButton,
        LABEL_START_X + MaxLabelWidth + ((TextBoxWidth)*2 / 3),
        10 + TextBoxHeight * 6);
    
    CheckBoxStartY = (TextBoxHeight * 7)+10;
    
    gtk_layout_put(GTK_LAYOUT(SettingsContainerControls),
        WriteFeautrePointsCheck, LABEL_START_X + 10, 10 + CheckBoxStartY);
    gtk_layout_put(GTK_LAYOUT(SettingsContainerControls),
        WriteExtrinsicParametersCheck, LABEL_START_X + 10,
        10 + CheckBoxStartY + LabelHeight);
    gtk_layout_put(GTK_LAYOUT(SettingsContainerControls),
        TangentialDistortionCheck, LABEL_START_X + 10,
        10 + CheckBoxStartY + LabelHeight * 2);
    gtk_layout_put(GTK_LAYOUT(SettingsContainerControls),
        CentralPrincipalPointCheck, LABEL_START_X + 10,
        10 + CheckBoxStartY + LabelHeight * 3);
    gtk_layout_put(GTK_LAYOUT(SettingsContainerControls),
        FlipVerticalCheck, LABEL_START_X + 10,
        10 + CheckBoxStartY + LabelHeight * 4);
    gtk_layout_put(GTK_LAYOUT(SettingsContainerControls),
        ShowUndistortedCheck, LABEL_START_X + 10,
        10 + CheckBoxStartY + LabelHeight * 5);
    gtk_layout_put(GTK_LAYOUT(SettingsContainerControls),
        AspectRatioCheck, LABEL_START_X + 10,
        10 + CheckBoxStartY + LabelHeight * 6);

    gtk_layout_put(GTK_LAYOUT(SettingsContainerControls),
        PreferencesCalibrateButton, LABEL_START_X,
        10 + CheckBoxStartY + LabelHeight * 8);
    gtk_widget_size_request(PreferencesCalibrateButton, &req);
    gtk_layout_put(GTK_LAYOUT(SettingsContainerControls),
        PreferencesSaveButton, LABEL_START_X + req.width + 10,
        10 + CheckBoxStartY + LabelHeight * 8);
    gtk_layout_put(GTK_LAYOUT(SettingsContainerControls),
        PreferencesCancelButton, LABEL_START_X + (req.width * 2) + 10,
        10 + CheckBoxStartY + LabelHeight * 8);
}

void SetValidation()
{
    g_signal_connect(G_OBJECT(BoardWidthText), "insert_text",
        G_CALLBACK(ValidateInt), NULL);
    g_signal_connect(G_OBJECT(BoardHeightText), "insert_text",
        G_CALLBACK(ValidateInt), NULL);
    g_signal_connect(G_OBJECT(SquareSizeText), "insert_text",
        G_CALLBACK(ValidateFloat), NULL);
    g_signal_connect(G_OBJECT(CalibrationImagesText), "insert_text",
        G_CALLBACK(ValidateInt), NULL);
    g_signal_connect(G_OBJECT(CalibrationDelayText), "insert_text",
        G_CALLBACK(ValidateInt), NULL);
    g_signal_connect (OutputFileButton, "clicked",
              G_CALLBACK (ButtonClicked), (gpointer) "0");
    g_signal_connect (PreferencesCalibrateButton, "clicked",
              G_CALLBACK (ButtonClicked), (gpointer) "1");
    g_signal_connect (PreferencesSaveButton, "clicked",
              G_CALLBACK (ButtonClicked), (gpointer) "2");
    g_signal_connect (PreferencesCancelButton, "clicked",
              G_CALLBACK (ButtonClicked), (gpointer) "3");
}

static void ButtonClicked(GtkWidget *w, gpointer data)
{
    int Btn = atoi((char*)data);
    switch(Btn)
    {
        case 0:
            OutputFileSelect = gtk_file_selection_new("Calibration Result");
            g_signal_connect (GTK_FILE_SELECTION (OutputFileSelect)->ok_button,
                "clicked", G_CALLBACK(ValidateFileName),
                (gpointer) OutputFileSelect);
            g_signal_connect_swapped(
                GTK_FILE_SELECTION(OutputFileSelect)->cancel_button, "clicked",
                G_CALLBACK(gtk_widget_destroy), OutputFileSelect);
            gtk_widget_show(OutputFileSelect);
        break;
        case 1:
            StartCalibration();
        break;
        case 2:
        break;
        case 3:
        break;
        default:
            g_print("Button Not Registered:%d\n", Btn);
        break;
    }
}

static void ValidateFileName(GtkWidget *w, GtkFileSelection *fs)
{
    gtk_entry_set_text(GTK_ENTRY(OutputFileText),
        gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs)));
    gtk_widget_destroy(OutputFileSelect);
}

void ValidateInt(GtkEntry *entry, const gchar *text, gint length,
    gint *position, gpointer data)
{
    GtkEditable *editable = GTK_EDITABLE(entry);
    int i, count = 0;
    gchar *result = g_new (gchar, length);
    for (i=0; i < length; i++)
    {
        if (isdigit(text[i]))
        {
            result[count++] = text[i];
        }
    }
    if(count>0)
    {
        g_signal_handlers_block_by_func(G_OBJECT(editable),
            (gpointer)G_CALLBACK(ValidateInt), data);
        gtk_editable_insert_text (editable, result, count, position);
        g_signal_handlers_unblock_by_func(G_OBJECT(editable),
            (gpointer)G_CALLBACK (ValidateInt), data);
    }
    g_signal_stop_emission_by_name (G_OBJECT (editable), "insert_text");
    g_free (result);
}

void ValidateFloat(GtkEntry *entry, const gchar *text, gint length,
    gint *position, gpointer data)
{
    GtkEditable *editable = GTK_EDITABLE(entry);
    int i, count = 0;
    gchar *result = g_new (gchar, length);
    for (i=0; i < length; i++)
    {
        if (isdigit(text[i]) || (text[i] == '.'))
        {
            result[count++] = text[i];
        }
    }
    if(count>0)
    {
        // g_print();
        g_signal_handlers_block_by_func(G_OBJECT(editable),
            (gpointer)G_CALLBACK(ValidateFloat), data);
        gtk_editable_insert_text (editable, result, count, position);
        g_signal_handlers_unblock_by_func(G_OBJECT(editable),
            (gpointer)G_CALLBACK (ValidateFloat), data);
    }
    g_signal_stop_emission_by_name (G_OBJECT (editable), "insert_text");
    g_free (result);
}

int ValidateForm()
{
    if(gtk_entry_get_text_length(GTK_ENTRY(BoardWidthText)) <= 0)
        return -1;
    else
        BoardWidth = atoi((char*)gtk_entry_get_text(GTK_ENTRY(BoardWidthText)));
    if(gtk_entry_get_text_length(GTK_ENTRY(BoardHeightText)) <= 0)
        return -1;
    else
        BoardHeight = atoi((char*)gtk_entry_get_text(
            GTK_ENTRY(BoardHeightText)));
    if(gtk_entry_get_text_length(GTK_ENTRY(SquareSizeText)) <= 0)
        return -1;
    else
        SquareSize = atof((char*)gtk_entry_get_text(GTK_ENTRY(SquareSizeText)));
    if(gtk_entry_get_text_length(GTK_ENTRY(OutputFileText))<=0)
        return -1;
    else
        OutputFileName = (char*)gtk_entry_get_text(GTK_ENTRY(OutputFileText));
    
    if(gtk_entry_get_text_length(GTK_ENTRY(CalibrationImagesText))<=0)
        return -1;
    else
        CalibrationImages = atoi((char*)gtk_entry_get_text(
            GTK_ENTRY(CalibrationImagesText)));
    if(gtk_entry_get_text_length(GTK_ENTRY(CalibrationDelayText))<=0)
        return -1;
    else
        CalibrationDelay = atoi((char*)gtk_entry_get_text(
            GTK_ENTRY(CalibrationDelayText)));
    
    WriteFeautrePoints = (bool) gtk_toggle_button_get_active(
        GTK_TOGGLE_BUTTON(WriteFeautrePointsCheck));
    WriteExtrinsicParameters = (bool) gtk_toggle_button_get_active(
        GTK_TOGGLE_BUTTON(WriteExtrinsicParametersCheck));
    TangentialDistortion = (bool) gtk_toggle_button_get_active(
        GTK_TOGGLE_BUTTON(TangentialDistortionCheck));
    CentralPrincipalPoint = (bool) gtk_toggle_button_get_active(
        GTK_TOGGLE_BUTTON(CentralPrincipalPointCheck));
    FlipVertical = (bool) gtk_toggle_button_get_active(
        GTK_TOGGLE_BUTTON(FlipVerticalCheck));
    ShowUndistorted = (bool) gtk_toggle_button_get_active(
        GTK_TOGGLE_BUTTON(ShowUndistortedCheck));
    FixAspectRatio = (bool) gtk_toggle_button_get_active(
        GTK_TOGGLE_BUTTON(AspectRatioCheck));
    SelectedPattern = gtk_combo_box_get_active(GTK_COMBO_BOX(
        SelectedPatternText));
    return 1;
}

void LoadDefaults()
{
    gtk_entry_set_text(GTK_ENTRY(BoardWidthText),"9");
    gtk_entry_set_text(GTK_ENTRY(BoardHeightText),"6");
    gtk_combo_box_set_active(
        GTK_COMBO_BOX(SelectedPatternText),PATTERN_CHESSBOARD);
    gtk_entry_set_text(GTK_ENTRY(SquareSizeText),"50");
    gtk_entry_set_text(GTK_ENTRY(CalibrationDelayText),"100");
    gtk_entry_set_text(GTK_ENTRY(CalibrationImagesText),"15");
    gtk_entry_set_text(GTK_ENTRY(OutputFileText),"Output.yml");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(AspectRatioCheck), true);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(
        TangentialDistortionCheck), true);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(
        CentralPrincipalPointCheck), true);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(
        WriteFeautrePointsCheck), true);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(
        WriteExtrinsicParametersCheck), true);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(
        ShowUndistortedCheck), true);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(
        FlipVerticalCheck), true);
}