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
        return false;
    }
    bool launchProject() override
    {
        return false;
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
        Array<RelativePath> files;
        for (int i = 0; i < getAllGroups().size(); ++i)
            findAllFilesToCompile(getAllGroups().getReference(i), files);

        MemoryOutputStream mo;
        writeMakefile(mo, files);

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
    void findAllFilesToCompile(const Project::Item& projectItem, Array<RelativePath>& results) const
    {
        if (projectItem.isGroup())
        {
            for (int i = 0; i < projectItem.getNumChildren(); ++i)
                findAllFilesToCompile(projectItem.getChild(i), results);
        }
        else
        {
            if (projectItem.shouldBeCompiled())
                results.add(RelativePath(projectItem.getFile(), getTargetFolder(), RelativePath::buildTargetFolder));
        }
    }

    void writeDefineFlags(OutputStream& out, const BuildConfiguration& config, const StringPairArray& extraDefs) const
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

        {
            String defs = createGCCPreprocessorFlags(mergePreprocessorDefs(defines, getAllPreprocessorDefs(config))).trimStart();
            if (defs.length() > 0)
                out << "  add_definitions(" << defs << ")" << newLine;
        }
    }

    void writeHeaderPathFlags(OutputStream& out, const BuildConfiguration& config, const StringArray& extraIncPaths) const
    {
        StringArray searchPaths(extraSearchPaths);


        searchPaths.add(RelativePath(project.getGeneratedCodeFolder(),
                                     getTargetFolder(), RelativePath::buildTargetFolder).toUnixStyle());

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

    void writePathList(OutputStream& out,  const BuildConfiguration& config, const StringArray& list, const String& indent = "") const
    {
        out << indent;
        for (int i = 0; i < list.size(); ++i)
            out << "    " << addQuotesIfContainsSpaces(FileHelpers::unixStylePath(replacePreprocessorTokens(config, list[i]))) << newLine << indent;

    }

    void writeCppFlags(OutputStream& out, const BuildConfiguration& config, const StringPairArray& extraDefs, const StringArray& extraIncPaths) const
    {
        writeDefineFlags(out, config, extraDefs);
        writeHeaderPathFlags(out, config, extraIncPaths);
    }

    void writeLinkLibraries(OutputStream& out) const
    {
        const StringArray& libs = isWindows() ? mingwLibs : (isOSX() ? xcodeLibs : linuxLibs);
        StringArray libraries;

        for (int i = 0; i < libs.size(); ++i)
        {
            String libName = libs[i];

            if (libName.startsWith("-l"))
                libName = libName.substring(2);

            libraries.add(addQuotesIfContainsSpaces(libName));
        }

        /*        if (getProject().isConfigFlagEnabled("JUCE_USE_CURL"))
                    libraries.add("${CURL_LIBRARIES}");

        				libraries.add("${FREETYPE_LIBRARIES}");*/

        libraries.addTokens(getExternalLibrariesString(), ";", "\"'");
        libraries.removeEmptyStrings();

        if (libraries.size() > 0)
        {
            out << "link_libraries(" << libraries.joinIntoString(" ") << ")" << newLine;
        }
    }

    void writeLinkDirectories(OutputStream& out, const BuildConfiguration& config) const
    {
        StringArray libraryDirs;

        for (const String& s : StringArray::fromTokens(config.getGCCLibraryPathFlags(), " ;", "\"'"))
        {
            if (s.startsWith("-L"))
                libraryDirs.add(s.substring(2).quoted());
        }

        {
            String linkDirs =  getCleanedStringArray(libraryDirs).joinIntoString(" ").trimStart();

            if (linkDirs.length() > 0)
                out << "  link_directories(" << linkDirs << ")" << newLine;
        }

    }

    void writeCompileFlags(OutputStream& out, const BuildConfiguration& config, const StringArray& flags) const
		{
        StringArray compileFlags = flags;
				
        if(config.isDebug())
        {
            compileFlags.add("-g");
            compileFlags.add("-gdb");
        }

        compileFlags.add("-O" + config.getGCCOptimisationFlag());
 
        if (compileFlags.size() == 0)
				  return;

				out  << "set(CMAKE_CXX_FLAGS_"  << config.getName().toUpperCase() <<  " " << getCleanedStringArray(compileFlags).joinIntoString(" ") << ")" << newLine;
		}


    void writeLinkerFlags(OutputStream& out, const BuildConfiguration& config) const
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
                out  << "set(LINK_FLAGS_"  << config.getName().toUpperCase() << " " << linkFlags.quoted() << ")" << newLine;
        }
    }

    void writeConfig(OutputStream& out, const BuildConfiguration& config) const
    {
        const String buildDirName("build");
        const String intermediatesDirName(buildDirName + "/intermediate/" + config.getName());
        String outputDir(buildDirName);
        StringArray compileFlags, extraIncludePaths, extraCompileFlags = StringArray::fromTokens(replacePreprocessorTokens(config, getExtraCompilerFlagsString()), "; ", "\"'"); 
		    StringPairArray extraDefinitions;
				String cppStandardToUse = getCppStandardString();

        if(cppStandardToUse.isEmpty())
            cppStandardToUse = "-std=c++11";

        compileFlags.add(cppStandardToUse);

				//for(auto t : StringArray::fromTokens(replacePreprocessorTokens(config, getExtraCompilerFlagsString()), "; ", "\"'")) {
					//
        for (int i = 0; i < extraCompileFlags.size(); ++i)
				{
					const String& t = extraCompileFlags[i];

          String arg;

					std::cerr << "extraCompilerFlags arg: " << t << std::endl;

					if(t.startsWith("-")) {
					  if(t.length() > 2)
							arg = t.substring(2);
						else
						  arg = extraCompileFlags[++i];

						if(t.startsWith("-I")) {
							RelativePath includePath(arg, RelativePath::projectFolder);
							extraIncludePaths.add(includePath.toUnixStyle());
			// BUG:				addToExtraSearchPaths(includePath);
							continue;
						}
						if(t.startsWith("-D")) {
							extraDefinitions.set(
								arg.upToFirstOccurrenceOf("=", false, false), 
								arg.fromFirstOccurrenceOf("=", false, false)
							);
							continue;
						}
					}
				  compileFlags.add(t);
				}

					std::cerr << "compileFlags arg: " << compileFlags.joinIntoString("|") << std::endl;

        out << "# Configuration: " << config.getName() << newLine;

        if (config.getTargetBinaryRelativePathString().isNotEmpty())
        {
            RelativePath binaryPath(config.getTargetBinaryRelativePathString(), RelativePath::projectFolder);
            outputDir = binaryPath.rebased(projectFolder, getTargetFolder(), RelativePath::buildTargetFolder).toUnixStyle();
        }

        out << "if(CMAKE_BUILD_TYPE STREQUAL " << config.getName().quoted() << ")" << newLine;

        writeCppFlags(out, config, extraDefinitions, extraIncludePaths);

        if (makefileIsDLL)
            out << "  set(BUILD_SHARED_LIBS ON)" << newLine;

        writeLinkDirectories(out, config);

        out << "endif()" << newLine;
        out << newLine;

        writeCompileFlags(out, config, compileFlags);
        writeLinkerFlags(out, config);

        String targetName(replacePreprocessorTokens(config, config.getTargetBinaryNameString()));

        if (projectType.isStaticLibrary() || projectType.isDynamicLibrary())
            targetName = getLibbedFilename(targetName);
        else
            targetName = targetName.upToLastOccurrenceOf(".", false, false) + makefileTargetSuffix;

        if (targetName != projectName)
            out << "set_target_properties(" <<   addQuotesIfContainsSpaces(projectName)
                << " PROPERTIES"
                << " OUTPUT_NAME_" << config.getName().toUpperCase() << " " << addQuotesIfContainsSpaces(targetName)
                << ")" << newLine;

        out << "# End of configuration: " << config.getName() << newLine;
        out << newLine;
    }

    void writeSources(OutputStream& out, const Array<RelativePath>& files) const
    {
        out << newLine;

        for (int i = 0; i < files.size(); ++i)
            if (shouldFileBeCompiledByDefault(files.getReference(i)))
                out << "    " << addQuotesIfContainsSpaces(files.getReference(i).toUnixStyle()) << newLine ;
    }

    void writeIncludeFind(OutputStream& out, const String& libraryName, bool required = false, const String& indent = "") const
    {
        String varName = libraryName.toUpperCase();

        out << indent << "include(Find"+libraryName+")" << newLine
            << indent << "if(" << varName << "_FOUND)" << newLine
            << indent << "  message(STATUS \"Found " << libraryName << " library version ${" << varName << "_VERSION_STRING}\")" << newLine;

        out << indent << "  include_directories(${" << varName << "_INCLUDE_DIRS})" << newLine;
        out << indent << "  link_libraries(${" << varName << "_LIBRARIES})" << newLine;

        if (required)
        {
            out << indent << "else()" << newLine
                << indent << "  message(FATAL_ERROR \"" << libraryName << " library not found!\")" << newLine;
        }
        else
        {
            out << indent << "  add_definitions(-DHAVE_" << varName << "=1)" << newLine;
        }

        out << indent << "endif()" << newLine;
    }

    void writeMakefile(OutputStream& out, const Array<RelativePath>& files) const
    {
        out << "# Automatically generated CMakeLists.txt, created by the Introjucer" << newLine
            << "# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!" << newLine
            << newLine;

        out << "cmake_minimum_required(VERSION 2.6)" << newLine;

        out  << newLine;

        out << "project(" <<   addQuotesIfContainsSpaces(projectName)  << ")"
            << newLine;

        out  << newLine;

        out << "if(NOT CMAKE_BUILD_TYPE)" << newLine
            << "  set(CMAKE_BUILD_TYPE " << escapeSpaces(getConfiguration(0)->getName()) << ")" << newLine
            << "endif()" << newLine;

        out  << newLine;

        out << "if(MINGW)" << newLine
            << "  add_definitions(-D__MINGW__=1)" << newLine
            << "elseif(UNIX)" << newLine
            << "  add_definitions(-DLINUX=1)" << newLine
            << "elseif(MSVC)" << newLine
            << "  add_definitions(-D_WINDOWS=1)" << newLine
            << "endif()" << newLine;

        out << "if(WIN32)" << newLine
            << "  add_definitions(-DWIN32=1)" << newLine
            << "endif()" << newLine;

        out  << newLine;

        if (projectType.isStaticLibrary() || projectType.isDynamicLibrary())
            out << "add_library(" << addQuotesIfContainsSpaces(projectName) << (projectType.isStaticLibrary() ? " STATIC" : " SHARED");
        else
            out << "add_executable(" << addQuotesIfContainsSpaces(projectName);

        writeSources(out, files);
        out << ")" << newLine;

        out << newLine;

        out << "if(UNIX)" << newLine;
        writeIncludeFind(out, "X11", true, "  ");
        out << "endif(UNIX)" << newLine;

			  out << "include(CheckLibraryExists)" << newLine;
				out << "check_library_exists(dl dlopen \"\" HAVE_LIBDL)" << newLine;
				out << "if(HAVE_LIBDL)" << newLine;
				out << "  link_libraries(dl)" << newLine ;
				out << "endif()" << newLine;

        writeIncludeFind(out, "Freetype", true);
        out  << newLine;

        if (getProject().isConfigFlagEnabled("JUCE_USE_CURL"))
        {
            writeIncludeFind(out, "CURL");
            out  << newLine;
        }

        for (ConstConfigIterator config(*this); config.next();)
            writeConfig(out, *config);

        writeLinkLibraries(out);

    }

    String getArchFlags(const BuildConfiguration& config) const
    {
        if (const CMakeBuildConfiguration* makeConfig = dynamic_cast<const CMakeBuildConfiguration*>(&config))
            if (! makeConfig->getArchitectureTypeVar().isVoid())
                return makeConfig->getArchitectureTypeVar();

        return ""; //-march=native";
    }

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


