#ifndef __SCANNER_H
#define __SCANNER_H

#include <opencv2/opencv.hpp>


#define MW_WIDTH            600
#define MW_HEIGHT           600
#define MW_CAMAREA_WIDTH    640
#define MW_CAMAREA_HEIGHT   480
#define MW_CA_X             50
#define MW_CA_Y             50
#define MWX                 20
#define MWY                 20
#define STEP_ANGLE          10

//Display                 *dpy;
//Window                  MainWindow;
extern GtkWidget				*MainWindow;
extern GtkWidget                *MainCamArea;

extern GtkWidget                *ThresholdlLabel;
extern GtkWidget                *ThresholdText;
extern GtkWidget                *RedFilterLabel;
extern GtkWidget                *GreenFilterLabel;
extern GtkWidget                *BlueFilterLabel;
extern GtkWidget                *RedSlider;
extern GtkWidget                *GreenSlider;
extern GtkWidget                *BlueSlider;

//extern GtkApplication			*Application;
extern int						AppStatus;
extern char*			 		VideoDevice;
extern int 						VideoDeviceCount;
extern int                      SelectedCam;
extern int                      AttrList[];
extern cv::VideoCapture         cap;

extern double                   Threshold;
extern int                      RedFilter;
extern int                      GreenFilter;
extern int                      BlueFilter;
extern char*                    ResultFileName;
extern int                      Steps;
/*extern XEvent                  MWEvent;
extern Visual                  *MWVisual;
extern Atom                    WindowCloseEvent;
bool                    Running = true;
int                     MWDepth;
GdkColor                MWColor;
int                     DpyScreen;
unsigned int            MWBorderWidth = 5;
unsigned long           DefWindowBackground = 0xE0E0E0E0;
XSetWindowAttributes    MWAttributes;
XFontStruct             *DefaultFont;
GC                      MainGC;*/

//GLint WindowAttributes[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, //Main Window Attributes
//                         GLX_DOUBLEBUFFER, None };

//Preferences variables
extern int      BoardWidth;
extern int      BoardHeight;
extern int      SelectedPattern;
extern float    SquareSize;
extern int      CalibrationImages;
extern float    AspectRatio;
extern int      CalibrationDelay;
extern bool     FixAspectRatio;
extern bool     WriteFeautrePoints;
extern bool     WriteExtrinsicParameters;
extern bool     TangentialDistortion;
extern bool     CentralPrincipalPoint;
extern bool     FlipVertical;
extern bool     UseVideoFile;
extern char*    OutputFileName;
extern bool     ShowUndistorted;

//Menu Variables

extern GtkWidget *MWMenuBox;

typedef struct CamInfoTag
{
    char Name[32];
    int  Id;
    CamInfoTag *Next;
}CamInfo;

typedef struct DataPointTag
{
    double x;
    double y;
    double z;
    double ox;
    double oy;
    double angle;
    double secdeg;
    double cosdeg;
    double sindeg;
    double secrad;
    double cosrad;
    double sinrad;
    DataPointTag* Prev;
}DataPoint;

extern CamInfo *CamDetails;

//Menu Functions
void InitMainMenu(void);

//Main Functions
void SetVideoDevice(const char*, int);
void InitAll(void);

//Settings Functions
void ShowSettingsWindow(void);
int StartCalibration(void);

//Preferences Functions

#endif
