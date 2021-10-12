# This program is running the @OnePieceChapterBot Telegram Bot Account. #

What it does:

It fetches onepiece-tube.com (german website) every 10 minutes for a new chapter/episode and notifies all saved users about it.

/help for commands


Get Started:

git clone --recursive https://www.github.com/llytaii/onepiece_bot.git


Build:

Dependencies (Ubuntu):
```
sudo apt install libboost-all-dev libssl-dev make cmake g++
```
(not sure if this list is complete)

[included]

https://github.com/slowriot/libtelegram

https://github.com/elnormous/HTTPRequest


In projects root:
```
mkdir build
cd build
cmake ..
make
```

Run:

Create a Telegram Bot using the BotFather-TelegramBot, then in projects root:
```
mkdir res
cd res
touch bot.data chapter.data episode.data user.data
```
and fill the files with (without <>-brackets):

bot.data: <bot-token> <admin-id>

chapter.data: <next_chapter_number>

episode.data: <next_episode_number>

user.data:

then run.