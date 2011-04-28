import io
import httplib2
import json

http = httplib2.Http()

n = raw_input("Finish this: /dev/pts/")

sr = io.FileIO("/dev/pts/" + n, "r")
sw = io.FileIO("/dev/pts/" + n, "w")

def on_receive(act):
	# "act" is a JSON variable with a value "action" and then other
	# parameters attached to it
	if act['action'] == 'httpreq':
		print "HTTP Request " + act['url'] + ":"
		resp, content = http.request(act['url'])
		send_msg(json.dumps(content))
		print "Data sent."
	else:
		print "Unrecognized action \"" + act['action'] + "\""

def send_msg(msg):
	p = chr(len(msg) >> 8) + chr(len(msg) & 0xFF)
	sw.write(p + msg)

while True:
	r = sr.read(2)
	l = (ord(r[0]) << 8) + ord(r[1])
	print "Received command of length " + str(l)
	d = json.loads(str(sr.read(l)))
	on_receive(d)