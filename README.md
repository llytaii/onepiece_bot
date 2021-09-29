# This program is running the @OnePieceChapterBot Telegram Bot Account. #

What it does:

It fetches onepiece-tube.com every 10 minutes for a new chapter/episode and notifies all saved users about it.

Commands:

/start  saves your user id.
/end    deletes your user id.

Get Started:

git clone --recursive https://www.github.com/llytaii/onepiece_bot.git

Build:

In projects root:
```
mkdir build
cd build
cmake ..
make
```
Dependencies (Ubuntu):
```
sudo apt install libboost-all-dev libssl-dev make cmake g++
```

(not sure if this list is complete)

[included]

https://github.com/slowriot/libtelegram

https://github.com/elnormous/HTTPRequest

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