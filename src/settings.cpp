#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include <gtk/gtk.h>
#include <gtkgl/gtkglarea.h>
#include <epoxy/gl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <time.h>
#include <X11/Xlib.h>

#include "main.hpp"
#include "settings.hpp"
#include "preferences.hpp"
#include "display.hpp"
#include "idle.hpp"

//Function Declarations

void StartCapture(int);

void SettingsWindowClosing(GtkWidget *widget, gpointer callback_data);
int RetrieveCamInfo(char*);

void StoreCamInfo(char* Name);
void SetSelectedCam(GtkComboBox *widget, gpointer user_data);

void DisplaySelectionFrame();
void RunCalibrationAndSave(cv::vector<cv::vector<cv::Point2f> > ImagePoints,
    cv::Size ImageSize, cv::Size BoardSize, cv::Mat& CameraMatrix,
    cv::Mat& DistCoeffs);
void CalculateChessboardCorners(cv::Size BoardSize, cv::vector<cv::Point3f>& corners);

//Variables
bool                SettingsWindowClosed = FALSE;
bool                IsCamSelected = FALSE;
int                 RefreshRate=20;
GtkWidget           *SettingsContainerControls;
int                 VideoDeviceCount;
int                 CalibrationFlags = 0;
cv::VideoCapture    cap;
//int                 count = 0;
//GdkDrawable         test;
static cv::Mat             edges;
static unsigned int        RefreshDelay;

void ShowSettingsWindow()
{
    SettingsWindowClosed = FALSE;
    SettingsWindow =  gtk_window_new(GTK_WINDOW_TOPLEVEL);
    SettingsContainerMain = gtk_hbox_new(FALSE, 0);
    SettingsContainerControls = gtk_layout_new(NULL, NULL);
    SettingsContainerCam = gtk_vbox_new(FALSE, 0);

    CamArea = gtk_gl_area_new (AttrList);
    gtk_widget_set_events(GTK_WIDGET(CamArea), GDK_EXPOSURE_MASK);

    gtk_window_set_transient_for(GTK_WINDOW(SettingsWindow),
        GTK_WINDOW(MainWindow));
    gtk_window_set_modal (GTK_WINDOW(SettingsWindow), TRUE);
    gtk_window_set_resizable(GTK_WINDOW(SettingsWindow), FALSE);
    gtk_window_set_title (GTK_WINDOW(SettingsWindow), "Settings");
    gtk_window_set_position(GTK_WINDOW(SettingsWindow),
        GTK_WIN_POS_CENTER_ON_PARENT);
    /*gtk_window_set_default_size (GTK_WINDOW(SettingsWindow),
		SW_WIDTH , SW_HEIGHT);*/
    gtk_widget_set_size_request(SettingsWindow, SW_WIDTH, SW_HEIGHT);
    gtk_quit_add_destroy(1, GTK_OBJECT(CamArea));

    gtk_container_add(GTK_CONTAINER(SettingsWindow), SettingsContainerMain);
    gtk_box_pack_start(GTK_BOX(SettingsContainerMain),
        SettingsContainerControls, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(SettingsContainerMain), SettingsContainerCam,
        FALSE, FALSE, 0);
    gtk_widget_set_size_request(SettingsContainerControls,
        CONTROLS_WIDTH, SW_HEIGHT);

    ComboCamSelect = gtk_combo_box_text_new ();

    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(ComboCamSelect),
        "Select Camera");
    PopulateCamList();
    gtk_combo_box_set_active(GTK_COMBO_BOX(ComboCamSelect),0);

    gtk_box_pack_start(GTK_BOX(SettingsContainerCam), ComboCamSelect,
        FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(SettingsContainerCam), CamArea,
        FALSE, FALSE, 0);

    //gtk_signal_connect (GTK_OBJECT(CamArea), "expose-event",
    //	GTK_SIGNAL_FUNC(render), NULL);
    gtk_widget_set_usize(CamArea, CA_WIDTH, CA_HEIGHT);
    //gtk_widget_set_size_request(CamArea, CA_WIDTH, CA_HEIGHT);
    //GTK_WIDGET_SET_FLAGS(CamArea,GTK_CAN_FOCUS);
    gtk_layout_set_size(GTK_LAYOUT(SettingsContainerControls),
        CONTROLS_WIDTH, SW_HEIGHT);
    g_signal_connect(SettingsWindow, "destroy",
        G_CALLBACK(SettingsWindowClosing), NULL);
    g_signal_connect(GTK_COMBO_BOX(ComboCamSelect), "changed",
        G_CALLBACK(SetSelectedCam), NULL);
    //gtk_container_add(GTK_CONTAINER(SettingsWindow),CamArea);
    // gtk_widget_show(CamArea);
    // initgl();
    gtk_widget_show_all(SettingsWindow);
    ShowPreferences();
    CamAreaWidth = CamArea->allocation.width;
    CamAreaHeight = CamArea->allocation.height;
    //pthread_create(&ThreadId, NULL, &StartCapture, NULL);
    //Thread = g_thread_new(NULL, StartCapture, NULL);
    DisplaySelectionFrame();
}

void DisplaySelectionFrame()
{
    while(IsCamSelected && !SettingsWindowClosed)
    {
        StartCapture(SelectedCam);
    }
}

void SettingsWindowClosing(GtkWidget *widget, gpointer callback_data)
{
    SettingsWindowClosed = TRUE;
    cap.release();
    //g_print("Settings Window Closed");
}

void StartCapture(int camno)
{
    // if(event->type == GDK_MAP)
    // {
    // 	g_print("\n %d Expose",++count);
    // }
    //gtk_gl_area_swapbuffers(GTK_GL_AREA(area));
    
    //gdk_threads_enter();
    if(SelectedCam < 0)
    {
        return;
    }

    IplImage img;
    //CvRect metadata;
    cv::Mat frame;
    // gtk_widget_grab_focus(GTK_WIDGET(CamArea));
//    while(!SettingsWindowClosed)
    if (gtk_gl_area_begingl(GTK_GL_AREA(CamArea)))
    {
        /*g_print("Test\n");
        glViewport(0, 0, (GLsizei)CamAreaWidth, (GLsizei)CamAreaHeight);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60, CamAreaWidth / CamAreaHeight, 1.0, 100.0);
        // gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
        //glOrtho(0.0, w, h, 0.0, 1.0, 100.0);
        glMatrixMode(GL_MODELVIEW);
        glDrawBuffer(GL_BACK);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);*/
        cap >> frame;
        cv::flip(frame, frame, 0);
        img = frame;
        LoadImage(&img, CamAreaWidth, CamAreaHeight, CamArea);
        gtk_gl_area_endgl(GTK_GL_AREA(CamArea));
        CheckEvents(RefreshRate);
    }else
    {
        g_print("Problem Loading OpenGL");
    }

    //gdk_threads_leave();

    //draw_an_object ();
}

void PopulateCamList()
{
    int fd;
    std::stringstream VideoDevice;
    VideoDeviceCount = 0;
    struct v4l2_capability video_cap;
    for(VideoDeviceCount = 0; ; VideoDeviceCount++)
    {
        VideoDevice.str("");
        VideoDevice<<"/dev/video"<<VideoDeviceCount;
        if((fd = open(VideoDevice.str().c_str(), O_RDONLY)) == -1)
        {
            close(fd);
            break;
        }
        else
        {
            if(ioctl(fd, VIDIOC_QUERYCAP, &video_cap) == -1)
                perror("cam_info: Can't get capabilities");
            else
            {
                gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(ComboCamSelect),
                    (gchar*)video_cap.card);
                StoreCamInfo((char*)video_cap.card);
            }
        }
    }
}

void StoreCamInfo(char* value)
{
    CamInfo *Current;
    if(!CamDetails)
    {
    CamDetails = (CamInfo*)malloc(sizeof(CamInfo));
    sprintf(CamDetails->Name, "%s", value);
    CamDetails->Id = VideoDeviceCount;
    CamDetails->Next = NULL;
    }
    else
    {
        Current = CamDetails;
        while(Current->Next != NULL)
        {
            Current = Current->Next;
        }
        Current->Next = (CamInfo*)malloc(sizeof(CamInfo));
        sprintf(Current->Next->Name, "%s", value);
        Current->Next->Id = VideoDeviceCount;
        Current->Next->Next = NULL;
    }
}

void SetSelectedCam(GtkComboBox *widget, gpointer user_data)
{
    IsCamSelected = false;
    char value[32];
    sprintf(value, "%s", gtk_combo_box_get_active_text(
        GTK_COMBO_BOX(ComboCamSelect)));
    // g_print("\nCombo selected %s", value);
    SelectedCam = RetrieveCamInfo(value);
    cap.release();
    cap.open(SelectedCam);
    if(SelectedCam >= 0)
    {
        IsCamSelected = true;
        DisplaySelectionFrame();
    }
}

int RetrieveCamInfo(char* Name)
{
    CamInfo *Current;
    Current = CamDetails;
    while(Current != NULL)
    {
        if(strcmp(Current->Name , Name) == 0)
        {
            //g_print("Cam Found\nId: %d\n",Current->Id);
            return Current->Id;
        }
        Current = Current->Next;
    }
    return -1;
}

int StartCalibration()
{
    IplImage img;
    if(ValidateForm()>0)
    {
        cv::Mat CameraMatrix, DistCoeffs;
        cv::vector<cv::vector<cv::Point2f> > ImagePoints;
        cv::vector<cv::string> imageList;
        cv::Size ImageSize;
        cv::Size BoardSize;
        cv::vector<cv::Point2f> pointBuf;
        bool found;
        BoardSize.width = BoardWidth;
        BoardSize.height = BoardHeight;
        MarkedTimeStamp = clock();
        if(!cap.isOpened())
        {
            cap.open(SelectedCam);
        }
        for(int i=0;;++i)
        {
            cv::Mat view;
            cap>>view;
            bool BlinkOutput = false;
            if(ImagePoints.size() >= CalibrationImages)
            {
                RunCalibrationAndSave(ImagePoints, ImageSize, BoardSize,
                    CameraMatrix, DistCoeffs);
                break;
            }
            ImageSize = view.size();
            if(FlipVertical)
                flip(view, view, 0);
            switch(SelectedPattern)
            {
            case PATTERN_CHESSBOARD:
                found = findChessboardCorners( view, BoardSize, pointBuf,
                    CV_CALIB_CB_ADAPTIVE_THRESH |
                    CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE);
            break;
            case PATTERN_CIRCLES_GRID:
                found = findCirclesGrid( view, BoardSize, pointBuf );
            break;
            case PATTERN_ASYMMETRIC_CIRCLES_GRID:
                found = findCirclesGrid( view, BoardSize,
                    pointBuf, cv::CALIB_CB_ASYMMETRIC_GRID );
            break;
            default:
                found = false;
            break;
            }
            if (found)
            {
                if(SelectedPattern == PATTERN_CHESSBOARD)
                {
                    cv::Mat viewGray;
                    cvtColor(view, viewGray, cv::COLOR_BGR2GRAY);
                    cornerSubPix(viewGray, pointBuf,
                        cv::Size(11,11), cv::Size(-1,-1),
                        cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER,
                        30, 0.1 ));
                }
                ImagePoints.push_back(pointBuf);
                BlinkOutput = cap.isOpened();

                drawChessboardCorners(view, BoardSize,
                    cv::Mat(pointBuf),found);
            }
            if(BlinkOutput)
            {
                bitwise_not(view, view);
                BlinkOutput = false;
            }
            img = view;
            if (gtk_gl_area_begingl(GTK_GL_AREA(CamArea)))
            {
                LoadImage(&img, CamAreaWidth, CamAreaHeight, CamArea);
                gtk_gl_area_endgl(GTK_GL_AREA(CamArea));
            }
            CheckEvents(CalibrationDelay);
            if(SettingsWindowClosed)
            {
                break;
            }
        }
    }   

}

void RunCalibrationAndSave(cv::vector<cv::vector<cv::Point2f> > ImagePoints,
    cv::Size ImageSize, cv::Size BoardSize, cv::Mat& CameraMatrix,
    cv::Mat& DistCoeffs)
{
    CalibrationFlags = 0;
    cv::vector<cv::Mat> rvecs, tvecs;
    cv::vector<float> ReProjErrs;
    double TotalAvgErr = 0;
    cv::vector<cv::Point2f> ImagePoints2;
    int i, TotalPoints = 0;
    double TotalErr = 0, err;

    if(FixAspectRatio)
        CalibrationFlags |= CV_CALIB_FIX_ASPECT_RATIO;
    if(TangentialDistortion)
        CalibrationFlags |= CV_CALIB_ZERO_TANGENT_DIST;
    if(CentralPrincipalPoint)
        CalibrationFlags |= CV_CALIB_FIX_PRINCIPAL_POINT;

    //Run Calibration
    CameraMatrix = cv::Mat::eye(3, 3, CV_64F);
    if(FixAspectRatio)
        CameraMatrix.at<double>(0,0) = AspectRatio;
    DistCoeffs = cv::Mat::zeros(8, 1, CV_64F);
    cv::vector<cv::vector<cv::Point3f> > ObjectPoints(1);
    CalculateChessboardCorners(BoardSize, ObjectPoints[0]);
    ObjectPoints.resize(ImagePoints.size(),ObjectPoints[0]);
    double rms = calibrateCamera(ObjectPoints, ImagePoints, ImageSize,
        CameraMatrix, DistCoeffs, rvecs, tvecs,
        CalibrationFlags | CV_CALIB_FIX_K4 | CV_CALIB_FIX_K5);
    printf("RMS error reported by calibrateCamera: %g\n", rms);
    bool ok = cv::checkRange(CameraMatrix) && cv::checkRange(DistCoeffs);
    //Compute Reprojection Errors

    ReProjErrs.resize(ObjectPoints.size());
    for(int i =0;i < (int)ObjectPoints.size(); i++)
    {
        projectPoints(cv::Mat(ObjectPoints[i]), rvecs[i], tvecs[i],
            CameraMatrix, DistCoeffs, ImagePoints2);
        err = norm(cv::Mat(ImagePoints[i]), cv::Mat(ImagePoints2), CV_L2);
        int n = (int)ObjectPoints[i].size();
        ReProjErrs[i] = (float)std::sqrt(err*err/n);
        TotalErr += err*err;
        TotalPoints += n;
    }
    TotalAvgErr = std::sqrt(TotalErr/TotalPoints);
    printf("%s. avg reprojection error = %.2f\n",
        ok ? "Calibration succeeded" : "Calibration failed",
        TotalAvgErr);

    //Save Camera Parameters
    if(ok)
    {
        cv::FileStorage fs(OutputFileName, cv::FileStorage::WRITE);
        time_t tm;
        time( &tm );
        struct tm *t2 = localtime( &tm );
        char buf[1024];
        strftime( buf, sizeof(buf)-1, "%c", t2 );

        fs << "CalibrationTime" << buf;

        if( !rvecs.empty() || !ReProjErrs.empty() )
            fs << "CalibrationImages" << (int)std::max(rvecs.size(),
                ReProjErrs.size());
        fs << "ImageWidth" << ImageSize.width;
        fs << "ImageHeight" << ImageSize.height;
        fs << "BoardWidth" << BoardWidth;
        fs << "BoardHeight" << BoardHeight;
        fs << "SquareSize" << SquareSize;

        if( CalibrationFlags & CV_CALIB_FIX_ASPECT_RATIO )
            fs << "AspectRatio" << AspectRatio;

        if( CalibrationFlags )
        {
            sprintf( buf, "Flags: %s%s%s%s",
                CalibrationFlags & CV_CALIB_USE_INTRINSIC_GUESS ?
                    " +use_intrinsic_guess" : "",
                CalibrationFlags & CV_CALIB_FIX_ASPECT_RATIO ?
                    " +fix_aspectRatio" : "",
                CalibrationFlags & CV_CALIB_FIX_PRINCIPAL_POINT ?
                    " +fix_principal_point" : "",
                CalibrationFlags & CV_CALIB_ZERO_TANGENT_DIST ?
                    " +zero_tangent_dist" : "" );
            cvWriteComment( *fs, buf, 0 );

        }

        fs << "CalibrationFlags" << CalibrationFlags;

        fs << "CameraMatrix" << CameraMatrix;
        fs << "DistortionCoefficients" << DistCoeffs;

        fs << "AvgReprojectionError" << TotalAvgErr;
        if( !ReProjErrs.empty() )
            fs << "PerViewReprojectionErrors" << cv::Mat(ReProjErrs);

        if( !rvecs.empty() && !tvecs.empty() )
        {
            CV_Assert(rvecs[0].type() == tvecs[0].type());
            cv::Mat bigmat((int)rvecs.size(), 6, rvecs[0].type());
            for( int i = 0; i < (int)rvecs.size(); i++ )
            {
                cv::Mat r = bigmat(cv::Range(i, i+1), cv::Range(0,3));
                cv::Mat t = bigmat(cv::Range(i, i+1), cv::Range(3,6));

                CV_Assert(rvecs[i].rows == 3 && rvecs[i].cols == 1);
                CV_Assert(tvecs[i].rows == 3 && tvecs[i].cols == 1);
                //*.t() is MatExpr (not Mat) so we can use assignment operator
                r = rvecs[i].t();
                t = tvecs[i].t();
            }
            cvWriteComment( *fs, "a set of 6-tuples (rotation vector + translation vector) for each view", 0 );
            fs << "ExtrinsicParameters" << bigmat;
        }

        if( !ImagePoints.empty() )
        {
            cv::Mat imagePtMat((int)ImagePoints.size(), (int)ImagePoints[0].size(), CV_32FC2);
            for( int i = 0; i < (int)ImagePoints.size(); i++ )
            {
                cv::Mat r = imagePtMat.row(i).reshape(2, imagePtMat.cols);
                cv::Mat imgpti(ImagePoints[i]);
                imgpti.copyTo(r);
            }
            fs << "ImagePoints" << imagePtMat;
        }
    }
}

void CalculateChessboardCorners(cv::Size BoardSize,
    cv::vector<cv::Point3f>& Corners)
{
    Corners.resize(0);

    switch(SelectedPattern)
    {
      case PATTERN_CHESSBOARD:
      case PATTERN_CIRCLES_GRID:
        for( int i = 0; i < BoardSize.height; i++ )
            for( int j = 0; j < BoardSize.width; j++ )
                Corners.push_back(cv::Point3f(float(j*SquareSize),
                                          float(i*SquareSize), 0));
        break;

      case PATTERN_ASYMMETRIC_CIRCLES_GRID:
        for( int i = 0; i < BoardSize.height; i++ )
            for( int j = 0; j < BoardSize.width; j++ )
                Corners.push_back(cv::Point3f(float((2*j + i % 2)*SquareSize),
                                          float(i*SquareSize), 0));
        break;

      default:
        // CV_Error(CV_StsBadArg, "Unknown pattern type\n");
        break;
    }
}
