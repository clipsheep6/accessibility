#! /bin/bash
for file in $(find -name "*.gn")
do
echo $file
~/component_code/prebuilts/build-tools/linux-x86/bin/gn format $file
done
