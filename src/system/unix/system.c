/**
 * File:
 *     system.c
 *
 * Package:
 *     System
 *
 * Description:
 *     This library implements the system functions for an expression interpreter.
 *
 * Copyright:
 *     Copyright (C) 2005 Roberto Luiz Souza Monteiro;
 *     Copyright (C) 2013, 2015 Roberto Luiz Souza Monteiro,
 *                              Hernane Borges de Barros Pereira,
 *                              Marcelo A. Moret.
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
 * RCS: @(#) $Id: system.c,v 2.4 2015/10/10 15:14:00 monteiro Exp $
 * 
 */

#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "system.h"

#define EXPRESSION_SIZE  4096
#define BUFFER_SIZE      65536

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status System_PrintFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Print function wrapper.
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
 *     Print a object on stdout.
 */
Gua_Status System_PrintFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_Object objectAsString;
    Gua_String errMessage;
        
    Gua_ClearObject(objectAsString);
    
    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    if (argc > 2) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_STRING) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_ARRAY) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_MATRIX) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_FILE))) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    Gua_StringToPObject(object, " ");

    if (argc == 2) {
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            printf("%ld", Gua_ObjectToInteger(argv[1]));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            if (fabs(Gua_ObjectToReal(argv[1])) >= ZERO) {
                printf("%g", Gua_ObjectToReal(argv[1]));
            } else {
                printf("%g", 0.0);
            }
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX) {
            if (fabs(Gua_ObjectToReal(argv[1])) >= ZERO) {
                printf("%g", Gua_ObjectToReal(argv[1]));
            } else {
                printf("%g", 0.0);
            }
            if (fabs(Gua_ObjectToImaginary(argv[1])) >= ZERO) {
                printf("%+g*i", Gua_ObjectToImaginary(argv[1]));
            } else {
                printf("%+g*i", 0.0);
            }
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_STRING) {
            printf("%s", Gua_ObjectToString(argv[1]));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_ARRAY) {
            Gua_ArrayToString(&argv[1], &objectAsString);
            printf("%s", Gua_ObjectToString(objectAsString));
            Gua_FreeObject(&objectAsString);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_MATRIX) {
            Gua_MatrixToString(&argv[1], &objectAsString);
            printf("%s", Gua_ObjectToString(objectAsString));
            Gua_FreeObject(&objectAsString);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_FILE) {
            printf("File%lu\n", (Gua_Integer)Gua_GetFileHandle((Gua_File *)Gua_ObjectToFile(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_HANDLE) {
            printf("%s%lu\n", (Gua_String)Gua_GetHandleType((Gua_Handle *)Gua_ObjectToHandle(argv[1])), (Gua_Integer)Gua_GetHandlePointer((Gua_Handle *)Gua_ObjectToHandle(argv[1])));
        }
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status System_PrintlnFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Println function wrapper.
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
 *     Print a object on stdout and the line feed character.
 */
Gua_Status System_PrintlnFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_Object objectAsString;
    Gua_String errMessage;
    
    Gua_ClearObject(objectAsString);
    
    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    if (argc > 2) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "wrong number of arguments for function", Gua_ObjectToString(argv[0]));
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    if (!((Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_STRING) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_ARRAY) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_MATRIX) || (Gua_ObjectType(argv[1]) == OBJECT_TYPE_FILE))) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    Gua_LinkStringToPObject(object, " ");
    Gua_SetStoredPObject(object);
    
    if (argc == 2) {
        if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_INTEGER) {
            printf("%ld\n", Gua_ObjectToInteger(argv[1]));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_REAL) {
            if (fabs(Gua_ObjectToReal(argv[1])) >= ZERO) {
                printf("%g\n", Gua_ObjectToReal(argv[1]));
            } else {
                printf("%g\n", 0.0);
            }
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_COMPLEX) {
            if (fabs(Gua_ObjectToReal(argv[1])) >= ZERO) {
                printf("%g", Gua_ObjectToReal(argv[1]));
            } else {
                printf("%g", 0.0);
            }
            if (fabs(Gua_ObjectToImaginary(argv[1])) >= ZERO) {
                printf("%+g*i\n", Gua_ObjectToImaginary(argv[1]));
            } else {
                printf("%+g*i\n", 0.0);
            }
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_STRING) {
            printf("%s\n", Gua_ObjectToString(argv[1]));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_ARRAY) {
            Gua_ArrayToString(&argv[1], &objectAsString);
            printf("%s\n", Gua_ObjectToString(objectAsString));
            Gua_FreeObject(&objectAsString);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_MATRIX) {
            Gua_MatrixToString(&argv[1], &objectAsString);
            printf("%s\n", Gua_ObjectToString(objectAsString));
            Gua_FreeObject(&objectAsString);
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_FILE) {
            printf("File%lu\n", (Gua_Integer)Gua_GetFileHandle((Gua_File *)Gua_ObjectToFile(argv[1])));
        } else if (Gua_ObjectType(argv[1]) == OBJECT_TYPE_HANDLE) {
            printf("%s%lu\n", (Gua_String)Gua_GetHandleType((Gua_Handle *)Gua_ObjectToHandle(argv[1])), (Gua_Integer)Gua_GetHandlePointer((Gua_Handle *)Gua_ObjectToHandle(argv[1])));
        }
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status System_SourceFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Source function wrapper.
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
 *     Read a script file and try to execute it.
 */
Gua_Status System_SourceFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    FILE *fp;
    Gua_String script;
    Gua_String p;
    Gua_Integer length;
    Gua_Short status;
    Gua_String errMessage;
    
    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
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
    
    fp = fopen(Gua_ObjectToString(argv[1]), "r");
    
    if (fp == NULL) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can not open file", Gua_ObjectToString(argv[1]));
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
    fseek(fp, 0, SEEK_END);
    length = ftell(fp) + 1;
    fseek(fp, 0, SEEK_SET);
    
    script = (char *)Gua_Alloc(sizeof(char) * length + 1);
    memset(script, '\0', length + 1);
    
    status = GUA_OK;
    
    if (length > 0) {
        fread(script, sizeof(char), length, fp);
        p = script;
        p = Gua_Evaluate(nspace, p, object, &status, error);
    }
    
    fclose(fp);
    
    Gua_Free(script);
    
    if (status != GUA_OK) {
        return GUA_ERROR;
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status System_ExecFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
 *
 * Description:
 *     Exec function wrapper.
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
 *     Execute a system command.
 */
Gua_Status System_ExecFunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_String errMessage;
    
    Gua_ClearPObject(object);

    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);
        
        return GUA_ERROR;
    }
    
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
    
    Gua_IntegerToPObject(object, system(Gua_ObjectToString(argv[1])));
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status System_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
 *
 * Description:
 *     Install the system functions.
 *
 * Arguments:
 *     nspace,    a pointer to a structure containing the variable and function namespace;
 *     argc,      the number of command line arguments;
 *     argv,      the command line arguments;
 *     env,       a pointer to the environment variables;
 *     error,     a pointer to the error message;
 *
 * Results:
 *     Install the system functions.
 */
Gua_Status System_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_Function function;
    Gua_Object object;
    Gua_String errMessage;
    
    Gua_LinkCFunctionToFunction(function, System_ExecFunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "exec", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "exec");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, System_PrintFunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "print", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "print");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, System_PrintlnFunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "println", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "println");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, System_SourceFunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "source", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "source");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    Gua_LinkStringToObject(object, SYSTEM_HOST);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "SYS_HOST", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "SYS_HOST");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkStringToObject(object, SYSTEM_ARCH);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "SYS_ARCH", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "SYS_ARCH");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     SYSTEM_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, SYSTEM_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "SYS_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "SYS_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    return GUA_OK;
}

