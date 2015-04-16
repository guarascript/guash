/**
 * File:
 *     glfw.c
 *
 * Package:
 *     Glfw
 *
 * Description:
 *     This library implements an OpenGL GLFW wrapper
 *     for the GuaraScript interpreter.
 *
 * Copyright:
 *     Copyright (C) 2015 Roberto Luiz Souza Monteiro.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * RCS: @(#) $Id: glfw.c,v 1.0 2015/03/03 11:33:00 monteiro Exp $
 * 
 */

#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "glfw.h"

#define EXPRESSION_SIZE  4096
#define ERROR_SIZE  65536

Gua_Namespace *globalNameSpace;

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glfw_GLFWcharfun(GLFWwindow *window, unsigned int codepoint)
 *
 * Description:
 *     GLFW Unicode character callback.
 *
 * Arguments:
 *     window,       the window that received the event;
 *     codepoint,    the Unicode code point of the character.
 *
 * Results:
 *     This function is a wrapper to the Unicode character callback.
 */
void Glfw_GLFWcharfun(GLFWwindow *window, unsigned int codepoint)
{
    Gua_Handle *h;
    Gua_Object callback;
    Gua_Function function;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    Gua_String errMessage;
    
    Gua_ClearObject(callback);
    Gua_ClearObject(object);
    
    /* Get the callback function name. */
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "$GLFW_CHARCALLBACK[\"%p\"]", window);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &callback, &status, error);
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
    
    /* Prepare the arguments array to be passed to the callback function. */
    argc = 3;
    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
    
    Gua_ClearArguments(argc, argv);
    
    /* The callback function name. */
    Gua_LinkStringToObject(argv[0], Gua_ObjectToString(callback));
    Gua_SetStoredObject(argv[0]);
    
    /* Pass the window handle as the first formal argument. */
    Gua_NewHandle(h, "GLFWwindow", window);
    Gua_HandleToObject(argv[1], (struct Gua_Handle *)h);
    
    /* Pass the other function argument. */
    Gua_IntegerToObject(argv[2], codepoint);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    /* Call the callback function. */
    if (Gua_GetFunction(globalNameSpace, Gua_ObjectToString(callback), &function) == GUA_OK) {
        status = function.pointer(globalNameSpace, argc, argv, &object, error);
    } else {
        status = GUA_ERROR;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "undefined function", Gua_ObjectToString(callback));
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /* Free the allocated resources. */
    Gua_FreeArguments(argc, argv);
    
    if (!Gua_IsObjectStored(callback)) {
        Gua_FreeObject(&callback);
    }
    
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    /* Show the error message, if any. */
    if (status != GUA_OK) {
        if (!((status == GUA_RETURN) || (status == GUA_EXIT))) {
            printf("\nError: %s", error);
            
            Gua_Free(error);
            
            exit(1);
        }
    }
    
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glfw_GLFWcharmodsfun(GLFWwindow *window, unsigned int codepoint, int mods)
 *
 * Description:
 *     GLFW Unicode character with modifiers callback.
 *
 * Arguments:
 *     window,       the window that received the event;
 *     codepoint,    the Unicode code point of the character;
 *     mods,         bit field describing which modifier keys were held down.
 *
 * Results:
 *     This function is a wrapper to the Unicode character with modifiers callback.
 */
void Glfw_GLFWcharmodsfun(GLFWwindow *window, unsigned int codepoint, int mods)
{
    Gua_Handle *h;
    Gua_Object callback;
    Gua_Function function;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    Gua_String errMessage;
    
    Gua_ClearObject(callback);
    Gua_ClearObject(object);
    
    /* Get the callback function name. */
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "$GLFW_CHARMODSCALLBACK[\"%p\"]", window);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &callback, &status, error);
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
    
    /* Prepare the arguments array to be passed to the callback function. */
    argc = 4;
    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
    
    Gua_ClearArguments(argc, argv);
    
    /* The callback function name. */
    Gua_LinkStringToObject(argv[0], Gua_ObjectToString(callback));
    Gua_SetStoredObject(argv[0]);
    
    /* Pass the window handle as the first formal argument. */
    Gua_NewHandle(h, "GLFWwindow", window);
    Gua_HandleToObject(argv[1], (struct Gua_Handle *)h);
    
    /* Pass the other function argument. */
    Gua_IntegerToObject(argv[2], codepoint);
    Gua_IntegerToObject(argv[3], mods);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    /* Call the callback function. */
    if (Gua_GetFunction(globalNameSpace, Gua_ObjectToString(callback), &function) == GUA_OK) {
        status = function.pointer(globalNameSpace, argc, argv, &object, error);
    } else {
        status = GUA_ERROR;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "undefined function", Gua_ObjectToString(callback));
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /* Free the allocated resources. */
    Gua_FreeArguments(argc, argv);
    
    if (!Gua_IsObjectStored(callback)) {
        Gua_FreeObject(&callback);
    }
    
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    /* Show the error message, if any. */
    if (status != GUA_OK) {
        if (!((status == GUA_RETURN) || (status == GUA_EXIT))) {
            printf("\nError: %s", error);
            
            Gua_Free(error);
            
            exit(1);
        }
    }
    
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glfw_GLFWcursorenterfun(GLFWwindow *window, int entered)
 *
 * Description:
 *     GLFW cursor boundary crossing callback.
 *
 * Arguments:
 *     window,     the window that received the event;
 *     entered,    GL_TRUE if the cursor entered the window's client area, or GL_FALSE if it left it.
 *
 * Results:
 *     This function is a wrapper to the cursor boundary crossing callback.
 */
void Glfw_GLFWcursorenterfun(GLFWwindow *window, int entered)
{
    Gua_Handle *h;
    Gua_Object callback;
    Gua_Function function;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    Gua_String errMessage;
    
    Gua_ClearObject(callback);
    Gua_ClearObject(object);
    
    /* Get the callback function name. */
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "$GLFW_CURSORENTERCALLBACK[\"%p\"]", window);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &callback, &status, error);
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
    
    /* Prepare the arguments array to be passed to the callback function. */
    argc = 3;
    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
    
    Gua_ClearArguments(argc, argv);
    
    /* The callback function name. */
    Gua_LinkStringToObject(argv[0], Gua_ObjectToString(callback));
    Gua_SetStoredObject(argv[0]);
    
    /* Pass the window handle as the first formal argument. */
    Gua_NewHandle(h, "GLFWwindow", window);
    Gua_HandleToObject(argv[1], (struct Gua_Handle *)h);
    
    /* Pass the other function argument. */
    Gua_IntegerToObject(argv[2], entered);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    /* Call the callback function. */
    if (Gua_GetFunction(globalNameSpace, Gua_ObjectToString(callback), &function) == GUA_OK) {
        status = function.pointer(globalNameSpace, argc, argv, &object, error);
    } else {
        status = GUA_ERROR;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "undefined function", Gua_ObjectToString(callback));
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /* Free the allocated resources. */
    Gua_FreeArguments(argc, argv);
    
    if (!Gua_IsObjectStored(callback)) {
        Gua_FreeObject(&callback);
    }
    
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    /* Show the error message, if any. */
    if (status != GUA_OK) {
        if (!((status == GUA_RETURN) || (status == GUA_EXIT))) {
            printf("\nError: %s", error);
            
            Gua_Free(error);
            
            exit(1);
        }
    }
    
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glfw_GLFWcursorposfun(GLFWwindow *window, double xpos, double ypos)
 *
 * Description:
 *     GLFW cursor position callback.
 *
 * Arguments:
 *     window,    the window that received the event;
 *     xpos,      the new x-coordinate, in screen coordinates, of the cursor;
 *     ypos,      the new y-coordinate, in screen coordinates, of the cursor.
 *
 * Results:
 *     This function is a wrapper to the cursor position callback.
 */
void Glfw_GLFWcursorposfun(GLFWwindow *window, double xpos, double ypos)
{
    Gua_Handle *h;
    Gua_Object callback;
    Gua_Function function;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    Gua_String errMessage;
    
    Gua_ClearObject(callback);
    Gua_ClearObject(object);
    
    /* Get the callback function name. */
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "$GLFW_CURSORPOSCALLBACK[\"%p\"]", window);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &callback, &status, error);
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
    
    /* Prepare the arguments array to be passed to the callback function. */
    argc = 4;
    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
    
    Gua_ClearArguments(argc, argv);
    
    /* The callback function name. */
    Gua_LinkStringToObject(argv[0], Gua_ObjectToString(callback));
    Gua_SetStoredObject(argv[0]);
    
    /* Pass the window handle as the first formal argument. */
    Gua_NewHandle(h, "GLFWwindow", window);
    Gua_HandleToObject(argv[1], (struct Gua_Handle *)h);
    
    /* Pass the other function argument. */
    Gua_RealToObject(argv[2], xpos);
    Gua_RealToObject(argv[3], ypos);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    /* Call the callback function. */
    if (Gua_GetFunction(globalNameSpace, Gua_ObjectToString(callback), &function) == GUA_OK) {
        status = function.pointer(globalNameSpace, argc, argv, &object, error);
    } else {
        status = GUA_ERROR;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "undefined function", Gua_ObjectToString(callback));
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /* Free the allocated resources. */
    Gua_FreeArguments(argc, argv);
    
    if (!Gua_IsObjectStored(callback)) {
        Gua_FreeObject(&callback);
    }
    
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    /* Show the error message, if any. */
    if (status != GUA_OK) {
        if (!((status == GUA_RETURN) || (status == GUA_EXIT))) {
            printf("\nError: %s", error);
            
            Gua_Free(error);
            
            exit(1);
        }
    }
    
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glfw_GLFWdropfun(GLFWwindow *window, int count, const char **names)
 *
 * Description:
 *     GLFW file drop callback.
 *
 * Arguments:
 *     window,    the window that received the event;
 *     count,     the number of dropped files;
 *     names,     the UTF-8 encoded path names of the dropped files.
 *
 * Results:
 *     This function is a wrapper to the file drop callback.
 */
void Glfw_GLFWdropfun(GLFWwindow *window, int count, const char **names)
{
    Gua_Handle *h;
    Gua_Object callback;
    Gua_Function function;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_String expression;
    Gua_String p;
    Gua_Object array;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    Gua_String errMessage;
    
    Gua_ClearObject(callback);
    Gua_ClearObject(object);
    
    /* Get the callback function name. */
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "$GLFW_DROPCALLBACK[\"%p\"]", window);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &callback, &status, error);
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
    
    /* Convert an array of strings to a GuaraScript array object. */
    Gua_ArgvToArray(count, (char **)names, &array);
    
    /* Prepare the arguments array to be passed to the callback function. */
    argc = 4;
    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
    
    Gua_ClearArguments(argc, argv);
    
    /* The callback function name. */
    Gua_LinkStringToObject(argv[0], Gua_ObjectToString(callback));
    Gua_SetStoredObject(argv[0]);
    
    /* Pass the window handle as the first formal argument. */
    Gua_NewHandle(h, "GLFWwindow", window);
    Gua_HandleToObject(argv[1], (struct Gua_Handle *)h);
    
    /* Pass the other function argument. */
    Gua_IntegerToObject(argv[2], count);
    Gua_LinkObjects(argv[3], array);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    /* Call the callback function. */
    if (Gua_GetFunction(globalNameSpace, Gua_ObjectToString(callback), &function) == GUA_OK) {
        status = function.pointer(globalNameSpace, argc, argv, &object, error);
    } else {
        status = GUA_ERROR;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "undefined function", Gua_ObjectToString(callback));
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /* Free the allocated resources. */
    Gua_FreeArguments(argc, argv);
    
    if (!Gua_IsObjectStored(callback)) {
        Gua_FreeObject(&callback);
    }
    
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    /* Show the error message, if any. */
    if (status != GUA_OK) {
        if (!((status == GUA_RETURN) || (status == GUA_EXIT))) {
            printf("\nError: %s", error);
            
            Gua_Free(error);
            
            exit(1);
        }
    }
    
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glfw_GLFWerrorfun(int error, const char *description)
 *
 * Description:
 *     GLFW error callback.
 *
 * Arguments:
 *     error,          an error code;
 *     description,    a UTF-8 encoded string describing the error.
 *
 * Results:
 *     This function is a wrapper to the error callback.
 */
void Glfw_GLFWerrorfun(int error, const char *description)
{
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String errMessage;
    
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "eval(GLFW_ERRORCALLBACK + \"(%d, \"%s\");\")", error, description);
    
    errMessage = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *errMessage = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &object, &status, errMessage);
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    if (status != GUA_OK) {
        printf("\nError: %s", errMessage);
        
        Gua_Free(expression);
        Gua_Free(errMessage);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(errMessage);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     void Glfw_GLFWkeyfun(GLFWwindow *window, int key, int scancode, int action, int mods)
 *
 * Description:
 *     GLFW keyboard key callback.
 *
 * Arguments:
 *     window,      the window that received the event;
 *     key,         the keyboard key that was pressed or released;
 *     scancode,    the system-specific scancode of the key;
 *     action,      GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT;
 *     mods,        bit field describing which modifier keys were held down.
 *
 * Results:
 *     This function is a wrapper to the keyboard key callback.
 */
void Glfw_GLFWkeyfun(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    Gua_Handle *h;
    Gua_Object callback;
    Gua_Function function;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_String expression;
    Gua_String p;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    Gua_String errMessage;
    
    Gua_ClearObject(callback);
    Gua_ClearObject(object);
    
    /* Get the callback function name. */
    expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
    sprintf(expression, "$GLFW_KEYCALLBACK[\"%p\"]", window);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    p = expression;
    p = Gua_Expression(globalNameSpace, p, &callback, &status, error);
    
    if (status != GUA_OK) {
        printf("\nError: %s", error);
        
        Gua_Free(expression);
        Gua_Free(error);
        
        exit(1);
    }
    
    Gua_Free(expression);
    Gua_Free(error);
    
    /* Prepare the arguments array to be passed to the callback function. */
    argc = 6;
    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
    
    Gua_ClearArguments(argc, argv);
    
    /* The callback function name. */
    Gua_LinkStringToObject(argv[0], Gua_ObjectToString(callback));
    Gua_SetStoredObject(argv[0]);
    
    /* Pass the window handle as the first formal argument. */
    Gua_NewHandle(h, "GLFWwindow", window);
    Gua_HandleToObject(argv[1], (struct Gua_Handle *)h);
    
    /* Pass the other function argument. */
    Gua_IntegerToObject(argv[2], key);
    Gua_IntegerToObject(argv[3], scancode);
    Gua_IntegerToObject(argv[4], action);
    Gua_IntegerToObject(argv[5], mods);
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    /* Call the callback function. */
    if (Gua_GetFunction(globalNameSpace, Gua_ObjectToString(callback), &function) == GUA_OK) {
        status = function.pointer(globalNameSpace, argc, argv, &object, error);
    } else {
        status = GUA_ERROR;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "undefined function", Gua_ObjectToString(callback));
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /* Free the allocated resources. */
    Gua_FreeArguments(argc, argv);
    
    if (!Gua_IsObjectStored(callback)) {
        Gua_FreeObject(&callback);
    }
    
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    /* Show the error message, if any. */
    if (status != GUA_OK) {
        if (!((status == GUA_RETURN) || (status == GUA_EXIT))) {
            printf("\nError: %s", error);
            
            Gua_Free(error);
            
            exit(1);
        }
    }
    
    Gua_Free(error);
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Glfw_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Function wrapper.
 *
 * Arguments:
 *     nspace,    a pointer to a structure Gua_Namespace. Must do a cast before use it;
 *     argc,      the number of arguments to pass to the function;
 *     argv,      an array containing the arguments to the function;
 *                argv[0] is the function name;
 *     object,    a structure containing the return object of the function;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     The return object of the wrapped function.
 */
Gua_Status Glfw_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_Handle *h;
    int arg2i;
    int arg3i;
    void *arg1p;
    void *arg4p;
    void *arg5p;
    GLFWwindow *window;
    Gua_String expression;
    Gua_String p;
    Gua_Status status;
    Gua_Object o;
    Gua_String errMessage;
    
    arg2i = 0;
    arg3i = 0;
    
    arg1p = NULL;
    arg4p = NULL;
    arg5p = NULL;
    
    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);

        return GUA_ERROR;
    }

    if (strcmp(Gua_ObjectToString(argv[0]), "glfwCreateCursor") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwCreateStandardCursor") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwCreateWindow") == 0) {
        if (argc < 4) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
		if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[3]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (argc >= 5) {
            h = (Gua_Handle *)Gua_ObjectToHandle(argv[4]);
            
            if (strcmp((Gua_String)Gua_GetHandleType(h), "GLFWmonitor") != 0) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 4 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            
            arg4p = Gua_GetHandlePointer((Gua_Handle *)h);
        }
        
        if (argc == 6) {
            h = (Gua_Handle *)Gua_ObjectToHandle(argv[5]);
            
            if (strcmp((Gua_String)Gua_GetHandleType(h), "GLFWwindow") != 0) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 5 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            
            arg5p = Gua_GetHandlePointer((Gua_Handle *)h);
        }
        
        window = (GLFWwindow *)glfwCreateWindow(Gua_ObjectToInteger(argv[1]), Gua_ObjectToInteger(argv[2]), Gua_ObjectToString(argv[3]), (GLFWmonitor *)arg4p,  (GLFWwindow *)arg5p);
        
        Gua_NewHandle(h, "GLFWwindow", window);
        
        Gua_HandleToPObject(object, (struct Gua_Handle *)h);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwDefaultWindowHints") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwDestroyCursor") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwDestroyWindow") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_HANDLE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "GLFWwindow") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        glfwDestroyWindow((GLFWwindow *)arg1p);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwExtensionSupported") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetClipboardString") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetCurrentContext") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetCursorPos") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetFramebufferSize") == 0) {
        if (argc != 4) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_HANDLE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[3]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "GLFWwindow") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        glfwGetFramebufferSize((GLFWwindow *)arg1p, &arg2i, &arg3i);
        
        Gua_IntegerToObject(o, arg2i);
        Gua_SetStoredObject(o);
        if (Gua_SetVariable((Gua_Namespace *)nspace, Gua_ObjectToString(argv[2]), &o, SCOPE_STACK) != GUA_OK) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "can't set variable", Gua_ObjectToString(argv[2]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
        }
        
        Gua_IntegerToObject(o, arg3i);
        Gua_SetStoredObject(o);
        if (Gua_SetVariable((Gua_Namespace *)nspace, Gua_ObjectToString(argv[3]), &o, SCOPE_STACK) != GUA_OK) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "can't set variable", Gua_ObjectToString(argv[3]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetGammaRamp") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetInputMode") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetJoystickAxes") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetJoystickButtons") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetJoystickName") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetKey") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetMonitorName") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetMonitorPhysicalSize") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetMonitorPos") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetMonitors") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetMouseButton") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetPrimaryMonitor") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetProcAddress") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetTime") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_RealToPObject(object, glfwGetTime());
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetVersion") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetVersionString") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetVideoMode") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetVideoModes") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetWindowAttrib") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetWindowFrameSize") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetWindowMonitor") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetWindowPos") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetWindowSize") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwGetWindowUserPointer") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwHideWindow") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwIconifyWindow") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwInit") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwJoystickPresent") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwMakeContextCurrent") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_HANDLE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "GLFWwindow") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        glfwMakeContextCurrent((GLFWwindow *)arg1p);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwPollEvents") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glfwPollEvents();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwPostEmptyEvent") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwRestoreWindow") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetCharCallback") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_HANDLE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "GLFWwindow") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLFW_CHARCALLBACK[\"%p\"] = \"%s\";", arg1p, Gua_ObjectToString(argv[2]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glfwSetCharCallback((GLFWwindow *)arg1p, (GLFWcharfun)Glfw_GLFWcharfun);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetCharModsCallback") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_HANDLE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "GLFWwindow") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLFW_CHARMODSCALLBACK[\"%p\"] = \"%s\";", arg1p, Gua_ObjectToString(argv[2]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glfwSetCharModsCallback((GLFWwindow *)arg1p, (GLFWcharmodsfun)Glfw_GLFWcharmodsfun);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetClipboardString") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetCursor") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetCursorEnterCallback") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_HANDLE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "GLFWwindow") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLFW_CURSORENTERCALLBACK[\"%p\"] = \"%s\";", arg1p, Gua_ObjectToString(argv[2]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glfwSetCursorEnterCallback((GLFWwindow *)arg1p, (GLFWcursorenterfun)Glfw_GLFWcursorenterfun);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetCursorPos") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetCursorPosCallback") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_HANDLE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "GLFWwindow") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLFW_CURSORPOSCALLBACK[\"%p\"] = \"%s\";", arg1p, Gua_ObjectToString(argv[2]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glfwSetCursorPosCallback((GLFWwindow *)arg1p, (GLFWcursorposfun)Glfw_GLFWcursorposfun);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetDropCallback") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_HANDLE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "GLFWwindow") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLFW_DROPCALLBACK[\"%p\"] = \"%s\";", arg1p, Gua_ObjectToString(argv[2]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glfwSetDropCallback((GLFWwindow *)arg1p, (GLFWdropfun)Glfw_GLFWdropfun);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetErrorCallback") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLFW_ERRORCALLBACK = \"%s\";", Gua_ObjectToString(argv[1]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glfwSetErrorCallback((GLFWerrorfun)Glfw_GLFWerrorfun);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetFramebufferSizeCallback") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetGamma") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetGammaRamp") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetInputMode") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetKeyCallback") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_HANDLE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "GLFWwindow") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        expression = (char *)Gua_Alloc(sizeof(char) * EXPRESSION_SIZE);
        sprintf(expression, "$GLFW_KEYCALLBACK[\"%p\"] = \"%s\";", arg1p, Gua_ObjectToString(argv[2]));
        
        p = expression;
        p = Gua_Expression(nspace, p, object, &status, error);
        if (!Gua_IsPObjectStored(object)) {
            Gua_FreeObject(object);
        }
        
        Gua_Free(expression);
        
        glfwSetKeyCallback((GLFWwindow *)arg1p, (GLFWkeyfun)Glfw_GLFWkeyfun);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetMonitorCallback") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetMouseButtonCallback") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetScrollCallback") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetTime") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetWindowCloseCallback") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetWindowFocusCallback") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetWindowIconifyCallback") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetWindowPos") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetWindowPosCallback") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetWindowRefreshCallback") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetWindowShouldClose") == 0) {
        if (argc != 3) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_HANDLE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "GLFWwindow") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        glfwSetWindowShouldClose((GLFWwindow *)arg1p, Gua_ObjectToInteger(argv[2]));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetWindowSize") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetWindowSizeCallback") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetWindowTitle") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSetWindowUserPointer") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwShowWindow") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSwapBuffers") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_HANDLE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "GLFWwindow") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        glfwSwapBuffers((GLFWwindow *)arg1p);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwSwapInterval") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_INTEGER) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glfwSwapInterval(Gua_ObjectToInteger(argv[1]));
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwTerminate") == 0) {
        if (argc != 1) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        glfwTerminate();
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwWaitEvents") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwWindowHint") == 0) {
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glfwWindowShouldClose") == 0) {
        if (argc != 2) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[1]) != OBJECT_TYPE_HANDLE) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
        
        if (strcmp((Gua_String)Gua_GetHandleType(h), "GLFWwindow") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        arg1p = Gua_GetHandlePointer((Gua_Handle *)h);
        
        Gua_IntegerToPObject(object, glfwWindowShouldClose((GLFWwindow *)arg1p));
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Glfw_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
 *
 * Description:
 *     Install the extension functions.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     argc,      the number of command line arguments;
 *     argv,      the command line arguments;
 *     env,       a pointer to the environment variables;
 *     error,     a pointer to the error message.
 *
 * Results:
 *     Install the extension functions.
 */
Gua_Status Glfw_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_Function function;
    Gua_Object object;
    Gua_String errMessage;
    
    globalNameSpace = nspace;
    
    /* Define the function wrapper to each extension function... */
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwCreateCursor", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwCreateCursor");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwCreateStandardCursor", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwCreateStandardCursor");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwCreateWindow", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwCreateWindow");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwDefaultWindowHints", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwDefaultWindowHints");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwDestroyCursor", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwDestroyCursor");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwDestroyWindow", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwDestroyWindow");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwExtensionSupported", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwExtensionSupported");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetClipboardString", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetClipboardString");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetCurrentContext", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetCurrentContext");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetCursorPos", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetCursorPos");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetFramebufferSize", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetFramebufferSize");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetGammaRamp", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetGammaRamp");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetInputMode", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetInputMode");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetJoystickAxes", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetJoystickAxes");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetJoystickButtons", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetJoystickButtons");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetJoystickName", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetJoystickName");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetKey", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetKey");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetMonitorName", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetMonitorName");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetMonitorPhysicalSize", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetMonitorPhysicalSize");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetMonitorPos", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetMonitorPos");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetMonitors", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetMonitors");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetMouseButton", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetMouseButton");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetPrimaryMonitor", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetPrimaryMonitor");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetProcAddress", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetProcAddress");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetTime", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetTime");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetVersion", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetVersion");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetVersionString", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetVersionString");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetVideoMode", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetVideoMode");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetVideoModes", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetVideoModes");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetWindowAttrib", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetWindowAttrib");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetWindowFrameSize", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetWindowFrameSize");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetWindowMonitor", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetWindowMonitor");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetWindowPos", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetWindowPos");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetWindowSize", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetWindowSize");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwGetWindowUserPointer", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwGetWindowUserPointer");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwHideWindow", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwHideWindow");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwIconifyWindow", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwIconifyWindow");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwInit", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwInit");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwJoystickPresent", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwJoystickPresent");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwMakeContextCurrent", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwMakeContextCurrent");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwPollEvents", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwPollEvents");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwPostEmptyEvent", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwPostEmptyEvent");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwRestoreWindow", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwRestoreWindow");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetCharCallback", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetCharCallback");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetCharModsCallback", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetCharModsCallback");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetClipboardString", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetClipboardString");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetCursor", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetCursor");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetCursorEnterCallback", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetCursorEnterCallback");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetCursorPos", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetCursorPos");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetCursorPosCallback", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetCursorPosCallback");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetDropCallback", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetDropCallback");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetErrorCallback", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetErrorCallback");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetFramebufferSizeCallback", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetFramebufferSizeCallback");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetGamma", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetGamma");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetGammaRamp", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetGammaRamp");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetInputMode", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetInputMode");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetKeyCallback", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetKeyCallback");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetMonitorCallback", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetMonitorCallback");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetMouseButtonCallback", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetMouseButtonCallback");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetScrollCallback", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetScrollCallback");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetTime", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetTime");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetWindowCloseCallback", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetWindowCloseCallback");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetWindowFocusCallback", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetWindowFocusCallback");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetWindowIconifyCallback", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetWindowIconifyCallback");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetWindowPos", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetWindowPos");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetWindowPosCallback", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetWindowPosCallback");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetWindowRefreshCallback", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetWindowRefreshCallback");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetWindowShouldClose", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetWindowShouldClose");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetWindowSize", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetWindowSize");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetWindowSizeCallback", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetWindowSizeCallback");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetWindowTitle", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetWindowTitle");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSetWindowUserPointer", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSetWindowUserPointer");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwShowWindow", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwShowWindow");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSwapBuffers", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSwapBuffers");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwSwapInterval", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwSwapInterval");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwTerminate", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwTerminate");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwWaitEvents", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwWaitEvents");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwWindowHint", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwWindowHint");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glfw_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glfwWindowShouldClose", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glfwWindowShouldClose");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }

    /* Define each extension constant... */
    Gua_IntegerToObject(object, GLFW_ACCUM_ALPHA_BITS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_ACCUM_ALPHA_BITS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_ACCUM_ALPHA_BITS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_ACCUM_BLUE_BITS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_ACCUM_BLUE_BITS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_ACCUM_BLUE_BITS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_ACCUM_GREEN_BITS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_ACCUM_GREEN_BITS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_ACCUM_GREEN_BITS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_ACCUM_RED_BITS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_ACCUM_RED_BITS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_ACCUM_RED_BITS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_ALPHA_BITS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_ALPHA_BITS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_ALPHA_BITS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_ANY_RELEASE_BEHAVIOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_ANY_RELEASE_BEHAVIOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_ANY_RELEASE_BEHAVIOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_API_UNAVAILABLE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_API_UNAVAILABLE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_API_UNAVAILABLE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_ARROW_CURSOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_ARROW_CURSOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_ARROW_CURSOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_AUTO_ICONIFY);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_AUTO_ICONIFY", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_AUTO_ICONIFY");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_AUX_BUFFERS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_AUX_BUFFERS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_AUX_BUFFERS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_BLUE_BITS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_BLUE_BITS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_BLUE_BITS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_CLIENT_API);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_CLIENT_API", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_CLIENT_API");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_CONNECTED);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_CONNECTED", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_CONNECTED");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_CONTEXT_RELEASE_BEHAVIOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_CONTEXT_RELEASE_BEHAVIOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_CONTEXT_RELEASE_BEHAVIOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_CONTEXT_REVISION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_CONTEXT_REVISION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_CONTEXT_REVISION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_CONTEXT_ROBUSTNESS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_CONTEXT_ROBUSTNESS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_CONTEXT_ROBUSTNESS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_CONTEXT_VERSION_MAJOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_CONTEXT_VERSION_MAJOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_CONTEXT_VERSION_MAJOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_CONTEXT_VERSION_MINOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_CONTEXT_VERSION_MINOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_CONTEXT_VERSION_MINOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_CROSSHAIR_CURSOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_CROSSHAIR_CURSOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_CROSSHAIR_CURSOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_CURSOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_CURSOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_CURSOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_CURSOR_DISABLED);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_CURSOR_DISABLED", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_CURSOR_DISABLED");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_CURSOR_HIDDEN);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_CURSOR_HIDDEN", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_CURSOR_HIDDEN");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_CURSOR_NORMAL);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_CURSOR_NORMAL", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_CURSOR_NORMAL");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_DECORATED);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_DECORATED", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_DECORATED");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_DEPTH_BITS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_DEPTH_BITS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_DEPTH_BITS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_DISCONNECTED);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_DISCONNECTED", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_DISCONNECTED");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_DONT_CARE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_DONT_CARE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_DONT_CARE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_DOUBLEBUFFER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_DOUBLEBUFFER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_DOUBLEBUFFER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_FLOATING);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_FLOATING", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_FLOATING");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_FOCUSED);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_FOCUSED", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_FOCUSED");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_FORMAT_UNAVAILABLE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_FORMAT_UNAVAILABLE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_FORMAT_UNAVAILABLE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_GREEN_BITS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_GREEN_BITS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_GREEN_BITS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_HAND_CURSOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_HAND_CURSOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_HAND_CURSOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_HRESIZE_CURSOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_HRESIZE_CURSOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_HRESIZE_CURSOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_IBEAM_CURSOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_IBEAM_CURSOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_IBEAM_CURSOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_ICONIFIED);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_ICONIFIED", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_ICONIFIED");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_INVALID_ENUM);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_INVALID_ENUM", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_INVALID_ENUM");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_INVALID_VALUE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_INVALID_VALUE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_INVALID_VALUE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_JOYSTICK_1);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_JOYSTICK_1", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_JOYSTICK_1");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_JOYSTICK_10);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_JOYSTICK_10", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_JOYSTICK_10");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_JOYSTICK_11);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_JOYSTICK_11", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_JOYSTICK_11");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_JOYSTICK_12);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_JOYSTICK_12", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_JOYSTICK_12");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_JOYSTICK_13);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_JOYSTICK_13", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_JOYSTICK_13");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_JOYSTICK_14);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_JOYSTICK_14", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_JOYSTICK_14");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_JOYSTICK_15);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_JOYSTICK_15", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_JOYSTICK_15");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_JOYSTICK_16);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_JOYSTICK_16", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_JOYSTICK_16");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_JOYSTICK_2);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_JOYSTICK_2", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_JOYSTICK_2");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_JOYSTICK_3);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_JOYSTICK_3", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_JOYSTICK_3");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_JOYSTICK_4);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_JOYSTICK_4", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_JOYSTICK_4");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_JOYSTICK_5);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_JOYSTICK_5", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_JOYSTICK_5");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_JOYSTICK_6);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_JOYSTICK_6", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_JOYSTICK_6");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_JOYSTICK_7);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_JOYSTICK_7", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_JOYSTICK_7");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_JOYSTICK_8);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_JOYSTICK_8", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_JOYSTICK_8");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_JOYSTICK_9);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_JOYSTICK_9", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_JOYSTICK_9");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_JOYSTICK_LAST);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_JOYSTICK_LAST", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_JOYSTICK_LAST");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_0);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_0", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_0");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_1);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_1", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_1");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_2);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_2", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_2");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_3);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_3", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_3");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_4);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_4", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_4");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_5);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_5", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_5");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_6);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_6", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_6");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_7);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_7", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_7");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_8);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_8", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_8");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_9);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_9", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_9");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_A);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_A", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_A");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_APOSTROPHE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_APOSTROPHE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_APOSTROPHE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_B);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_B", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_B");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_BACKSLASH);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_BACKSLASH", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_BACKSLASH");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_BACKSPACE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_BACKSPACE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_BACKSPACE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_C);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_C", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_C");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_CAPS_LOCK);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_CAPS_LOCK", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_CAPS_LOCK");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_COMMA);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_COMMA", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_COMMA");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_D);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_D", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_D");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_DELETE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_DELETE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_DELETE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_DOWN);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_DOWN", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_DOWN");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_E);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_E", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_E");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_END);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_END", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_END");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_ENTER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_ENTER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_ENTER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_EQUAL);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_EQUAL", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_EQUAL");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_ESCAPE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_ESCAPE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_ESCAPE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_F);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_F", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_F");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_F1);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_F1", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_F1");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_F10);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_F10", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_F10");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_F11);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_F11", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_F11");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_F12);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_F12", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_F12");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_F13);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_F13", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_F13");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_F14);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_F14", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_F14");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_F15);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_F15", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_F15");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_F16);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_F16", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_F16");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_F17);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_F17", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_F17");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_F18);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_F18", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_F18");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_F19);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_F19", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_F19");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_F2);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_F2", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_F2");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_F20);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_F20", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_F20");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_F21);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_F21", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_F21");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_F22);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_F22", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_F22");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_F23);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_F23", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_F23");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_F24);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_F24", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_F24");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_F25);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_F25", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_F25");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_F3);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_F3", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_F3");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_F4);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_F4", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_F4");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_F5);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_F5", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_F5");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_F6);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_F6", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_F6");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_F7);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_F7", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_F7");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_F8);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_F8", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_F8");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_F9);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_F9", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_F9");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_G);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_G", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_G");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_GRAVE_ACCENT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_GRAVE_ACCENT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_GRAVE_ACCENT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_H);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_H", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_H");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_HOME);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_HOME", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_HOME");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_I);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_I", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_I");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_INSERT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_INSERT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_INSERT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_J);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_J", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_J");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_K);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_K", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_K");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_KP_0);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_KP_0", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_KP_0");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_KP_1);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_KP_1", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_KP_1");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_KP_2);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_KP_2", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_KP_2");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_KP_3);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_KP_3", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_KP_3");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_KP_4);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_KP_4", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_KP_4");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_KP_5);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_KP_5", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_KP_5");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_KP_6);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_KP_6", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_KP_6");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_KP_7);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_KP_7", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_KP_7");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_KP_8);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_KP_8", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_KP_8");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_KP_9);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_KP_9", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_KP_9");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_KP_ADD);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_KP_ADD", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_KP_ADD");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_KP_DECIMAL);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_KP_DECIMAL", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_KP_DECIMAL");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_KP_DIVIDE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_KP_DIVIDE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_KP_DIVIDE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_KP_ENTER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_KP_ENTER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_KP_ENTER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_KP_EQUAL);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_KP_EQUAL", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_KP_EQUAL");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_KP_MULTIPLY);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_KP_MULTIPLY", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_KP_MULTIPLY");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_KP_SUBTRACT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_KP_SUBTRACT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_KP_SUBTRACT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_L);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_L", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_L");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_LAST);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_LAST", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_LAST");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_LEFT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_LEFT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_LEFT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_LEFT_ALT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_LEFT_ALT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_LEFT_ALT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_LEFT_BRACKET);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_LEFT_BRACKET", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_LEFT_BRACKET");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_LEFT_CONTROL);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_LEFT_CONTROL", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_LEFT_CONTROL");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_LEFT_SHIFT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_LEFT_SHIFT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_LEFT_SHIFT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_LEFT_SUPER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_LEFT_SUPER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_LEFT_SUPER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_M);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_M", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_M");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_MENU);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_MENU", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_MENU");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_MINUS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_MINUS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_MINUS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_N);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_N", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_N");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_NUM_LOCK);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_NUM_LOCK", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_NUM_LOCK");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_O);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_O", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_O");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_P);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_P", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_P");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_PAGE_DOWN);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_PAGE_DOWN", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_PAGE_DOWN");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_PAGE_UP);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_PAGE_UP", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_PAGE_UP");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_PAUSE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_PAUSE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_PAUSE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_PERIOD);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_PERIOD", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_PERIOD");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_PRINT_SCREEN);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_PRINT_SCREEN", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_PRINT_SCREEN");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_Q);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_Q", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_Q");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_R);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_R", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_R");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_RIGHT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_RIGHT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_RIGHT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_RIGHT_ALT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_RIGHT_ALT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_RIGHT_ALT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_RIGHT_BRACKET);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_RIGHT_BRACKET", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_RIGHT_BRACKET");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_RIGHT_CONTROL);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_RIGHT_CONTROL", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_RIGHT_CONTROL");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_RIGHT_SHIFT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_RIGHT_SHIFT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_RIGHT_SHIFT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_RIGHT_SUPER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_RIGHT_SUPER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_RIGHT_SUPER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_S);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_S", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_S");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_SCROLL_LOCK);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_SCROLL_LOCK", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_SCROLL_LOCK");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_SEMICOLON);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_SEMICOLON", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_SEMICOLON");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_SLASH);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_SLASH", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_SLASH");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_SPACE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_SPACE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_SPACE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_T);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_T", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_T");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_TAB);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_TAB", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_TAB");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_U);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_U", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_U");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_UNKNOWN);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_UNKNOWN", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_UNKNOWN");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_UP);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_UP", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_UP");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_V);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_V", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_V");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_W);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_W", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_W");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_WORLD_1);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_WORLD_1", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_WORLD_1");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_WORLD_2);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_WORLD_2", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_WORLD_2");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_X);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_X", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_X");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_Y);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_Y", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_Y");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_KEY_Z);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_KEY_Z", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_KEY_Z");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_LOSE_CONTEXT_ON_RESET);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_LOSE_CONTEXT_ON_RESET", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_LOSE_CONTEXT_ON_RESET");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_MOD_ALT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_MOD_ALT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_MOD_ALT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_MOD_CONTROL);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_MOD_CONTROL", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_MOD_CONTROL");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_MOD_SHIFT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_MOD_SHIFT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_MOD_SHIFT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_MOD_SUPER);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_MOD_SUPER", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_MOD_SUPER");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_MOUSE_BUTTON_1);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_MOUSE_BUTTON_1", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_MOUSE_BUTTON_1");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_MOUSE_BUTTON_2);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_MOUSE_BUTTON_2", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_MOUSE_BUTTON_2");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_MOUSE_BUTTON_3);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_MOUSE_BUTTON_3", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_MOUSE_BUTTON_3");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_MOUSE_BUTTON_4);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_MOUSE_BUTTON_4", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_MOUSE_BUTTON_4");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_MOUSE_BUTTON_5);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_MOUSE_BUTTON_5", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_MOUSE_BUTTON_5");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_MOUSE_BUTTON_6);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_MOUSE_BUTTON_6", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_MOUSE_BUTTON_6");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_MOUSE_BUTTON_7);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_MOUSE_BUTTON_7", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_MOUSE_BUTTON_7");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_MOUSE_BUTTON_8);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_MOUSE_BUTTON_8", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_MOUSE_BUTTON_8");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_MOUSE_BUTTON_LAST);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_MOUSE_BUTTON_LAST", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_MOUSE_BUTTON_LAST");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_MOUSE_BUTTON_LEFT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_MOUSE_BUTTON_LEFT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_MOUSE_BUTTON_LEFT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_MOUSE_BUTTON_MIDDLE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_MOUSE_BUTTON_MIDDLE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_MOUSE_BUTTON_MIDDLE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_MOUSE_BUTTON_RIGHT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_MOUSE_BUTTON_RIGHT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_MOUSE_BUTTON_RIGHT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_NO_CURRENT_CONTEXT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_NO_CURRENT_CONTEXT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_NO_CURRENT_CONTEXT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_NO_RESET_NOTIFICATION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_NO_RESET_NOTIFICATION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_NO_RESET_NOTIFICATION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_NO_ROBUSTNESS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_NO_ROBUSTNESS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_NO_ROBUSTNESS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_NOT_INITIALIZED);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_NOT_INITIALIZED", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_NOT_INITIALIZED");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_OPENGL_ANY_PROFILE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_OPENGL_ANY_PROFILE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_OPENGL_ANY_PROFILE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_OPENGL_API);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_OPENGL_API", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_OPENGL_API");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_OPENGL_COMPAT_PROFILE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_OPENGL_COMPAT_PROFILE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_OPENGL_COMPAT_PROFILE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_OPENGL_CORE_PROFILE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_OPENGL_CORE_PROFILE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_OPENGL_CORE_PROFILE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_OPENGL_DEBUG_CONTEXT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_OPENGL_DEBUG_CONTEXT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_OPENGL_DEBUG_CONTEXT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_OPENGL_ES_API);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_OPENGL_ES_API", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_OPENGL_ES_API");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_OPENGL_FORWARD_COMPAT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_OPENGL_FORWARD_COMPAT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_OPENGL_FORWARD_COMPAT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_OPENGL_PROFILE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_OPENGL_PROFILE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_OPENGL_PROFILE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_OUT_OF_MEMORY);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_OUT_OF_MEMORY", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_OUT_OF_MEMORY");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_PLATFORM_ERROR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_PLATFORM_ERROR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_PLATFORM_ERROR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_PRESS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_PRESS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_PRESS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_RED_BITS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_RED_BITS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_RED_BITS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_REFRESH_RATE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_REFRESH_RATE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_REFRESH_RATE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_RELEASE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_RELEASE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_RELEASE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_RELEASE_BEHAVIOR_FLUSH);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_RELEASE_BEHAVIOR_FLUSH", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_RELEASE_BEHAVIOR_FLUSH");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_RELEASE_BEHAVIOR_NONE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_RELEASE_BEHAVIOR_NONE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_RELEASE_BEHAVIOR_NONE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_REPEAT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_REPEAT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_REPEAT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_RESIZABLE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_RESIZABLE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_RESIZABLE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_SAMPLES);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_SAMPLES", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_SAMPLES");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_SRGB_CAPABLE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_SRGB_CAPABLE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_SRGB_CAPABLE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_STENCIL_BITS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_STENCIL_BITS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_STENCIL_BITS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_STEREO);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_STEREO", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_STEREO");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_STICKY_KEYS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_STICKY_KEYS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_STICKY_KEYS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_STICKY_MOUSE_BUTTONS);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_STICKY_MOUSE_BUTTONS", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_STICKY_MOUSE_BUTTONS");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_VERSION_MAJOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_VERSION_MAJOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_VERSION_MAJOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_VERSION_MINOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_VERSION_MINOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_VERSION_MINOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_VERSION_REVISION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_VERSION_REVISION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_VERSION_REVISION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_VERSION_UNAVAILABLE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_VERSION_UNAVAILABLE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_VERSION_UNAVAILABLE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_VISIBLE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_VISIBLE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_VISIBLE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLFW_VRESIZE_CURSOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLFW_VRESIZE_CURSOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLFW_VRESIZE_CURSOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     GUA_GLFW_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, GUA_GLFW_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GUA_GLFW_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GUA_GLFW_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /* Call the GLFW Initializer. */
    if (!glfwInit()) {
        return GUA_ERROR;
    }
    
    return GUA_OK;
}

