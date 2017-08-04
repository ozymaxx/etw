# ETW - Eat the Whistle
This is a slight modification of [Eat the Whistle](http://www.ggsoft.org/etw/), an open source soccer game. This version saves video frames together with motion data of the ball and players. You can put the frames in a video to re-play a game.

## Output
All the data is put in a directory whose name is a timestamp. This directory contains the following:
* **Frames**: All video frames in the game, in bmp format
* **Ball motion data**: Same as the directory name, a list of tuples `(ballxcoord,ballycoord,team_number,player_number,timestamp,isrighthalf_of_field)`
* **Player motion data**: Each in the following format `samename_as_directory-teamnumber-playernumber`, containing a list of tuples `(playerxcoord,playerycoord,timestamp,isrighthalf_of_field)`

## Credits
Ozan Can Altıok - oaltiok15 at ku dot edu dot tr - [Koç University Intelligent User Interfaces Laboratory](http://iui.ku.edu.tr)
