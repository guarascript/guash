CC = gcc

CP = cp -rf

CD = cd

SRC_TREE = .

SRC_DIR = ${SRC_TREE}/src

INC_DIR = ${SRC_TREE}/include

LIB_DIR =

BIN_DIR = ${SRC_TREE}/bin

DOC_DIR = ${SRC_TREE}/doc

TEST_DIR = ${SRC_TREE}/test

IDE_DIR = ${SRC_TREE}/tools/ide

PREFIX = /usr/local

LANGUAGE_SPECS_DIR = /usr/share/gtksourceview-3.0/language-specs/

EXE_NAME = guash
GLUT_EXE_NAME = glutguash
GLFW_EXE_NAME = glfwguash

CFLAGS = -g -Wall -iquote "${INC_DIR}" -D _LINUX_
GLUT_CFLAGS = -g -Wall -I /usr/X11R6/include/ -iquote "${INC_DIR}" -D _LINUX_ -D _OPENGL_ -D _GLUT_
GLFW_CFLAGS = -g -Wall `pkg-config --cflags glfw3` -I /usr/X11R6/include/ -iquote "${INC_DIR}" -D _LINUX_ -D _OPENGL_ -D _GLFW_

SQLITE_CFLAGS = -DSQLITE_THREADSAFE=0 -DSQLITE_OMIT_LOAD_EXTENSION

LDFLAGS =
GLUT_LDFLAGS =
GLFW_LDFLAGS =

LIBS = -l m
GLUT_LIBS = -L /usr/X11R6/lib -l X11 -l glut -l GL -l GLU
GLFW_LIBS = -L /usr/X11R6/lib `pkg-config --static --libs glfw3` -l GLU

all: $(BIN_DIR)/$(EXE_NAME)

$(BIN_DIR)/$(EXE_NAME): $(SRC_DIR)/array/array.o $(SRC_DIR)/complex/complex.o $(SRC_DIR)/file/unix/file.o $(SRC_DIR)/fs/unix/fs.o $(SRC_DIR)/shell/guash.o $(SRC_DIR)/interpreter/interp.o $(SRC_DIR)/math/math.o $(SRC_DIR)/matrix/matrix.o $(SRC_DIR)/numeric/numeric.o $(SRC_DIR)/printf/printf.o $(SRC_DIR)/sqlite/sqlite.o $(SRC_DIR)/sqlite/sqlite3.o $(SRC_DIR)/string/string.o $(SRC_DIR)/system/unix/system.o $(SRC_DIR)/tui/unix/tui.o $(SRC_DIR)/utf8/utf8.o
	$(CC) $(LDFLAGS) -o $(BIN_DIR)/$(EXE_NAME) $(SRC_DIR)/array/array.o $(SRC_DIR)/complex/complex.o $(SRC_DIR)/file/unix/file.o $(SRC_DIR)/fs/unix/fs.o $(SRC_DIR)/shell/guash.o $(SRC_DIR)/interpreter/interp.o $(SRC_DIR)/math/math.o $(SRC_DIR)/matrix/matrix.o $(SRC_DIR)/numeric/numeric.o $(SRC_DIR)/printf/printf.o $(SRC_DIR)/sqlite/sqlite.o $(SRC_DIR)/sqlite/sqlite3.o $(SRC_DIR)/string/string.o $(SRC_DIR)/system/unix/system.o $(SRC_DIR)/tui/unix/tui.o $(SRC_DIR)/utf8/utf8.o $(LIBS)

$(BIN_DIR)/$(GLUT_EXE_NAME): $(SRC_DIR)/array/array.o $(SRC_DIR)/complex/complex.o $(SRC_DIR)/file/unix/file.o $(SRC_DIR)/fs/unix/fs.o $(SRC_DIR)/gl/gl.o $(SRC_DIR)/glutil/glm.o $(SRC_DIR)/glu/glu.o $(SRC_DIR)/glut/glut.o $(SRC_DIR)/glutil/glutil.o glutguash.o $(SRC_DIR)/interpreter/interp.o $(SRC_DIR)/math/math.o $(SRC_DIR)/matrix/matrix.o $(SRC_DIR)/numeric/numeric.o $(SRC_DIR)/printf/printf.o $(SRC_DIR)/sqlite/sqlite.o $(SRC_DIR)/sqlite/sqlite3.o $(SRC_DIR)/string/string.o $(SRC_DIR)/system/unix/system.o $(SRC_DIR)/tui/unix/tui.o $(SRC_DIR)/utf8/utf8.o
	$(CC) $(GLUT_LDFLAGS) -o $(BIN_DIR)/$(GLUT_EXE_NAME) $(SRC_DIR)/array/array.o $(SRC_DIR)/complex/complex.o $(SRC_DIR)/file/unix/file.o $(SRC_DIR)/fs/unix/fs.o $(SRC_DIR)/gl/gl.o $(SRC_DIR)/glutil/glm.o $(SRC_DIR)/glu/glu.o $(SRC_DIR)/glut/glut.o $(SRC_DIR)/glutil/glutil.o $(SRC_DIR)/shell/glutguash.o $(SRC_DIR)/interpreter/interp.o $(SRC_DIR)/math/math.o $(SRC_DIR)/matrix/matrix.o $(SRC_DIR)/numeric/numeric.o $(SRC_DIR)/printf/printf.o $(SRC_DIR)/sqlite/sqlite.o $(SRC_DIR)/sqlite/sqlite3.o $(SRC_DIR)/string/string.o $(SRC_DIR)/system/unix/system.o $(SRC_DIR)/tui/unix/tui.o $(SRC_DIR)/utf8/utf8.o $(LIBS) $(GLUT_LIBS)

$(BIN_DIR)/$(GLFW_EXE_NAME): $(SRC_DIR)/array/array.o $(SRC_DIR)/complex/complex.o $(SRC_DIR)/file/unix/file.o $(SRC_DIR)/fs/unix/fs.o $(SRC_DIR)/gl/gl.o $(SRC_DIR)/glfw/glfw.o $(SRC_DIR)/glutil/glm.o $(SRC_DIR)/glu/glu.o $(SRC_DIR)/glutil/glutil.o glfwguash.o $(SRC_DIR)/interpreter/interp.o $(SRC_DIR)/math/math.o $(SRC_DIR)/matrix/matrix.o $(SRC_DIR)/numeric/numeric.o $(SRC_DIR)/printf/printf.o $(SRC_DIR)/sqlite/sqlite.o $(SRC_DIR)/sqlite/sqlite3.o $(SRC_DIR)/string/string.o $(SRC_DIR)/system/unix/system.o $(SRC_DIR)/tui/unix/tui.o $(SRC_DIR)/utf8/utf8.o
	$(CC) $(GLFW_LDFLAGS) -o $(BIN_DIR)/$(GLFW_EXE_NAME) $(SRC_DIR)/array/array.o $(SRC_DIR)/complex/complex.o $(SRC_DIR)/file/unix/file.o $(SRC_DIR)/fs/unix/fs.o $(SRC_DIR)/gl/gl.o $(SRC_DIR)/glfw/glfw.o $(SRC_DIR)/glutil/glm.o $(SRC_DIR)/glu/glu.o $(SRC_DIR)/glutil/glutil.o $(SRC_DIR)/shell/glfwguash.o $(SRC_DIR)/interpreter/interp.o $(SRC_DIR)/math/math.o $(SRC_DIR)/matrix/matrix.o $(SRC_DIR)/numeric/numeric.o $(SRC_DIR)/printf/printf.o $(SRC_DIR)/sqlite/sqlite.o $(SRC_DIR)/sqlite/sqlite3.o $(SRC_DIR)/string/string.o $(SRC_DIR)/system/unix/system.o $(SRC_DIR)/tui/unix/tui.o $(SRC_DIR)/utf8/utf8.o $(LIBS) $(GLFW_LIBS)

array.o: $(SRC_DIR)/array/array.c $(INC_DIR)/array.h
	$(CC) $(CFLAGS) -o $(SRC_DIR)/array/array.o -c $(SRC_DIR)/array/array.c

complex.o: $(SRC_DIR)/complex/complex.c $(INC_DIR)/complex.h
	$(CC) $(CFLAGS) -o $(SRC_DIR)/complex/complex.o -c $(SRC_DIR)/complex/complex.c
	
file.o: $(SRC_DIR)/file/unix/file.c $(INC_DIR)/file.h
	$(CC) $(CFLAGS) -o $(SRC_DIR)/file/unix/file.o -c $(SRC_DIR)/file/unix/file.c
	
fs.o: $(SRC_DIR)/fs/unix/fs.c $(INC_DIR)/fs.h
	$(CC) $(CFLAGS) -o $(SRC_DIR)/fs/unix/fs.o -c $(SRC_DIR)/file/unix/fs.c
	
gl.o: $(SRC_DIR)/gl/gl.c
	$(CC) $(CFLAGS) -o $(SRC_DIR)/gl/gl.o -c $(SRC_DIR)/gl/gl.c

glm.o: $(SRC_DIR)/glutil/glm.c
	$(CC) $(CFLAGS) -o $(SRC_DIR)/glutil/glm.o -c $(SRC_DIR)/glutil/glm.c
	
glfwguash.o: $(SRC_DIR)/shell/guash.c
	$(CC) $(GLFW_CFLAGS) -o $(SRC_DIR)/shell/glfwguash.o -c $(SRC_DIR)/shell/guash.c
	
glutguash.o: $(SRC_DIR)/shell/guash.c
	$(CC) $(GLUT_CFLAGS) -o $(SRC_DIR)/shell/glutguash.o -c $(SRC_DIR)/shell/guash.c
	
glu.o: $(SRC_DIR)/glu/glu.c
	$(CC) $(CFLAGS) -o $(SRC_DIR)/glu/glu.o -c $(SRC_DIR)/glu/glu.c
	
glfw.o: $(SRC_DIR)/glfw/glfw.c
	$(CC) $(CFLAGS) -o $(SRC_DIR)/glfw/glfw.o -c $(SRC_DIR)/glfw/glfw.c
	
glut.o: $(SRC_DIR)/glut/glut.c
	$(CC) $(CFLAGS) -o $(SRC_DIR)/glut/glut.o -c $(SRC_DIR)/glut/glut.c
	
glutil.o: $(SRC_DIR)/glutil/glutil.c
	$(CC) $(CFLAGS) -o $(SRC_DIR)/glutil/glutil.o -c $(SRC_DIR)/glutil/glutil.c
	
guash.o: $(SRC_DIR)/shell/guash.c
	$(CC) $(CFLAGS) -o $(SRC_DIR)/shell/guash.o -c $(SRC_DIR)/shell/guash.c

interp.o: $(SRC_DIR)/interpreter/interp.c $(INC_DIR)/interp.h
	$(CC) $(CFLAGS) -o $(SRC_DIR)/interpreter/interp.o -c $(SRC_DIR)/interpreter/interp.c

math.o: $(SRC_DIR)/math/math.c $(INC_DIR)/math.h
	$(CC) $(CFLAGS) -o $(SRC_DIR)/math/math.o -c $(SRC_DIR)/math/math.c

matrix.o: $(SRC_DIR)/matrix/matrix.c $(INC_DIR)/matrix.h
	$(CC) $(CFLAGS) -o $(SRC_DIR)/matrix/matrix.o -c $(SRC_DIR)/matrix/matrix.c

numeric.o: $(SRC_DIR)/numeric/numeric.c $(INC_DIR)/numeric.h
	$(CC) $(CFLAGS) -o $(SRC_DIR)/numeric/numeric.o -c $(SRC_DIR)/numeric/numeric.c

printf.o: $(SRC_DIR)/printf/printf.c $(INC_DIR)/printf.h
	$(CC) $(CFLAGS) -o $(SRC_DIR)/printf/printf.o -c $(SRC_DIR)/printf/printf.c

sqlite.o: $(SRC_DIR)/sqlite/sqlite.c $(INC_DIR)/sqlite.h
	$(CC) $(CFLAGS) -o $(SRC_DIR)/sqlite/sqlite.o -c $(SRC_DIR)/sqlite/sqlite.c

$(SRC_DIR)/sqlite/sqlite3.o: $(SRC_DIR)/sqlite/sqlite3.c
	$(CC) $(SQLITE_CFLAGS) -o $(SRC_DIR)/sqlite/sqlite3.o -c $(SRC_DIR)/sqlite/sqlite3.c

string.o: $(SRC_DIR)/string/string.c $(INC_DIR)/string.h
	$(CC) $(CFLAGS) -o $(SRC_DIR)/string/string.o -c $(SRC_DIR)/string/string.c

system.o: $(SRC_DIR)/system/unix/system.c $(INC_DIR)/system.h
	$(CC) $(CFLAGS) -o $(SRC_DIR)/system/unix/system.o -c $(SRC_DIR)/system/unix/system.c

tui.o: $(SRC_DIR)/tui/unix/tui.c $(INC_DIR)/tui.h
	$(CC) $(CFLAGS) -o $(SRC_DIR)/tui/unix/tui.o -c $(SRC_DIR)/tui/unix/tui.c

utf8.o: $(SRC_DIR)/utf8/utf8.c $(INC_DIR)/utf8.h
	$(CC) $(CFLAGS) -o $(SRC_DIR)/utf8/utf8.o -c $(SRC_DIR)/utf8/utf8.c

beautiful: $(SRC_TREE)/tools/tab2spaces/tab2spaces
	$(SRC_TREE)/tools/tab2spaces/tab2spaces.sh $(SRC_TREE)/tools/tab2spaces $(SRC_DIR)/array/*.c
	$(SRC_TREE)/tools/tab2spaces/tab2spaces.sh $(SRC_TREE)/tools/tab2spaces $(SRC_DIR)/complex/*.c
	$(SRC_TREE)/tools/tab2spaces/tab2spaces.sh $(SRC_TREE)/tools/tab2spaces $(SRC_DIR)/file/unix/*.c
	$(SRC_TREE)/tools/tab2spaces/tab2spaces.sh $(SRC_TREE)/tools/tab2spaces $(SRC_DIR)/fs/unix/*.c
	$(SRC_TREE)/tools/tab2spaces/tab2spaces.sh $(SRC_TREE)/tools/tab2spaces $(SRC_DIR)/gl/*.c
	$(SRC_TREE)/tools/tab2spaces/tab2spaces.sh $(SRC_TREE)/tools/tab2spaces $(SRC_DIR)/glu/*.c
	$(SRC_TREE)/tools/tab2spaces/tab2spaces.sh $(SRC_TREE)/tools/tab2spaces $(SRC_DIR)/glfw/*.c
	$(SRC_TREE)/tools/tab2spaces/tab2spaces.sh $(SRC_TREE)/tools/tab2spaces $(SRC_DIR)/glut/*.c
	$(SRC_TREE)/tools/tab2spaces/tab2spaces.sh $(SRC_TREE)/tools/tab2spaces $(SRC_DIR)/glutil/*.c
	$(SRC_TREE)/tools/tab2spaces/tab2spaces.sh $(SRC_TREE)/tools/tab2spaces $(SRC_DIR)/interpreter/*.c
	$(SRC_TREE)/tools/tab2spaces/tab2spaces.sh $(SRC_TREE)/tools/tab2spaces $(SRC_DIR)/math/*.c
	$(SRC_TREE)/tools/tab2spaces/tab2spaces.sh $(SRC_TREE)/tools/tab2spaces $(SRC_DIR)/matrix/*.c
	$(SRC_TREE)/tools/tab2spaces/tab2spaces.sh $(SRC_TREE)/tools/tab2spaces $(SRC_DIR)/numeric/*.c
	$(SRC_TREE)/tools/tab2spaces/tab2spaces.sh $(SRC_TREE)/tools/tab2spaces $(SRC_DIR)/printf/*.c
	$(SRC_TREE)/tools/tab2spaces/tab2spaces.sh $(SRC_TREE)/tools/tab2spaces $(SRC_DIR)/shell/*.c
	$(SRC_TREE)/tools/tab2spaces/tab2spaces.sh $(SRC_TREE)/tools/tab2spaces $(SRC_DIR)/sqlite/*.c
	$(SRC_TREE)/tools/tab2spaces/tab2spaces.sh $(SRC_TREE)/tools/tab2spaces $(SRC_DIR)/string/*.c
	$(SRC_TREE)/tools/tab2spaces/tab2spaces.sh $(SRC_TREE)/tools/tab2spaces $(SRC_DIR)/system/unix/*.c
	$(SRC_TREE)/tools/tab2spaces/tab2spaces.sh $(SRC_TREE)/tools/tab2spaces $(SRC_DIR)/tui/unix/*.c
	$(SRC_TREE)/tools/tab2spaces/tab2spaces.sh $(SRC_TREE)/tools/tab2spaces $(SRC_DIR)/utf8/*.c
	$(SRC_TREE)/tools/tab2spaces/tab2spaces.sh $(SRC_TREE)/tools/tab2spaces $(INC_DIR)/*.h

test: all
	$(BIN_DIR)/$(EXE_NAME) $(TEST_DIR)/test.gua $(BIN_DIR)/$(EXE_NAME) $(TEST_DIR) > $(TEST_DIR)/test.log

test_glutguash: $(BIN_DIR)/$(GLUT_EXE_NAME)
	$(BIN_DIR)/$(GLUT_EXE_NAME) $(TEST_DIR)/test.gua $(BIN_DIR)/$(GLUT_EXE_NAME) $(TEST_DIR) > $(TEST_DIR)/test.log

test_glfwguash: $(BIN_DIR)/$(GLFW_EXE_NAME)
	$(BIN_DIR)/$(GLFW_EXE_NAME) $(TEST_DIR)/test.gua $(BIN_DIR)/$(GLFW_EXE_NAME) $(TEST_DIR) > $(TEST_DIR)/test.log

$(SRC_TREE)/tools/tab2spaces/tab2spaces: $(SRC_TREE)/tools/tab2spaces/tab2spaces.c
	$(CC) -o $(SRC_TREE)/tools/tab2spaces/tab2spaces $(SRC_TREE)/tools/tab2spaces/tab2spaces.c
	$(SRC_TREE)/tools/tab2spaces/tab2spaces.sh

install: all
	$(CP) $(BIN_DIR)/$(EXE_NAME) $(PREFIX)/bin
	$(CP) $(BIN_DIR)/*.sh $(PREFIX)/bin

install_glutguash: $(BIN_DIR)/$(GLUT_EXE_NAME)
	$(CP) $(BIN_DIR)/$(GLUT_EXE_NAME) $(PREFIX)/bin
	$(CP) $(BIN_DIR)/*.sh $(PREFIX)/bin

install_glfwguash: $(BIN_DIR)/$(GLFW_EXE_NAME)
	$(CP) $(BIN_DIR)/$(GLFW_EXE_NAME) $(PREFIX)/bin
	$(CP) $(BIN_DIR)/*.sh $(PREFIX)/bin

install_language_specs: $(IDE_DIR)/language-specs/gua.lang
	$(CP) $(IDE_DIR)/language-specs/* $(LANGUAGE_SPECS_DIR)

clean:
	rm -rf $(SRC_TREE)/array/*~ $(SRC_TREE)/array/*.bak $(SRC_DIR)/array/*~ $(SRC_DIR)/array/*.bak $(SRC_DIR)/array/*.o 
	rm -rf $(SRC_TREE)/complex/*~ $(SRC_TREE)/complex/*.bak $(SRC_DIR)/complex/*~ $(SRC_DIR)/complex/*.bak $(SRC_DIR)/complex/*.o 
	rm -rf $(SRC_TREE)/file/unix/*~ $(SRC_TREE)/file/unix/*.bak $(SRC_DIR)/file/unix/*~ $(SRC_DIR)/file/unix/*.bak $(SRC_DIR)/file/unix/*.o
	rm -rf $(SRC_TREE)/fs/unix/*~ $(SRC_TREE)/fs/unix/*.bak $(SRC_DIR)/fs/unix/*~ $(SRC_DIR)/fs/unix/*.bak $(SRC_DIR)/fs/unix/*.o
	rm -rf $(SRC_TREE)/gl/*~ $(SRC_TREE)/gl/*.bak $(SRC_DIR)/gl/*~ $(SRC_DIR)/gl/*.bak $(SRC_DIR)/gl/*.o 
	rm -rf $(SRC_TREE)/glu/*~ $(SRC_TREE)/glu/*.bak $(SRC_DIR)/glu/*~ $(SRC_DIR)/glu/*.bak $(SRC_DIR)/glu/*.o 
	rm -rf $(SRC_TREE)/glfw/*~ $(SRC_TREE)/glfw/*.bak $(SRC_DIR)/glfw/*~ $(SRC_DIR)/glfw/*.bak $(SRC_DIR)/glfw/*.o 
	rm -rf $(SRC_TREE)/glut/*~ $(SRC_TREE)/glut/*.bak $(SRC_DIR)/glut/*~ $(SRC_DIR)/glut/*.bak $(SRC_DIR)/glut/*.o 
	rm -rf $(SRC_TREE)/glutil/*~ $(SRC_TREE)/glutil/*.bak $(SRC_DIR)/glutil/*~ $(SRC_DIR)/glutil/*.bak $(SRC_DIR)/glutil/*.o 
	rm -rf $(SRC_TREE)/interpreter/*~ $(SRC_TREE)/interpreter/*.bak $(SRC_DIR)/interpreter/*~ $(SRC_DIR)/interpreter/*.bak $(SRC_DIR)/interpreter/*.o 
	rm -rf $(SRC_TREE)/math/*~ $(SRC_TREE)/math/*.bak $(SRC_DIR)/math/*~ $(SRC_DIR)/math/*.bak $(SRC_DIR)/math/*.o
	rm -rf $(SRC_TREE)/matrix/*~ $(SRC_TREE)/matrix/*.bak $(SRC_DIR)/matrix/*~ $(SRC_DIR)/matrix/*.bak $(SRC_DIR)/matrix/*.o
	rm -rf $(SRC_TREE)/numeric/*~ $(SRC_TREE)/numeric/*.bak $(SRC_DIR)/numeric/*~ $(SRC_DIR)/numeric/*.bak $(SRC_DIR)/numeric/*.o
	rm -rf $(SRC_TREE)/printf/*~ $(SRC_TREE)/printf/*.bak $(SRC_DIR)/printf/*~ $(SRC_DIR)/printf/*.bak $(SRC_DIR)/printf/*.o
	rm -rf $(SRC_TREE)/shell/*~ $(SRC_TREE)/shell/*.bak $(SRC_DIR)/shell/*~ $(SRC_DIR)/shell/*.bak $(SRC_DIR)/shell/*.o
	rm -rf $(SRC_TREE)/sqlite/*~ $(SRC_TREE)/sqlite/*.bak $(SRC_DIR)/sqlite/*~ $(SRC_DIR)/sqlite/*.bak $(SRC_DIR)/sqlite/*.o
	rm -rf $(SRC_TREE)/string/*~ $(SRC_TREE)/string/*.bak $(SRC_DIR)/string/*~ $(SRC_DIR)/string/*.bak $(SRC_DIR)/string/*.o
	rm -rf $(SRC_TREE)/system/unix/*~ $(SRC_TREE)/system/unix/*.bak $(SRC_DIR)/system/unix/*~ $(SRC_DIR)/system/unix/*.bak $(SRC_DIR)/system/unix/*.o
	rm -rf $(SRC_TREE)/tui/unix/*~ $(SRC_TREE)/tui/unix/*.bak $(SRC_DIR)/tui/unix/*~ $(SRC_DIR)/tui/unix/*.bak $(SRC_DIR)/tui/unix/*.o
	rm -rf $(SRC_TREE)/utf8/*~ $(SRC_TREE)/utf8/*.bak $(SRC_DIR)/utf8/*~ $(SRC_DIR)/utf8/*.bak $(SRC_DIR)/utf8/*.o 
	rm -rf $(INC_DIR)/*~ $(INC_DIR)/*.bak
	rm -rf $(LIB_DIR)/*.so
	rm -rf $(BIN_DIR)/$(EXE_NAME)
	rm -rf $(BIN_DIR)/$(GLFW_EXE_NAME)
	rm -rf $(BIN_DIR)/$(GLUT_EXE_NAME)
	rm -rf $(DOC_DIR)/*~ $(DOC_DIR)/*.bak
	rm -rf $(TEST_DIR)/*~ $(TEST_DIR)/*.bak $(TEST_DIR)/*.log $(TEST_DIR)/data.txt $(TEST_DIR)/test.csv $(TEST_DIR)/test.db
	rm -rf $(IDE_DIR)/language-specs/*~

