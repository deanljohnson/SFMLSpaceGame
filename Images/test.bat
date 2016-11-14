REM magick a10000.png -gravity center -crop 200x140+0+0 a10000b.png
magick a10000.png -gravity center -extent 159x159 a10000.png
magick a10001.png -gravity center -extent 159x159 a10001.png
magick a10002.png -gravity center -extent 159x159 a10002.png
magick a10003.png -gravity center -extent 159x159 a10003.png
magick a10004.png -gravity center -extent 159x159 a10004.png
magick a10005.png -gravity center -extent 159x159 a10005.png
magick a10006.png -gravity center -extent 159x159 a10006.png
magick a10007.png -gravity center -extent 159x159 a10007.png
magick a10008.png -gravity center -extent 159x159 a10008.png
magick a10009.png -gravity center -extent 159x159 a10009.png
magick a10010.png -gravity center -extent 159x159 a10010.png
magick a10011.png -gravity center -extent 159x159 a10011.png
magick a10012.png -gravity center -extent 159x159 a10012.png
magick a10013.png -gravity center -extent 159x159 a10013.png
magick a10014.png -gravity center -extent 159x159 a10014.png
magick a10015.png -gravity center -extent 159x159 a10015.png

REM montage a10000.png a10001.png a10002.png a10003.png a10004.png a10005.png a10006.png a10007.png a10008.png a10009.png a10010.png a10011.png a10012.png a10013.png a10014.png a10015.png -montage -tile 4x4 -geometry 240x180+0+0 -background transparent asteroid-one.png