/**
 * @file MainWindow.h
 * @author Akshay Chipkar
 * @brief Main Window for the 3dScanner
 * @version 0.2
 * @date 2025-11-17
 * 
 */

 #ifndef __MAIN_WINDOW__
 #define __MAIN_WINDOW__
    #include <QWindow>

    class MainWindow: public QWidget
    {
        public:
        MainWindow(int _width = 0, int _height = 0);

        virtual void ~MainWindow();
        
    };

 #endif