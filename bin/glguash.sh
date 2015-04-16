#! /bin/sh
#
#     This shell script sets up LD_LIBRARY_PATH so that the GUASH
#     libraries are found first.
#
# Copyright (c) 2015 by Roberto Luiz Souza Monteiro.
#
# RCS: @(#) $Id: guash.sh,v 1.1 2015/02/22 12:39:00 monteiro Exp $

PWD=`pwd`
SCRIPT_PATH=`dirname $0`
cd ${SCRIPT_PATH}
cd ..
PRODUCT_HOME=`pwd`
cd ${PWD}

GUASH_BIN=${PRODUCT_HOME}/bin
GUASH_LIB=${PRODUCT_HOME}/lib

GUASH_EXE_FILE=${GUASH_BIN}/glutguash

# set up the environment.
HOST_TYPE=`/bin/uname -srvm`

LD_LIBRARY_PATH="${GUASH_LIB}:${LD_LIBRARY_PATH}"
export LD_LIBRARY_PATH

GUASH_LIBRARY_PATH="${GUASH_LIB}"
export GUASH_LIBRARY_PATH

# invoke the real executable.
exec "$GUASH_EXE_FILE" "$@"
