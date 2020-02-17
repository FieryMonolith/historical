CC =	g++
LD =	g++
CODE_DIR = TreeLoader/Code

OBJECTS =	\
	Objs/TreeNode.o \
	Objs/GammaFunctions.o \
	Objs/Utilities.o \
	Objs/Tree.o \
	Objs/TreeIterator.o \
	Objs/Avida.o \
	Objs/random.o \
	Objs/ProgramInterface.o \
	Objs/main.o

all: Bin/TreeLoader

Bin/TreeLoader:	$(OBJECTS)
	$(LD) -o $@ $(OBJECTS)

Objs/random.o:	$(CODE_DIR)/Support/Interface/random.h \
		$(CODE_DIR)/Support/Source/random.cpp
	$(CC) -I $(CODE_DIR) -o $@ -c $(CODE_DIR)/Support/Source/random.cpp

Objs/TreeNode.o: 	$(CODE_DIR)/PhylogeneticTree/Interface/iTreeNode.h \
			$(CODE_DIR)/PhylogeneticTree/Interface/iOrganism.h \
			$(CODE_DIR)/PhylogeneticTree/Include/TreeNode.h \
			$(CODE_DIR)/PhylogeneticTree/Source/TreeNode.cpp
	$(CC) -I $(CODE_DIR) -o $@ -c $(CODE_DIR)/PhylogeneticTree/Source/TreeNode.cpp

Objs/GammaFunctions.o:	$(CODE_DIR)/Support/Interface/OutputStream.h \
			$(CODE_DIR)/PhylogeneticTree/Interface/iOrganism.h \
			$(CODE_DIR)/PhylogeneticTree/Interface/iTreeNode.h \
			$(CODE_DIR)/PhylogeneticTree/Interface/TreeIterator.h \
			$(CODE_DIR)/PhylogeneticTree/Interface/Tree.h \
			$(CODE_DIR)/PhylogeneticTree/Interface/GammaFunctions.h \
			$(CODE_DIR)/PhylogeneticTree/Source/GammaFunctions.cpp
	$(CC) -I $(CODE_DIR) -o $@ -c $(CODE_DIR)/PhylogeneticTree/Source/GammaFunctions.cpp

Objs/Utilities.o:	$(CODE_DIR)/PhylogeneticTree/Interface/iOrganism.h \
			$(CODE_DIR)/PhylogeneticTree/Interface/iTreeNode.h \
			$(CODE_DIR)/PhylogeneticTree/Interface/Tree.h \
			$(CODE_DIR)/PhylogeneticTree/Interface/TreeIterator.h \
			$(CODE_DIR)/Support/Interface/random.h \
			$(CODE_DIR)/PhylogeneticTree/Interface/Utilities.h \
			$(CODE_DIR)/PhylogeneticTree/Source/Utilities.cpp
	$(CC) -I $(CODE_DIR) -o $@ -c $(CODE_DIR)/PhylogeneticTree/Source/Utilities.cpp

Objs/Tree.o:		$(CODE_DIR)/PhylogeneticTree/Interface/TreeIterator.h \
			$(CODE_DIR)/PhylogeneticTree/Include/TreeNode.h \
			$(CODE_DIR)/PhylogeneticTree/Interface/iOrganism.h \
			$(CODE_DIR)/PhylogeneticTree/Interface/Tree.h \
			$(CODE_DIR)/PhylogeneticTree/Source/Tree.cpp
	$(CC) -I $(CODE_DIR) -o $@ -c $(CODE_DIR)/PhylogeneticTree/Source/Tree.cpp

Objs/TreeIterator.o:	$(CODE_DIR)/PhylogeneticTree/Include/TreeNode.h \
			$(CODE_DIR)/PhylogeneticTree/Interface/TreeIterator.h \
			$(CODE_DIR)/PhylogeneticTree/Source/TreeIterator.cpp
	$(CC) -I $(CODE_DIR) -o $@ -c $(CODE_DIR)/PhylogeneticTree/Source/TreeIterator.cpp

Objs/Avida.o:		$(CODE_DIR)/PhylogeneticTree/Interface/iOrganism.h \
			$(CODE_DIR)/Organisms/Interface/Avida.h \
			$(CODE_DIR)/Organisms/Source/Avida.cpp
	$(CC) -I $(CODE_DIR) -o $@ -c $(CODE_DIR)/Organisms/Source/Avida.cpp

Objs/ProgramInterface.o:	$(CODE_DIR)/Support/Interface/random.h \
			$(CODE_DIR)/Support/Interface/OutputStream.h \
			$(CODE_DIR)/Organisms/Interface/Avida.h \
			$(CODE_DIR)/PhylogeneticTree/Interface/GammaFunctions.h \
			$(CODE_DIR)/PhylogeneticTree/Interface/Utilities.h \
			$(CODE_DIR)/PhylogeneticTree/Interface/iTreeNode.h \
			$(CODE_DIR)/PhylogeneticTree/Interface/Tree.h \
			$(CODE_DIR)/ProgramInterface.h \
			$(CODE_DIR)/ProgramInterface.cpp
	$(CC) -I $(CODE_DIR) -o $@ -c $(CODE_DIR)/ProgramInterface.cpp

Objs/main.o:		$(CODE_DIR)/ProgramInterface.h \
			$(CODE_DIR)/main.cpp
	$(CC) -I $(CODE_DIR) -o $@ -c $(CODE_DIR)/main.cpp

clean:
	rm $(OBJECTS); rm Bin/TreeLoader; find . -name '*~' -exec rm -f {} \;
