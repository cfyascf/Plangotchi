import firebase_admin
from firebase_admin import db
import os

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

os.environ["HTTP_PROXY"] = "http://rb-proxy-ca1.bosch.com:8080"
os.environ["HTTPS_PROXY"] = "http://rb-proxy-ca1.bosch.com:8080"
os.environ["HTTP_PROXY_USER"] = "http://disrct:ets%40bosch207@rb-proxy-ca1.bosch.com:8080"
os.environ["HTTPS_PROXY_USER"] = "http://disrct:ets%40bosch207@rb-proxy-ca1.bosch.com:8080"

databaseURL = 'https://plangotchi-default-rtdb.firebaseio.com/stats'
cred_obj = firebase_admin.credentials.Certificate(r'C:\Users\disrct\Desktop\yasmimcf\PLANTGOTCHI\PLANGOTCHI\plangotchi\plangotchi-sdk.json')
default_app = firebase_admin.initialize_app(cred_obj, {'databaseURL':databaseURL})

def to_fb(oscar):
    ref = db.reference('stats')
    new_stat_ref = ref.set({
        "health": oscar.health,
        "humor": oscar.humor,
        "thirst": oscar.thirst,
        "life": oscar.life,
        "status": oscar.alive
    })

oscar = Plangotchi()
to_fb(oscar)

