import json
import requests
import time
import firebase_admin as db
import threading
import os

"""
bem2ct
gaussKronrod754
"""

# -------------- * MAIN CLASS * --------------

class Plangotchi:
    humor: str
    health: str
    thirst: str
    life: int
    alive: bool

    def __init__(self, humor = "Feliz", health = "Saudavel", thirst = "Satisfeito", life = 5, alive = True):
        self.humor = humor
        self.health = health
        self.thirst = thirst
        self.life = life
        self.alive = alive

    def calculate_humor(self, luminosity, temperature, presence):
        value = (luminosity + temperature + presence) / 3

        if(value < 2):
            self.humor = "Triste"
        
        elif(value >= 2 and value > 4):
            self.humor = "Feliz"
        
        elif(value >= 4):
            self.humor = "Ok"
        

    def calculate_health(self, moisture, luminosity, temperature):
        value = (moisture + luminosity + temperature) / 3

        if(value < 2):
            self.health = "Doente"

        elif(value >= 2 and value > 4):
            self.health = "Saudavel"

        elif(value >= 4):
            self.health = "Sobrevivendo"

    def calculate_thirst(self, moisture):
        if(moisture < 3):
            self.thirst = "Com sede"
        
        elif(moisture >= 3 and moisture <= 4):
            self.thirst = "Satisfeito"
        
        elif(value > 4):
            self.thirst = "Encharcado"

    def calculate_life(self):
        if(humor > 2 or health < 2):
            self.life = self.life - 2

        elif(humor < 4 or health < 4):
            self.life = self.life - 1

    def calculate_status(self):
        if(self.life == 0):
            self.alive = False

# -------------- * FIREBASE CONFIG * --------------

databaseURL = 'https://plangotchi-default-rtdb.firebaseio.com/'
cred_obj = firebase_admin.credentials.Certificate('plangotchi-sdk.json')
default_app = firebase_admin.initialize_app(cred_obj, {'databaseURL':databaseURL})

os.environ["HTTP_PROXY"] = "http://rb-proxy-ca1.bosch.com:8080"
os.environ["HTTPS_PROXY"] = "http://rb-proxy-ca1.bosch.com:8080"
os.environ["HTTP_PROXY_USER"] = "http://disrct:ets%40bosch207@rb-proxy-ca1.bosch.com:8080"
os.environ["HTTPS_PROXY_USER"] = "http://disrct:ets%40bosch207@rb-proxy-ca1.bosch.com:8080"

# -------------- * VARIABLES * --------------

temp_data = []
lumi_data = []
moist_data = []
pres_data = []
last_flag = 1

# -------------- * FUNCTIONS * --------------


# .. makes requests to get real time data from 
# firebase and returns as an array ..
def signal():
    proxies = {
    "http": "http://disrct:ets%40bosch207@rb-proxy-ca1.bosch.com:8080",
    "https": "http://disrct:ets%40bosch207@rb-proxy-ca1.bosch.com:8080"
    }

    url_temperature = 'https://esp32andfirebase-ec772-default-rtdb.firebaseio.com/sensor/temperature.json'
    url_luminosity = 'https://esp32andfirebase-ec772-default-rtdb.firebaseio.com/sensor/luminosity.json'
    url_moisture = 'https://esp32andfirebase-ec772-default-rtdb.firebaseio.com/sensor/moisture.json'
    url_presence = 'https://esp32andfirebase-ec772-default-rtdb.firebaseio.com/sensor/presence.json'
    url_flag = 'https://esp32andfirebase-ec772-default-rtdb.firebaseio.com/sensor/flag.json'

    temperature = float(requests.get(url_temperature, proxies=proxies).content)
    luminosity = float(requests.get(url_luminosity, proxies=proxies).content)
    moisture = float(requests.get(url_moisture, proxies=proxies).content)
    presence = float(requests.get(url_presence, proxies=proxies).content)
    flag = float(requests.get(url_flag, proxies=proxies).content)

    return temperature, luminosity, moisture, presence, flag


# .. fills data arrays using compressed
# data to a gap from 1 to 5 ..
def get_data(signal):
    if(signal[4] == last_flag):
        return 0 

    last_flag = signal[4] # .. so repeated data won't get into the arrays ..

    temp_value = ((signal[0] - 18) * (5 - 1) / (30 - 18)) + 1
    lumi_value = signal[1]
    moist_value = ((signal[2] - 20) * (5 - 1) / (90 - 20)) + 1

    last_index = len(lumi_data) - 1
    sum = 0
    i = last_index
    while i > last_index - 5:
        sum = sum + pres_data[i]
        i -= 1

    pres_value = sum

    temp_data.append(temp_value)
    lumi_data.append(lumi_value)
    moist_data.append(moist_value)
    pres_data.append(pres_value)


# .. based on the data arrays, calculate
# oscar's stats using class methods ..
def update_stats(oscar):
    last_index = len(lumi_data) - 1

    oscar.calculate_humor(lumi_data[last_index], temp_data[last_index], pres_data[last_index])
    oscar.calculate_health(moist_data[last_index], lumi_data[last_index], temp_data[last_index])
    oscar.calculate_thirst(moist_data[last_index])
    oscar.calculate_life()
    oscar.calculate_status()


# .. saves the calculated stats
# on firebase realtime database ..
def to_fb(oscar):
    ref = db.reference('stats')
    new_stat_ref = ref.push({
        "health": oscar.health,
        "humor": oscar.humor,
        "thirst": oscar.thirst,
        "life": oscar.life,
        "status": oscar.status
    })


if __name__ == '__main__':
    oscar = Plangotchi()

    getdata_th = threading.Thread(target=get_data, args=(signal()))
    stats_th = threading.Thread(target=update_stats, args=(oscar))
    fb_th = threading.Thread(target=to_fb, args=(oscar))

    getdata_th.start()
    stats_th.start()
    fb_th.start()









