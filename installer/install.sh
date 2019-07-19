#!/bin/sh
echo "Cleaning data..."
rm data/highscores
echo "Installing CubiColor..."
mkdir -p $HOME/.cubigames/cubicolor
app=$HOME/.cubigames/cubicolor
echo "Copying files..."
cp -r data/* $app/
cp cubicolor.desktop $app/
echo "Patching play.sh ..."
echo "#!/bin/sh\napp=$app$(cat $app/play.sh)" > $app/play.sh
echo "Patching cubicolor.desktop"
echo "$(cat $app/cubicolor.desktop)\nicon=$app/cubicolor.png" > $app/cubicolor.desktop
echo "Adding command..."
sudo unlink /usr/local/bin/cubicolor 
sudo ln -s $app/play.sh /usr/local/bin/cubicolor 
echo "Creating desktop shortcut..."
#Find desktop dir
desktop=$(xdg-user-dir DESKTOP)
cp $app/cubicolor.desktop $desktop/
echo "CubiColor installed!"
