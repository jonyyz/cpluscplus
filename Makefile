CC=cl.exe
CFLAGS=/nologo /EHsc /D_WIN32_WINDOWS
INCLUDES=D:\dev\boost_1_70_0
BOOST=D:\dev\boost_1_70_0\stage\lib

EXECUTABLE_NAME=myapp.exe
SRC_FILES=\
	main.cpp \
	ActionRegistry.cpp \
	BarkAction.cpp

DEL_OBJ_FILES=DEL /Q *.obj >nul 2>&1

all: $(EXECUTABLE_NAME)

$(EXECUTABLE_NAME): $(SRC_FILES)
	$(CC) $(CFLAGS) /Fe$(EXECUTABLE_NAME) $(SRC_FILES) /I $(INCLUDES) /link /LIBPATH:$(BOOST)
	$(DEL_OBJ_FILES)

clean:
	DEL /Q $(EXECUTABLE_NAME) >nul 2>&1
	$(DEL_OBJ_FILES)
