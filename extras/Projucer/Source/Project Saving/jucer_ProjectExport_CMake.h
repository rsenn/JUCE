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


    enum JuceLinkage
    {
        buildJuceSources = 1,
        buildJuceAsLib = 2,
    };

    enum OptimisationLevel
    {
        optimisationOff = 1,
        optimiseMinSize = 2,
        optimiseMaxSpeed = 3
    };

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

        if(getPackagesString().isEmpty())
            getPackagesValue() = "freetype2";

        initialiseDependencyPathValues(TargetOS::linux);
    }

    //==============================================================================
    bool canLaunchProject() override
    {
        return true;
    }
    bool launchProject() override
    {
        File buildDir(getTargetLocationString());
        ChildProcess proc;
        String output;
        bool success;
        const char* cmakeCmd[] =
        {
            "cmake-gui", "-DCMAKE_VERBOSE_MAKEFILE=TRUE", ".",
            nullptr
        };

        std::cerr << "LOG - build dir: " << buildDir.getFullPathName() << std::endl;
        buildDir.setAsCurrentWorkingDirectory();

        proc.start(StringArray(cmakeCmd), ChildProcess::wantStdOut|ChildProcess::wantStdErr);
        proc.waitForProcessToFinish(30*1000);

        output = proc.readAllProcessOutput();

        success = (proc.getExitCode() == 0);
        /*
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
                }*/

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

    bool isAndroid() const override                        { return false; }
    bool isAndroidAnt() const override                     { return false; }
    bool isAndroidStudio() const override                  { return false; }
    bool isCodeBlocks() const override                     { return false; }
    bool isiOS() const override                            { return false; }
    bool isMakefile() const override                       { return false; }
    bool isVisualStudio() const override                   { return false; }
    bool isXcode() const override                          { return false; }
    bool supportsAAX() const override                      { return true; }
    bool supportsAU() const override                       { return true; }
    bool supportsAUv3() const override                     { return true; }
    bool supportsRTAS() const override                     { return true; }
    bool supportsStandalone() const override               { return true; }
    bool supportsVST() const override                      { return true; }
    bool supportsVST3() const override                     { return true; }
    bool supportsUserDefinedConfigurations() const override { return true; }

   void addPlatformSpecificSettingsForProjectType (const ProjectType& type) override
    {
        if (type.isStaticLibrary())
            makefileTargetSuffix = ".a";

        else if (type.isDynamicLibrary())
            makefileTargetSuffix = ".so";

        else if (type.isAudioPlugin())
            makefileIsDLL = true;
    }

    Value getCppStandardValue() { return getSetting(Ids::cppLanguageStandard); }
    String getCppStandardString() const { return settings[Ids::cppLanguageStandard]; }

    Value getJuceLinkageValue() { return getSetting(Ids::juceLinkage); }
    String getJuceLinkageString() const { return settings[Ids::juceLinkage]; }

    Value getPackagesValue()             { return getSetting (Ids::packages); }
    String getPackagesString() const      { return settings [Ids::packages]; }

    void createExporterProperties(PropertyListBuilder& props) override
    {
        static const char* juceLinkage[] = { "Build JUCE from sources", "Build JUCE as library", 0 };
        const int juceLinkageValues[]     = { buildJuceSources, buildJuceAsLib,  0 };

        props.add(new ChoicePropertyComponent(getJuceLinkageValue(), "Linkage type",
                                              StringArray(juceLinkage),
                                              Array<var> (juceLinkageValues)),
                  "The linkage type for this configuration");



        props.add(new TextPropertyComponent (getPackagesValue(), "Packages", 256, true),
                  "A list pkg-config(1) configured packages to add.");
    }

    //==============================================================================
    void create(const OwnedArray<LibraryModule>&) const override
    {
        Array<RelativePath> sourceFiles, libSourceFiles;

        for (int i = 0; i < getAllGroups().size(); ++i)
        {
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
            if (getWarningLevel() == 0)
                getWarningLevelValue() = 4;

            setValueIfVoid(getLibrarySearchPathValue(), "${CMAKE_SYSTEM_LIBRARY_PATH}");
        }

        Value getWarningLevelValue()
        {
            return getValue(Ids::winWarningLevel);
        }
        int getWarningLevel() const
        {
            return config [Ids::winWarningLevel];
        }

        Value shouldGenerateDebugSymbolsValue()
        {
            return getValue(Ids::alwaysGenerateDebugSymbols);
        }
        bool shouldGenerateDebugSymbols() const
        {
            return config [Ids::alwaysGenerateDebugSymbols];
        }

        Value getUsingRuntimeLibDLL()
        {
            return getValue(Ids::useRuntimeLibDLL);
        }
        String getUsingRuntimeLibDLLString() const
        {
            return config[Ids::useRuntimeLibDLL];
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
            static const char* optimisationLevels[] = { "No optimisation", "Minimise size", "Maximise speed", 0 };
            const int optimisationLevelValues[]     = { optimisationOff, optimiseMinSize, optimiseMaxSpeed, 0 };

            props.add(new ChoicePropertyComponent(getOptimisationLevel(), "Optimisation",
                                                  StringArray(optimisationLevels),
                                                  Array<var> (optimisationLevelValues)),
                      "The optimisation level for this configuration");

            static const char* warningLevelNames[] = { "Low", "Medium", "High", nullptr };
            const int warningLevels[] = { 2, 3, 4 };

            props.add(new ChoicePropertyComponent(getWarningLevelValue(), "Warning Level",
                                                  StringArray(warningLevelNames), Array<var> (warningLevels, numElementsInArray(warningLevels))));


            {
                static const char* runtimeNames[] = { "(Default)", "Use static runtime", "Use DLL runtime", nullptr };
                const var runtimeValues[] = { var(), var(false), var(true) };

                props.add(new ChoicePropertyComponent(getUsingRuntimeLibDLL(), "Runtime Library",
                                                      StringArray(runtimeNames), Array<var> (runtimeValues, numElementsInArray(runtimeValues))));
            }
        }
    };

    BuildConfiguration::Ptr createBuildConfig(const ValueTree& tree) const override
    {
        return new CMakeBuildConfiguration(project, tree, *this);
    }


    static const char* getOptimisationLevelString(int level)
    {
        switch (level)
        {
            case optimiseMaxSpeed:
                return "Full";
            case optimiseMinSize:
                return "MinSpace";
            default:
                return "Disabled";
        }
    }

private:

    //==============================================================================
    void findFilesToCompile(const Project::Item& projectItem, Array<RelativePath>& results, bool libFiles = false) const
    {
        if (projectItem.isGroup())
        {
            for (int i = 0; i < projectItem.getNumChildren(); ++i)
            {
                //                if((projectItem.getID() == "__jucelibfiles") != libFiles) continue;

                findFilesToCompile(projectItem.getChild(i), results, libFiles);
            }
        }
        else
        {
            const File& f = projectItem.getFile();

            if (projectItem.shouldBeCompiled() ||  f.hasFileExtension(headerFileExtensions))
            {
                if (!projectItem.shouldBeAddedToBinaryResources())
                {
                    RelativePath path(f, getTargetFolder(), RelativePath::buildTargetFolder);

                    bool isJuceLibrarySource = (path.toUnixStyle().contains("modules/juce_") /*&& path.toUnixStyle().endsWith(".cpp")*/);

                    if (isJuceLibrarySource == libFiles)
                        results.add(path);
                }
            }
        }
    }

    //==============================================================================
    static void writeAddDefinitions(OutputStream& out, const BuildConfiguration& config,
                                    const StringPairArray& extraDefs)
    {
        StringPairArray defines(extraDefs);

        if (config.isDebug())
        {
            defines.set("DEBUG", "1");
            defines.set("_DEBUG", "1");
        }
        else
        {
            defines.set("NDEBUG", "1");
        }

        if (defines.size() > 0)
            out << "  add_definitions(" << createGCCPreprocessorFlags(defines).trimStart() << ")" << newLine;
    }

    //==============================================================================
    static void writeIncludeDirectories(OutputStream& out, const StringArray& searchPaths)
    {
        if (searchPaths.size())
        {
            out << "  include_directories(" << newLine;
            writePathList(out, getCleanedStringArray(searchPaths), "  ");
            out << ")" << newLine;
        }
    }

    //==============================================================================
    static void writePathList(OutputStream& out, const StringArray& list,
                              const String& indent = "")
    {
        out << indent;

        for (int i = 0; i < list.size(); ++i)
        {
            out << "    "
                << addQuotesIfContainsSpaces(FileHelpers::unixStylePath(list[i]))
                << newLine << indent;
        }
    }

    //==============================================================================
    void writeCppFlags(OutputStream& out, const BuildConfiguration& config,
                       StringPairArray& extraDefs) const
    {
        writeAddDefinitions(out, config, extraDefs);
        writeIncludeDirectories(out, config.getHeaderSearchPaths());
    }

    //==============================================================================
    void writeLinkLibraries(OutputStream& out, const StringArray& libs) const
    {
        StringArray libraries;

        for (int i = 0; i < libs.size(); ++i)
        {
            String libName = libs[i];

            if (libName.startsWith("-l"))
                libName = libName.substring(2);

            libraries.add(addQuotesIfContainsSpaces(libName));
        }

        libraries.addTokens(getExternalLibrariesString(), ";", "\"'");
        libraries.removeEmptyStrings();

        if (libraries.size() > 0)
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
    void writeCompileFlags(OutputStream& out, const BuildConfiguration* config,
                           const StringArray& flags) const
    {
        StringArray compileFlagsGCC, compileFlagsMSVC;

        for (int i = 0; i < flags.size(); ++i)
        {
            if (!flags[i].startsWith("-I") && !flags[i].startsWith("/I"))
            {
                compileFlagsGCC.add(flags[i]);
                compileFlagsMSVC.add(flags[i]);
            }
        }

        String cppStandardToUse = getCppStandardString();

        if (cppStandardToUse.isEmpty())
            cppStandardToUse = "-std=c++11";

        compileFlagsGCC.add(cppStandardToUse);

        if (config)
        {
            if (config->isDebug())
            {
                compileFlagsGCC.add("-g");
                compileFlagsGCC.add("-ggdb");
                compileFlagsGCC.add("-O0");

                //compileFlagsMSVC.add("/Zi"); // debug symbols
                compileFlagsMSVC.add("/ZI"); // edit&continue info
                compileFlagsMSVC.add("/FdDebug"); // PDB file name

                compileFlagsMSVC.add("/Od");
                compileFlagsMSVC.add("/D");
                compileFlagsMSVC.add("/MTd");
            }
            else
            {
                compileFlagsGCC.add("-O"  + config->getGCCOptimisationFlag());
                compileFlagsGCC.add("-fomit-frame-pointer");

                compileFlagsMSVC.add("/O" + config->getGCCOptimisationFlag());
                compileFlagsMSVC.add("/MT");
            }
        }
        else
        {

            compileFlagsGCC.add("-fexceptions");
            compileFlagsGCC.add("-frtti");

            compileFlagsMSVC.add("/GL");          // link-time code-gen
            compileFlagsMSVC.add("/GS");          // enable security checks
            compileFlagsMSVC.add("/analyze-");    // Disable native analysis
            compileFlagsMSVC.add("/W2");          // Warning level 2
            compileFlagsMSVC.add("/Zc:wchar_t");  //  wchar_t is a native type, not a typedef
            compileFlagsMSVC.add("/Gm-");         // disable minimal rebuild
            compileFlagsMSVC.add("/Zc:inline");   // remove unreferenced function
            compileFlagsMSVC.add("/fp:precise");
            compileFlagsMSVC.add("/GR");          // C++ RTTI
            compileFlagsMSVC.add("/Gd");
            compileFlagsMSVC.add("/EHsc");        // enable C++ EH & extern "C" defaults to nothrow
            compileFlagsMSVC.add("/nologo");
        }

        {
            String varSuffix = (config ? (String("_") + config->getName().toUpperCase()) : "");
            out << "if(MSVC)" << newLine
                << "  set(CMAKE_CXX_FLAGS" << varSuffix << " \"${CMAKE_CXX_FLAGS" << varSuffix << "} " << getCleanedStringArray(compileFlagsMSVC).joinIntoString(" ") << "\")" << newLine
                << "else()" << newLine
                << "  set(CMAKE_CXX_FLAGS" << varSuffix << " \"${CMAKE_CXX_FLAGS" << varSuffix << "} " << getCleanedStringArray(compileFlagsGCC).joinIntoString(" ") << "\")" << newLine
                << "endif()" << newLine;
        }

        out << newLine;
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
    StringPairArray getDefaultVars(OutputStream& out, StringPairArray& extraDefs, StringArray& extraIncludes) const
    {
        StringPairArray vars;
        StringArray defaultCompileFlags =  StringArray::fromTokens(getExtraCompilerFlagsString(), "; ", "\"'");

        extraIncludes.insert(0,
                             RelativePath(project.getGeneratedCodeFolder(), getTargetFolder(), RelativePath::buildTargetFolder).toUnixStyle()
                            );

        writeCompileFlags(out, nullptr, defaultCompileFlags);

        for (ConstConfigIterator config(*this); config.next();)
        {
            StringArray configCompileFlags = defaultCompileFlags, compileFlags/*=
		StringArray::fromTokens(replacePreprocessorTokens(*config, getExtraCompilerFlagsString()), "; ", "\"'")*/;

            for (int i = 0; i < configCompileFlags.size(); ++i)
            {
                if (configCompileFlags[i].startsWith("-I") || configCompileFlags[i].startsWith("/I"))
                    compileFlags.add(configCompileFlags[i]);
            }

            for (int i = 0; i < compileFlags.size(); ++i)
            {
                const String& t = compileFlags[i];

                String arg;

                if (t.startsWith("-"))
                {
                    if (t.length() > 2)
                        arg = t.substring(2);
                    else
                        arg = compileFlags[++i];

                    if (t.startsWith("-I"))
                    {
                        RelativePath includePath(arg, RelativePath::projectFolder);
                        extraIncludes.add(includePath.toUnixStyle());
                        continue;
                    }
                    if (t.startsWith("-D"))
                    {
                        StringArray def = StringArray::fromTokens(arg.unquoted(), "=" , "\"");

                        extraDefs.set(def[0], def[1].unquoted());
                        continue;
                    }
                }

                configCompileFlags.insert(0, t);
                std::cerr << "extraCompilerFlags arg: " << t << std::endl;
            }

            writeCompileFlags(out, &(*config), configCompileFlags);
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

        if (fileName.startsWith("lib"))
            fileName = fileName.substring(3);

        if (fileName.endsWith("_dll"))
            fileName = fileName.substring(0, 4);

        if (fileName.endsWith(".so"))
            fileName = fileName.substring(0, fileName.length() - 3);

        if (!config.isDebug() && fileName.endsWith("_d"))
            fileName = fileName.substring(0, fileName.length() - 2);
        //else if(config.isDebug() && !fileName.endsWith("_d"))
        //    fileName = fileName + "_d";

        return fileName;
    }

    //==============================================================================
    void writeConfig(OutputStream& out, const BuildConfiguration& config, const String& targetName,
                     StringPairArray& extraDefs, StringPairArray& targetProps) const
    {
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

        out << "if(CMAKE_BUILD_TYPE MATCHES " << config.getName().quoted() << ")" << newLine;

        writeCppFlags(out, config, extraDefs);
        writeLinkDirectories(out, config);

        out << "endif()" << newLine;

        if (config.isDebug())
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
    static void writeIncludeFind(OutputStream& out, const String& libraryName, bool required = false, const String& haveDef = "")
    {
        String varName = libraryName.toUpperCase();
        String haveDefine = haveDef;

        if (haveDefine.isEmpty())
            haveDefine = "HAVE_" + varName + "=1";

        out << "  include(Find"+libraryName+")" << newLine
            << "  if(" << varName << "_FOUND)" << newLine
            << "    message(STATUS \"Found " << libraryName << " library version ${" << varName << "_VERSION_STRING}\")" << newLine;

        out << "    include_directories(${" << varName << "_INCLUDE_DIRS})" << newLine;
        out << "    link_libraries(${" << varName << "_LIBRARIES})" << newLine;

        if (required)
            out << "  else()" << newLine
                << "    message(FATAL_ERROR \"" << libraryName << " library not found!\")" << newLine;
        else
            out << "    add_definitions(-D" + haveDefine + ")" << newLine;

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
        StringArray libs = linuxLibs;

        out << "elseif(UNIX)" << newLine;
        out << "  # --- Check for X11 and dlopen(3) on UNIX systems -------------" << newLine;
        out << "  include(CheckLibraryExists)" << newLine;
        if (proj.getModules().isModuleEnabled("juce_gui_basics"))
        {
            writeIncludeFind(out, "X11", true);
            out << newLine;

            libs.add("${X11_ICE_LIB}");
            libs.add("${X11_SM_LIB}");
        }

        if (proj.getModules().isModuleEnabled("juce_opengl"))
            writeLibraryCheck(out, "GL", "glXSwapBuffers");

        writeLibraryCheck(out, "dl", "dlopen");
        writeLibraryCheck(out, "pthread", "pthread_create");

        if (proj.getModules().isModuleEnabled("juce_audio_devices"))
            //writeLibraryCheck(out, "asound", "snd_pcm_open");
            writeIncludeFind(out, "ALSA", true);

        writeLinkLibraries(out, libs);

        out << "endif()" << newLine;
        out << newLine;
    }

    //==============================================================================
    void writePackageChecks(OutputStream& out, Project& proj)  const
    {
    }

    //==============================================================================
    void writeWindowsChecks(OutputStream& out, Project& proj)  const
    {
        StringArray libs = mingwLibs;

        out << "if(WIN32)" << newLine;
        out << "  # --- Add winsock, winmm and other required libraries on Windows systems -------------" << newLine;

        if (proj.getModules().isModuleEnabled("juce_core"))
        {
            libs.addIfNotAlreadyThere("shlwapi");
            libs.addIfNotAlreadyThere("version");
            libs.addIfNotAlreadyThere("wininet");
            libs.addIfNotAlreadyThere("winmm");
            libs.addIfNotAlreadyThere("ws2_32");
        }

        if (proj.getModules().isModuleEnabled("juce_gui_basics"))
        {
            libs.addIfNotAlreadyThere("imm32");
        }

        if (proj.getModules().isModuleEnabled("juce_gui_extra"))
        {
            libs.addIfNotAlreadyThere("oleaut32");
        }

        if (proj.getModules().isModuleEnabled("juce_opengl"))
            libs.addIfNotAlreadyThere("opengl32");

        if (proj.getModules().isModuleEnabled("juce_audio_devices"))
            libs.addIfNotAlreadyThere("winmm");

        writeLinkLibraries(out, libs);
    }

    //==============================================================================
    void writeMakefile(OutputStream& out, const Array<RelativePath>& sourceFiles, const Array<RelativePath>& libFiles) const
    {
        bool isLibrary = (projectType.isStaticLibrary() || projectType.isDynamicLibrary());
        StringPairArray vars;
        StringPairArray extraDefinitions;
        StringArray includeDirs;
        String libraryType, buildType;
        RelativePath projectFile = rebaseFromProjectFolderToBuildTarget(RelativePath(getProject().getFile().getFileName(), RelativePath::projectFolder));

        StringPairArray targetProperties;

        ConstConfigIterator configIt(*this);

        String targetFileName, targetName = projectName;

        targetProperties.set("LINKER_LANGUAGE", "CXX");

        if (projectType.isAudioPlugin())
            targetProperties.set("PREFIX", "\"\"");

        if (configIt.next())
        {
            targetName = (*configIt).getTargetBinaryNameString();

            if (targetName.endsWith("_d"))
                targetName = targetName.substring(0, targetName.length() - 2);
        }

        targetName = targetName.replace(" ", "_");

        libraryType = ((projectType.isDynamicLibrary() && !targetName.startsWith("lib")) || projectType.isAudioPlugin()) ? " MODULE" : "";
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

        if (!isLibrary)
        {
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

        out << "if(WIN32)" << newLine
            << "  add_definitions(-DWIN32=1)" << newLine
            << "  if(MSVC)" << newLine
            << "    add_definitions(-D_WINDOWS=1)" << newLine
            << "  elseif(MINGW)" << newLine
            << "    add_definitions(-D__MINGW__=1)" << newLine
            << "  else()" << newLine
            << "    get_property(WIN64 GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS)" << newLine
            << "    if(WIN64)" << newLine
            << "      add_definitions(-D_WIN64=1)" << newLine
            << "    else()" << newLine
            << "      add_definitions(-D_WIN32=1)" << newLine
            << "    endif()" << newLine
            << "  endif()" << newLine
            << "elseif(UNIX)" << newLine
            << "  add_definitions(-DLINUX=1)" << newLine
            << "endif()" << newLine;
        out << newLine;



        vars = getDefaultVars(out, extraDefinitions, includeDirs);

        if (isLibrary)
        {
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

            if (projectType.isAudioPlugin())
                targetProperties.set("PREFIX", "");
        }

        targetProperties.set(String(isLibrary ? "SO" : "") + "VERSION", "0.0");

        writeWindowsChecks(out, const_cast<CMakeProjectExporter*>(this)->getProject());
        writeLinuxChecks(out, const_cast<CMakeProjectExporter*>(this)->getProject());

        out << "# --- Check for Freetype library -------------" << newLine;
        writeIncludeFind(out, "Freetype", false);
        out << newLine;

        if (project.isConfigFlagEnabled("JUCE_USE_CURL"))
        {
            out << "# --- Check for libcurl -------------" << newLine;
            writeIncludeFind(out, "CURL", false, "JUCE_USE_CURL=1");
            out << newLine;
        }

        out << newLine;

        writePackageChecks(out, const_cast<CMakeProjectExporter*>(this)->getProject());

        out << "set(LIBSOURCES";
        writeSources(out, libFiles);
        out << ")" << newLine;

        out << "set(SOURCES";
        writeSources(out, sourceFiles);
        out << ")" << newLine;

        out << "if(JUCE_LIBRARY)" << newLine
            << "  link_libraries(${JUCE_LIBRARY})" << newLine
            << "  if(JUCE_LIBRARY MATCHES \"dll.a\")" << newLine
            << "    add_definitions(-DJUCE_DLL=1)" << newLine
            << "  endif()" << newLine
            << "else()" << newLine
            << "  set(SOURCES ${LIBSOURCES} ${SOURCES})" << newLine
            << "endif()" << newLine;
        out << newLine;

        writeIncludeDirectories(out, includeDirs);

        for (ConstConfigIterator config(*this); config.next();)
            writeConfig(out, *config, targetName, extraDefinitions,  targetProperties);

        if (isLibrary || projectType.isAudioPlugin())
            out << "add_library(" << addQuotesIfContainsSpaces(targetName) << libraryType;
        else
            out << "add_executable(" << addQuotesIfContainsSpaces(targetName);

        out << " ${SOURCES}" << newLine;

        out << ")" << newLine;

        StringArray propNames = targetProperties.getAllKeys();

        out << "set_target_properties("  << addQuotesIfContainsSpaces(targetName) << " PROPERTIES" << newLine;

        for (int i = 0; i < propNames.size(); ++i)
        {
            const String& key = propNames[i];
            out << "    " << key << " " << targetProperties[key].quoted() << newLine;
        }

        out << ")" << newLine;
        out << newLine;

        out << "# Check if the last path components begins with 'Juce', if so, assume no bin/lib-subdirectories." << newLine
            << "string(REGEX MATCH \".*[\\\\/][Jj][Uu][Cc][Ee][^/\\\\]*$\" INSTDIR \"${CMAKE_INSTALL_PREFIX}\")" << newLine;

        out << "if(INSTDIR STREQUAL \"\")"  << newLine
            << "  set(INSTDIR \"${CMAKE_INSTALL_PREFIX}/" << (isLibrary ? "${LIB_PATH_NAME}" : "bin") << "\")"  << newLine
            << "endif()" << newLine;
        out << newLine;

        out << "install(TARGETS " << addQuotesIfContainsSpaces(targetName) << " DESTINATION \"${INSTDIR}\"" << ")" << newLine;
        out << newLine;
    }

    //==============================================================================
    String getArchFlags(const BuildConfiguration& config) const
    {
        if (const CMakeBuildConfiguration* makeConfig = dynamic_cast<const CMakeBuildConfiguration*>(&config))
            if (!makeConfig->getArchitectureTypeVar().isVoid())
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


