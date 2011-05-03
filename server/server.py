import io
import httplib2
import json
import base64, sys

if len(sys.argv) < 2:
	print "Usage: python server.py [/dev/pts/ number]"
	exit()

http = httplib2.Http()

n = sys.argv[1]
sr = io.FileIO("/dev/pts/" + n, "r")
sw = io.FileIO("/dev/pts/" + n, "w")

print "Launched HTTP serial server foir /dev/pts/" + n + "..."

def on_receive(id, act):
	# "act" is a JSON variable with a value "action" and then other
	# parameters attached to it
	if act['action'] == 'httpreq':
		sys.stdout.write("HTTP " + act['method'] + " " + act['url'] + ": ")
		resp, content = http.request(act['url'],
			act['method'] if act.has_key('method') else 'GET',
			body=act['body'] if act.has_key('body') else None,
			headers=act['headers'] if act.has_key('headers') else None)
		print content
		send_message(id, mark_len(json.dumps({"action": "httpreq", 'code': resp['status']})) + mark_len(content))
		print "Data sent."
	else:
		print "Unrecognized action \"" + act['action'] + "\""

def mark_len(msg):
	return chr(len(msg) >> 8) + chr(len(msg) & 0xFF) + msg

def send_message(id, msg):
	msg = chr(id >> 8) + chr(id & 0xFF) + msg
	sw.write(mark_len(msg))

while True:
	r = sr.read(2)
	#print "[DEBUG] Start message: " + str(r)
	l = (ord(r[0]) << 8) + ord(r[1])
	print "Receiving command of length " + str(l)

	data = ""
	while len(data) < l + 2:
		data += str(sr.read(l + 2 - len(data)))
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
