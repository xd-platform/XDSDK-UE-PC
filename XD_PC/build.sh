#!/bin/bash
export LANG="en_US.UTF-8"

PROJECT_DIR=$(cd "$(dirname $0)";pwd)
UEVersion="UE_4.27"
ProjectName="XD_PC"
PRODUCT_DIR=${PROJECT_DIR}/Product

#-ue4exe="C:\Program Files\Epic Games\UE_4.27\Engine\Binaries\Win64\UE4Editor-Cmd.exe"

doBuild() {
    sh /Users/Shared/Epic\ Games/${UEVersion}/Engine/Build/BatchFiles/RunUAT.sh \
    BuildCookRun \
    -nocompileeditor \
    -installed \
    -nop4 \
    -project=${PROJECT_DIR}/${ProjectName}.uproject \
    -cook \
    -stage \
    -archive \
    -archivedirectory=${PRODUCT_DIR} \
    -package \
    -ue4exe=/Users/Shared/Epic\ Games/${UEVersion}/Engine/Binaries/Mac/UE4Editor.app/Contents/MacOS/UE4Editor \
    -compressed \
    -ddc=InstalledDerivedDataBackendGraph \
    -pak -prereqs -nodebuginfo \
    -targetplatform=$1 \
    -build \
    -target=${ProjectName} \
    -clientconfig=$2 \
    -utf8output
}

#cleanPlugin() {
#    cd $1
#    rm -rf Binaries
#    rm -rf Intermediate
#    cd ..
#    cd ..
#}



rm -rf $PRODUCT_DIR
#mkdir -p $PRODUCT_DIR

#Win32,Win64,HoloLens,Mac,XboxOne,PS4,IOS,Android,HTML5,Linux,LinuxAArch64,AllDesktop,TVOS,Switch,Lumin
#'Debug', 'DebugGame', 'Development', 'Shipping', 'Test'
doBuild Win64 Shipping



