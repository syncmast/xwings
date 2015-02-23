##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=progtest
ConfigurationName      :=Debug
WorkspacePath          := "/home/user/Projects/xwings"
ProjectPath            := "/home/user/Projects/xwings/examples/progtest"
IntermediateDirectory  :=$(WorkspacePath)lib/obj
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=user
Date                   :=07.10.2013
CodeLitePath           :="/home/user/.codelite"
LinkerName             :=g++
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(WorkspacePath)bin/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="progtest.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch)$(WorkspacePath)/include $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)stdctrls $(LibrarySwitch)system $(LibrarySwitch)xgdi $(LibrarySwitch)imgfmt $(LibrarySwitch)xexts 
ArLibs                 :=  "stdctrls" "system" "xgdi" "imgfmt" "xexts" 
LibPath                := $(LibraryPathSwitch)$(WorkspacePath)/lib 

##
## Common variables
## AR, CXX, CC, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := g++
CC       := gcc
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
LD_LIBRARY_PATH:=/home/user/Projects/xwings/lib
Objects0=$(IntermediateDirectory)/MainTest$(ObjectSuffix) $(IntermediateDirectory)/TestForm$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d "../../.build-debug/system" "../../.build-debug/xgdi" "../../.build-debug/imgfmt" "../../.build-debug/stdctrls" "../../.build-debug/xexts" $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

"../../.build-debug/system":
	@$(MakeDirCommand) "../../.build-debug"
	@echo stam > "../../.build-debug/system"


"../../.build-debug/xgdi":
	@$(MakeDirCommand) "../../.build-debug"
	@echo stam > "../../.build-debug/xgdi"


"../../.build-debug/imgfmt":
	@$(MakeDirCommand) "../../.build-debug"
	@echo stam > "../../.build-debug/imgfmt"


"../../.build-debug/stdctrls":
	@$(MakeDirCommand) "../../.build-debug"
	@echo stam > "../../.build-debug/stdctrls"


"../../.build-debug/xexts":
	@$(MakeDirCommand) "../../.build-debug"
	@echo stam > "../../.build-debug/xexts"




$(IntermediateDirectory)/.d:
	@test -d $(WorkspacePath)lib/obj || $(MakeDirCommand) $(WorkspacePath)lib/obj

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/MainTest$(ObjectSuffix): MainTest.cpp $(IntermediateDirectory)/MainTest$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/user/Projects/xwings/examples/progtest/MainTest.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/MainTest$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/MainTest$(DependSuffix): MainTest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/MainTest$(ObjectSuffix) -MF$(IntermediateDirectory)/MainTest$(DependSuffix) -MM "MainTest.cpp"

$(IntermediateDirectory)/MainTest$(PreprocessSuffix): MainTest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/MainTest$(PreprocessSuffix) "MainTest.cpp"

$(IntermediateDirectory)/TestForm$(ObjectSuffix): TestForm.cpp $(IntermediateDirectory)/TestForm$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/user/Projects/xwings/examples/progtest/TestForm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/TestForm$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/TestForm$(DependSuffix): TestForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/TestForm$(ObjectSuffix) -MF$(IntermediateDirectory)/TestForm$(DependSuffix) -MM "TestForm.cpp"

$(IntermediateDirectory)/TestForm$(PreprocessSuffix): TestForm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/TestForm$(PreprocessSuffix) "TestForm.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/MainTest$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/MainTest$(DependSuffix)
	$(RM) $(IntermediateDirectory)/MainTest$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/TestForm$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/TestForm$(DependSuffix)
	$(RM) $(IntermediateDirectory)/TestForm$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) "../../.build-debug/progtest"


