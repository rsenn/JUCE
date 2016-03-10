# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

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

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCE_UNIT_TESTS=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=3.0.0" -D "JUCE_APP_VERSION_HEX=0x30000" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../../../modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0
  CXXFLAGS += $(CFLAGS) -std=c++14
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 

  TARGET := JuceDemo
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

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "JUCE_UNIT_TESTS=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=3.0.0" -D "JUCE_APP_VERSION_HEX=0x30000" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../../../modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -O2
  CXXFLAGS += $(CFLAGS) -std=c++14
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -fvisibility=hidden -L/usr/X11R6/lib/ -lGL -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 

  TARGET := JuceDemo
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

OBJECTS := \
  $(OBJDIR)/Main_90ebc5c2.o \
  $(OBJDIR)/MainWindow_499ac812.o \
  $(OBJDIR)/IntroScreen_73347059.o \
  $(OBJDIR)/AnimationDemo_9fc7e4e9.o \
  $(OBJDIR)/AudioLatencyDemo_78d49233.o \
  $(OBJDIR)/AudioPlaybackDemo_ea12adf6.o \
  $(OBJDIR)/AudioRecordingDemo_435914e6.o \
  $(OBJDIR)/AudioSettingsDemo_6d710bfe.o \
  $(OBJDIR)/AudioSynthesiserDemo_1b3666f6.o \
  $(OBJDIR)/Box2DDemo_51053c42.o \
  $(OBJDIR)/CameraDemo_a10f7e50.o \
  $(OBJDIR)/ChildProcessDemo_2c2b949e.o \
  $(OBJDIR)/CodeEditorDemo_d5c17a65.o \
  $(OBJDIR)/ComponentTransformsDemo_377f7889.o \
  $(OBJDIR)/CryptographyDemo_232d46d7.o \
  $(OBJDIR)/DialogsDemo_19d585d0.o \
  $(OBJDIR)/FontsDemo_6da1e5e9.o \
  $(OBJDIR)/GraphicsDemo_85142ef6.o \
  $(OBJDIR)/ImagesDemo_ecf13aa3.o \
  $(OBJDIR)/JavaScript_d6e42eb5.o \
  $(OBJDIR)/KeyMappingsDemo_cf05c709.o \
  $(OBJDIR)/LiveConstantDemo_e3b18a9b.o \
  $(OBJDIR)/LookAndFeelDemo_25c558a3.o \
  $(OBJDIR)/MDIDemo_c476c2d7.o \
  $(OBJDIR)/MidiDemo_75ebc30c.o \
  $(OBJDIR)/MultithreadingDemo_ff566eaa.o \
  $(OBJDIR)/MultiTouch_595f3a2e.o \
  $(OBJDIR)/NetworkingDemo_fbd1b19f.o \
  $(OBJDIR)/OpenGLDemo_fdac55da.o \
  $(OBJDIR)/OpenGLDemo2D_48eeadec.o \
  $(OBJDIR)/PropertiesDemo_8ff1a8de.o \
  $(OBJDIR)/SystemInfoDemo_984acd28.o \
  $(OBJDIR)/TimersAndEventsDemo_f2656547.o \
  $(OBJDIR)/UnitTestsDemo_5c21762.o \
  $(OBJDIR)/ValueTreesDemo_9657d84f.o \
  $(OBJDIR)/VideoDemo_b55a1560.o \
  $(OBJDIR)/WebBrowserDemo_3df2dbdf.o \
  $(OBJDIR)/WidgetsDemo_fb0454.o \
  $(OBJDIR)/WindowsDemo_e8cfa428.o \
  $(OBJDIR)/XMLandJSONDemo_5bc88df3.o \
  $(OBJDIR)/BinaryData_ce4232d4.o \
  $(OBJDIR)/juce_audio_basics_399a455e.o \
  $(OBJDIR)/juce_audio_devices_c1c9ba9c.o \
  $(OBJDIR)/juce_audio_formats_f04b043c.o \
  $(OBJDIR)/juce_audio_processors_eb9ae116.o \
  $(OBJDIR)/juce_audio_utils_cf18005c.o \
  $(OBJDIR)/juce_box2d_7c8de7fc.o \
  $(OBJDIR)/juce_core_1ee54a40.o \
  $(OBJDIR)/juce_cryptography_3b1f489a.o \
  $(OBJDIR)/juce_data_structures_84790dfc.o \
  $(OBJDIR)/juce_events_584896b4.o \
  $(OBJDIR)/juce_graphics_f9afc18.o \
  $(OBJDIR)/juce_gui_basics_90929794.o \
  $(OBJDIR)/juce_gui_extra_b81d9e1c.o \
  $(OBJDIR)/juce_opengl_1890bee0.o \
  $(OBJDIR)/juce_video_86a19d3c.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(RESOURCES)
##	@$(ECHO) Linking JuceDemo
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	$(Q)$(BLDCMD)

clean:
##	@$(ECHO) Cleaning JuceDemo
	$(Q)$(CLEANCMD)

strip:
##	@$(ECHO) Stripping JuceDemo
	-@strip --strip-unneeded $(OUTDIR)/$(TARGET)

$(OBJDIR)/Main_90ebc5c2.o: ../../Source/Main.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling Main.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MainWindow_499ac812.o: ../../Source/MainWindow.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling MainWindow.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/IntroScreen_73347059.o: ../../Source/IntroScreen.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling IntroScreen.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AnimationDemo_9fc7e4e9.o: ../../Source/Demos/AnimationDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling AnimationDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AudioLatencyDemo_78d49233.o: ../../Source/Demos/AudioLatencyDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling AudioLatencyDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AudioPlaybackDemo_ea12adf6.o: ../../Source/Demos/AudioPlaybackDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling AudioPlaybackDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AudioRecordingDemo_435914e6.o: ../../Source/Demos/AudioRecordingDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling AudioRecordingDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AudioSettingsDemo_6d710bfe.o: ../../Source/Demos/AudioSettingsDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling AudioSettingsDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AudioSynthesiserDemo_1b3666f6.o: ../../Source/Demos/AudioSynthesiserDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling AudioSynthesiserDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Box2DDemo_51053c42.o: ../../Source/Demos/Box2DDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling Box2DDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CameraDemo_a10f7e50.o: ../../Source/Demos/CameraDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling CameraDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ChildProcessDemo_2c2b949e.o: ../../Source/Demos/ChildProcessDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling ChildProcessDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CodeEditorDemo_d5c17a65.o: ../../Source/Demos/CodeEditorDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling CodeEditorDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ComponentTransformsDemo_377f7889.o: ../../Source/Demos/ComponentTransformsDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling ComponentTransformsDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CryptographyDemo_232d46d7.o: ../../Source/Demos/CryptographyDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling CryptographyDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/DialogsDemo_19d585d0.o: ../../Source/Demos/DialogsDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling DialogsDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FontsDemo_6da1e5e9.o: ../../Source/Demos/FontsDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling FontsDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/GraphicsDemo_85142ef6.o: ../../Source/Demos/GraphicsDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling GraphicsDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ImagesDemo_ecf13aa3.o: ../../Source/Demos/ImagesDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling ImagesDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/JavaScript_d6e42eb5.o: ../../Source/Demos/JavaScript.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling JavaScript.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/KeyMappingsDemo_cf05c709.o: ../../Source/Demos/KeyMappingsDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling KeyMappingsDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LiveConstantDemo_e3b18a9b.o: ../../Source/Demos/LiveConstantDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling LiveConstantDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LookAndFeelDemo_25c558a3.o: ../../Source/Demos/LookAndFeelDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling LookAndFeelDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MDIDemo_c476c2d7.o: ../../Source/Demos/MDIDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling MDIDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MidiDemo_75ebc30c.o: ../../Source/Demos/MidiDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling MidiDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MultithreadingDemo_ff566eaa.o: ../../Source/Demos/MultithreadingDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling MultithreadingDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MultiTouch_595f3a2e.o: ../../Source/Demos/MultiTouch.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling MultiTouch.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/NetworkingDemo_fbd1b19f.o: ../../Source/Demos/NetworkingDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling NetworkingDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/OpenGLDemo_fdac55da.o: ../../Source/Demos/OpenGLDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling OpenGLDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/OpenGLDemo2D_48eeadec.o: ../../Source/Demos/OpenGLDemo2D.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling OpenGLDemo2D.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PropertiesDemo_8ff1a8de.o: ../../Source/Demos/PropertiesDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling PropertiesDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SystemInfoDemo_984acd28.o: ../../Source/Demos/SystemInfoDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling SystemInfoDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TimersAndEventsDemo_f2656547.o: ../../Source/Demos/TimersAndEventsDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling TimersAndEventsDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/UnitTestsDemo_5c21762.o: ../../Source/Demos/UnitTestsDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling UnitTestsDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ValueTreesDemo_9657d84f.o: ../../Source/Demos/ValueTreesDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling ValueTreesDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/VideoDemo_b55a1560.o: ../../Source/Demos/VideoDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling VideoDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/WebBrowserDemo_3df2dbdf.o: ../../Source/Demos/WebBrowserDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling WebBrowserDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/WidgetsDemo_fb0454.o: ../../Source/Demos/WidgetsDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling WidgetsDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/WindowsDemo_e8cfa428.o: ../../Source/Demos/WindowsDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling WindowsDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/XMLandJSONDemo_5bc88df3.o: ../../Source/Demos/XMLandJSONDemo.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling XMLandJSONDemo.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling BinaryData.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_basics_399a455e.o: ../../../../modules/juce_audio_basics/juce_audio_basics.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_audio_basics.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_devices_c1c9ba9c.o: ../../../../modules/juce_audio_devices/juce_audio_devices.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_audio_devices.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_formats_f04b043c.o: ../../../../modules/juce_audio_formats/juce_audio_formats.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_audio_formats.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_processors_eb9ae116.o: ../../../../modules/juce_audio_processors/juce_audio_processors.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_audio_processors.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_utils_cf18005c.o: ../../../../modules/juce_audio_utils/juce_audio_utils.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_audio_utils.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_box2d_7c8de7fc.o: ../../../../modules/juce_box2d/juce_box2d.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_box2d.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_1ee54a40.o: ../../../../modules/juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_core.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_cryptography_3b1f489a.o: ../../../../modules/juce_cryptography/juce_cryptography.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_cryptography.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_84790dfc.o: ../../../../modules/juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_data_structures.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_584896b4.o: ../../../../modules/juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_events.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_f9afc18.o: ../../../../modules/juce_graphics/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_graphics.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_90929794.o: ../../../../modules/juce_gui_basics/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_gui_basics.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_b81d9e1c.o: ../../../../modules/juce_gui_extra/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_gui_extra.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_opengl_1890bee0.o: ../../../../modules/juce_opengl/juce_opengl.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_opengl.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_video_86a19d3c.o: ../../../../modules/juce_video/juce_video.cpp
	-@mkdir -p $(OBJDIR)
##	@$(ECHO) "Compiling juce_video.cpp"
	$(Q)$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
