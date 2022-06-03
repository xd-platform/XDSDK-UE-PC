#!/bin/sh

cyanPrint() {
    echo -e "\033[0;36m$1\033[0;m"
}

greenPrint() {
    echo -e "\033[0;32m$1\033[0;m"
}

redPrint() {
    echo -e "\033[0;31m$1\033[0;m"
}

# 脚本所在路径
ShellPath=$(cd "$(dirname $0)";pwd)
# 项目路径
ProjectPath=$ShellPath/XD_PC

cyanPrint "删除Binaries"
rm -rf $ProjectPath/Binaries

cyanPrint "删除Intermediate"
rm -rf $ProjectPath/Intermediate

cyanPrint "删除Saved"
rm -rf $ProjectPath/Saved

cyanPrint "删除DerivedDataCache"
rm -rf $ProjectPath/DerivedDataCache

cyanPrint "开始删除Plugins缓存"


list=`find $ProjectPath/Plugins -name "Binaries"`
for path in $list
do
    cyanPrint "删除$path"
done

list=`find $ProjectPath/Plugins -name "Intermediate"`
for path in $list
do
    cyanPrint "删除$path"
done


#zip -q -r XDGSDK-UE4_$1.zip Plugins/
