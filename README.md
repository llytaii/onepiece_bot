# This program is running the @OnePieceChapterBot Telegram Bot Account. #

What it does:

It fetches onepiece-tube.com every 10 minutes for a new chapter/episode and notifies all saved users about it.

Commands:

/start saves the telegram user id.
/end deletes the saved telegram user id.

Build:

These instructions only work under Linux/WSL 2.

Create the folders res and bin under the projects root folder.

Under /res create these files with the following content (without <>-brackets):

bot.data: <bot-token> <admin-id>

chapter.data: <current_chapter_number>

episode.data: <current_episode_number>

user.data: none


In VS-Code hit Ctrl + Shift + B or run

"/bin/g++ -g ./src/*.cpp -o ./bin/opb -std=c++17 -I ./dep/HTTPRequest -I ./dep/libtelegram/include -pthread -lcrypto -lssl"

in project folder.

Dependencies:

g++

Ubuntu/WSL:

libboost-dev
libssl-dev
(not sure if this list is complete)

Arch:

boost

[included]

https://github.com/slowriot/libtelegram

https://github.com/elnormous/HTTPRequest