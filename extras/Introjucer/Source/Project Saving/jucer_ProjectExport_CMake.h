/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2015 - ROLI Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/

class CMakeProjectExporter  : public ProjectExporter
{
public:
    //===============================================================e===============
    static const char* getName() noexcept
    {
        return "CMake";
    }

    //==============================================================================
    static const char* getValueTreeTypeName()
    {
        return "CMAKE";
    }


    //==============================================================================
    static CMakeProjectExporter* createForSettings(Project& project, const ValueTree& settings)
    {
        // this will also import legacy jucer files where CMake only worked for Windows,
        // had valueTreetTypeName "CMAKE", and there was no OS distinction
        if (settings.hasType(getValueTreeTypeName()) || settings.hasType("CMAKE"))
            return new CMakeProjectExporter(project, settings);

        return nullptr;
    }


    //==============================================================================
    CMakeProjectExporter(Project& p, const ValueTree& t)   : ProjectExporter(p, t)
    {
        name = getName();

        if (getTargetLocationString().isEmpty())
            getTargetLocationValue() = getDefaultBuildsRootFolder() + "CMake";

        initialiseDependencyPathValues(TargetOS::linux);
    }

    //==============================================================================
    bool canLaunchProject() override
    {
        return true;
    }
    bool launchProject() override
    {
        //RelativePath buildPath( getTargetFolder().getChildFile("CMakeLists.txt"), getTargetFolder(), RelativePath::buildTargetFolder);
        //File buildDir(buildPath.toUnixStyle());
        //String location = getTargetLocationValue().getValue();
        File buildDir(getTargetLocationString()); //(buildPath.toUnixStyle());
        ChildProcess proc;
        String output;
        bool success;
        const char* cmakeCmd[] = {
            "cmake", "-DCMAKE_VERBOSE_MAKEFILE=TRUE", ".",
            nullptr
        };

        std::cerr << "LOG - build dir: " << buildDir.getFullPathName() << std::endl;
                buildDir.setAsCurrentWorkingDirectory();

        proc.start(StringArray(cmakeCmd), ChildProcess::wantStdOut|ChildProcess::wantStdErr);
        proc.waitForProcessToFinish(30*1000);

        output = proc.readAllProcessOutput();

        success = ( proc.getExitCode() == 0);

        {
            GroupComponent groupbox;
            TextButton closebutton("&close");
            TextEditor editbox;


            closebutton.setSize(200,50);

            editbox.setSize(400,300);
            editbox.setMultiLine(true);
            editbox.setText(output);

            groupbox.setSize(500,400);
            groupbox.addAndMakeVisible(editbox);
            groupbox.addAndMakeVisible(closebutton);

            DialogWindow::showModalDialog("cmake output", &groupbox, nullptr, Colour(0,0,0), true, true, true);
        }

        return success;
    }

    bool usesMMFiles() const override
    {
        return false;
    }
    bool isCMake() const override
    {
        return true;
    }
    bool isWindows() const override
    {
        return true;
    }
    bool isLinux() const override
    {
        return true;
    }
    bool isOSX() const override
    {
        return true;
    }
    bool canCopeWithDuplicateFiles() override
    {
        return false;
    }
    Value getCppStandardValue()
    {
        return getSetting(Ids::cppLanguageStandard);
    }
    String getCppStandardString() const
    {
        return settings[Ids::cppLanguageStandard];
    }

    void createExporterProperties(PropertyListBuilder& properties) override
    {
        static const char* cppStandardNames[]  = { "C++03", "C++11", nullptr };
        static const char* cppStandardValues[] = { "-std=c++03", "-std=c++11", nullptr };

        properties.add(new ChoicePropertyComponent(getCppStandardValue(),
                                                   "C++ standard to use",
                                                   StringArray(cppStandardNames),
                                                   Array<var> (cppStandardValues)),
                       "The C++ standard to specify in the makefile");
    }

    //==============================================================================
    void create(const OwnedArray<LibraryModule>&) const override
    {
        Array<RelativePath> sourceFiles, libSourceFiles;

        for (int i = 0; i < getAllGroups().size(); ++i) {
            findFilesToCompile(getAllGroups().getReference(i), sourceFiles, false);
            findFilesToCompile(getAllGroups().getReference(i), libSourceFiles, true);
        }

        MemoryOutputStream mo;
        writeMakefile(mo, sourceFiles, libSourceFiles);

        overwriteFileIfDifferentOrThrow(getTargetFolder().getChildFile("CMakeLists.txt"), mo);
    }

protected:
    //==============================================================================
    class CMakeBuildConfiguration  : public BuildConfiguration
    {
    public:
        CMakeBuildConfiguration(Project& p, const ValueTree& settings, const ProjectExporter& e)
            : BuildConfiguration(p, settings, e)
        {
            setValueIfVoid(getLibrarySearchPathValue(), "${CMAKE_SYSTEM_LIBRARY_PATH}");
        }

        Value getArchitectureType()
        {
            return getValue(Ids::winArchitecture);
        }
        var getArchitectureTypeVar() const
        {
            return config [Ids::winArchitecture];
        }

        var getDefaultOptimisationLevel() const override
        {
            return var((int)(isDebug() ? gccO0 : gccO3));
        }

        void createConfigProperties(PropertyListBuilder& props) override
        {
            addGCCOptimisationProperty(props);

            static const char* const archNames[] = { "(Default)", "<None>",       "32-bit (-m32)", "64-bit (-m64)", "ARM v6",       "ARM v7" };
            const var archFlags[]                = { var(),       var(String()), "-m32",         "-m64",           "-march=armv6", "-march=armv7" };

            props.add(new ChoicePropertyComponent(getArchitectureType(), "Architecture",
                                                  StringArray(archNames, numElementsInArray(archNames)),
                                                  Array<var> (archFlags, numElementsInArray(archFlags))));
        }
    };

    BuildConfiguration::Ptr createBuildConfig(const ValueTree& tree) const override
    {
        return new CMakeBuildConfiguration(project, tree, *this);
    }

private:
    //==============================================================================
    void findFilesToCompile(const Project::Item& projectItem, Array<RelativePath>& results, bool libFiles = false) const
    {
        if (projectItem.isGroup())
        {
            for (int i = 0; i < projectItem.getNumChildren(); ++i) {
//                if((projectItem.getID() == "__jucelibfiles") != libFiles) continue;

                findFilesToCompile(projectItem.getChild(i), results, libFiles);
            }
        }
        else
        {
            const File& f = projectItem.getFile();

            if (projectItem.shouldBeCompiled() ||  f.hasFileExtension (headerFileExtensions)) {
                if(!projectItem.shouldBeAddedToBinaryResources()) {
                    RelativePath path(f, getTargetFolder(), RelativePath::buildTargetFolder);

                    bool isJuceLibrarySource = (path.toUnixStyle().contains("modules/juce_") /*&& path.toUnixStyle().endsWith(".cpp")*/);

                    if(isJuceLibrarySource == libFiles)
                        results.add(path);
                }
            }
        }
    }

    //==============================================================================
    static void writeDefineFlags(OutputStream& out, const BuildConfiguration& config,
                                 const StringPairArray& extraDefs)
    {
        StringPairArray defines(extraDefs);

        if(config.isDebug())
        {
            defines.set("DEBUG", "1");
            defines.set("_DEBUG", "1");
        }
        else
        {
            defines.set("NDEBUG", "1");
        }

        if(defines.size() > 0)
            out << "  add_definitions(" << createGCCPreprocessorFlags(defines).trimStart() << ")" << newLine;
    }

    //==============================================================================
    void writeHeaderPathFlags(OutputStream& out, const BuildConfiguration& config,
                              const StringArray& extraIncPaths) const
    {
        StringArray searchPaths(extraSearchPaths);

        searchPaths.add(RelativePath(project.getGeneratedCodeFolder(), getTargetFolder(),
                                     RelativePath::buildTargetFolder).toUnixStyle());

        searchPaths.addArray(config.getHeaderSearchPaths());
        searchPaths.addArray(extraIncPaths);

        /*
        searchPaths.insert(0, "${FREETYPE_INCLUDE_DIRS}");
        searchPaths.insert(0, "${FREETYPE_INCLUDE_DIR_ft2build}");
         */
        searchPaths = getCleanedStringArray(searchPaths);

        out << "  include_directories(" << newLine;
        writePathList(out, config, searchPaths, "  ");
        out << ")" << newLine;
    }

    //==============================================================================
    void writePathList(OutputStream& out, const BuildConfiguration& config,
                       const StringArray& list,
                       const String& indent = "") const
    {
        out << indent;

        for(int i = 0; i < list.size(); ++i)
        {
            out << "    "
                << addQuotesIfContainsSpaces(FileHelpers::unixStylePath(list[i]))
                << newLine << indent;
        }
    }

    //==============================================================================
    void writeCppFlags(OutputStream& out, const BuildConfiguration& config,
                   StringPairArray& extraDefs, StringArray& extraIncPaths) const
    {
        writeDefineFlags(out, config, extraDefs);
        writeHeaderPathFlags(out, config, extraIncPaths);
    }

    //==============================================================================
    void writeLinkLibraries(OutputStream& out, const StringArray& libs) const
    {
        StringArray libraries;

        for(int i = 0; i < libs.size(); ++i)
        {
            String libName = libs[i];

            if(libName.startsWith("-l"))
                libName = libName.substring(2);

            libraries.add(addQuotesIfContainsSpaces(libName));
        }

        libraries.addTokens(getExternalLibrariesString(), ";", "\"'");
        libraries.removeEmptyStrings();

        if(libraries.size() > 0)
        {
            out << "  link_libraries(" << libraries.joinIntoString(" ") << ")" << newLine;
        }
    }

    //==============================================================================
    void writeLinkDirectories(OutputStream& out, const BuildConfiguration& config) const
    {
        StringArray libraryDirs, libraryPathFlags = StringArray::fromTokens(config.getGCCLibraryPathFlags(), " ;", "\"'");

        for (int i = 0; i < libraryPathFlags.size(); ++i)
        {
			const String& s = libraryPathFlags[i];

            if (s.startsWith("-L"))
                libraryDirs.add(s.substring(2).quoted());
        }

        {
            String linkDirs =  getCleanedStringArray(libraryDirs).joinIntoString(" ").trimStart();

            if (linkDirs.length() > 0)
                out << "  link_directories(" << linkDirs << ")" << newLine;
        }
    }

    //==============================================================================
    void setCompileFlags(StringPairArray& vars, const BuildConfiguration& config,
                 const StringArray& flags) const
    {
        StringArray compileFlags = flags;

        if(config.isDebug())
        {
            compileFlags.add("-g");
            compileFlags.add("-ggdb");
        }

        compileFlags.add("-O" + config.getGCCOptimisationFlag());

        if(compileFlags.size() == 0)
            return;

        vars.set("CMAKE_CXX_FLAGS_" + config.getName().toUpperCase(),
                 getCleanedStringArray(compileFlags).joinIntoString(" "));
    }


    //==============================================================================
    void setLinkerFlags(StringPairArray& vars, const BuildConfiguration& config) const
    {
        StringArray flags(makefileExtraLinkerFlags);

        if (makefileIsDLL)
            flags.add("-shared");

        if (! config.isDebug())
            flags.add("-fvisibility=hidden");

        flags.add(replacePreprocessorTokens(config, getExtraLinkerFlagsString()).trim());

        {
            String linkFlags = getCleanedStringArray(flags).joinIntoString(" ").trimStart();

            if (linkFlags.length() > 0)
                vars.set("LINK_FLAGS_" + config.getName().toUpperCase(), linkFlags);
        }
    }

    //==============================================================================
    StringPairArray getDefaultVars(StringPairArray& extraDefs, StringArray& extraIncPaths) const
    {
        StringPairArray vars;

        for (ConstConfigIterator config(*this); config.next();)
        {
            StringArray compileFlags, extraCompileFlags = 
            StringArray::fromTokens(replacePreprocessorTokens(*config, getExtraCompilerFlagsString()), "; ", "\"'");

            String cppStandardToUse = getCppStandardString();

            if(cppStandardToUse.isEmpty())
                cppStandardToUse = "-std=c++11";

            compileFlags.add(cppStandardToUse);

            for (int i = 0; i < extraCompileFlags.size(); ++i)
            {
                const String& t = extraCompileFlags[i];

                String arg;

                //std::cerr << "extraCompilerFlags arg: " << t << std::endl;

                if (t.startsWith("-"))
                {
                    if(t.length() > 2)
                        arg = t.substring(2);
                    else
                        arg = extraCompileFlags[++i];

                    if (t.startsWith("-I"))
                    {
                        RelativePath includePath(arg, RelativePath::projectFolder);
                        extraIncPaths.add(includePath.toUnixStyle());
                        continue;
                    }
                    if (t.startsWith("-D"))
                    {
                        //compileFlags.add(t);

                        StringArray def = StringArray::fromTokens(arg.unquoted(), "=" , "\"");

                        extraDefs.set(def[0], def[1].unquoted());
                        continue;
                    }
                }

                //compileFlags.add(t);
            }

            setCompileFlags(vars, *config, compileFlags);
            setLinkerFlags(vars, *config);

            //std::cerr << "compileFlags arg: " << compileFlags.joinIntoString("|") << std::endl;
        }
        return vars;
    }
    
    
    //==============================================================================
    String getTargetFilename(const BuildConfiguration& config) const
    {
       bool isLibrary = (projectType.isStaticLibrary() || projectType.isDynamicLibrary());
       String fileName(replacePreprocessorTokens(config, config.getTargetBinaryNameString()));

        if (isLibrary)
            fileName = getLibbedFilename(fileName).upToLastOccurrenceOf(".", false, false);
        else
            fileName = fileName.upToLastOccurrenceOf(".", false, false) + makefileTargetSuffix;

        if (fileName .startsWith("lib"))
            fileName = fileName.substring(3);

        if (fileName .endsWith("_dll"))
            fileName = fileName.substring(0, 4);

        if(fileName.endsWith(".so"))
            fileName = fileName.substring(0, fileName.length() - 3);

        if (!config.isDebug() && fileName.endsWith("_d"))
            fileName = fileName.substring(0, fileName.length() - 2);
        else if(config.isDebug() && !fileName.endsWith("_d"))
            fileName = fileName + "_d";
            return fileName;
       }

    //==============================================================================
    void writeConfig(OutputStream& out, const BuildConfiguration& config, const String& targetName,
                     StringPairArray& extraDefs, StringArray& extraIncPaths, StringPairArray& targetProps) const
    {
     //   bool isLibrary = (projectType.isStaticLibrary() || projectType.isDynamicLibrary());
        const String buildDirName("build");
        const String intermediatesDirName(buildDirName + "/intermediate/" + config.getName());
        String targetFileName = getTargetFilename(config);
        String outputDir(buildDirName);

        out << "# Configuration: " << config.getName() << newLine;

        if (config.getTargetBinaryRelativePathString().isNotEmpty())
        {
            RelativePath binaryPath(config.getTargetBinaryRelativePathString(), RelativePath::projectFolder);
            outputDir = binaryPath.rebased(projectFolder, getTargetFolder(), RelativePath::buildTargetFolder).toUnixStyle();
        }

        out << "if(CMAKE_BUILD_TYPE STREQUAL " << config.getName().quoted() << ")" << newLine;

        writeCppFlags(out, config, extraDefs, extraIncPaths);
        writeLinkDirectories(out, config);
        
        out << "endif()" << newLine;
       /* out << newLine;
        out << "set(TARGET_NAME_" << config.getName().toUpperCase() << " \"" << targetFileName << "\")" << newLine;
        */
        
        if(config.isDebug())
            targetProps.set("DEBUG_POSTFIX", "_d");

        if (targetFileName != targetName)
        {
            String key = "OUTPUT_NAME";
            
            targetProps.set(key, targetFileName);
        }

        out << "# End of configuration: " << config.getName() << newLine;
        out << newLine;
    }

    //==============================================================================
    void writeSources(OutputStream& out, const Array<RelativePath>& files) const
    {
        out << newLine;

        for (int i = 0; i < files.size(); ++i)
        {
            const RelativePath& f = files.getReference(i);

            if (shouldFileBeCompiledByDefault(f) || f.hasFileExtension(headerFileExtensions))
                out << "    " << addQuotesIfContainsSpaces(files.getReference(i).toUnixStyle()) << newLine ;
        }
    }

    //==============================================================================
    static void writeIncludeFind(OutputStream& out, const String& libraryName, bool required = false)
    {
        String varName = libraryName.toUpperCase();

        out << "  include(Find"+libraryName+")" << newLine
            << "  if(" << varName << "_FOUND)" << newLine
            << "    message(STATUS \"Found " << libraryName << " library version ${" << varName << "_VERSION_STRING}\")" << newLine;

        out << "    include_directories(${" << varName << "_INCLUDE_DIRS})" << newLine;
        out << "    link_libraries(${" << varName << "_LIBRARIES})" << newLine;

        if (required)
        {
            out << "  else()" << newLine
                << "    message(FATAL_ERROR \"" << libraryName << " library not found!\")" << newLine;
        }
        else
        {
            out << "    add_definitions(-DHAVE_" << varName << "=1)" << newLine;
        }

        out << "  endif()" << newLine;
    }

    //==============================================================================
    static void writeLibraryCheck(OutputStream& out, const String& libName, const String& symbolName) 
    {
        String libFlag = "HAVE_LIB" + libName.toUpperCase();

        out << "  check_library_exists(" << libName << " " << symbolName << " \"\" " << libFlag << ")" << newLine;
        out << "  if(" << libFlag << ")" << newLine;
        out << "    link_libraries(" << libName << ")" << newLine ;
        out << "  endif()" << newLine;

        out << newLine;
        }
        
    //==============================================================================
    void writeLinuxChecks(OutputStream& out, Project& proj) const
    {
        out << "if(UNIX)" << newLine;
        out << "  # --- Check for X11 and dlopen(3) on UNIX systems -------------" << newLine;
        if(proj.getModules().isModuleEnabled ("juce_gui_basics")) {
            writeIncludeFind(out, "X11", true);
            out << newLine;
        }
  
        out << "  include(CheckLibraryExists)" << newLine;

        if(proj.getModules().isModuleEnabled ("juce_opengl"))
            writeLibraryCheck(out, "GL", "glXSwapBuffers");

        writeLibraryCheck(out, "dl", "dlopen");
        writeLibraryCheck(out, "pthread", "pthread_create");

        if(proj.getModules().isModuleEnabled ("juce_audio_devices"))
          //writeLibraryCheck(out, "asound", "snd_pcm_open");
           writeIncludeFind(out, "ALSA", true);
  
        writeLinkLibraries(out, linuxLibs);

        out << "endif()" << newLine;
        out << newLine;
    }

    //==============================================================================
    void writeWindowsChecks(OutputStream& out, Project& proj)  const
    {
        StringArray libs = mingwLibs;

        out << "if(WIN32)" << newLine;
        out << "  # --- Add winsock, winmm and other required libraries on Windows systems -------------" << newLine;

        if(proj.getModules().isModuleEnabled ("juce_core")) {
             libs.addIfNotAlreadyThere("shlwapi");
             libs.addIfNotAlreadyThere("version");
             libs.addIfNotAlreadyThere("wininet");
             libs.addIfNotAlreadyThere("winmm");
             libs.addIfNotAlreadyThere("ws2_32");
        }
       
        if(proj.getModules().isModuleEnabled ("juce_gui_basics")) {
             libs.addIfNotAlreadyThere("imm32");
        }
  
        if(proj.getModules().isModuleEnabled ("juce_opengl"))
             libs.addIfNotAlreadyThere("opengl32");

        if(proj.getModules().isModuleEnabled ("juce_audio_devices"))
          libs.addIfNotAlreadyThere("winmm");
        
         writeLinkLibraries(out, libs);

        out << "endif()" << newLine;
        out << newLine;
    }

    //==============================================================================
    void writeMakefile(OutputStream& out, const Array<RelativePath>& sourceFiles, const Array<RelativePath>& libFiles) const
    {
        bool isLibrary = (projectType.isStaticLibrary() || projectType.isDynamicLibrary());
        StringPairArray vars;
        StringPairArray extraDefinitions;
        StringArray extraIncludePaths;
         String libraryType, buildType;
        RelativePath projectFile = rebaseFromProjectFolderToBuildTarget (RelativePath (getProject().getFile().getFileName(), RelativePath::projectFolder));
  
        StringPairArray targetProperties;
  
        ConstConfigIterator configIt(*this);
  
        String targetFileName, targetName = projectName;

        targetProperties.set("LINKER_LANGUAGE", "CXX");

        if(projectType.isAudioPlugin())
          targetProperties.set("PREFIX", "\"\"");
  
        if (configIt.next())
        {
            targetName = (*configIt).getTargetBinaryNameString();

            if(targetName.endsWith("_d"))
              targetName = targetName.substring(0, targetName.length() - 2);
        }

        targetName = targetName.replace(" ", "_");
  
        libraryType = (projectType.isDynamicLibrary() && !targetName.startsWith("lib") || projectType.isAudioPlugin()) ? " MODULE" : "";
        buildType = isLibrary ? libraryType : "EXE";
        
        out << "# Automatically generated CMakeLists.txt, created by the Introjucer" << newLine
            << "# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!" << newLine
            << newLine;
  
        out << "cmake_minimum_required(VERSION 2.8)" << newLine;
        out << newLine;
  
        out << "project(" << addQuotesIfContainsSpaces(projectName)  << " C CXX)" << newLine;
        out << newLine;

        out << "add_custom_command(OUTPUT \"${CMAKE_SOURCE_DIR}/CMakeLists.txt\"" << newLine 
            << "    COMMAND Introjucer --resave " << addQuotesIfContainsSpaces(projectFile.getFileName()) << newLine
            << "    MAIN_DEPENDENCY \"${CMAKE_SOURCE_DIR}/" <<  projectFile.toUnixStyle() << "\"" <<  newLine
            << "    WORKING_DIRECTORY \"${CMAKE_SOURCE_DIR}/" <<  projectFile.getParentDirectory().toUnixStyle() << "\"" << newLine
            << "    COMMENT \"Regenerating CMakeLists.txt from " << projectFile.getFileName() << "\"" << newLine
            << ")" << newLine;
        out << newLine;
        
        out << "if(NOT DEFINED CONFIG)" << newLine;
        out << "  set(CONFIG \"Debug\" CACHE STRING \"Configuration, either Release or Debug\")" << newLine;
        out << "endif()" << newLine;
        out << newLine;

        out << "set(JUCE_LIBRARY \"\" CACHE FILEPATH \"External built JUCE library\")" << newLine;
        out << "set(STATIC_LINK OFF CACHE BOOL \"Build static " << (isLibrary ? "library" : "binary") << "\")" << newLine;
        out << newLine;
        
        if(!isLibrary) {
            out << "if(STATIC_LINK)" << newLine
            << "  set(CMAKE_" << buildType << "_LINKER_FLAGS \"-static\")" << newLine
            << "endif()" << newLine;
            out << newLine;
        }
  
        out << "if(DEFINED CONFIG)" << newLine
            << "  set(CMAKE_BUILD_TYPE \"${CONFIG}\")" << newLine
            << "else()" << newLine
            << "  set(CMAKE_BUILD_TYPE \"Debug;Release\")" << newLine
            << "endif()" << newLine
            << newLine;

        out << "if(NOT CMAKE_BUILD_TYPE)" << newLine
            << "  set(CMAKE_BUILD_TYPE " << escapeSpaces(getConfiguration(0)->getName()) << ")" << newLine
            << "endif()" << newLine;
        out << newLine;
  
        out << "if(MINGW)" << newLine
            << "  add_definitions(-D__MINGW__=1)" << newLine
            << "elseif(UNIX)" << newLine
            << "  add_definitions(-DLINUX=1)" << newLine
            << "elseif(MSVC)" << newLine
            << "  add_definitions(-D_WINDOWS=1)" << newLine
            << "endif()" << newLine;
        out << newLine;
  
        out << "if(WIN32)" << newLine
            << "  add_definitions(-DWIN32=1)" << newLine
            << "endif()" << newLine;
        out << newLine;

        vars = getDefaultVars(extraDefinitions, extraIncludePaths);
        
        if (isLibrary) {
            out << "if(NOT DEFINED BUILD_SHARED_LIBS)" << newLine;
            out << "  set(BUILD_SHARED_LIBS " << ((makefileIsDLL || !projectType.isStaticLibrary()) ? "ON" : "OFF") << ")" << newLine;
            out << "endif()" << newLine;
            out << newLine;
        }

        StringArray varNames = vars.getAllKeys();
  
        for (int i = 0; i < varNames.size(); ++i)
        {
            const String& key = varNames[i];

            out << "set(" << key << " " << addQuotesIfContainsSpaces(vars[key]) << ")" << newLine;
        }
  
        out << newLine;

        if (isLibrary)
        {
            out << "# -- Setup up proper library directory  -----------" << newLine;
            out << "get_property(LIB64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)" << newLine;
            out << newLine;
            out << "if(\"${LIB64}\" STREQUAL \"TRUE\")" << newLine;
            out << "  set(LIB_PATH_NAME lib64)" << newLine;
            out << "else()" << newLine;
            out << "  set(LIB_PATH_NAME lib)" << newLine;
            out << "endif()" << newLine;
            out << newLine;
            
            targetProperties.set("PREFIX", "0.0");
            targetProperties.set("VERSION", "0.0");
        }
  
        writeLinuxChecks(out, const_cast<CMakeProjectExporter*>(this)->getProject());
        writeWindowsChecks(out, const_cast<CMakeProjectExporter*>(this)->getProject());

        out << "# --- Check for Freetype library -------------" << newLine;
        writeIncludeFind(out, "Freetype", false);
        out << newLine;

        if (project.isConfigFlagEnabled("JUCE_USE_CURL"))
        {
            out << "# --- Check for libcurl -------------" << newLine;
            writeIncludeFind(out, "CURL");
            out << newLine;
        }
  
        out << newLine;

        out << "set(LIBSOURCES";
        writeSources(out, libFiles);
        out << ")" << newLine;
        
        out << "set(SOURCES";
        writeSources(out, sourceFiles);
        out << ")" << newLine;
        
        out << "if(JUCE_LIBRARY)" << newLine
            << "  link_libraries(${JUCE_LIBRARY})" << newLine
            << "else()" << newLine
            << "  set(SOURCES ${LIBSOURCES} ${SOURCES})" << newLine
            << "endif()" << newLine;
        out << newLine;

        for(ConstConfigIterator config(*this); config.next();)
            writeConfig(out, *config, targetName, extraDefinitions, extraIncludePaths, targetProperties);
   
        if(isLibrary || projectType.isAudioPlugin())
            out << "add_library(" << addQuotesIfContainsSpaces(targetName) << libraryType;
        else
            out << "add_executable(" << addQuotesIfContainsSpaces(targetName);

        out << " ${SOURCES}" << newLine;

        out << ")" << newLine;
  
        StringArray propNames = targetProperties.getAllKeys();
  
        out << "set_target_properties("  << addQuotesIfContainsSpaces(targetName) << " PROPERTIES" << newLine;

        for(int i = 0; i < propNames.size(); ++i)
        {
            const String& key = propNames[i];
            out << "    " << key << " " << targetProperties[key].quoted() << newLine;
        }
  
        out << ")" << newLine;
        out << newLine;
     /*   
         out << "if(CMAKE_BUILD_TYPE STREQUAL \"Debug\")" << newLine
             << "  set_target_properties(" << addQuotesIfContainsSpaces(targetName) << " PROPERTIES OUTPUT_NAME \"${TARGET_NAME_DEBUG}\")" <<  newLine
             << "endif()" << newLine;
       */  

        
        out << newLine;
        
        out << "# Check if the last path components begins with 'Juce', if so, assume no bin/lib-subdirectories." << newLine
            << "string(REGEX MATCH \".*[\\\\/][Jj][Uu][Cc][Ee][^/\\\\]*$\" INSTDIR \"${CMAKE_INSTALL_PREFIX}\")" << newLine;
            
        out << "if(INSTDIR STREQUAL \"\")"  << newLine
            << "  set(INSTDIR \"${CMAKE_INSTALL_PREFIX}/" << (isLibrary ? "${LIB_PATH_NAME}" : "bin") << "\")"  << newLine
            << "endif()" << newLine;
        out << newLine;
        
        out << "install(TARGETS " << addQuotesIfContainsSpaces(targetName) << " DESTINATION \"${INSTDIR}\"" << ")" << newLine;
    }

    //==============================================================================
    String getArchFlags(const BuildConfiguration& config) const
    {
        if(const CMakeBuildConfiguration* makeConfig = dynamic_cast<const CMakeBuildConfiguration*>(&config))
            if(!makeConfig->getArchitectureTypeVar().isVoid())
                return makeConfig->getArchitectureTypeVar();

        return ""; //-march=native";
    }

    //==============================================================================
    void initialiseDependencyPathValues(TargetOS::OS os)
    {
        vst2Path.referTo(Value(new DependencyPathValueSource(getSetting(Ids::vstFolder),
                                                             Ids::vst2Path,
                                                             os)));

        vst3Path.referTo(Value(new DependencyPathValueSource(getSetting(Ids::vst3Folder),
                                                             Ids::vst3Path,
                                                             os)));
    }


    JUCE_DECLARE_NON_COPYABLE(CMakeProjectExporter)
};


