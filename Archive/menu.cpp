#include <gtk/gtk.h>
#include "main.hpp"

GtkWidget *MWMenuBox;
GtkWidget *MWMenuBar;

GtkWidget *MWFileMenu;
GtkWidget *MWFileMI;
GtkWidget *MWStartMI;
GtkWidget *MWQuitMI;

GtkWidget *MWEditMenu;
GtkWidget *MWEditMI;
GtkWidget *MWSettingsMI;

GtkWidget *MWHelpMenu;
GtkWidget *MWHelpMI;
GtkWidget *MWAboutMI;

static void MenuSettingsActivate(GtkWidget* app, gpointer user_data)
{
    ShowSettingsWindow();
}

void InitMainMenu()
{
	//MWMenuBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0); 					//Create vbox(container to hold menubar)
	MWMenuBox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(MainWindow), MWMenuBox);					//Attach vbox to mainwindow
	MWMenuBar = gtk_menu_bar_new();												//Create menubar

    MWFileMenu = gtk_menu_new();												//Create Container for submenus
    MWFileMI = gtk_menu_item_new_with_mnemonic("_File");						//Create MenuItem for File menu
    MWStartMI = gtk_menu_item_new_with_mnemonic("_Start");
	MWQuitMI = gtk_menu_item_new_with_mnemonic("_Quit");						//Create MenuItem for Quit menu
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(MWFileMI), MWFileMenu);				//Set name for file menu
    gtk_menu_shell_append(GTK_MENU_SHELL(MWFileMenu), MWStartMI);
    gtk_menu_shell_append(GTK_MENU_SHELL(MWFileMenu), MWQuitMI);				//Append Quit menu to FileMenu
    gtk_menu_shell_append(GTK_MENU_SHELL(MWMenuBar), MWFileMI);					//Attach file menu to menubar

    MWEditMenu = gtk_menu_new();
    MWEditMI = gtk_menu_item_new_with_mnemonic("_Edit");
    MWSettingsMI = gtk_menu_item_new_with_mnemonic("S_ettings");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(MWEditMI), MWEditMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(MWEditMenu), MWSettingsMI);
    gtk_menu_shell_append(GTK_MENU_SHELL(MWMenuBar), MWEditMI);

    MWHelpMenu = gtk_menu_new();
    MWHelpMI = gtk_menu_item_new_with_mnemonic("_Help");
    MWAboutMI = gtk_menu_item_new_with_mnemonic("_About");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(MWHelpMI), MWHelpMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(MWHelpMenu), MWAboutMI);
    gtk_menu_shell_append(GTK_MENU_SHELL(MWMenuBar), MWHelpMI);

    gtk_box_pack_start(GTK_BOX(MWMenuBox), MWMenuBar, FALSE, FALSE, 0);       	//Attach menubar to vbox

    g_signal_connect(G_OBJECT(MWQuitMI), "activate",
        G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(MWSettingsMI), "activate",
        G_CALLBACK(MenuSettingsActivate), NULL);
}
