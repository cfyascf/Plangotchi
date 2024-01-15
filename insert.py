import json
import requests
import pyodbc
import time
import firebase_admin as db
import threading

"""
bem2ct
gaussKronrod754
"""

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
            self.humor = "Ok"
        
        elif(value >= 4):
            self.humor = "Feliz"
        

    def calculate_health(self, moisture, luminosity, temperature):
        value = (moisture + luminosity + temperature) / 3

        if(value < 2):
            self.health = "Doente"

        elif(value >= 2 and value > 4):
            self.health = "Sobrevivendo"

        elif(value >= 4):
            self.health = "Saudavel"

    def calculate_thirst(self, moisture):
        if(moisture < 50):
            self.thirst = "Com sede"
        
        elif(moisture >= 50 and moisture <= 70):
            self.thirst = "Satisfeito"
        
        elif(value > 70):
            self.thirst = "Encharcado"

    def calculate_life(self):
        if(humor > 2 or health < 2):
            self.life = self.life - 2

        elif(humor < 4 or health < 4):
            self.life = self.life - 1

    def calculate_status(self):
        if(self.life == 0):
            self.alive = False

# -------------- * SQL CONFIG * --------------

server = 'CTNB1553\\SQLEXPRESS01'
database = 'yasminteste'
driver= '{ODBC Driver 17 for SQL Server}'
cnxn = pyodbc.connect('DRIVER='+driver+';SERVER='+server+'; PORT=1433; DATABASE='+database)
cursor = cnxn.cursor()

# -------------- * FIREBASE CONFIG * --------------

databaseURL = 'https://plangotchi-default-rtdb.firebaseio.com/'
cred_obj = firebase_admin.credentials.Certificate('plangotchi-sdk.json')
default_app = firebase_admin.initialize_app(cred_obj, {'databaseURL':databaseURL})

# -------------- * VARIABLES * --------------

temp_data = []
lumi_data = []
moist_data = []
pres_data = []
last_flag = 1

# -------------- * FUNCTIONS * --------------

# get data
def sinal():
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

# data into sql
def insert_data(sinal):
    if(signal[4] == last_flag):
        return 0

    last_flag = signal[4]
    
    cursor.execute(f"INSERT dbo.Sensor (Temperature, Humidity, Luminosity, Presence) VALUES ({sinal[0]}, {sinal[1]}, {sinal[2]}, {sinal[3]});")
    cnxn.commit()
    print("Data inserted with success!")


def select_data(list, index):
    cursor.execute(f"SELECT {table}, timestamp FROM dbo.Sensor")
    for row in cursor.fetchall():
        list.append(row[index])

    print("Table recovered with success!")

def update_stats(oscar):
    last_index = len(lumi_data) - 1
    oscar.calculate_health(moist_data[last_index], lumi_data[last_index], temp_data[last_index])
    oscar.calculate_humor(lumi_data[last_index], temp_data[last_index], pres_data[last_index])
    oscar.calculate_thirst(moist_data[last_index])
    oscar.calculate_life()
    oscar.calculate_status()

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

    insert_th = threading.Thread(target=insert_data, args=(signal,))

    select_temp_th = threading.Thread(target=select_data, args=(temp_data, 0))
    select_lumi_th = threading.Thread(target=select_data, args=(lumi_data, 1))
    select_moist_th = threading.Thread(target=select_data, args=(moist_data, 2))
    select_pres_th = threading.Thread(target=select_data, args=(pres_data, 3))

    stats_th = threading.Thread(target=update_stats, args=(oscar))

    fb_th = threading.Thread(target=to_fb, args=(oscar))

    insert_th.start()
    select_temp_th.start()
    select_lumi_th.start()
    select_moist_th.start()
    select_pres_th.start()
    stats_th.start()
    fb_th.start()









