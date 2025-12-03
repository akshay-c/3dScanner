/**
 * @file MWMenu.h
 * @author Akshay Chipkar
 * @brief  Menu for the MainWindow for the 3dScanner
 * @version 0.2
 * @date 2025-11-17
 * 
 */

 #ifndef __M_W_MENU__
 #define __M_W_MENU__
    #include <QMenu>
    #include <QAction>

    class MWMenu: public QMenu
    {
        public:
        MWMenu(int _width = 0, int _height = 0);

        virtual void ~MWMenu();
        
    };

 #endif