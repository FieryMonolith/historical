CC =	g++
LD =	ar

INC_DIR =	../Include
INT_DIR =	../Interface
CODE_DIR =	../Source
OBJ_DIR =	../Objs
OUT_DIR =	../Lib

OBJECTS =	\
	$(OBJ_DIR)/System.o \
	$(OBJ_DIR)/OpenglSurface.o \
	$(OBJ_DIR)/Mouse.o \
	$(OBJ_DIR)/Keyboard.o \
	$(OBJ_DIR)/XWindows/System_Spec.o \
	$(OBJ_DIR)/XWindows/OpenglSurface_Spec.o \
	$(OBJ_DIR)/XWindows/Mouse_Spec.o \
	$(OBJ_DIR)/XWindows/Keyboard_Spec.o

OUTPUT = $(OUT_DIR)/libsSystem.a

all: $(OUTPUT)

clean:
	rm $(OBJECTS) $(OUTPUT);

$(OUT_DIR)/libsSystem.a:	$(OBJECTS)
	$(LD) -rs -o $@ $(OBJECTS)

$(OBJ_DIR)/System.o:	\
	$(INT_DIR)/BuiltinTypes.h \
	$(INT_DIR)/sSystem.h \
	$(INT_DIR)/iSurface.h \
	$(INC_DIR)/iSurface_System.h \
	$(INT_DIR)/iMouse.h \
	$(INT_DIR)/iKeyboard.h \
	$(INC_DIR)/System.h \
	$(CODE_DIR)/System.cpp
	$(CC) -I $(INC_DIR) -I $(INT_DIR) -o $@ -c $(CODE_DIR)/System.cpp

#$(OBJ_DIR)/OpenglSurface.o:	\
#	$(CC) -I $(INC_DIR) -I $(INT_DIR) -o $@ -c $(CODE_DIR)/OpenglSurface.cpp

$(OBJ_DIR)/Mouse.o:	\
	$(INT_DIR)/BuiltinTypes.h \
	$(INT_DIR)/iMouse.h \
	$(INC_DIR)/Mouse.h \
	$(CODE_DIR)/Mouse.cpp
	$(CC) -I $(INC_DIR) -I $(INT_DIR) -o $@ -c $(CODE_DIR)/Mouse.cpp

$(OBJ_DIR)/Keyboard.o:	\
	$(INT_DIR)/BuiltinTypes.h \
	$(INT_DIR)/iKeyboard.h \
	$(INC_DIR)/Keyboard.h \
	$(CODE_DIR)/Keyboard.cpp
	$(CC) -I $(INC_DIR) -I $(INT_DIR) -o $@ -c $(CODE_DIR)/Keyboard.cpp

$(OBJ_DIR)/XWindows/System_Spec.o:	\
	$(CC) -I $(INC_DIR) -I $(INT_DIR) -o $@ -c $(CODE_DIR)/XWindows/System_Spec.cpp

$(OBJ_DIR)/XWindows/OpenglSurface_Spec.o:	\
	$(CC) -I $(INC_DIR) -I $(INT_DIR) -o $@ -c $(CODE_DIR)/XWindows/OpenglSurface_Spec.cpp

$(OBJ_DIR)/XWindows/Mouse_Spec.o:	\
	$(CC) -I $(INC_DIR) -I $(INT_DIR) -o $@ -c $(CODE_DIR)/XWindows/Mouse_Spec.cpp

$(OBJ_DIR)/XWindows/Keyboard_Spec.o:	\
	$(CC) -I $(INC_DIR) -I $(INT_DIR) -o $@ -c $(CODE_DIR)/XWindows/Keyboard_Spec.cpp
