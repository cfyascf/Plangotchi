import json
import requests
import pyodbc
import time

"""
bem2ct
gaussKronrod754
"""

# connect to sql
server = 'CTNB1553\\SQLEXPRESS01'
database = 'tempdbo'
username = 'admin'
password = 'admin'
driver= '{ODBC Driver 17 for SQL Server}'
cnxn = pyodbc.connect('DRIVER='+driver+';SERVER='+server+'; PORT=1433; DATABASE='+database+';UID='+username+';PWD='+ password)
cursor = cnxn.cursor()

# get data
def sinal():
    proxy = {
    "http": "http://disrct:ets%40bosch207@rb-proxy-ca1.bosch.com:8080",
    "https": "http://disrct:ets%40bosch207@rb-proxy-ca1.bosch.com:8080"
    }

    url_temperature = 'https://esp32andfirebase-ec772-default-rtdb.firebaseio.com/Sensor/temperatura.json'
    url_humidity = 'https://esp32andfirebase-ec772-default-rtdb.firebaseio.com/Sensor/umidade.json'
    url_luminosity = 'https://esp32andfirebase-ec772-default-rtdb.firebaseio.com/Sensor/luminosidade.json'

    temperature = float(requests.get(url_temperature, proxies=proxies).content)
    humidity = float(requests.get(url_humidity, proxies=proxies).content)
    luminosity = float(requests.get(url_luminosity, proxies=proxies).content)

    return temperature, humidity, luminosity


# data into sql
def insert_data(sinal):
    cursor.execute(f"INSERT dbo.Sensor (Temperatura, Umidade, Luminosidade) VALUES ({sinal[0]}, {sinal[1]}, {sinal[2]});")
    con.commit()
    print("Data inserted with success!")

def select_data(table, list):
    cursor.execute(f"SELECT {table}, timestamp FROM dbo.Sensor")
    row = cursor.fetchone()
    lista_tempo=[]

    while row:
        list.append(row[0])
        listatempo.append(str(row[1]))
        row = cursor.fetchone()

    print("Table recovered with success!")







