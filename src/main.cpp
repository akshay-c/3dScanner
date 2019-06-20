#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gtkgl/gtkglarea.h>
#include <GL/gl.h>
#include <opencv2/core/types_c.h>
#include <math.h>

#include "main.hpp"
#include "menu.hpp"
#include "display.hpp"
#include "preferences.hpp"
#include "uart.hpp"
#include "idle.hpp"
#include "filehandling.hpp"

static void ButtonClicked(GtkWidget *widget, gpointer data);
static void SliderChanged(GtkSpinButton *spinbutton, gpointer data);
void        MainWindowClosing(GtkWidget *widget, gpointer callback_data);
int StartMotor(int angle);

void        ShowFeed(void);
void        HideFeed(void);
void        TakeSample(void);
void        TakeTestShot(void);
void        ShowSampleImage(void);
void        ShowTestImage(void);
void        ShowDiffImage(void);
void        SetFilteredImage(void);
void        ShowFilteredImage(void);
void        StartScan(void);
void        InitCamContainer(void);
void        InitPrefContainer(void);
void        SetReferenceImage(void);
void        GetDataPoints(void);
void        ShowDataPoints(void);
void        ProcessData(void);
void        LaserON(void);
void        LaserOFF(void);
void        RecreateDataPoints(int x, int y,double angle, void*);
double      to_degrees(double);
double      to_radians(double);
double      sec(double angle);
double      cosec(double angle);
double      cot(double angle);

GtkWidget       *MainWindow;
GtkWidget       *MainCamArea;
GtkWidget       *MWContainerMain;
GtkWidget       *MWContainerCam;
GtkWidget       *MWContainerPref;
GtkWidget       *StartFeedButton;
GtkWidget       *StartScanButton;
GtkWidget       *StopFeedButton;


GtkWidget       *ThresholdLabel;
GtkWidget       *RedFilterLabel;
GtkWidget       *GreenFilterLabel;
GtkWidget       *BlueFilterLabel;
GtkWidget       *FillRangeLabel;
GtkWidget       *BlankRangeLabel;
GtkWidget       *LaserAngleLabel;
GtkWidget       *CamAngleLabel;
GtkWidget       *ThresholdSlider;
GtkWidget       *RedSlider;
GtkWidget       *GreenSlider;
GtkWidget       *BlueSlider;
GtkWidget       *FillRangeSlider;
GtkWidget       *BlankRangeSlider;
GtkWidget       *LaserAngleSlider;
GtkWidget       *CamAngleSlider;
GtkWidget       *LaserOnButton;
GtkWidget       *LaserOffButton;
GtkWidget       *SampleImageButton;
GtkWidget       *CaptureTestButton;
GtkWidget       *DisplaySampleButton;
GtkWidget       *DisplayTestButton;
GtkWidget       *DisplayDiffButton;
GtkWidget       *DisplayFilteredButton;
GtkWidget       *SetAxisButton;
GtkWidget       *ExtractDataButton;
GtkWidget       *FilterGroupFrame;
GtkWidget       *ReferenceGroupFrame;
GtkRequisition  reqs;


//GtkApplication  *Application;
int             AppStatus;
char*           VideoDevice;
int             SelectedCam = -1;
int             AttrList[] = {GDK_GL_RGBA,GDK_GL_DEPTH_SIZE, 24,
                    GDK_GL_DOUBLEBUFFER, GDK_GL_NONE};
bool            StartFeed = false;

double          Threshold = 50.0;
int             RedFilter = 70;
int             GreenFilter = 0;
int             BlueFilter = 0;
int             LblHeight;
int             LblWidth;
double          LaserAngle = 50.0;
double          CamAngle = 0;
int             ReferenceAxis = 0;
int             Steps = //((int)(360%STEP_ANGLE)==0)?(int)((360/STEP_ANGLE)-1):
                    (int)(360/STEP_ANGLE);
double          CurrentAngle = 0;
unsigned long   PointCount = 0;
DataPoint       *LastPoint = NULL;

CamInfo* CamDetails = NULL;
cv::Mat SampleImage;
cv::Mat DiffImage;
cv::Mat TestImage;
cv::Mat FilteredImage;
cv::Mat ReferenceImage;
cv::Mat DataPointImage;
cv::Mat frame;

/*static void MWActivate(GtkApplication* app, gpointer user_data)
{
    MainWindow = gtk_application_window_new(app);
    gtk_window_set_title (GTK_WINDOW (MainWindow), "3D Scanner");
    gtk_widget_override_background_color(GTK_WINDOW(MainWindow),
        GTK_STATE_NORMAL, &MWColor);
    gtk_window_set_default_size (GTK_WINDOW (MainWindow), MW_WIDTH , MW_HEIGHT);
    gtk_window_maximize (GTK_WINDOW (MainWindow));
    InitMainMenu();
    gtk_widget_show_all (MainWindow);
}*/

void SetVideoDevice(const char* DeviceName, int Length)
{
    VideoDevice = (char*) realloc(VideoDevice, Length * sizeof(char));
    strcpy(VideoDevice, DeviceName);
}

void InitAll()
{
    VideoDevice = (char*) malloc(sizeof(char));
    MainCamArea = gtk_gl_area_new (AttrList);
    MWContainerMain = gtk_fixed_new();
    MWContainerCam = gtk_layout_new(NULL, NULL);
    MWContainerPref = gtk_layout_new(NULL, NULL);
    InitCamContainer();
    InitPrefContainer();
    gtk_fixed_put(GTK_FIXED(MWContainerMain), MWContainerCam, 0, 0);
    
    // gtk_widget_override_color(MWContainerCam, NULL, c);
    gtk_widget_show_all(MWContainerCam);
    gtk_fixed_put(GTK_FIXED(MWContainerMain), MWContainerPref,
        MW_CAMAREA_WIDTH + 50, 0);
    
    // gtk_container_add(GTK_CONTAINER(MainWindow), MWContainerMain);
    gtk_box_pack_start(GTK_BOX(MWMenuBox), MWContainerMain, FALSE, FALSE, 0);
    /* This packs the button into the fixed containers window. 
    gtk_fixed_put (GTK_FIXED (fixed), button, i*50, i*50);*/

    gtk_widget_set_events(GTK_WIDGET(MainCamArea), GDK_EXPOSURE_MASK);
    gtk_quit_add_destroy(1, GTK_OBJECT(MainCamArea));
}

int main(int argc, char *argv[]) {

    /* Secure gtk */
    gdk_threads_init();

    /* Obtain gtk's global lock */
    gdk_threads_enter();

    gtk_init(&argc, &argv);
    /*Application = gtk_application_new("fossee.scanner3d",
        G_APPLICATION_FLAGS_NONE);
    g_signal_connect(Application, "activate", G_CALLBACK(MWActivate), NULL);
    AppStatus = g_application_run(G_APPLICATION(Application), argc, argv);
    g_object_unref(Application);*/
    MainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (MainWindow), "3D Scanner");
    gtk_window_set_position(GTK_WINDOW(MainWindow), GTK_WIN_POS_CENTER);
    /*gtk_widget_override_background_color(GTK_WINDOW(MainWindow),
        GTK_STATE_NORMAL, &MWColor);*/
    gtk_window_set_default_size(GTK_WINDOW(MainWindow), MW_WIDTH , MW_HEIGHT);
    gtk_window_maximize (GTK_WINDOW (MainWindow));
    InitMainMenu();
    InitAll();
    g_signal_connect(G_OBJECT(MainWindow), "destroy",
        G_CALLBACK(MainWindowClosing), NULL);
    g_signal_connect (StartFeedButton, "clicked",
              G_CALLBACK (ButtonClicked), (gpointer) "0");
    g_signal_connect (StopFeedButton, "clicked",
              G_CALLBACK (ButtonClicked), (gpointer) "1");
    g_signal_connect (SampleImageButton, "clicked",
              G_CALLBACK (ButtonClicked), (gpointer) "2");
    g_signal_connect (StartScanButton, "clicked",
              G_CALLBACK (ButtonClicked), (gpointer) "3");
    g_signal_connect (CaptureTestButton, "clicked",
              G_CALLBACK (ButtonClicked), (gpointer) "4");
    g_signal_connect (DisplayDiffButton, "clicked",
              G_CALLBACK (ButtonClicked), (gpointer) "5");
    g_signal_connect (DisplayFilteredButton, "clicked",
              G_CALLBACK (ButtonClicked), (gpointer) "6");
    g_signal_connect (DisplaySampleButton, "clicked",
              G_CALLBACK (ButtonClicked), (gpointer) "7");
    g_signal_connect (DisplayTestButton, "clicked",
              G_CALLBACK (ButtonClicked), (gpointer) "8");
    g_signal_connect (SetAxisButton, "clicked",
              G_CALLBACK (ButtonClicked), (gpointer) "9");
    g_signal_connect (ExtractDataButton, "clicked",
              G_CALLBACK (ButtonClicked), (gpointer) "10");
    g_signal_connect (LaserOnButton, "clicked",
              G_CALLBACK (ButtonClicked), (gpointer) "11");
    g_signal_connect (LaserOffButton, "clicked",
              G_CALLBACK (ButtonClicked), (gpointer) "12");

    g_signal_connect (ThresholdSlider, "value-changed",
              G_CALLBACK (SliderChanged), (gpointer) "0");
    g_signal_connect (RedSlider, "value-changed",
              G_CALLBACK (SliderChanged), (gpointer) "1");
    g_signal_connect (GreenSlider, "value-changed",
              G_CALLBACK (SliderChanged), (gpointer) "2");
    g_signal_connect (BlueSlider, "value-changed",
              G_CALLBACK (SliderChanged), (gpointer) "3");

    OpenPort("/dev/ttyACM0", READ_WRITE_UART);
    ConfigurePort(BAUD_9600, CHAR_SIZE_8, PARITY_NONE, 1, FLOW_CONTROL_NONE);
    gtk_widget_show_all(MainWindow);
    gtk_main();
    /* Release gtk's global lock */
    gdk_threads_leave();
    return 0;
}

static void SliderChanged(GtkSpinButton *spinbutton, gpointer data)
{
    int SpinId = atoi((char*)data);
    switch(SpinId)
    {
        case 0:
            Threshold = (double)gtk_spin_button_get_value(
                GTK_SPIN_BUTTON(ThresholdSlider));
            // g_print("Threshold: %f\n",Threshold);
        break;
        case 1:
            RedFilter = (int)gtk_spin_button_get_value_as_int(
                GTK_SPIN_BUTTON(RedSlider));
        break;
        case 2:
            GreenFilter = (int)gtk_spin_button_get_value_as_int(
                GTK_SPIN_BUTTON(GreenSlider));
        break;
        case 3:
            BlueFilter = (int)gtk_spin_button_get_value_as_int(
                GTK_SPIN_BUTTON(BlueSlider));
        break;
        default:
            g_print("Component Not Registered:%d\n", SpinId);
        break;
    }
}

void MainWindowClosing(GtkWidget *widget, gpointer callback_data)
{
    HideFeed();
    ClosePort();
    gtk_main_quit();
}

void CamIdle(int FeedCount)
{
    if((!cap.isOpened()) && (SelectedCam >= 0))
    {
        cap.open(SelectedCam);
    }
    if(cap.isOpened())
    {
        for(int i=0; i<FeedCount; i++)
        {
            cap >> frame;
            while(gtk_events_pending())
            {
                gtk_main_iteration();
            }
        }
    }

}

void ShowFeed()
{
    StartFeed = true;
    if((!cap.isOpened()) && (SelectedCam >= 0))
    {
        cap.open(SelectedCam);
    }
    if(cap.isOpened())
    {
        IplImage img;
        MarkedTimeStamp = clock();
        while(StartFeed)
        {
            if (gtk_gl_area_begingl(GTK_GL_AREA(MainCamArea)))
            {
                cap >> frame;
                cv::flip(frame, frame, 0);
                img = frame;
                LoadImage(&img, MW_CAMAREA_WIDTH,
                    MW_CAMAREA_HEIGHT, MainCamArea);
                CheckEvents(CalibrationDelay);
                gtk_gl_area_endgl(GTK_GL_AREA(MainCamArea));
            }
        else
        {
            g_print("Problem Loading OpenGL");
        }
        }
    }
}

void HideFeed()
{
    StartFeed = false;
    cap.release();
}

void TakeSample()
{
    if(!cap.isOpened())
    {
        cap.open(SelectedCam);
    }
    while(!cap.grab());
            cap.retrieve(frame, 0);
    cv::flip(frame, SampleImage, 0);
}

void TakeTestShot()
{
    if(!cap.isOpened())
        cap.open(SelectedCam);
    cap >> frame;
    cv::flip(frame, TestImage, 0);
}

void ShowSampleImage()
{
    IplImage img;
    HideFeed();
    if (gtk_gl_area_begingl(GTK_GL_AREA(MainCamArea)))
    {
        img = SampleImage;
        LoadImage(&img, MW_CAMAREA_WIDTH,
            MW_CAMAREA_HEIGHT, MainCamArea);
        gtk_gl_area_endgl(GTK_GL_AREA(MainCamArea));
    }
}


void ShowDiffImage()
{
    IplImage img;
    HideFeed();
    if (gtk_gl_area_begingl(GTK_GL_AREA(MainCamArea)))
    {
        cv::absdiff(SampleImage, TestImage, DiffImage);
        img = DiffImage;
        LoadImage(&img, MW_CAMAREA_WIDTH,
            MW_CAMAREA_HEIGHT, MainCamArea);
        gtk_gl_area_endgl(GTK_GL_AREA(MainCamArea));
    }
}

void SetFilteredImage()
{
    cv::Mat TempImage = SampleImage;
    cv::Mat foregroundMask = cv::Mat::zeros(
        DiffImage.rows, DiffImage.cols, CV_8UC3);
    double dist;
    for(int j=0; j<DiffImage.rows; ++j)
    {
        int max=0,min = 255;
        for(int i=0; i<DiffImage.cols; ++i)
        {
            cv::Vec3b pix = TempImage.at<cv::Vec3b>(j,i);
            cv::Vec3b pix2 = DiffImage.at<cv::Vec3b>(j,i);
            dist = (pix2[0]*pix2[0] + pix2[1]*pix2[1] + pix2[2]*pix2[2]);
            dist = sqrt(dist);

            if((dist>Threshold) && (pix[2]>pix[1]) &&
                (pix[2]>pix[0]) && (dist>Threshold) &&
                (((RedFilter < 0) && (pix[2] < (RedFilter*-1))) ||
                    ((RedFilter >= 0) && (pix[2] > RedFilter))) &&
                (((GreenFilter < 0) && (pix[1] < (GreenFilter*-1))) ||
                    ((GreenFilter >= 0) && (pix[1] > GreenFilter))) &&
                (((BlueFilter < 0) && (pix[0] < (BlueFilter*-1))) ||
                    ((BlueFilter >= 0) && (pix[0] > BlueFilter)))
                )
                {
                    pix[0] = 0;
                    pix[1] = 0;
                    pix[2] = 255;
                    foregroundMask.at<cv::Vec3b>(j,i) = pix;
                }
            }
        }
        FilteredImage = foregroundMask;
}
void ShowFilteredImage()
{
    IplImage img;
    img = FilteredImage;
    HideFeed();
    if(gtk_gl_area_begingl(GTK_GL_AREA(MainCamArea)))
        {
            LoadImage(&img, MW_CAMAREA_WIDTH,
                MW_CAMAREA_HEIGHT, MainCamArea);
            CheckEvents(100);
            gtk_gl_area_endgl(GTK_GL_AREA(MainCamArea));
        }
}

void SetReferenceImage()
{
    IplImage img;
    int max = 0, row;
    SetFilteredImage();
    cv::Mat foregroundMask = cv::Mat::zeros(
                FilteredImage.rows, FilteredImage.cols, CV_8UC3);
    for(int j=0; j<FilteredImage.cols; ++j)
    {
        int count = 0;
        for(int i=0; i<FilteredImage.rows; ++i)
        {
            cv::Vec3b pix = FilteredImage.at<cv::Vec3b>(i,j);
            if(pix[2] == 255)
            {
                count++;
                printf("Count: %d, x: %d, y: %d\n",count,i,j);
            }
        }
        if(count>max)
        {
            max=count;
            row =j;
            printf("Count: %d\n",count);
        }
    }
    cv::Vec3b pix;
    ReferenceAxis = row;
    printf("ReferenceAxis: %d\n",ReferenceAxis);
    for(int i=0; i<FilteredImage.cols; ++i)
    {
        pix[2] = 255;
        pix[1] = 0;
        pix[0] = 0;
        foregroundMask.at<cv::Vec3b>(i,row) = pix;
        pix = SampleImage.at<cv::Vec3b>(i,row);
        // printf("r: %d g: %d b: %d\n",pix[2],pix[1],pix[0]);
    }

    cv::flip(foregroundMask, ReferenceImage, 0);
    // cv::flip(DiffImage, frame, 0);
    img = ReferenceImage;
    HideFeed();
    LoadImage(&img, MW_CAMAREA_WIDTH,
        MW_CAMAREA_HEIGHT, MainCamArea);
    CheckEvents(100);
    gtk_gl_area_endgl(GTK_GL_AREA(MainCamArea));
    // printf("Rows: %d Cols: %d\n",TestImage.rows,TestImage.cols);
}

void ShowTestImage()
{
    IplImage img;
    HideFeed();
    if (gtk_gl_area_begingl(GTK_GL_AREA(MainCamArea)))
    {
        img = TestImage;
        LoadImage(&img, MW_CAMAREA_WIDTH,
            MW_CAMAREA_HEIGHT, MainCamArea);
        gtk_gl_area_endgl(GTK_GL_AREA(MainCamArea));
    }
}


void StartScan()
{
    if((!cap.isOpened()) && (SelectedCam >= 0))
    {
        cap.open(SelectedCam);
    }
    if(SelectedCam>=0)
    {
        PointCount = 0;
        LastPoint = NULL;
        CurrentAngle = 0;
        StartMotor(STEP_ANGLE);
        for(int i=0;i<Steps;i++)
        {
            LaserON();
            StartMotor(STEP_ANGLE);
            CamIdle(15);
            TakeSample();
            ShowSampleImage();
            LaserOFF();
            CamIdle(15);
            TakeTestShot();
            ShowTestImage();
            CamIdle(15);
            ProcessData();
        }
        if(PointCount>0)
        {
            DataPoint * temp;
            std::string data;
            char buf[1024];
            WriteHeader(ResultFileName,PointCount,0);
            printf("PointCount: %ld\n", PointCount);
            while(LastPoint)
            {
                sprintf(buf,"%lf %lf %lf",LastPoint->x,LastPoint->y,LastPoint->z);
                data = buf;
                FileWriteLine(data.c_str(), data.size());
                sprintf(buf,"%lf %lf %lf %lf %lf %lf %lf %lf %lf", LastPoint->ox,
                    LastPoint->oy, LastPoint->angle, LastPoint->sinrad,
                    LastPoint->cosrad, LastPoint->secrad, LastPoint->sindeg,
                    LastPoint->cosdeg, LastPoint->secdeg);
                data =buf;
                FileWriteLine2(data.c_str(), data.size());
                temp = LastPoint;
                LastPoint = LastPoint->Prev;
                free(temp);
                printf("%ld\n",--PointCount);
                usleep(20000);
            }
        }
        printf("finished\n");
        LastPoint = NULL;
    }
}

void ProcessData()
{
    LaserAngle = gtk_spin_button_get_value(
        GTK_SPIN_BUTTON(LaserAngleSlider));
    CamAngle = gtk_spin_button_get_value(
        GTK_SPIN_BUTTON(CamAngleSlider));
    ShowDiffImage();
    CamIdle(4);
    SetFilteredImage();
    ShowFilteredImage();
    CamIdle(4);
    GetDataPoints();
    ShowDataPoints();
    CurrentAngle += STEP_ANGLE;
    for(int i=0; i<DataPointImage.rows;i++)
    {
        for(int j=0;j<DataPointImage.cols;j++)
        {
            cv::Vec3b pix = DataPointImage.at<cv::Vec3b>(i,j);
            if(pix[2]==255)
            {
                PointCount++;
                DataPoint *NewPoint = (DataPoint*)malloc(sizeof(DataPoint));
                NewPoint->Prev = LastPoint;
                RecreateDataPoints((j - ReferenceAxis), i,
                    CurrentAngle, (void*)NewPoint);
                LastPoint = NewPoint;
                /*if(LastPoint->Prev)
                {
                    printf("Prev x: %f,y: %f",
                    LastPoint->Prev->x,LastPoint->Prev->y);
                }*/
            }
        }
    }

    // RecreateDataPoints();
}

static void ButtonClicked(GtkWidget *w, gpointer data)
{
    int Btn = atoi((char*)data);
    switch(Btn)
    {
        case 0:
            ShowFeed();
        break;
        case 1:
            HideFeed();
        // WriteHeader(ResultFileName,0,0);
        break;
        case 2:
            TakeSample();
        break;
        case 3:
            StartScan();
        break;
        case 4:
            TakeTestShot();
        break;
        case 5:
            ShowDiffImage();
        break;
        case 6:
            SetFilteredImage();
            ShowFilteredImage();
        break;
        case 7:
            ShowSampleImage();
        break;
        case 8:
            ShowTestImage();
        break;
        case 9:
            SetReferenceImage();
        break;
        case 10:
            GetDataPoints();
            ShowDataPoints();
        break;
        case 11:
            LaserON();
        break;
        case 12:
            LaserOFF();
        break;
        default:
            g_print("Button Not Registered:%d\n", Btn);
        break;
    }
}

void InitCamContainer()
{
    StartScanButton = gtk_button_new_with_mnemonic("S_tart Scan");
    StartFeedButton = gtk_button_new_with_mnemonic("Start Fee_d");
    StopFeedButton = gtk_button_new_with_mnemonic("St_op Feed");

    gtk_widget_set_size_request(MainCamArea,
        MW_CAMAREA_WIDTH, MW_CAMAREA_HEIGHT);
    gtk_widget_set_usize(MainCamArea, MW_CAMAREA_WIDTH, MW_CAMAREA_HEIGHT);
    
    gtk_widget_set_size_request(MWContainerCam,
        MW_CAMAREA_WIDTH + 50, MW_HEIGHT);
    gtk_widget_set_usize(MWContainerCam, MW_CAMAREA_WIDTH + 50, MW_HEIGHT);

    gtk_layout_put(GTK_LAYOUT(MWContainerCam), MainCamArea, MW_CA_X, MW_CA_Y);
    gtk_layout_put(GTK_LAYOUT(MWContainerCam), StartFeedButton,
        MW_CA_X, MW_CA_Y + MW_CAMAREA_HEIGHT + 10);
    gtk_layout_put(GTK_LAYOUT (MWContainerCam), StopFeedButton,
        MW_CA_X + 100, MW_CA_Y + MW_CAMAREA_HEIGHT + 10);
    gtk_layout_put(GTK_LAYOUT (MWContainerCam), StartScanButton,
        MW_CA_X + (100 * 2), MW_CA_Y + MW_CAMAREA_HEIGHT + 10);
}

void InitPrefContainer()
{
    FilterGroupFrame = gtk_frame_new("Filters");
    ReferenceGroupFrame = gtk_frame_new("Reference");
    LaserOnButton = gtk_button_new_with_mnemonic("Laser On");
    LaserOffButton = gtk_button_new_with_mnemonic("Laser Off");
    SampleImageButton = gtk_button_new_with_mnemonic("Capture Sa_mple");
    CaptureTestButton = gtk_button_new_with_mnemonic("Capture Test");
    DisplaySampleButton = gtk_button_new_with_mnemonic("Show Sa_mple");
    DisplayTestButton = gtk_button_new_with_mnemonic("Show Test");
    DisplayDiffButton = gtk_button_new_with_mnemonic("Show Difference");
    DisplayFilteredButton = gtk_button_new_with_mnemonic("Show Filtered");
    SetAxisButton =gtk_button_new_with_mnemonic("Set Ref Axis");
    ExtractDataButton =gtk_button_new_with_mnemonic("Get DataPoints");
    ThresholdLabel = gtk_label_new((gchar*)"Threshold");
    RedFilterLabel = gtk_label_new((gchar*)"Red Filter");
    GreenFilterLabel = gtk_label_new((gchar*)"Green Filter");
    BlueFilterLabel = gtk_label_new((gchar*)"Blue Filter");
    FillRangeLabel = gtk_label_new((gchar*)"Fill Range");
    BlankRangeLabel = gtk_label_new((gchar*)"Error Range");
    LaserAngleLabel = gtk_label_new((gchar*)"Laser Angle");
    CamAngleLabel = gtk_label_new((gchar*)"Cam Angle");
    ThresholdSlider = gtk_spin_button_new_with_range((gdouble) 0,
        (gdouble)100, (gdouble)0.1);
    RedSlider = gtk_spin_button_new_with_range((gdouble) -255,
        (gdouble)255, (gdouble)1.0);
    GreenSlider = gtk_spin_button_new_with_range((gdouble) -255,
        (gdouble)255, (gdouble)1.0);
    BlueSlider = gtk_spin_button_new_with_range((gdouble) -255,
        (gdouble)255, (gdouble)1.0);
    FillRangeSlider = gtk_spin_button_new_with_range((gdouble) 1,
        (gdouble)1024, (gdouble)1.0);
    BlankRangeSlider = gtk_spin_button_new_with_range((gdouble) 0,
        (gdouble)1024, (gdouble)1.0);
    LaserAngleSlider = gtk_spin_button_new_with_range((gdouble) -90,
        (gdouble)90, (gdouble)0.1);
    CamAngleSlider = gtk_spin_button_new_with_range((gdouble) -90,
        (gdouble)90, (gdouble)0.1);

    gtk_widget_set_size_request(MWContainerPref,
        MW_CAMAREA_WIDTH, MW_HEIGHT);
    gtk_widget_set_usize(MWContainerPref, MW_CAMAREA_WIDTH, MW_HEIGHT);

    gtk_layout_put(GTK_LAYOUT(MWContainerPref), ThresholdLabel , 10, 100);
    gtk_widget_size_request(ThresholdLabel, &reqs);
    LblHeight = reqs.height + 40;
    LblWidth = reqs.width + 30;
    gtk_layout_put(GTK_LAYOUT(MWContainerPref), ThresholdSlider ,
        10 + LblWidth, 100);

    /*gtk_widget_set_size_request(RedSlider,
        LblWidth*2, LblHeight);
    gtk_widget_set_size_request(GreenSlider,
        LblWidth*2, LblHeight);
    gtk_widget_set_size_request(BlueSlider,
        LblWidth*2, LblHeight);*/
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(RedSlider),(gdouble)RedFilter);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(ThresholdSlider),
        (gdouble)Threshold);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(RedSlider),
        (gdouble)RedFilter);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(GreenSlider),
        (gdouble)GreenFilter);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(BlueSlider),
        (gdouble)BlueFilter);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(LaserAngleSlider),
        (gdouble)LaserAngle);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(CamAngleSlider),
        (gdouble)CamAngle);

    gtk_layout_put(GTK_LAYOUT(MWContainerPref), RedFilterLabel ,
        10 , 100 + (LblHeight));
    gtk_layout_put(GTK_LAYOUT(MWContainerPref), RedSlider ,
        10 + LblWidth, 100 + (LblHeight));
    
    gtk_layout_put(GTK_LAYOUT(MWContainerPref), GreenFilterLabel , 10,
        100+(LblHeight*2));
    gtk_layout_put(GTK_LAYOUT(MWContainerPref), GreenSlider ,
        10 + (LblWidth), 100 +(LblHeight*2));
    
    gtk_layout_put(GTK_LAYOUT(MWContainerPref), BlueFilterLabel , 10,
        100 + (LblHeight*3));
    gtk_layout_put(GTK_LAYOUT(MWContainerPref), BlueSlider ,
        10 + (LblWidth), 100 + (LblHeight*3));

    gtk_widget_set_size_request(FilterGroupFrame,
        200, 120 + (LblHeight*3));
    gtk_widget_set_usize(FilterGroupFrame, 200, 120 + (LblHeight*2));
    gtk_layout_put(GTK_LAYOUT(MWContainerPref), FilterGroupFrame, 2, 75);

    gtk_layout_put(GTK_LAYOUT(MWContainerPref), FillRangeLabel ,
        240 , 100 );
    gtk_layout_put(GTK_LAYOUT(MWContainerPref), FillRangeSlider ,
        240 + LblWidth, 100);

    gtk_layout_put(GTK_LAYOUT(MWContainerPref), BlankRangeLabel ,
        240 , 100 + (LblHeight));
    gtk_layout_put(GTK_LAYOUT(MWContainerPref), BlankRangeSlider ,
        240 + LblWidth, 100 + (LblHeight));

    gtk_layout_put(GTK_LAYOUT(MWContainerPref), LaserAngleLabel ,
        240 , 100 + (LblHeight*2));
    gtk_layout_put(GTK_LAYOUT(MWContainerPref), LaserAngleSlider ,
        240 + LblWidth, 100 + (LblHeight*2));

    gtk_layout_put(GTK_LAYOUT(MWContainerPref), CamAngleLabel ,
        240 , 100 + (LblHeight*3));
    gtk_layout_put(GTK_LAYOUT(MWContainerPref), CamAngleSlider ,
        240 + LblWidth, 100 + (LblHeight*3));

    gtk_widget_set_size_request(ReferenceGroupFrame,
        200, 120 + (LblHeight*3));
    gtk_widget_set_usize(ReferenceGroupFrame, 200, 120 + (LblHeight*2));
    gtk_layout_put(GTK_LAYOUT(MWContainerPref), ReferenceGroupFrame, 232, 75);

    gtk_layout_put(GTK_LAYOUT(MWContainerPref), SampleImageButton, 10,
        100 + (LblHeight)*4);
    gtk_layout_put(GTK_LAYOUT(MWContainerPref), CaptureTestButton ,
        10 + (LblWidth +50), 100 + (LblHeight*4));
    
    gtk_layout_put(GTK_LAYOUT(MWContainerPref), DisplaySampleButton, 10,
        100 + (LblHeight)*5);
    gtk_layout_put(GTK_LAYOUT(MWContainerPref), DisplayTestButton ,
        10 + (LblWidth +50), 100 + (LblHeight*5));
    gtk_layout_put(GTK_LAYOUT(MWContainerPref), LaserOnButton ,
        10 + (LblWidth +50)*2, 100 + (LblHeight*5));

    gtk_layout_put(GTK_LAYOUT(MWContainerPref), DisplayDiffButton, 10,
        100 + (LblHeight)*6);
    gtk_layout_put(GTK_LAYOUT(MWContainerPref), DisplayFilteredButton ,
        10 + (LblWidth +50), 100 + (LblHeight*6));
    gtk_layout_put(GTK_LAYOUT(MWContainerPref), LaserOffButton ,
        10 + (LblWidth +50)*2, 100 + (LblHeight*6));

    gtk_layout_put(GTK_LAYOUT(MWContainerPref), SetAxisButton, 10,
        100 + (LblHeight)*7);
    gtk_layout_put(GTK_LAYOUT(MWContainerPref), ExtractDataButton,
        10 + (LblWidth +50), 100 + (LblHeight*7));
}

void GetDataPoints()
{
    cv::Vec3b pix,setpix;
    setpix[0] = 0;
    setpix[1] = 0;
    setpix[2] = 0;
    int FillRange = (int)gtk_spin_button_get_value(
        GTK_SPIN_BUTTON(FillRangeSlider));
    int ErrorRange = (int)gtk_spin_button_get_value(
        GTK_SPIN_BUTTON(BlankRangeSlider));
    int ErrorCount = 0;
    DataPointImage = cv::Mat::zeros(
        FilteredImage.rows, FilteredImage.cols, CV_8UC3);
    for(int i=0; i<FilteredImage.cols; i++)                                      //here rows represent columns
    {
        for(int j=0; j<FilteredImage.rows;j++)
        {
            pix = FilteredImage.at<cv::Vec3b>(j,i);
            if(pix[2] == 255)
            {
                do
                {
                    pix = FilteredImage.at<cv::Vec3b>(++j,i);
                    if(pix[2]==255)
                    {
                        ErrorCount=0;
                        DataPointImage.at<cv::Vec3b>(j,(i-1)) = setpix;
                        DataPointImage.at<cv::Vec3b>(j,(i)) = pix;
                    }
                    else
                    {
                        ErrorCount++;
                    }
                }while((pix[2] == 255) || (ErrorCount <= ErrorRange));
            }
        }
    }
    /*for(int i =0;i<frame.rows;i++)
    {
        for(int j=0; j<frame.cols;j++)
        {
                pix = frame.at<cv::Vec3b>(i,j);
                if(pix[2] == 255)
                {
                    printf("DPI x: %d, y: %d\n",j,i);
                }
        }
    }*/
    // printf("DPI x: %d, y: %d\n",DataPointImage.cols,DataPointImage.rows);
    // cv::flip(DiffImage, frame, 0);
}

void ShowDataPoints()
{
    IplImage img;
    if(gtk_gl_area_begingl(GTK_GL_AREA(MainCamArea)))
    {
        img = DataPointImage;
        HideFeed();
        LoadImage(&img, MW_CAMAREA_WIDTH,
            MW_CAMAREA_HEIGHT, MainCamArea);
        CheckEvents(100);
        gtk_gl_area_endgl(GTK_GL_AREA(MainCamArea));
    }
}
void LaserON()
{
    char Command = 'l',Response;
    WriteBytes(&Command, 1);
    ReadBytes(1, &Response);
    // StartMotor(360);
}

int StartMotor(int angle)
{
    char Command[3] = {
        's', (char)(angle>>8), (char)angle
    };
    char Response[3];
    int ResponseAngle;
    WriteBytes(Command, 3);
    ReadBytes(3, Response);
    ResponseAngle = (Response[0] <<8) | Response[1];
    if(Response[2] == Command[0])
    {
        ResponseAngle = ResponseAngle * (-1) ;
    }
    return ResponseAngle;
}

void LaserOFF()
{
    char Command = 'f',Response;
    WriteBytes(&Command, 1);
    ReadBytes(1, &Response);
}

void RecreateDataPoints(int x, int y, double angle, void *Memory)
{
    DataPoint* Out = (DataPoint*)Memory;
    double hyp;
    hyp = x * sec(to_radians((double)LaserAngle));
    Out->x = hyp * cos(to_radians(angle));
    Out->y = (double) y;
    Out->z = hyp * sin(to_radians(angle));
    if(CamAngle!=0)
    {
        // Out->y = y + (Out->z * tan(to_radians((double)CamAngle)));
        Out->z = Out->z + y * tan(to_radians(CamAngle));
        Out->y = y - hyp * sin(to_radians(CamAngle));
        //Out->z = Out->z * cos(to_radians(CamAngle));
        //Out->x = Out->x -(y * cot(to_radians(CamAngle)));
        printf("Code Reached");
        // Out->z = newz * sec(to_radians(vAdj));
    }
    Out->ox = (double)x;
    Out->oy = (double)y;
    Out->angle = (double)CamAngle;
    Out->secrad = tan(to_radians((double)CamAngle));
    Out->cosrad = cos(to_radians((double)angle));
    Out->sinrad = sin(to_radians((double)angle));
    Out->secdeg = sec((double)angle);
    Out->cosdeg = cos((double)angle);
    Out->sindeg = sin((double)angle);
}

double sec(double angle)
{
  return (1 / cos(angle));
}

double cosec(double angle)
{
  return (1 / sin(angle));
}

double cot(double angle)
{
        return(1 / tan(angle));
}

double to_degrees(double rad)
{
  return rad * 180.0 * 7/ 22;
}

double to_radians(double deg)
{
  return (deg * 22) / (180.0*7);
}