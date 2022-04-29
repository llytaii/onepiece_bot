from asyncore import write
import logging
import requests
import time
import threading
import random

from telegram import Update
from telegram.error import Unauthorized, BadRequest
from telegram.ext import Updater, CommandHandler, MessageHandler, Filters, CallbackContext

# LOGGING 
logging.basicConfig(
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s', level=logging.INFO
)
logger = logging.getLogger(__name__)


# USER
with open('users.txt', 'r') as f:
    users = [int(i) for i in f.read().split()]

def write_users():
    with open('users.txt', 'w') as f:
        [f.write(str(u) + ' ') for u in users]

# BOT 
with open('token.txt') as f:
    token = f.read().strip()

updater = Updater(token)
dispatcher = updater.dispatcher
bot = updater.bot

def notify_all(text):
    global bot
    global users
    removal_list = []
    for u in users:
        try:
            bot.send_message(chat_id=u, text=text)
        except Unauthorized:
            removal_list.append(u)
        except BadRequest:
            removal_list.append(u)

    for x in removal_list:
        users.remove(x)

    write_users()

def start(update: Update, context: CallbackContext) -> None:
    global users
    id = update.message.chat_id
    if id in users:
        answer = "You already subscribed!"
    else:
        users.append(id)
        write_users()
        answer = "You subscribed!"
    
    context.bot.send_message(chat_id=id, text=answer)

def help(update: Update, context: CallbackContext) -> None:
    context.bot.send_message(chat_id=update.effective_chat.id, text="Receive updates for onepiece-tube.com!")

# FETCHER
with open('fetcher.txt', 'r') as f:
    next_chapter = int(f.readline().strip())
    next_episode = int(f.readline().strip())

def write_fetcher():
    global next_chapter
    global next_episode

    with open('fetcher.txt', 'w') as f:
        f.write(str(next_chapter) + '\n')
        f.write(str(next_episode))

def check_url_for(url, text):
    return text in requests.get(url).text

def run_fetcher():
    global next_chapter
    global next_episode
    global users

    while(True):
        if(check_url_for("https://onepiece-tube.com/kapitel-mangaliste","/kapitel/" + str(next_chapter))):
            notify_all("https://onepiece-tube.com/kapitel/" + str(next_chapter) + "/1")
            next_chapter += 1
            write_fetcher()

        if(check_url_for("http://onepiece-tube.com/episoden-streams","/folge/" + str(next_episode))):
            notify_all("https://onepiece-tube.com/folge/" + str(next_episode))
            next_episode += 1
            write_fetcher()
    
        min = random.randint(5, 10)
        time.sleep(60 * min)


def main() -> None:
    dispatcher.add_handler(CommandHandler("start", start))
    dispatcher.add_handler(CommandHandler("help", help))

    fetcher = threading.Thread(target=run_fetcher, daemon=True)
    fetcher.start()

    updater.start_polling()
    updater.idle()
   

if __name__ == '__main__':
    main()    