import io
import httplib2
import json
import img2txt
import base64, sys

if len(sys.argv) < 2:
	print "Usage: python server.py [/dev/pts/ number]"
	exit()

http = httplib2.Http()

n = sys.argv[1]
sr = io.FileIO("/dev/pts/" + n, "r")
sw = io.FileIO("/dev/pts/" + n, "w")

def on_receive(id, act):
	# "act" is a JSON variable with a value "action" and then other
	# parameters attached to it
	if act['action'] == 'httpreq':
		print "HTTP Request " + act['url'] + ":"
		resp, content = http.request(act['url'])
		print resp
		send_json(id, {"action": "httpreq", 'code': resp['status'], "body": content})
		print "Data sent."
	else:
		print "Unrecognized action \"" + act['action'] + "\""

def send_json(id, data):
	msg = chr(id >> 8) + chr(id & 0xFF) + json.dumps(data)
	p = chr(len(msg) >> 8) + chr(len(msg) & 0xFF)
	sw.write(p + msg)

while True:
	r = sr.read(2)
	print "[DEBUG] Start message: " + str(r)
	l = (ord(r[0]) << 8) + ord(r[1])
	print "Receiving command of length " + str(l)

	data = ""
	while len(data) < l + 2:
		data += str(sr.read(l + 2 - len(data)))
		print " . read " + str(len(data))
	print "Command (length " + str(len(data[2:])) + "): " + data[2:]

	# parse message
	id = (ord(data[0]) << 8) + ord(data[1])
	jd = json.loads(data[2:])
	try:
		on_receive(id, jd)
	except Exception, e:
		# dummy message
		send_json(id, None)
		print "[ERROR] in on_receive: " + str(e)
