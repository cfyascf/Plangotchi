import json
import requests
import pyodbc
import time

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

    def __init__(self, humor, health, thirst, life = 5, alive = True):
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
database = 'tempdbo'
username = 'admin'
password = 'admin'
driver= '{ODBC Driver 17 for SQL Server}'
cnxn = pyodbc.connect('DRIVER='+driver+';SERVER='+server+'; PORT=1433; DATABASE='+database+';UID='+username+';PWD='+ password)
cursor = cnxn.cursor()

# -------------- * VARIABLES * --------------

temp_data = []
lumi_data = []
moist_data = []
pres_data = []
last_flag = 0

# -------------- * FUNCTIONS * --------------

# get data
def sinal():
    proxies = {
    "http": "http://disrct:ets%40bosch207@rb-proxy-ca1.bosch.com:8080",
    "https": "http://disrct:ets%40bosch207@rb-proxy-ca1.bosch.com:8080"
    }

    url_temperature = 'https://esp32andfirebase-ec772-default-rtdb.firebaseio.com/sensor/temperature.json'
    url_humidity = 'https://esp32andfirebase-ec772-default-rtdb.firebaseio.com/sensor/moisture.json'
    url_luminosity = 'https://esp32andfirebase-ec772-default-rtdb.firebaseio.com/sensor/luminosity.json'
    url_presence = 'https://esp32andfirebase-ec772-default-rtdb.firebaseio.com/sensor/presence.json'
    url_flag = 'https://esp32andfirebase-ec772-default-rtdb.firebaseio.com/sensor/flag.json'

    temperature = float(requests.get(url_temperature, proxies=proxies).content)
    humidity = float(requests.get(url_humidity, proxies=proxies).content)
    luminosity = float(requests.get(url_luminosity, proxies=proxies).content)
    presence = float(requests.get(url_presence, proxies=proxies).content)
    flag = float(requests.get(url_flag, proxies=proxies).content)

    return temperature, humidity, luminosity, presence, flag

# data into sql
def insert_data(sinal):
    if(signal[4] == last_flag):
        return 0

    last_flag = signal[4]
    
    cursor.execute(f"INSERT dbo.Sensor (Temperature, Humidity, Luminosity, Presence) VALUES ({sinal[0]}, {sinal[1]}, {sinal[2]}, {sinal[3]});")
    con.commit()
    print("Data inserted with success!")

def select_data(table, list):
    cursor.execute(f"SELECT {table}, timestamp FROM dbo.Sensor")
    row = cursor.fetchone()
    lista_tempo=[]

    list=[]

    while row:
        list.append(row[0])
        listatempo.append(str(row[1]))
        row = cursor.fetchone()

    print("Table recovered with success!")


if __name__ == '__main__':

    insert_th = threading.Thread(target=insert_data, args=(signal))









