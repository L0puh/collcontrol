BUILDDIR=build
IS_VERBOSE=0

while getopts "v" opt; do
 case $opt in
 v)
   echo " + verbose is on"
   IS_VERBOSE=1
   ;;
 esac
done

if [ ! -d "$BUILDDIR" ]; then
   echo " + creating build folder..."
   mkdir -p build
   if [ $IS_VERBOSE == 1 ]; then
      cmake ..
   else 
      echo " + verbose is off"
      cmake .. >> /dev/null
   fi
   cd build

else 
   echo " + verbose is off"
   echo " + build folder exists"
   cd build
fi
echo " + running collcontrol "
if [ $IS_VERBOSE == 1 ]; then
   make
   cd ..
   ./build/collcontrol 
else
   make >> /dev/null
   cd ..
   ./build/collcontrol >> /dev/null
fi

