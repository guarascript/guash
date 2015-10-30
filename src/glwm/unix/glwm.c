/*
 * File:
 *     glwm.c
 *
 * Package:
 *     Glwm
 *
 * Description:
 *     This file implements an OpenGL window management library.
 *
 * Copyright:
 *     Copyright (C) 2015 Roberto Luiz Souza Monteiro.
 *
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with this program; if not, write to the Free Software
 *     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * RCS: @(#) $Id: glwm.c,v 1.0 2015/10/26 17:22:00 monteiro Exp $
 * 
 */

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include<GL/gl.h>
#include<GL/glx.h>

#include<X11/X.h>
#include<X11/Xatom.h>
#include<X11/Intrinsic.h>
#include<X11/Xlib.h>
#include<X11/Xutil.h>

#include "glwm.h"

#define BUFFER_SIZE  4096

Display *deviceDisplay;
XContext deviceContext;
unsigned int displayMode;
int exitMainLoop;
Glwm_ListItem *windowList;

/**
 * Group:
 *     C
 *
 * Function:
 *     int Glwm_ComparePointers(void *p1, void *p2)
 *
 * Description:
 *     Compare two values p1 and p2.
 *
 * Arguments:
 *     p1,    a pointer to a value to compare;
 *     p2,    a pointer to a value to compare.
 *
 * Results:
 *     The function returns -1 if p1 is less than p2, 0 if p1 is equal p2
 *     and 1 if p1 is greater than p2.
 */
int Glwm_ComparePointers(void *p1, void *p2)
{
    if (p1 < p2) {
        return -1;
    } else if (p1 > p2) {
        return 1;
    }
    
    return 0;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Glwm_ListItem *Glwm_AddListItem(Glwm_ListItem **list, void *data)
 *
 * Description:
 *     Add an object to a list.
 *
 * Arguments:
 *     list,    a pointer to the list;
 *     data,    a pointer to the data to be stored in the list.
 *
 * Results:
 *     The function adds an object to a list.
 */
Glwm_ListItem *Glwm_AddListItem(Glwm_ListItem **list, void *data)
{
    Glwm_ListItem *newListItem;
    Glwm_ListItem *item;
    
    newListItem = (Glwm_ListItem *)malloc(sizeof(Glwm_ListItem));
    newListItem->data = data;
    newListItem->previous = NULL;
    newListItem->next = NULL;
    
    if (*list == NULL) {
        *list = newListItem;
    } else {
        item = *list;
        while (1) {
            if (item->next) {
                item = (Glwm_ListItem *)(item->next);
            } else {
                break;
            }
        }
        newListItem->previous = item;
        item->next = newListItem;
    }
    
    return newListItem;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     int Glwm_DeleteListItem(Glwm_ListItem **list, void *data, int (*cmpfunc)(void *p1, void *p2))
 *
 * Description:
 *     Deletes an object from a list.
 *
 * Arguments:
 *     list,       a pointer to the list;
 *     data,       a pointer to the data to be deleted from the list;
 *     cmpfunc,    a pointer to a function to compare two values.
 *
 * Results:
 *     The function deletes an object from a list.
 */
int Glwm_DeleteListItem(Glwm_ListItem **list, void *data, int (*cmpfunc)(void *p1, void *p2))
{
    Glwm_ListItem *previous;
    Glwm_ListItem *next;
    Glwm_ListItem *item;
    
    if (*list == NULL) {
        return 0;
    } else {
        item = *list;
        while (1) {
            if (cmpfunc(item->data, data) == 0) {
                previous = (Glwm_ListItem *)(item->previous);
                next = (Glwm_ListItem *)(item->next);
                
                if (previous) {
                    previous->next = (Glwm_ListItem *)(next);
                }
                if (next) {
                    next->previous = (Glwm_ListItem *)(previous);
                }
                if (!previous) {
                    if (!next) {
                        *list = NULL;
                    } else {
                        *list = (Glwm_ListItem *)(next);
                    }
                }
                
                item->data = NULL;
                item->previous = NULL;
                item->next = NULL;
                
                free(item);
                
                return 1;
            }
            
            if (item->next) {
                item = (Glwm_ListItem *)(item->next);
            } else {
                break;
            }
        }
    }
    
    return 0;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void *Glwm_FindListItem(Glwm_ListItem *list, void *data, int (*cmpfunc)(void *p1, void *p2))
 *
 * Description:
 *     Searches an object in a list.
 *
 * Arguments:
 *     list,       a pointer to the list;
 *     data,       a pointer to the data to be searched from the list;
 *     cmpfunc,    a pointer to a function to compare two values.
 *
 * Results:
 *     The function searches an object in a list.
 */
void *Glwm_FindListItem(Glwm_ListItem *list, void *data, int (*cmpfunc)(void *p1, void *p2))
{
    Glwm_ListItem *item;
    
    if (list == NULL) {
        return NULL;
    } else {
        item = list;
        while (item) {
            if (cmpfunc(item->data, data) == 0) {
                return data;
            }
            item = (Glwm_ListItem *)(item->next);
        }
    }
    
    return NULL;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_FreeList(Glwm_ListItem *list, void (*freefunc)(void *data))
 *
 * Description:
 *     Frees a list allocated memory.
 *
 * Arguments:
 *     list,        a pointer to the list;
 *     freefunc,    a pointer to a function to frees data resources.
 *
 * Results:
 *     The function frees the list allocated memory.
 */
void Glwm_FreeList(Glwm_ListItem *list, void (*freefunc)(void *data))
{
    if (list == NULL) {
        return;
    } else {
        Glwm_FreeList((Glwm_ListItem *)(list->next), freefunc);
        
        freefunc(list->data);
        
        free(list);
    }
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_ParseVarAssignment(char *expression, char *variable, char *value)
 *
 * Description:
 *     Parses a variable assignment like VARIABLE=VALUE.
 *
 * Arguments:
 *     expression,    the resource string;
 *     variable,      the variable name;
 *     value,         the variable value.
 *
 * Results:
 *     The function returns the variable name and its object.
 */
void Glwm_ParseVarAssignment(char *expression, char *variable, char *value)
{
    char *p;
    char *n;
    char *v;
    int token;
    
    #define VARIABLE 0
    #define VALUE    1
    
    p = expression;
    n = variable;
    v = value;
    
    token = VARIABLE;
    
    while (isspace(*p)) {
        p++;
    }
    
    while (*p) {
        if ((*p == '=') && (token == VARIABLE)) {
            token = VALUE;
            p++;
            while (isspace(*p)) {
                p++;
            }
        }
        if (token == VARIABLE) {
            if (!isspace(*p)) {
                *n = *p;
                n++;
            }
        }
        if (token == VALUE) {
            *v = *p;
            v++;
        }
        if (*p) {
            p++;
        }
    }
    
    *n = '\0';
    *v = '\0';
}

/**
 * Group:
 *     C
 *
 * Function:
 *     int Glwm_Init(int *argc, char *argv[], char **env)
 *
 * Description:
 *     Initialize the library and negotiate a session with the window system.
 *
 * Arguments:
 *     argc,    a pointer to the program’s unmodified argc variable from main function;
 *     argv,    the program’s unmodified argv variable from main function;
 *     env,     the program’s unmodified env variable from main function.
 *
 * Results:
 *     The function initialize the library and negotiate a session with the window system.
 */
int Glwm_Init(int *argc, char *argv[], char **env)
{
    char *displayName;
    char **environ;
    char *name;
    char *value;
    int i;
    
    displayName = NULL;
    
    /* Get the environment variables. */
    name = (char *)malloc(sizeof(char) * BUFFER_SIZE);
    value = (char *)malloc(sizeof(char) * BUFFER_SIZE);
    
    environ = env;
    
    while (*environ) {
        Glwm_ParseVarAssignment(*environ, name, value);
        
        if (strcmp(name, "DISPLAY") == 0) {
            if (displayName == NULL) {
                displayName = (char *)malloc(sizeof(char) * BUFFER_SIZE);
            }
            strcpy(displayName, value);
            break;
        }
        
        environ++;
    }
    
    i = 0;
    while (i < *argc) {
        if (strcmp(argv[i], "-display") == 0) {
            i++;
            if (displayName == NULL) {
                displayName = (char *)malloc(sizeof(char) * BUFFER_SIZE);
            }
            strcpy(displayName, argv[i]);
            break;
        }
        i++;
    }
    
    deviceDisplay = XOpenDisplay(displayName);
    deviceContext = XUniqueContext();
    
    free(name);
    free(value);
    
    if(deviceDisplay == NULL) {
        return 0;
    }
    
    if (displayName != NULL) {
        free(displayName);
    }
    
    displayMode = GLWM_RGBA | GLWM_SINGLE;
    
    windowList = NULL;
    
    return 1;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_InitDisplayMode(unsigned int mode)
 *
 * Description:
 *     Sets the default display mode.
 *
 * Arguments:
 *     mode,    the display mode.
 *
 * Results:
 *     The function sets the default display mode.
 */
void Glwm_InitDisplayMode(unsigned int mode)
{
    displayMode = mode;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Glwm_Window *Glwm_CreateWindow(char *title)
 *
 * Description:
 *     Creates a top-level window.
 *
 * Arguments:
 *     title,     the window's title.
 *
 * Results:
 *     The funcion creates a top-level window.
 */
Glwm_Window *Glwm_CreateWindow(char *title)
{
    Glwm_Window *w;
    
    GLint attributes[50];
    int n;
    XVisualInfo *visual;
    Window root;
    Colormap colormap;
    XSetWindowAttributes windowAttributes;
    Window window;
    Atom protocols;
    GLXContext context;
    
    memset(attributes, 0, sizeof(attributes));
    n = 0;
    
    if (displayMode & GLWM_ACCUM) {
        attributes[n] = GLX_ACCUM_RED_SIZE;
        n++;
        attributes[n] = 1;
        n++;
        attributes[n] = GLX_ACCUM_GREEN_SIZE;
        n++;
        attributes[n] = 1;
        n++;
        attributes[n] = GLX_ACCUM_BLUE_SIZE;
        n++;
        attributes[n] = 1;
        n++;
        if (displayMode & GLWM_ALPHA) {
            attributes[n] = GLX_ACCUM_ALPHA_SIZE;
            n++;
            attributes[n] = 1;
            n++;
        }
    }
    if (displayMode & GLWM_ALPHA) {
        attributes[n] = GLX_ALPHA_SIZE;
        n++;
        attributes[n] = 1;
        n++;
    }
    if (displayMode & GLWM_DEPTH) {
        attributes[n] = GLX_DEPTH_SIZE;
        n++;
        attributes[n] = 24;
        n++;
    }
    if (displayMode & GLWM_DOUBLE) {
        attributes[n] = GLX_DOUBLEBUFFER;
        n++;
        attributes[n] = True;
        n++;
    }
    if (displayMode & GLWM_INDEX) {
        attributes[n] = GLX_BUFFER_SIZE;
        n++;
        attributes[n] = 8;
        n++;
    } else {
        attributes[n] = GLX_RED_SIZE;
        n++;
        attributes[n] = 1;
        n++;
        attributes[n] = GLX_GREEN_SIZE;
        n++;
        attributes[n] = 1;
        n++;
        attributes[n] = GLX_BLUE_SIZE;
        n++;
        attributes[n] = 1;
        n++;
        if (displayMode & GLWM_ALPHA) {
            attributes[n] = GLX_ALPHA_SIZE;
            n++;
            attributes[n] = 1;
            n++;
        }
    }
    if (displayMode & GLWM_RGBA) {
        attributes[n] = GLX_RGBA;
        n++;
    }
    if (displayMode & GLWM_STENCIL) {
        attributes[n] = GLX_STENCIL_SIZE;
        n++;
        attributes[n] = 1;
        n++;
    }
    if (displayMode & GLWM_STEREO) {
        attributes[n] = GLX_STEREO;
        n++;
        attributes[n] = True;
        n++;
    }
    attributes[n] = None;
    
    visual = glXChooseVisual(deviceDisplay, 0, attributes);
    
    if(visual == NULL) {
        return NULL;
    }
    
    root = DefaultRootWindow(deviceDisplay);
    
    colormap = XCreateColormap(deviceDisplay, root, visual->visual, AllocNone);
    
    windowAttributes.colormap = colormap;
    windowAttributes.event_mask = StructureNotifyMask | ExposureMask | VisibilityChangeMask | FocusChangeMask | KeyPressMask | KeyReleaseMask | EnterWindowMask | LeaveWindowMask | PointerMotionMask | ButtonPressMask | ButtonReleaseMask;
    
    window = XCreateWindow(deviceDisplay, root, GLWM_DEFAULT_X, GLWM_DEFAULT_Y, GLWM_DEFAULT_WIDTH, GLWM_DEFAULT_HEIGHT, 0, visual->depth, InputOutput, visual->visual, CWColormap | CWEventMask, &windowAttributes);
    
    protocols = XInternAtom(deviceDisplay, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(deviceDisplay, window, &protocols, 1);
    
    XMapWindow(deviceDisplay, window);
    XStoreName(deviceDisplay, window, title);
    
    context = glXCreateContext(deviceDisplay, visual, NULL, GL_TRUE);
    glXMakeCurrent(deviceDisplay, window, context);
    
    w = (Glwm_Window *)malloc(sizeof(Glwm_Window));
    memset(w, 0, sizeof(Glwm_Window));
    
    w->width = GLWM_DEFAULT_WIDTH;
    w->height = GLWM_DEFAULT_HEIGHT;
    w->x = GLWM_DEFAULT_X;
    w->y = GLWM_DEFAULT_Y;
    w->display = deviceDisplay;
    w->visual = visual;
    w->colormap = colormap;
    w->window = window;
    w->context = context;
    w->closeState = 0;
    
    XSaveContext(deviceDisplay, window, deviceContext, (XPointer) w);
    
    Glwm_AddWindowToList(w);
    
    return w;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_FreeWindow(void *data)
 *
 * Description:
 *     Frees the specified window resources.
 *
 * Arguments:
 *     data,    the window's handle.
 *
 * Results:
 *     The function frees the specified window resources.
 */
void Glwm_FreeWindow(void *data)
{
    Glwm_Window *window;
    
    window = (Glwm_Window *)data;
    
    glXMakeCurrent((Display *)(window->display), window->window, window->context);
    
    XDeleteContext(deviceDisplay, window->window, deviceContext);
    
    glXDestroyContext((Display *)(window->display), window->context);
    XUnmapWindow((Display *)(window->display), window->window);
    XDestroyWindow((Display *)(window->display), window->window);
    XFreeColormap((Display *)(window->display), window->colormap);
    XFree(window->visual);
    
    XFlush((Display *)(window->display));
    
    free(window);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_DestroyWindow(Glwm_Window *window)
 *
 * Description:
 *     Destroys the specified window.
 *
 * Arguments:
 *     window,    the window's handle.
 *
 * Results:
 *     The function destroys the specified window.
 */
void Glwm_DestroyWindow(Glwm_Window *window)
{
    if (Glwm_FindWindowInList(window) != NULL) {
        Glwm_FreeWindow(window);
        
        Glwm_DeleteWindowFromList(window);
    }
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_SetWindowTitle(Glwm_Window *window, char *title)
 *
 * Description:
 *     Sets the window's title.
 *
 * Arguments:
 *     window,    the window's handle;
 *     title,     the window's title.
 *
 * Results:
 *     The function sets the window's title.
 */
void Glwm_SetWindowTitle(Glwm_Window *window, char *title)
{
    XTextProperty windowName;
    windowName.value = (unsigned char *) title;
    windowName.encoding = XA_STRING;
    windowName.format = 8;
    windowName.nitems = strlen(title);
    
    XSetWMName((Display *)(window->display), window->window, &windowName);
    XFlush((Display *)(window->display));
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_SetWindowPos(Glwm_Window *window, int x, int y)
 *
 * Description:
 *     Sets the window's position.
 *
 * Arguments:
 *     window,    the window's handle;
 *     x,         the window's x location in pixels;
 *     y,         the window's y location in pixels.
 *
 * Results:
 *     The function sets the window's geometry.
 */
void Glwm_SetWindowPos(Glwm_Window *window, int x, int y)
{
    window->x = x;
    window->y = y;
    
    XMoveWindow((Display *)(window->display), window->window, x, y);
    XFlush((Display *)(window->display));
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_SetWindowSize(Glwm_Window *window, int width, int height)
 *
 * Description:
 *     Sets the window's size.
 *
 * Arguments:
 *     window,    the window's handle;
 *     width,     the window's width;
 *     height,    the window's height.
 *
 * Results:
 *     The function sets the window's geometry.
 */
void Glwm_SetWindowSize(Glwm_Window *window, int width, int height)
{
    window->width = width;
    window->height = height;
    
    XResizeWindow((Display *)(window->display), window->window, width, height);
    XFlush((Display *)(window->display));
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_HideWindow(Glwm_Window *window)
 *
 * Description:
 *     Hides the window.
 *
 * Arguments:
 *     window,    the window's handle
 *
 * Results:
 *     The function hides the window.
 */
void Glwm_HideWindow(Glwm_Window *window)
{
    XUnmapWindow((Display *)(window->display), window->window);
    XFlush((Display *)(window->display));
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_ShowWindow(Glwm_Window *window)
 *
 * Description:
 *     Hides the window.
 *
 * Arguments:
 *     window,    the window's handle
 *
 * Results:
 *     The function hides the window.
 */
void Glwm_ShowWindow(Glwm_Window *window)
{
    XMapRaised((Display *)(window->display), window->window);
    XFlush((Display *)(window->display));
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_RegisterDisplayFunc(Glwm_Window *window, void (*function)(void *window))
 *
 * Description:
 *     Sets the window's display function callback.
 *
 * Arguments:
 *     window,      the window's handle;
 *     function,    the function callback.
 *
 * Results:
 *     The function sets the window's draw function callback.
 */
void Glwm_RegisterDisplayFunc(Glwm_Window *window, void (*function)(void *window))
{
    window->displayFunc = function;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_RegisterReshapeFunc(Glwm_Window *window, void (*function)(void *window))
 *
 * Description:
 *     Sets the window's reshape function callback.
 *
 * Arguments:
 *     window,      the window's handle;
 *     function,    the function callback.
 *
 * Results:
 *     The function sets the window's update function callback.
 */
void Glwm_RegisterReshapeFunc(Glwm_Window *window, void (*function)(void *window))
{
    window->reshapeFunc = function;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_RegisterIdleFunc(Glwm_Window *window, void (*function)(void *window))
 *
 * Description:
 *     Sets the window's idle function callback.
 *
 * Arguments:
 *     window,      the window's handle;
 *     function,    the function callback.
 *
 * Results:
 *     The function sets the window's idle function callback.
 */
void Glwm_RegisterIdleFunc(Glwm_Window *window, void (*function)(void *window))
{
    window->idleFunc = function;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_RegisterKeyPressFunc(Glwm_Window *window, void (*function)(void *window, unsigned char c, int x, int y))
 *
 * Description:
 *     Sets the window's key press function callback.
 *
 * Arguments:
 *     window,      the window's handle;
 *     function,    the function callback.
 *
 * Results:
 *     The function sets the window's key press function callback.
 */
void Glwm_RegisterKeyPressFunc(Glwm_Window *window, void (*function)(void *window, unsigned char c, int x, int y))
{
    window->keyPressFunc = function;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_RegisterKeyReleaseFunc(Glwm_Window *window, void (*function)(void *window, unsigned char c, int x, int y))
 *
 * Description:
 *     Sets the window's key release function callback.
 *
 * Arguments:
 *     window,      the window's handle;
 *     function,    the function callback.
 *
 * Results:
 *     The function sets the window's key release function callback.
 */
void Glwm_RegisterKeyReleaseFunc(Glwm_Window *window, void (*function)(void *window, unsigned char c, int x, int y))
{
    window->keyReleaseFunc = function;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_RegisterMouseEnterFunc(Glwm_Window *window, void (*function)(void *window))
 *
 * Description:
 *     Sets the window's mouse enter function callback.
 *
 * Arguments:
 *     window,      the window's handle;
 *     function,    the function callback.
 *
 * Results:
 *     The function sets the window's mouse enter function callback.
 */
void Glwm_RegisterMouseEnterFunc(Glwm_Window *window, void (*function)(void *window))
{
    window->mouseEnterFunc = function;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_RegisterMouseLeaveFunc(Glwm_Window *window, void (*function)(void *window))
 *
 * Description:
 *     Sets the window's mouse leave function callback.
 *
 * Arguments:
 *     window,      the window's handle;
 *     function,    the function callback.
 *
 * Results:
 *     The function sets the window's mouse leave function callback.
 */
void Glwm_RegisterMouseLeaveFunc(Glwm_Window *window, void (*function)(void *window))
{
    window->mouseLeaveFunc = function;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_RegisterMouseMotionFunc(Glwm_Window *window, void (*function)(void *window, int x, int y))
 *
 * Description:
 *     Sets the window's mouse function callback.
 *
 * Arguments:
 *     window,      the window's handle;
 *     function,    the function callback.
 *
 * Results:
 *     The function sets the window's mouse function callback.
 */
void Glwm_RegisterMouseMotionFunc(Glwm_Window *window, void (*function)(void *window, int x, int y))
{
    window->mouseMotionFunc = function;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_RegisterMouseButtonPressFunc(Glwm_Window *window, void (*function)(void *window, unsigned int button, unsigned int state, int x, int y))
 *
 * Description:
 *     Sets the window's mouse button press function callback.
 *
 * Arguments:
 *     window,      the window's handle;
 *     function,    the function callback.
 *
 * Results:
 *     The function sets the window's mouse button press function callback.
 */
void Glwm_RegisterMouseButtonPressFunc(Glwm_Window *window, void (*function)(void *window, unsigned int button, unsigned int state, int x, int y))
{
    window->mouseButtonPressFunc = function;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_RegisterMouseButtonReleaseFunc(Glwm_Window *window, void (*function)(void *window, unsigned int button, unsigned int state, int x, int y))
 *
 * Description:
 *     Sets the window's mouse button release function callback.
 *
 * Arguments:
 *     window,      the window's handle;
 *     function,    the function callback.
 *
 * Results:
 *     The function sets the window's mouse button release function callback.
 */
void Glwm_RegisterMouseButtonReleaseFunc(Glwm_Window *window, void (*function)(void *window, unsigned int button, unsigned int state, int x, int y))
{
    window->mouseButtonReleaseFunc = function;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_RegisterCloseFunc(Glwm_Window *window, void (*function)(void *window))
 *
 * Description:
 *     Sets the window's close function callback.
 *
 * Arguments:
 *     window,      the window's handle;
 *     function,    the function callback.
 *
 * Results:
 *     The function sets the window's close function callback.
 */
void Glwm_RegisterCloseFunc(Glwm_Window *window, void (*function)(void *window))
{
    window->closeFunc = function;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_MakeCurrent(Glwm_Window *window)
 *
 * Description:
 *     Sets the current window.
 *
 * Arguments:
 *     window,    the window's handle.
 *
 * Results:
 *     The function sets the current window.
 */
void Glwm_MakeCurrent(Glwm_Window *window)
{
    glXMakeCurrent((Display *)(window->display), window->window, window->context);
    XFlush((Display *)(window->display));
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_SwapBuffers(Glwm_Window *window)
 *
 * Description:
 *     Swaps the buffers of the specified window if double buffered.
 *
 * Arguments:
 *     window,    the window's handle.
 *
 * Results:
 *     The function swaps the buffers of the specified window if double buffered.
 */
void Glwm_SwapBuffers(Glwm_Window *window)
{
    glXSwapBuffers((Display *)(window->display), (Window)(window->window));
    XFlush((Display *)(window->display));
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_MainLoop(void)
 *
 * Description:
 *     Enters the event processing loop.
 *
 * Arguments:
 *
 * Results:
 *     The function enters the event processing loop.
 */
void Glwm_MainLoop(void)
{
    exitMainLoop = 0;
    
    while(!exitMainLoop) {
        Glwm_MainIteration();
        
        if (windowList == NULL) {
            break;
        }
    }
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_MainIteration(void)
 *
 * Description:
 *     Process one iteraction of the event processing loop.
 *
 * Arguments:
 *
 * Results:
 *     The function process one iteraction of the event processing loop.
 */
void Glwm_MainIteration(void)
{
    XEvent event;
    Glwm_Window *window;
    char keyText;
    KeySym keySym;
    Window r;
    Window w;
    int rx;
    int ry;
    int wx;
    int wy;
    unsigned int mask;
    unsigned int mods;
    Glwm_ListItem *item;
    
    window = NULL;
    
    if (XPending(deviceDisplay)) {
        XNextEvent(deviceDisplay, &event);
        
        if (XFindContext(deviceDisplay, event.xany.window, deviceContext, (XPointer *) &window) != 0) {
            return;
        }
        
        if (event.type == Expose) {
            glXMakeCurrent((Display *)(window->display), window->window, window->context);
            if (window->displayFunc != NULL) {
                window->displayFunc(window);
            }
            
            XFlush((Display *)(window->display));
        } else if(event.type == KeyPress) {
            if (XLookupString(&event.xkey, &keyText, 1, &keySym, 0) == 1) {
                glXMakeCurrent((Display *)(window->display), window->window, window->context);
                if (window->keyPressFunc != NULL) {
                    XQueryPointer(window->display, window->window, &r, &w, &rx, &ry, &wx, &wy, &mask);
                    window->keyPressFunc(window, keySym, wx, wy);
                }
            }
            
            XFlush((Display *)(window->display));
        } else if(event.type == KeyRelease) {
            if (XLookupString(&event.xkey, &keyText, 1, &keySym, 0) == 1) {
                glXMakeCurrent((Display *)(window->display), window->window, window->context);
                if (window->keyReleaseFunc != NULL) {
                    XQueryPointer(window->display, window->window, &r, &w, &rx, &ry, &wx, &wy, &mask);
                    window->keyReleaseFunc(window, keySym, wx, wy);
                }
            }
            
            XFlush((Display *)(window->display));
        } else if(event.type == EnterNotify) {
            if (window->mouseEnterFunc != NULL) {
                window->mouseEnterFunc(window);
            }
            
            XFlush((Display *)(window->display));
        } else if(event.type == LeaveNotify) {
            if (window->mouseLeaveFunc != NULL) {
                window->mouseLeaveFunc(window);
            }
            
            XFlush((Display *)(window->display));
        } else if(event.type == MotionNotify) {
            if (window->mouseMotionFunc != NULL) {
                window->mouseMotionFunc(window, event.xmotion.x, event.xmotion.y);
            }
            
            XFlush((Display *)(window->display));
        } else if(event.type == ButtonPress) {
            if (window->mouseButtonPressFunc != NULL) {
                mods = 0;
                if (event.xbutton.state & ShiftMask) {
                    mods = mods | GLWM_MOD_SHIFT;
                } else if (event.xbutton.state & ControlMask) {
                    mods = mods | GLWM_MOD_CONTROL;
                } else if (event.xbutton.state & Mod1Mask) {
                    mods = mods | GLWM_MOD_ALT;
                } else if (event.xbutton.state & Mod4Mask) {
                    mods = mods | GLWM_MOD_SUPER;
                }
                
                window->mouseButtonPressFunc(window, event.xbutton.button - 1, mods, event.xbutton.x, event.xbutton.y);
            }
            
            XFlush((Display *)(window->display));
        } else if(event.type == ButtonRelease) {
            if (window->mouseButtonReleaseFunc != NULL) {
                mods = 0;
                if (event.xbutton.state & ShiftMask) {
                    mods = mods | GLWM_MOD_SHIFT;
                } else if (event.xbutton.state & ControlMask) {
                    mods = mods | GLWM_MOD_CONTROL;
                } else if (event.xbutton.state & Mod1Mask) {
                    mods = mods | GLWM_MOD_ALT;
                } else if (event.xbutton.state & Mod4Mask) {
                    mods = mods | GLWM_MOD_SUPER;
                }
                
                window->mouseButtonReleaseFunc(window, event.xbutton.button - 1, mods, event.xbutton.x, event.xbutton.y);
            }
            
            XFlush((Display *)(window->display));
        } else if (event.type == ClientMessage) {
            if (window->closeFunc != NULL) {
                window->closeFunc(window);
            }
        } else if (event.type == ConfigureNotify) {
            XConfigureEvent xce = event.xconfigure;
            
            if (xce.width != window->width) {
                window->width = xce.width;
            }
            if (xce.height != window->height) {
                window->height = xce.height;
            }
            if (xce.x != window->x) {
                window->x = xce.x;
            }
            if (xce.y != window->y) {
                window->y = xce.y;
            }
            
            if (window->reshapeFunc != NULL) {
                window->reshapeFunc(window);
            }
            if (window->displayFunc != NULL) {
                window->displayFunc(window);
            }
            
            XFlush((Display *)(window->display));
        }
    } else {
        item = windowList;
        
        if (item == NULL) {
            return;
        } else {
            while (item) {
                window = (Glwm_Window *)(item->data);
                
                if (window->displayFunc != NULL) {
                    window->displayFunc(window);
                }
                if (window->idleFunc != NULL) {
                    window->idleFunc(window);
                }
                
                XFlush((Display *)(window->display));
                
                item = (Glwm_ListItem *)(item->next);
            }
        }
    }
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_ExitMainLoop(void)
 *
 * Description:
 *     This function exits the main event loop.
 *
 * Arguments:
 *
 * Results:
 *     This function exits the main event loop.
 */
void Glwm_ExitMainLoop(void)
{
    exitMainLoop = 1;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glwm_Terminate(void)
 *
 * Description:
 *     This function should be called before the application exits.
 *
 * Arguments:
 *
 * Results:
 *     This function should be called before the application exits.
 */
void Glwm_Terminate(void)
{
    XCloseDisplay(deviceDisplay);
    
    Glwm_FreeWindowList();
}

/**
 * Group:
 *     C
 *
 * Function:
 *     double Glwm_GetTime(void)
 *
 * Description:
 *     Returns the time as a float point number.
 *
 * Arguments:
 *
 * Results:
 *     This function returns the time as a float point number.
 */
double Glwm_GetTime(void)
{
    struct timeval tv;
    struct timezone tz;
    double t;
    
    gettimeofday(&tv, &tz);
    
    t = tv.tv_sec + (tv.tv_usec / 1000000.0);
    
    return t;
}

