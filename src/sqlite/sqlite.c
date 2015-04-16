/**
 * File:
 *     sqlite.c
 *
 * Package:
 *     Sqlite
 *
 * Description:
 *     This library implements the SQLite functions wrapper.
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
 * RCS: @(#) $Id: sqlite.c,v 1.1 2014/07/22 14:27:00 monteiro Exp $
 * 
 */

#include <stdlib.h>
#include <string.h>
#include "interp.h"
#include "sqlite.h"
#include "sqlite3.h"

#define ERROR_SIZE  65536

Gua_Namespace *globalNameSpace;

/**
 * Group:
 *     C
 *
 * Function:
 *     int Sqlite_ExecCallback(void *callback, int n, char **key, char **value)
 *
 * Description:
 *     SQLite exec function callback.
 *
 * Arguments:
 *     callback,    the GuaraScript function callback name;
 *     n,           the number of columns;
 *     key,         an array containing the columns names;
 *     value,       an array containing the columns values.
 *
 * Results:
 *     This function is a wrapper to the keyboard callback.
 */
int Sqlite_ExecCallback(void *callback, int n, char **value, char **key)
{
    Gua_Function function;
    Gua_Short argc;
    Gua_Object *argv;
    Gua_Object array;
    Gua_Object object;
    Gua_Status status;
    Gua_String error;
    Gua_Short returnCode;
    Gua_String errMessage;
    
    Gua_ClearObject(array);
    Gua_ClearObject(object);
    
    returnCode = 0;
    
    error = (Gua_String)Gua_Alloc(sizeof(char) * ERROR_SIZE);
    *error = '\0';
    
    /* Convert SQLite row data to a GuaraScript array. */
    Gua_KeyValuePairsToArray(n, key, value, &array);
    
    /* Prepare the arguments array to be passed to the callback function. */
    argc = 2;
    argv = (Gua_Object *)Gua_Alloc(sizeof(Gua_Object) * argc);
    
    Gua_ClearArguments(argc, argv);
    
    Gua_LinkStringToObject(argv[0], (Gua_String)callback);
    Gua_SetStoredObject(argv[0]);
    
    Gua_LinkObjects(argv[1], array);
    
    /* Call the callback function. */
    if (Gua_GetFunction(globalNameSpace, (Gua_String)callback, &function) == GUA_OK) {
        status = function.pointer(globalNameSpace, argc, argv, &object, error);
    } else {
        status = GUA_ERROR;
        
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "undefined function", (Gua_String)callback);
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /* Get the return code. */
    if (Gua_ObjectType(object) == OBJECT_TYPE_INTEGER) {
        returnCode = Gua_ObjectToInteger(object);
    }
    
    Gua_FreeArguments(argc, argv);
    
    if (!Gua_IsObjectStored(object)) {
        Gua_FreeObject(&object);
    }
    
    /* Show the error message, if any. */
    if (status != GUA_OK) {
        if (!((status == GUA_RETURN) || (status == GUA_EXIT))) {
            printf("\nError: %s", error);
            
            Gua_Free(error);
            
            return 1;
        }
    }
    
    Gua_Free(error);
    
    return returnCode;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Sqlite_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
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
Gua_Status Sqlite_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_Handle *h;
    sqlite3 *db;
    Gua_Short returnCode;
    Gua_String errMessage;
    
    globalNameSpace = nspace;
    
    Gua_ClearPObject(object);
    
    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);

        return GUA_ERROR;
    }

    if (strcmp(Gua_ObjectToString(argv[0]), "sqliteClose") == 0) {
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
	
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Sqlite") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        db = (sqlite3 *)Gua_GetHandlePointer((Gua_Handle *)h);
        
        if (sqlite3_close(db) != SQLITE_OK) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "could not close database");
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_SetHandlePointer((Gua_Handle *)h, NULL);
    } else if (strcmp(Gua_ObjectToString(argv[0]), "sqliteExecute") == 0) {
        if ((argc < 3) || (argc > 4)) {
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
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if (argc == 4) {
            if (Gua_ObjectType(argv[3]) != OBJECT_TYPE_STRING) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
        }
        
        h = (Gua_Handle *)Gua_ObjectToHandle(argv[1]);
	
        if (strcmp((Gua_String)Gua_GetHandleType(h), "Sqlite") != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "illegal argument 1 for function", Gua_ObjectToString(argv[0]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        db = (sqlite3 *)Gua_GetHandlePointer((Gua_Handle *)h);
        
        if (argc == 3) {
            returnCode = sqlite3_exec(db, Gua_ObjectToString(argv[2]), Sqlite_ExecCallback, NULL, &errMessage);
        } else {
            returnCode = sqlite3_exec(db, Gua_ObjectToString(argv[2]), Sqlite_ExecCallback, Gua_ObjectToString(argv[3]), &errMessage);
        }
        
        if (returnCode != SQLITE_OK) {
            strcat(error, errMessage);
            sqlite3_free(errMessage);
            
            if (returnCode != SQLITE_ABORT) {
                return GUA_ERROR;
            }
        }
    } else if (strcmp(Gua_ObjectToString(argv[0]), "sqliteOpen") == 0) {
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
        
        if (sqlite3_open(Gua_ObjectToString(argv[1]), &db) != SQLITE_OK) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "could not open or create database", Gua_ObjectToString(argv[1]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        Gua_NewHandle(h, "Sqlite", db);
        
        Gua_HandleToPObject(object, (struct Gua_Handle *)h);
    }
    
    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Sqlite_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
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
Gua_Status Sqlite_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_Function function;
    Gua_Object object;
    Gua_String errMessage;
    
    /* Define the function wrapper to each extension function... */
    Gua_LinkCFunctionToFunction(function, Sqlite_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "sqliteClose", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "sqliteClose");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Sqlite_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "sqliteExecute", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "sqliteExecute");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Sqlite_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "sqliteOpen", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "sqliteOpen");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     GUA_SQLITE_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, GUA_SQLITE_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "GUA_SQLITE_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "GUA_SQLITE_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    return GUA_OK;
}

