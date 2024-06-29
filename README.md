# ESP-32 Project

Plangotchi is kind of a Tamagochi, the only difference is that it's made based on an ESP-32 programmed 
using C++ language and a Python script.

## C++
The script in C++ is responsible to integrate the sensors that read humidity, luminosity, presence and send the
data to a Firebase database, and to display the data on a lcd too.

## Python
The script in Python is responsible to consume the data of the database and calculate Plangotchi's humor
and reproduce sounds based on it's humor.
