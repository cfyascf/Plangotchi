import json
import requests
import time
import firebase_admin
from firebase_admin import db
import threading
import os
import pygame as pg

"""
bem2ct
gaussKronrod754
"""

# -------------- * MAIN CLASS * --------------

class Plangotchi:
    humor: int
    health: int
    thirst: int
    life: int
    status: int

    def __init__(self, humor = 3, health = 3, thirst = 3, life = 5, status = 1):
        self.humor = humor
        self.health = health
        self.thirst = thirst
        self.life = life
        self.status = status

    def calculate_humor(self, luminosity, temperature, presence):
        self.humor = (luminosity + temperature + presence) / 3

    def calculate_health(self, moisture, luminosity, temperature):
        sel.health = (moisture + luminosity + temperature) / 3

    def calculate_thirst(self, moisture):
        self.thirst = moisture

    def calculate_life(self):
        if(humor > 2 or health < 2):
            self.life = self.life - 2

        elif(humor < 4 or health < 4):
            self.life = self.life - 1

    def calculate_status(self):
        if(self.life == 0):
            self.status = 0

# -------------- * FIREBASE CONFIG * --------------

databaseURL = 'https://plangotchi-default-rtdb.firebaseio.com/'
cred_obj = firebase_admin.credentials.Certificate(r'C:\Users\disrct\Desktop\yasmimcf\PLANTGOTCHI\PLANGOTCHI\plangotchi\plangotchi-sdk.json')
default_app = firebase_admin.initialize_app(cred_obj, {'databaseURL':databaseURL})

os.environ["HTTP_PROXY"] = "http://rb-proxy-ca1.bosch.com:8080"
os.environ["HTTPS_PROXY"] = "http://rb-proxy-ca1.bosch.com:8080"
os.environ["HTTP_PROXY_USER"] = "http://disrct:ets%40bosch207@rb-proxy-ca1.bosch.com:8080"
os.environ["HTTPS_PROXY_USER"] = "http://disrct:ets%40bosch207@rb-proxy-ca1.bosch.com:8080"

# -------------- * VARIABLES * --------------

temp_data = [1]
lumi_data = [1]
moist_data = [1]
pres_data = [1]
last_flag = 1

# -------------- * FUNCTIONS * --------------


# .. makes requests to get real time data from 
# firebase and returns as an array ..
def signal():
    ref = db.reference('sensors/temperature')
    temperature = ref.get()

    ref = db.reference('sensors/luminosity')
    luminosity = ref.get()

    ref = db.reference('sensors/moisture')
    moisture = ref.get()

    ref = db.reference('sensors/presence')
    presence = ref.get()

    ref = db.reference('sensors/flag')
    flag = ref.get()

    return temperature, luminosity, moisture, presence, flag


# .. fills data arrays using compressed
# data to a gap from 1 to 5 ..
def get_data(temp, lumi, moist, pres, flag):
    global last_flag

    if(flag == last_flag):
        return 0 

    last_flag = flag # .. so repeated data won't get into the arrays ..

    temp_value = ((temp - 18) * (5 - 1) / (30 - 18)) + 1
    lumi_value = lumi
    moist_value = ((moist - 20) * (5 - 1) / (90 - 20)) + 1
    pres_value = pres

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

    
    getdata_th = threading.Thread(target=get_data, args=signal())
    stats_th = threading.Thread(target=update_stats, args=[oscar])
    fb_th = threading.Thread(target=to_fb, args=[oscar])

    getdata_th.start()
    stats_th.start()
    fb_th.start()









