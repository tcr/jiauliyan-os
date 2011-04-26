import io
import httplib2

http = httplib2.Http()

n = raw_input("Finish this: /dev/pts/")

sr = io.FileIO("/dev/pts/" + n, "r")
sw = io.FileIO("/dev/pts/" + n, "w")

def on_receive(msg):
	print "Requesting " + msg + "..."
	resp, content = http.request("http://" + msg)
	send_msg(content)

def send_msg(msg):
	p = chr(len(msg) >> 8) + chr(len(msg) & 0xFF)
	sw.write(p + msg)

while True:
	r = sr.read(2)
	l = (ord(r[0]) << 8) + ord(r[1])
	print "Received string of length " + str(l)
	d = sr.read(l)
	on_receive(str(d))