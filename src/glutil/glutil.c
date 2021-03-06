/**
 * File:
 *     glutil.c
 *
 * Package:
 *    Glutil
 *
 * Description:
 *     This library implements an OpenGL Utility library wrapper
 *     for the GuaraScript interpreter.
 *
 * Copyright:
 *     Copyright (C) 2013 Roberto Luiz Souza Monteiro,
 *                        Hernane Borges de Barros Pereira,
 *                        Marcelo A. Moret.
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
 * RCS: @(#) $Id: glutil.c,v 1.1 2014/08/06 11:08:00 monteiro Exp $
 * 
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "glutil.h"
#include "glm.h"

/* The PPM specification define the max line width as 70 characters. */
#define MAX_LINE_WIDTH 70

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_String Glutil_LoadPPM(Gua_String file, Gua_Short *width, Gua_Short *height)
 *
 * Description:
 *     Reads a PPM file.
 *
 * Arguments:
 *     file,      file name;
 *     width,     image width;
 *     height,    image height.
 *
 * Retults:
 *     Returns a PPM file content.
 */
Gua_String Glutil_LoadPPM(Gua_String file, Gua_Short *width, Gua_Short *height)
{
    FILE *fp;
    Gua_Char buffer[MAX_LINE_WIDTH + 1];
    Gua_String image;
    Gua_Short w;
    Gua_Short h;
    Gua_Short m;
    Gua_Short n;
    Gua_Short status;
    
    image = NULL;
    
    w = 0;
    h = 0;
    m = 0;
    n = 0;
    
    status = GUA_OK;
    
    fp = fopen(file, "r");
    
    if (fp == NULL) {
        status = GUA_ERROR;
    } else {
        fgets(buffer, sizeof(buffer), fp);
        
        if (strncmp(buffer, "P6", 2) != 0) {
            status = GUA_ERROR;
        } else {
            while(!feof(fp)) {
                fgets(buffer, sizeof(buffer), fp);
                
                /* Comment. */
                if (buffer[0] == '#') {
                    continue;
                }
                
                /* Reads width, height and max color value. */
                if (n == 0) {
                    n = n + sscanf(buffer, "%d %d %d", &w, &h, &m);
                } else if (n == 1) {
                    n = n + sscanf(buffer, "%d %d", &h, &m);
                } else if (n == 2) {
                    n = n + sscanf(buffer, "%d", &m);
                }
                
                if (n == 3) {
                    break;
                }
            }
            
            /* Reads the raw image data. */
            if (n == 3) {
                image = (char *)Gua_Alloc(sizeof(char) * w * h * 3 + 1);
                memset(image, '\0', w * h * 3 + 1);
                
                fread(image, sizeof(char), w * h * 3, fp);
            }
            
            fclose(fp);
        }
    }
    
    *width = w;
    *height = h;
    
    if (status == GUA_OK) {
        return image;
    }
    
    return NULL;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Glutil_SavePPM(Gua_String file, Gua_String image, Gua_Short width, Gua_Short height)
 *
 * Description:
 *     Saves a PPM file.
 *
 * Arguments:
 *     file,      file name;
 *     width,     image width;
 *     height,    image height.
 *
 * Retults:
 *     Saves a PPM file.
 */
Gua_Status Glutil_SavePPM(Gua_String file, Gua_String image, Gua_Short width, Gua_Short height)
{
    FILE *fp;
    Gua_Short n;
    Gua_Short status;
    
    status = GUA_OK;
    
    /*
     * Creates the PPM image file.
     */
    fp = fopen(file, "w");
    
    if (fp != NULL) {
        /* Writes the PPM file header. */
        fputs("P6\n", fp);
        fprintf(fp, "%d %d\n", width, height);
        fputs("255\n", fp);
        
        /* Writes the raw image data. */
        n = fwrite(image, sizeof(char), width * height * 3, fp);
        
        if (n != width * height * 3) {
            status = GUA_ERROR;
        }
        
        fclose(fp);
    } else {
        status = GUA_ERROR;
    }
    
    return status;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Glutil_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
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
Gua_Status Glutil_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    GLint arg2i;
    GLint arg3i;
    GLfloat arg3f;
    GLMmodel *model;
    GLuint model_list;
    Gua_Object o;
    FILE *fp;
    Gua_String errMessage;
    
    arg2i = 0;
    arg3f = 0.0;
    model = NULL;
    model_list = 0;
    
    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);

        return GUA_ERROR;
    }

    if (strcmp(Gua_ObjectToString(argv[0]), "glutilLoadObj") == 0) {
        if (argc != 4) {
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
        if (!((Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_INTEGER) {
            arg2i = Gua_ObjectToInteger(argv[2]);
        } else if (Gua_ObjectType(argv[2]) == OBJECT_TYPE_REAL) {
            arg2i = roundl(Gua_ObjectToReal(argv[2]));            
        }
        if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) {
            arg3f = Gua_ObjectToInteger(argv[3]);
        } else if (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL) {
            arg3f = Gua_ObjectToReal(argv[3]);            
        }
        
        /*
         * Reads the model data, defines its vertices normals
         * and create an OpenGL list to reference the object.
         */
        model = glmReadOBJ(Gua_ObjectToString(argv[1]));
        glmUnitize(model);
        glmFacetNormals(model);
        glmVertexNormals(model, arg3f);
        model_list = glmList(model, arg2i);
        
        Gua_IntegerToPObject(object, model_list);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutilLoadPPM") == 0) {
        if (argc != 4) {
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
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (Gua_ObjectType(argv[3]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_LinkByteArrayToPObject(object, (Gua_String)Glutil_LoadPPM(Gua_ObjectToString(argv[1]), &arg2i, &arg3i), arg2i * arg3i * 3);
        
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
    } else if (strcmp(Gua_ObjectToString(argv[0]), "glutilSavePPM") == 0) {
        if (argc != 5) {
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
        if (Gua_ObjectType(argv[2]) != OBJECT_TYPE_STRING) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 2 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[3]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[3]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 3 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (!((Gua_ObjectType(argv[4]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[4]) == OBJECT_TYPE_REAL))) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 4 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        /*
         * Open the PPM image file.
         */
        fp = fopen(Gua_ObjectToString(argv[2]), "w+");
        
        if (fp != NULL) {
            /*
             * Writes the PPM file header.
             */
            fputs("P6\n", fp);
            fprintf(fp, "%ld %ld\n", Gua_ObjectToInteger(argv[3]), Gua_ObjectToInteger(argv[4]));
            fputs("255\n", fp);
            
            /*
             * Writes the raw image data.
             */
            fwrite(Gua_ObjectToString(argv[1]), sizeof(char), Gua_ObjectToInteger(argv[3]) * Gua_ObjectToInteger(argv[4]) * 3, fp);
            
            fclose(fp);
        }
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Glutil_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
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
Gua_Status Glutil_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_Function function;
    Gua_Object object;
    Gua_String errMessage;

    /* Define the function wrapper to each extension function... */
    Gua_LinkCFunctionToFunction(function, Glutil_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutilLoadObj", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutilLoadObj");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glutil_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutilLoadPPM", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutilLoadPPM");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Glutil_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "glutilSavePPM", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "glutilSavePPM");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /* Define each extension constant... */
    Gua_IntegerToObject(object, GLM_COLOR);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUTIL_COLOR", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUTIL_COLOR");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLM_FLAT);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUTIL_FLAT", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUTIL_FLAT");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLM_MATERIAL);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUTIL_MATERIAL", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUTIL_MATERIAL");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLM_NONE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUTIL_NONE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUTIL_NONE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLM_SMOOTH);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUTIL_SMOOTH", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUTIL_SMOOTH");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_IntegerToObject(object, GLM_TEXTURE);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GLUTIL_TEXTURE", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GLUTIL_TEXTURE");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     GUA_GLUTIL_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, GUA_GLUTIL_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GUA_GLUTIL_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GUA_GLUTIL_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    return GUA_OK;
}

