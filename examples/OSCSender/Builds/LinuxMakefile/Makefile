# Automatically generated makefile, created by the Projucer
# Don't edit this file! Your changes will be overwritten when you re-save the Projucer project!

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(CONFIG),Debug)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Debug
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DDEBUG=1 -D_DEBUG=1 -DJUCER_LINUX_MAKE_6D53C8B4=1 -DJUCE_APP_VERSION=1.0.0 -DJUCE_APP_VERSION_HEX=0x10000 -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../../../modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0
  CXXFLAGS += $(CFLAGS) -std=c++14
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ -lX11 -lXext -lXinerama -ldl -lfreetype -lpthread -lrt 

  TARGET := OSCSender
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

<<<<<<< 572868f5f2b4903d28a119402150dbc451c62113
  CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DNDEBUG=1 -DJUCER_LINUX_MAKE_6D53C8B4=1 -DJUCE_APP_VERSION=1.0.0 -DJUCE_APP_VERSION_HEX=0x10000 -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../../../modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -O3
  CXXFLAGS += $(CFLAGS) -std=c++11
=======
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_6D53C8B4=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../../../modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -O2
  CXXFLAGS += $(CFLAGS) -std=c++14
>>>>>>> enki fixes
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -fvisibility=hidden -L/usr/X11R6/lib/ -lX11 -lXext -lXinerama -ldl -lfreetype -lpthread -lrt 

  TARGET := OSCSender
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

OBJECTS := \
  $(OBJDIR)/Main_90ebc5c2.o \
  $(OBJDIR)/juce_core_75b14332.o \
  $(OBJDIR)/juce_data_structures_72d3da2c.o \
  $(OBJDIR)/juce_events_d2be882c.o \
  $(OBJDIR)/juce_graphics_9c18891e.o \
  $(OBJDIR)/juce_gui_basics_8a6da59c.o \
  $(OBJDIR)/juce_osc_be4cee16.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(RESOURCES)
##	@$(ECHO) Linking OSCSender
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	$(Q)$(BLDCMD)

clean:
##	@$(ECHO) Cleaning OSCSender
	$(Q)$(CLEANCMD)

strip:
##	@$(ECHO) Stripping OSCSender
	-@strip --strip-unneeded $(OUTDIR)/$(TARGET)

$(OBJDIR)/Main_90ebc5c2.o: ../../Source/Main.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling Main.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_75b14332.o: ../../JuceLibraryCode/juce_core.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_core.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_72d3da2c.o: ../../JuceLibraryCode/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_data_structures.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_d2be882c.o: ../../JuceLibraryCode/juce_events.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_events.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_9c18891e.o: ../../JuceLibraryCode/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_graphics.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_8a6da59c.o: ../../JuceLibraryCode/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_gui_basics.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_osc_be4cee16.o: ../../JuceLibraryCode/juce_osc.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_osc.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
