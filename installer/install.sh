#!/bin/sh
echo "Cleaning data..."
rm data/highscores
echo "Installing Cubigrav..."
mkdir -p $HOME/.cubigames/cubigrav
app=$HOME/.cubigames/cubigrav
echo "Copying files..."
cp -r data/* $app/
cp cubigrav.desktop $app/
echo "Patching play.sh ..."
echo "#!/bin/sh\napp=$app$(cat $app/play.sh)" > $app/play.sh
echo "Patching cubigrav.desktop"
echo "$(cat $app/cubigrav.desktop)\nicon=$app/cubigrav.png" > $app/cubigrav.desktop
echo "Adding command..."
sudo unlink /usr/local/bin/cubigrav 
sudo ln -s $app/play.sh /usr/local/bin/cubigrav 
echo "Creating desktop shortcut..."
#Find desktop dir
desktop=$(xdg-user-dir DESKTOP)
cp $app/cubigrav.desktop $desktop/
echo "cubigrav installed!"
