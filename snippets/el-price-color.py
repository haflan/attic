#!/usr/bin/python3

# Fetch electricity prices from hvakosterstrommen.no
# and set the color of a Philips Hue color bulb accordingly.

import requests
import sys
from datetime import datetime
from pathlib import Path
from os import path
import json
import time

BRIDGE = 'http://192.168.10.140'
TARGET = 7
username = open(f'{Path.home()}/.hue-user').read()

# Electricity prices
def fetch_prices(cache_file, dt):
    BASE_PATH = 'https://www.hvakosterstrommen.no'
    resp = requests.get(f'{BASE_PATH}/api/v1/prices/{dt.year}/{dt.strftime("%m-%d")}_NO1.json')
    with open(cache_file, 'w') as f:
        f.write(resp.text)

dt = datetime.now()
cache_file = f'/tmp/el-price-{dt.year}-{dt.month}-{dt.day}'
if not path.exists(cache_file):
    fetch_prices(cache_file, dt)
with open(cache_file) as f:
    prices = json.load(f)

prices = [p['NOK_per_kWh'] for p in prices]
avg = sum(prices)/len(prices)
min_p = min(prices)
max_p = max(prices)

if len(sys.argv) > 2 and sys.argv[2] == 'day':
    hours_to_show = range(len(prices))
else:
    hours_to_show = [dt.hour]

# Lights
for h in hours_to_show:
    # The values are chosen so that lowest price of day -> green and highest -> red.
    # (green=[0.30, 0.70], and red=[0.70, 0.30])
    off = 0.4 * (prices[h] - min_p) / (max_p - min_p)
    res = requests.put(f'{BRIDGE}/api/{username}/lights/{TARGET}/state', json={
        'on': True,
        'bri': int(254*int(sys.argv[1])/100),
        'xy': [0.3+off, 0.7-off],
        'transitiontime': 10 # centisec = 10 sec
    })
    print(res.content)
    time.sleep(1)
