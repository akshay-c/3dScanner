#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/types_c.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <gtk/gtk.h>
#include <gtkgl/gtkglarea.h>

static GLuint              Texture;

void InitTexture(IplImage *Image)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);

    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, Image->width, Image->height, 0,
        GL_BGR, GL_UNSIGNED_BYTE, Image->imageData);
}

void ApplyTexture(int ImgWidth, int ImgHeight, int DisplayWidth,
    int DisplayHeight)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glBegin(GL_QUADS);
    
    glTexCoord2f(0, 0);
    glVertex3f(0, 0, 0);
    glTexCoord2f(0, 1);
    glVertex3f(0, DisplayHeight, 0);
    glTexCoord2f(1, 1);
    glVertex3f(DisplayWidth, DisplayHeight, 0);
    glTexCoord2f(1, 0);
    glVertex3f(DisplayWidth, 0, 0);
    
    glEnd();
    glFlush();
    glDisable(GL_TEXTURE_2D);
}

void LoadImage(IplImage *Image, int DisplayWidth, int DisplayHeight,
    GtkWidget *Region)
{
    InitTexture(Image);
    glViewport(0, 0, DisplayWidth , DisplayHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, DisplayWidth , 0, DisplayHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    ApplyTexture(Image->width,Image->height, DisplayWidth, DisplayHeight);
    gtk_gl_area_swapbuffers(GTK_GL_AREA(Region));
}

