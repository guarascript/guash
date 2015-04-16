/**
 * File:
 *     fs.c
 *
 * Package:
 *     Fs
 *
 * Description:
 *     This library implements the file system access functions.
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
 * RCS: @(#) $Id: fs.c,v 1.2 2015/03/11 15:02:00 monteiro Exp $
 * 
 */

#include <dirent.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#ifdef _WINDOWS_
    #include <io.h>
    #define F_OK 0
    #define W_OK 2
    #define R_OK 4
    #define access(p,m) _access((p),(m))
#else
    #include <unistd.h>
#endif
#include "interp.h"
#include "fs.h"

#ifdef _WINDOWS_
    #include <windows.h>
    #define realpath(f,p) _fullpath((p),(f),_MAX_PATH)
#endif

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Fs_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
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
Gua_Status Fs_FunctionWrapper(void *nspace, Gua_Short argc, Gua_Object *argv, Gua_Object *object, Gua_String error)
{
    Gua_Short i;
    Gua_Short j;
    Gua_Short k;
    Gua_Short patternFound;
    Gua_String realPath;
    Gua_String unixPath;
    Gua_String fileName;
    Gua_String linkName;
    Gua_Short fileMode;
    struct stat fileStat;
    FILE *fs;
    FILE *ft;
    Gua_Char c;
    DIR *dirPointer;
    struct dirent *dirEntry;
    Gua_Element *previous;
    Gua_Element *newElement;
    Gua_String newString;
    Gua_String errMessage;
    
    Gua_ClearPObject(object);
    
    if (argc == 0) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s\n", "no function specified");
        strcat(error, errMessage);
        Gua_Free(errMessage);

        return GUA_ERROR;
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsCopy(source, target)
     *
     * Description:
     *     Copy (binary) source file to target file.
     */
    if (strcmp(Gua_ObjectToString(argv[0]), "fsCopy") == 0) {
        if (argc != 3) {
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
        
        /* Open input and output files in binary mode for Windows compatibility. */
        if ((fs = fopen(Gua_ObjectToString(argv[1]), "rb")) == NULL) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "could not open file", Gua_ObjectToString(argv[1]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        if ((ft = fopen(Gua_ObjectToString(argv[2]), "wb")) == NULL) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "could not open file", Gua_ObjectToString(argv[2]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
        
        while (!feof(fs)) {
            fread(&c, sizeof(Gua_Char), 1, fs);
            fwrite(&c, sizeof(Gua_Char), 1, ft);
        }
        
        fclose(fs);
        fclose(ft);
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsDelete(file)
     *
     * Description:
     *     Delete the specified file.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsDelete") == 0) {
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
        
        if (remove(Gua_ObjectToString(argv[1])) != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "can not remove file", Gua_ObjectToString(argv[1]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsExists(file)
     *
     * Description:
     *     Return TRUE if the specified file exists.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsExists") == 0) {
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
        
        if (Gua_ObjectLength(argv[1]) > 0) {
            fileName = Gua_ObjectToString(argv[1]);
            
            fileMode = F_OK;
            
            if (access(fileName, fileMode) == -1) {
                Gua_IntegerToPObject(object, 0);
            } else {
                Gua_IntegerToPObject(object, 1);
            }
        }
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsExtension(file)
     *
     * Description:
     *     Return the file name extension.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsExtension") == 0) {
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
        
        if (Gua_ObjectLength(argv[1]) > 0) {
            fileName = Gua_ObjectToString(argv[1]);
            
            /* Find the extension separator. */
            patternFound = 0;
            for (i = strlen(fileName) - 1; i > 0; i--) {
                if ((fileName[i] == '\\') || (fileName[i] == '/')) {
                    break;
                }
                if (fileName[i] == '.') {
                    patternFound = 1;
                    break;
                }
            }
            
            if (patternFound) {
                /* Skip the extension separator. */
                if (fileName[i] == '.') {
                    i++;
                }
                
                /* Get the file name. */
                newString = (Gua_String)malloc(sizeof(Gua_Char) * strlen(fileName) + 1);
                k = 0;
                for (j = i; j < strlen(fileName); j++) {
                    newString[k] = fileName[j];
                    k++;
                }
                newString[k] = '\0';
            } else {
                newString = (Gua_String)malloc(sizeof(Gua_Char));
                newString[0] = '\0';
            }
        } else {
            newString = (Gua_String)malloc(sizeof(Gua_Char));
            newString[0] = '\0';
        }
        
        Gua_StringToPObject(object, newString);
        
        Gua_Free(newString);
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsFullPath(file)
     *
     * Description:
     *     Return the full path for the specified file name.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsFullPath") == 0) {
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
        
        realPath = realpath(Gua_ObjectToString(argv[1]), NULL);
        
        /* Convert Windows path to Unix path. */
        if (realPath != NULL) {
            unixPath = (Gua_String)malloc(sizeof(Gua_Char) * strlen(realPath) + 1);
            for (i = 0; i < strlen(realPath); i++) {
                unixPath[i] = realPath[i] == '\\' ? '/' : realPath[i];
            }
            unixPath[i] = '\0';
            
            Gua_Free(realPath);
            
            Gua_LinkStringToPObject(object, unixPath);
        }
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsIsDirectory(file)
     *
     * Description:
     *     Return TRUE if the specified file is a directory.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsIsDirectory") == 0) {
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
        
        if (Gua_ObjectLength(argv[1]) > 0) {
            fileName = Gua_ObjectToString(argv[1]);
            
            if (stat(fileName, &fileStat) == -1) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "could not stat file ", fileName);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            
            if (S_ISDIR(fileStat.st_mode)) {
                Gua_IntegerToPObject(object, 1);
            } else {
                Gua_IntegerToPObject(object, 0);
            }
        }
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsIsExecutable(file)
     *
     * Description:
     *     Return TRUE if the specified file is a program.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsIsExecutable") == 0) {
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
        
        if (Gua_ObjectLength(argv[1]) > 0) {
            fileName = Gua_ObjectToString(argv[1]);
#ifdef _WINDOWS_
            if ((strstr(fileName, ".bat") != NULL) || (strstr(fileName, ".com") != NULL) || (strstr(fileName, ".exe") != NULL)) {
                Gua_IntegerToPObject(object, 1);
            } else {
                Gua_IntegerToPObject(object, 0);
            }
#else
            fileMode = X_OK;
            
            if (access(fileName, fileMode) == -1) {
                Gua_IntegerToPObject(object, 0);
            } else {
                Gua_IntegerToPObject(object, 1);
            }
#endif
        }
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsIsReadable(file)
     *
     * Description:
     *     Return TRUE if the specified file is readable.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsIsReadable") == 0) {
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
        
        if (Gua_ObjectLength(argv[1]) > 0) {
            fileName = Gua_ObjectToString(argv[1]);
            
            fileMode = R_OK;
            
            if (access(fileName, fileMode) == -1) {
                Gua_IntegerToPObject(object, 0);
            } else {
                Gua_IntegerToPObject(object, 1);
            }
        }
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsIsWritable(file)
     *
     * Description:
     *     Return TRUE if the specified file is writable.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsIsWritable") == 0) {
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
        
        if (Gua_ObjectLength(argv[1]) > 0) {
            fileName = Gua_ObjectToString(argv[1]);
            
            fileMode = W_OK;
            
            if (access(fileName, fileMode) == -1) {
                Gua_IntegerToPObject(object, 0);
            } else {
                Gua_IntegerToPObject(object, 1);
            }
        }
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsLink(source, target)
     *
     * Description:
     *     Create a symbolic link from source to target.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsLink") == 0) {
        if (argc != 3) {
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
        
        if ((Gua_ObjectLength(argv[1]) > 0) && (Gua_ObjectLength(argv[2]) > 0)) {
            fileName = Gua_ObjectToString(argv[1]);
            linkName = Gua_ObjectToString(argv[2]);
            
#ifdef _WINDOWS_
            Gua_IntegerToPObject(object, 0);
#else
            if (symlink(fileName, linkName) == -1) {
                Gua_IntegerToPObject(object, 0);
            } else {
                Gua_IntegerToPObject(object, 1);
            }
#endif
        }
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsList(path)
     *
     * Description:
     *     Return the list of files in the specified directory.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsList") == 0) {
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
        
        if (Gua_ObjectLength(argv[1]) > 0) {
            fileName = Gua_ObjectToString(argv[1]);
            
            if ((dirPointer = opendir(fileName)) == NULL) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "could not open directory", Gua_ObjectToString(argv[1]));
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            
            i = 0;
            
            while ((dirEntry = readdir(dirPointer)) != NULL) {
                if (i == 0) {
                    /* Create the first element. */
                    newElement = (Gua_Element *)Gua_Alloc(sizeof(Gua_Element));
                    /* The element key. */
                    Gua_IntegerToObject(newElement->key, i);
                    /* The element object. */
                    Gua_StringToObject(newElement->object, dirEntry->d_name);
                    
                    /* Set the target array chain. */
                    newElement->previous = NULL;
                    newElement->next = NULL;
                    
                    /* Link the first element. */
                    Gua_ArrayToPObject(object, (struct Gua_Element *)newElement, 1);
                    
                    previous = (Gua_Element *)Gua_PObjectToArray(object);
                } else {
                    /* Create a new element. */
                    newElement = (Gua_Element *)Gua_Alloc(sizeof(Gua_Element));
                    /* The element key. */
                    Gua_IntegerToObject(newElement->key, i);
                    /* The element object. */
                    Gua_StringToObject(newElement->object, dirEntry->d_name);
                    
                    /* Set the target array chain. */
                    newElement->previous = (struct Gua_Element *)previous;
                    newElement->next = NULL;
                    
                    previous->next = (struct Gua_Element *)newElement;
                    previous = newElement;
                }
                
                i++;
            }
            
            /* Update the array length entry. */
            Gua_SetPObjectLength(object, i + 1);
            
            closedir(dirPointer);
        }
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsMkDir(path)
     *
     * Description:
     *     Create the specified directory.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsMkDir") == 0) {
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
        
        if (Gua_ObjectLength(argv[1]) > 0) {
            fileName = Gua_ObjectToString(argv[1]);
            
#ifdef _WINDOWS_
            if (_mkdir(fileName) == -1) {
                Gua_IntegerToPObject(object, 0);
            } else {
                Gua_IntegerToPObject(object, 1);
            }
#else
            if (mkdir(fileName, 0777) == -1) {
                Gua_IntegerToPObject(object, 0);
            } else {
                Gua_IntegerToPObject(object, 1);
            }
#endif
        }
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsName(path)
     *
     * Description:
     *     Return the file name without the file path.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsName") == 0) {
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
        
        if (Gua_ObjectLength(argv[1]) > 0) {
            fileName = Gua_ObjectToString(argv[1]);
            
            /* Find the directory separator. */
            for (i = strlen(fileName) - 1; i > 0; i--) {
                if ((fileName[i] == '\\') || (fileName[i] == '/')) {
                    break;
                }
            }
            
            /* Skip the directory separator. */
            if ((fileName[i] == '\\') || (fileName[i] == '/')) {
                i++;
            }
            
            /* Get the file name. */
            newString = (Gua_String)malloc(sizeof(Gua_Char) * strlen(fileName) + 1);
            k = 0;
            for (j = i; j < strlen(fileName); j++) {
                newString[k] = fileName[j];
                k++;
            }
            newString[k] = '\0';
        } else {
            newString = (Gua_String)malloc(sizeof(Gua_Char));
            newString[0] = '\0';
        }
        
        Gua_StringToPObject(object, newString);
        
        Gua_Free(newString);
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsPath(path)
     *
     * Description:
     *     Return the file path without the file name.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsPath") == 0) {
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
        
        if (Gua_ObjectLength(argv[1]) > 0) {
            fileName = Gua_ObjectToString(argv[1]);
            
            /* Find the directory separator. */
            for (i = strlen(fileName) - 1; i > 0; i--) {
                if (((fileName[i] == '\\') || (fileName[i] == '/')) && (i != (strlen(fileName) - 1))) {
                    break;
                }
            }
            
            /* Get the file path. */
            newString = (Gua_String)malloc(sizeof(Gua_Char) * strlen(fileName) + 1);
            for (j = 0; j < i; j++) {
                newString[j] = fileName[j];
            }
            newString[j] = '\0';
        } else {
            newString = (Gua_String)malloc(sizeof(Gua_Char));
            newString[0] = '\0';
        }
        
        Gua_StringToPObject(object, newString);
        
        Gua_Free(newString);
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsRename(file)
     *
     * Description:
     *     Delete the specified file.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsRename") == 0) {
        if (argc != 3) {
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
        
        if (rename(Gua_ObjectToString(argv[1]), Gua_ObjectToString(argv[2])) != 0) {
            errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
            sprintf(errMessage, "%s %-.20s...\n", "can not rename file", Gua_ObjectToString(argv[1]));
            strcat(error, errMessage);
            Gua_Free(errMessage);
            
            return GUA_ERROR;
        }
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsSize(file)
     *
     * Description:
     *     Return the file size.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsSize") == 0) {
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
        
        if (Gua_ObjectLength(argv[1]) > 0) {
            fileName = Gua_ObjectToString(argv[1]);
            
            fileMode = 0;
            
            if (stat(fileName, &fileStat) == -1) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "could not stat file ", fileName);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            
            Gua_IntegerToPObject(object, fileStat.st_size);
        }
    /**
     * Group:
     *     Scripting
     *
     * Function:
     *     fsTime(file)
     *
     * Description:
     *     Return the modification time of the specified file.
     */
    } else if (strcmp(Gua_ObjectToString(argv[0]), "fsTime") == 0) {
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
        
        if (Gua_ObjectLength(argv[1]) > 0) {
            fileName = Gua_ObjectToString(argv[1]);
            
            fileMode = 0;
            
            if (stat(fileName, &fileStat) == -1) {
                errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
                sprintf(errMessage, "%s %-.20s...\n", "could not stat file ", fileName);
                strcat(error, errMessage);
                Gua_Free(errMessage);
                
                return GUA_ERROR;
            }
            
            Gua_RealToPObject(object, fileStat.st_mtime);
        }
    }

    return GUA_OK;
}

/**
 * Group:
 *     C
 *
 * Function:
 *     Gua_Status Fs_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
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
Gua_Status Fs_Init(void *nspace, int argc, char *argv[], char **env, Gua_String error)
{
    Gua_Function function;
    Gua_Object object;
    Gua_String errMessage;

    /* Define the function wrapper to each extension function... */
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsCopy", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsCopy");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsDelete", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsDelete");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsExists", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsExists");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsExtension", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsExtension");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsFullPath", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsFullPath");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsIsDirectory", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsIsDirectory");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsIsExecutable", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsIsExecutable");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsIsReadable", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsIsReadable");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsIsWritable", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsIsWritable");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsLink", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsLink");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsList", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsList");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsMkDir", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsMkDir");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsName", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsName");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsPath", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsPath");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsRename", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsRename");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsSize", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsSize");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    Gua_LinkCFunctionToFunction(function, Fs_FunctionWrapper);
    if (Gua_SetFunction((Gua_Namespace *)nspace, "fsTime", &function) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set function", "fsTime");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    /**
     * Group:
     *     Scripting
     *
     * Constant:
     *     FS_VERSION
     *
     * Description:
     *     Library version.
     */
    Gua_LinkStringToObject(object, FS_VERSION);
    Gua_SetStoredObject(object);
    if (Gua_SetVariable((Gua_Namespace *)nspace, "FS_VERSION", &object, SCOPE_GLOBAL) != GUA_OK) {
        errMessage = (Gua_String) Gua_Alloc(sizeof(char) * MAX_ERROR_MSG_SIZE + 1);
        sprintf(errMessage, "%s %-.20s...\n", "can't set variable", "FS_VERSION");
        strcat(error, errMessage);
        Gua_Free(errMessage);
    }
    
    return GUA_OK;
}

