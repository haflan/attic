#!/usr/bin/python3

# Script that finds all media files in a source directory and moves them to a target
# directory on the format <target>/<year>/<month>/ based on the media file date.

from exif import Image
from datetime import datetime as dt
import os
import sys
import re

DEBUG=False


if len(sys.argv) >= 3:
    NEW_MEDIA_DIR=sys.argv[1]
    SORTED_MEDIA_DIR=sys.argv[2]
else:
    NEW_MEDIA_DIR=os.environ['NEW_MEDIA_DIR']
    SORTED_MEDIA_DIR=os.environ['SORTED_MEDIA_DIR']

ts_fmt_exif = '%Y:%m:%d %H:%M:%S'
ts_fmts_filename = [
        re.compile('.+_(?P<year>\d{4})(?P<month>\d\d)(?P<day>\d\d)_(?P<hour>\d\d)(?P<min>\d\d)(?P<sec>\d\d).+')
]

# Tries to get the date of a media file, first by reading EXIF data,
# then by parsing the filename if unsuccessful.
# Returns date as a touple: (year, month, day).
def extract_img_date(filename):
    if '.mp4' not in filename:
        try:
            img = Image(open(filename, 'rb'))
            ts = dt.strptime(img.datetime, ts_fmt_exif)
            if DEBUG:
                print('Found date in exif data')
            return (str(ts.year), str(ts.month).rjust(2, '0'), str(ts.day).rjust(2, '0'))
        except Exception as e:
            pass
    # Then try different filename formats
    for fmt in ts_fmts_filename:
        match = fmt.match(filename)
        if match:
            if DEBUG:
                print('Found date in filename')
            return (match.group('year'), match.group('month'), match.group('day'))
    return None

# Takes a date touple (year, month, day) and creates <SORTED_MEDIA_DIR>/<year>/<month>
# if it doesn't exist already
def create_and_get_month_dir(date):
    month_dir=os.path.join(SORTED_MEDIA_DIR, date[0], date[1])
    if os.path.exists(month_dir):
        if DEBUG:
            print(f'Directory "{month_dir}" exists')
        return month_dir
    if DEBUG:
        print(f'SIMULATED: Directory "{month_dir}" created')
    else:
        os.makedirs(month_dir)
    return month_dir

if not os.path.exists(NEW_MEDIA_DIR):
    print(f'{NEW_MEDIA_DIR} does not exist')
    exit(1)

# Do the organization
for [root, dirs, files] in os.walk(NEW_MEDIA_DIR):
    for filename in files:
        if not filename:
            continue
        old_path = os.path.join(root, filename)
        date = extract_img_date(old_path)
        if not date:
            if DEBUG:
                print('No date found for ' + filename)
            continue
        month_dir = create_and_get_month_dir(date)
        new_path = os.path.join(month_dir, filename)
        if DEBUG:
            print(f'SIMULATED: Moved {old_path} to {new_path}')
        else:
            os.rename(old_path, new_path)
