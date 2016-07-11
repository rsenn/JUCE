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

<<<<<<< 572868f5f2b4903d28a119402150dbc451c62113
  CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DDEBUG=1 -D_DEBUG=1 -DJUCER_LINUX_MAKE_6D53C8B4=1 -DJUCE_APP_VERSION=1.0.0 -DJUCE_APP_VERSION_HEX=0x10000 -I /usr/include -I /usr/include/freetype2 -I ~/SDKs/VST3\ SDK -I ../../JuceLibraryCode -I ../../../../../modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -fPIC -O0 -Wall -Wshadow -Wno-missing-field-initializers -Wshadow -Wshorten-64-to-32 -Wstrict-aliasing -Wuninitialized -Wunused-parameter -Wconversion -Wsign-compare -Wint-conversion -Woverloaded-virtual -Wreorder -Wconstant-conversion -Wsign-conversion
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -Wl,--no-undefined -shared -L/usr/X11R6/lib/ -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 
=======
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_6D53C8B4=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I ~/SDKs/VST3\ SDK -I ../../JuceLibraryCode -I ../../../../../modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -fPIC -O0 -Wshadow -Wno-missing-field-initializers -Wshadow -Wshorten-64-to-32 -Wstrict-aliasing -Wuninitialized -Wunused-parameter -Wconversion -Wsign-compare -Wint-conversion -Woverloaded-virtual -Wreorder -Wconstant-conversion -Wsign-conversion
  CXXFLAGS += $(CFLAGS) -std=c++14
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -shared -L/usr/X11R6/lib/ -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 
>>>>>>> enki fixes

  TARGET := Surround.so
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
  CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DNDEBUG=1 -DJUCER_LINUX_MAKE_6D53C8B4=1 -DJUCE_APP_VERSION=1.0.0 -DJUCE_APP_VERSION_HEX=0x10000 -I /usr/include -I /usr/include/freetype2 -I ~/SDKs/VST3\ SDK -I ../../JuceLibraryCode -I ../../../../../modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -fPIC -O3 -Wall -Wshadow -Wno-missing-field-initializers -Wshadow -Wshorten-64-to-32 -Wstrict-aliasing -Wuninitialized -Wunused-parameter -Wconversion -Wsign-compare -Wint-conversion -Woverloaded-virtual -Wreorder -Wconstant-conversion -Wsign-conversion
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -Wl,--no-undefined -shared -fvisibility=hidden -L/usr/X11R6/lib/ -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 
=======
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_6D53C8B4=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I ~/SDKs/VST3\ SDK -I ../../JuceLibraryCode -I ../../../../../modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -fPIC -O2 -Wshadow -Wno-missing-field-initializers -Wshadow -Wshorten-64-to-32 -Wstrict-aliasing -Wuninitialized -Wunused-parameter -Wconversion -Wsign-compare -Wint-conversion -Woverloaded-virtual -Wreorder -Wconstant-conversion -Wsign-conversion
  CXXFLAGS += $(CFLAGS) -std=c++14
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -shared -fvisibility=hidden -L/usr/X11R6/lib/ -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 
>>>>>>> enki fixes

  TARGET := Surround.so
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

OBJECTS := \
  $(OBJDIR)/SurroundProcessor_c7ed9c55.o \
  $(OBJDIR)/juce_audio_basics_6b797ca1.o \
  $(OBJDIR)/juce_audio_devices_a742c38b.o \
  $(OBJDIR)/juce_audio_formats_5a29c68a.o \
  $(OBJDIR)/juce_audio_plugin_client_utils_35fbf7.o \
  $(OBJDIR)/juce_audio_plugin_client_VST2_fd137df.o \
  $(OBJDIR)/juce_audio_processors_dea3173d.o \
  $(OBJDIR)/juce_core_75b14332.o \
  $(OBJDIR)/juce_cryptography_6de2ebff.o \
  $(OBJDIR)/juce_data_structures_72d3da2c.o \
  $(OBJDIR)/juce_events_d2be882c.o \
  $(OBJDIR)/juce_graphics_9c18891e.o \
  $(OBJDIR)/juce_gui_basics_8a6da59c.o \
  $(OBJDIR)/juce_gui_extra_4a026f23.o \
  $(OBJDIR)/juce_opengl_cd70b4c2.o \
  $(OBJDIR)/juce_video_f128c512.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(RESOURCES)
##	@$(ECHO) Linking Surround
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	$(Q)$(BLDCMD)

clean:
##	@$(ECHO) Cleaning Surround
	$(Q)$(CLEANCMD)

strip:
##	@$(ECHO) Stripping Surround
	-@strip --strip-unneeded $(OUTDIR)/$(TARGET)

$(OBJDIR)/SurroundProcessor_c7ed9c55.o: ../../Source/SurroundProcessor.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling SurroundProcessor.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_basics_6b797ca1.o: ../../JuceLibraryCode/juce_audio_basics.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_audio_basics.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_devices_a742c38b.o: ../../JuceLibraryCode/juce_audio_devices.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_audio_devices.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_formats_5a29c68a.o: ../../JuceLibraryCode/juce_audio_formats.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_audio_formats.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_plugin_client_utils_35fbf7.o: ../../JuceLibraryCode/juce_audio_plugin_client_utils.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_plugin_client_utils.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_plugin_client_VST2_fd137df.o: ../../JuceLibraryCode/juce_audio_plugin_client_VST2.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_plugin_client_VST2.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_processors_dea3173d.o: ../../JuceLibraryCode/juce_audio_processors.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_audio_processors.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_75b14332.o: ../../JuceLibraryCode/juce_core.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_core.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_cryptography_6de2ebff.o: ../../JuceLibraryCode/juce_cryptography.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_cryptography.cpp"
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

$(OBJDIR)/juce_gui_extra_4a026f23.o: ../../JuceLibraryCode/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_gui_extra.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_opengl_cd70b4c2.o: ../../JuceLibraryCode/juce_opengl.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_opengl.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

<<<<<<< 572868f5f2b4903d28a119402150dbc451c62113
$(OBJDIR)/juce_video_f128c512.o: ../../JuceLibraryCode/juce_video.cpp
=======
$(OBJDIR)/juce_PluginUtilities_c8a83799.o: ../../../../../modules/juce_audio_plugin_client/utility/juce_PluginUtilities.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_PluginUtilities.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_video_11f0932d.o: ../../../../../modules/juce_video/juce_video.cpp
>>>>>>> enki fixes
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_video.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

<<<<<<< 572868f5f2b4903d28a119402150dbc451c62113
=======
$(OBJDIR)/juce_VST_Wrapper_371bbba2.o: ../../../../../modules/juce_audio_plugin_client/VST/juce_VST_Wrapper.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_VST_Wrapper.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_VST3_Wrapper_e8b59460.o: ../../../../../modules/juce_audio_plugin_client/VST3/juce_VST3_Wrapper.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_VST3_Wrapper.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

>>>>>>> enki fixes
-include $(OBJECTS:%.o=%.d)
