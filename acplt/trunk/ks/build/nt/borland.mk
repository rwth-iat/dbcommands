### -*-makefile-*-
### 
### Compiler & platform specific part for
### Borland C++ / make / Windows NT

### Filename conventions
O=.obj
A=.lib
EXE=.exe
ONCDIR = \oncrpc
LIBRPC = \oncrpc\bin\oncrpc.lib
PLTDIR = ..\..\..\plt
LIBPLT = $(PLTDIR)\build\nt\libplt.lib
KSDIR = ..\..
LIBKS = libks.lib
SRCDIR = $(KSDIR)\src\\
EXAMPLESSRCDIR = $(KSDIR)\examples\\

### Compiler
CXX = bcc32
CXX_FLAGS = -DNDEBUG -O2 -w
#CXX_FLAGS = 
CXX_EXTRA_FLAGS = -a8 -I. -I$(PLTDIR)\include -I$(KSDIR)\include -I$(ONCDIR) -DPLT_SYSTEM_NT=1

.SUFFIXES:	

.SUFFIXES:	.cpp .obj .lib .exe

all: $(LIBKS)

{$(SRCDIR)}.cpp{}.obj:
	@echo Compiling $<
	@$(CXX) @&&!
		-Jgx $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $<
!

{$(EXAMPLESSRCDIR)}.cpp{}.obj:
	@echo Compiling $<
	@$(CXX) @&&!
		-Jgx $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $<
!

templates.obj:	$(SRCDIR)templates.cpp
	@echo Compiling $<
	@$(CXX) @&&!
		-Jgd $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $(SRCDIR)templates.cpp
!

tmanager.obj:	$(EXAMPLESSRCDIR)tmanager.cpp
	@echo Compiling $<
	@$(CXX) @&&!
		-Jgd $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $(EXAMPLESSRCDIR)tmanager.cpp
!

tserver.obj:	$(EXAMPLESSRCDIR)tserver.cpp
	@echo Compiling $<
	@$(CXX) @&&!
		-Jgd $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $(EXAMPLESSRCDIR)tserver.cpp
!

tsclient.obj:	$(EXAMPLESSRCDIR)tsclient.cpp
	@echo Compiling $<
	@$(CXX) @&&!
		-Jgd $(CXX_EXTRA_FLAGS) $(CXX_FLAGS) -c -o$@ $(EXAMPLESSRCDIR)tsclient.cpp
!


### Include generic part

!INCLUDE ..\generic.mk

### Include auto dependencies (built with gcc)

!INCLUDE ..\depend.nt

### How to build things
.obj.exe :
	@echo Linking $@
	$(CXX) @&&!
		$< $(LIBKS) $(LIBPLT) $(LIBRPC)
!

tmanager.exe: tmanager.obj tmanager1.obj $(LIBKS)
	@echo Linking $@
	$(CXX) @&&!
		tmanager.obj tmanager1.obj $(LIBKS) $(LIBPLT) $(LIBRPC)
!

tserver.exe: tserver.obj tserver1.obj $(LIBKS)
	@echo Linking $@
	$(CXX) @&&!
		tserver.obj tserver1.obj $(LIBKS) $(LIBPLT) $(LIBRPC)
!

tsclient.exe: tsclient.obj tsclient1.obj $(LIBKS)
	@echo Linking $@
	$(CXX) @&&!
		tsclient.obj tsclient1.obj $(LIBKS) $(LIBPLT) $(LIBRPC)
!

$(LIBKS) : $(LIBKS_OBJECTS)
	$(PLTDIR)\build\nt\plt_ar tlib $@ $(LIBKS_OBJECTS1)
	$(PLTDIR)\build\nt\plt_ar tlib $@ $(LIBKS_OBJECTS2)
	$(PLTDIR)\build\nt\plt_ar tlib $@ $(LIBKS_OBJECTS3)
	$(PLTDIR)\build\nt\plt_ar tlib $@ $(LIBKS_OBJECTS4)

clean :
	del *.obj
	del *.exe

mrproper : clean
	del *.lib
	del *.err
	del *.sym
	del *.mbr


