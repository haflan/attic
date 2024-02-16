#!/usr/bin/python3

# Script for quickly sharing a single file (working as a server) or
# downloading a shared file (working as a client).
# When working as a server, it serves the single file given as argument and
# exits immediately after a single request has been made, i.e. when the file
# has hopefully been downloaded once.
# When acting as a client it downloads the file served by a vshare server
# running at the given IP and port (another computer)

import sys

if len(sys.argv) < 3 or sys.argv[1] not in ['s', 'g']:
    print("Use:")
    print("  to serve:          " + sys.argv[0] + " s <filename>")
    print("  to get / download: " + sys.argv[0] + " g <server>")
    exit()

# If arg is 'g', get file from the given server
if sys.argv[1] == 'g':
    import requests
    url = sys.argv[2]
    if 'http' not in url[:4]:
        url = 'http://' + url
    res = requests.get(url)
    filename = res.headers['Filename']
    with open(filename, 'wb') as f:
        f.write(res.content)
    print("Got file '" + filename + "'")
    exit(1)


# If arg is 's', share the given file
from http.server import BaseHTTPRequestHandler, HTTPServer
FILENAME=sys.argv[2]

class StoppableServer(HTTPServer):
    def serve_forever(self):
        self.stop = False
        while not self.stop:
            self.handle_request()

class OneShotServer(BaseHTTPRequestHandler):

    def do_GET(self):
        self.send_response(200)
        self.send_header('Content-type', 'application/octet-stream')
        self.send_header('Filename', FILENAME.split('/')[-1])
        self.end_headers()
        with open(FILENAME, 'rb') as f:
            self.wfile.write(f.read())
        print('File fetched. Closing server')
        self.server.stop = True


vsrv = StoppableServer(('0.0.0.0', 8001), OneShotServer)
print('Serving "' + FILENAME + '"')
try:
    vsrv.serve_forever()
except Exception:
    vsrv.shutdown()
