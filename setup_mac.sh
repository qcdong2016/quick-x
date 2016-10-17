#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BASENAME=`basename "$DIR"`
if [ "$BASENAME" = "Resources" ]; then
    DIR=`dirname "$DIR"`
    DIR=`dirname "$DIR"`
    DIR=`dirname "$DIR"`
elif [ "$BASENAME" = "bin" ]; then
    DIR=`dirname "$DIR"`
fi

if [ "$2" != "" ]; then
    CALL_BY_PKG=YES
    DIR=$2
fi;

QUICKX_ROOT="$DIR"

echo ""
echo "QUICKX_ROOT = \"$QUICKX_ROOT\""
echo ""

# set Xcode
defaults write com.apple.dt.Xcode IDEApplicationwideBuildSettings -dict-add QUICKX_ROOT "$QUICKX_ROOT"
defaults write com.apple.dt.Xcode IDESourceTreeDisplayNames -dict-add QUICKX_ROOT QUICKX_ROOT
IDEApplicationwideBuildSettings=`defaults read com.apple.dt.Xcode IDEApplicationwideBuildSettings`
IDESourceTreeDisplayNames=`defaults read com.apple.dt.Xcode IDESourceTreeDisplayNames`

echo "> Xcode settings updated."

# set quick player
defaults write org.cocos.quick.player QUICKX_ROOT "$QUICKX_ROOT"
echo "> quick player settings updated."

# set .bash_profile or .profile
if [ -f ~/.bash_profile ]; then
PROFILE_NAME=~/.bash_profile
else
PROFILE_NAME=~/.profile
fi

sed -e '/QUICKX_ROOT/d' $PROFILE_NAME | sed -e '/add by quick-cocos2d-x setup/d' > $PROFILE_NAME.tmp

DATE=`date "+DATE: %Y-%m-%d TIME: %H:%M:%S"`
echo "# add by quick-cocos2d-x setup, $DATE" >> $PROFILE_NAME.tmp
echo "export QUICKX_ROOT=\`cat ~/.QUICKX_ROOT\`" >> $PROFILE_NAME.tmp

DATE=`date "+%Y-%m-%d-%H%M%S"`
cp $PROFILE_NAME $PROFILE_NAME-$DATE.bak
cp $PROFILE_NAME.tmp $PROFILE_NAME
rm $PROFILE_NAME.tmp

echo "> $PROFILE_NAME updated."
echo "$QUICKX_ROOT" > ~/.QUICKX_ROOT
echo "> ~/.QUICKX_ROOT updated."
echo ""

if [ "$CALL_BY_PKG" != "" ]; then

    $QUICKX_ROOT/bin/install_luajit.sh
    ln -s $QUICKX_ROOT/player/mac/player.app $QUICKX_ROOT/player.app

# else

#     while true; do
#         read -p "Do you wish to install LuaJIT (Y/N) ? " yn
#         case $yn in
#             [Yy]* ) echo ""; $QUICKX_ROOT/bin/install_luajit.sh; break;;
#             [Nn]* ) exit;;
#             * ) echo "Please answer yes or no.";;
#         esac
#     done

fi;

echo ""
echo ""

echo "done."
echo ""

