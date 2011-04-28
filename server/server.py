import io
import httplib2
import json
from PIL import Image
from StringIO import StringIO


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
		send_json({"action": "httpreq", "result": content})
		print "Data sent."
	elif act['action'] == 'httpimage':
		print "Grabbing image from "+ act['url']+':'
		resp, content = http.request("http://" + act['url'])
		im = Image.open(StringIO(content))
		result = ''
		for y in range(im.size[1]): #Cycle through the image's pixels, one by one
		   for x in range(im.size[0]):
			  RGB = im.getpixel((x, y))             #Retrieve pixel RGB values
			  color = (RGB[0] + RGB[1] + RGB[2]) / 3  #Find the general darkness of the pixel
			  if color < 19:
				 result += '&'  #Darkest/black pixels
			  elif color < 50:
				 result += '8'
			  elif color < 75:
				 result += '0'
			  elif color < 100:
				 result += '$'
			  elif color < 130:
				 result += '2'
			  elif color < 165:
				 result += '1'
			  elif color < 180:
				 result += '|'
			  elif color < 200:
				 result += ';'
			  elif color < 218:
				 result += ':'
			  elif color < 229:
				 result += "'"
			  elif color <= 255:
				 result += ' '  #Lightest/white pixels
		   #result += "\n"
		# convert to max 80px width
		result += "\n"
		send_json({"action": "httpimage", "result": result})
		print "image sent."
	else:
		print "Unrecognized action \"" + act['action'] + "\""

def send_json(data):
	msg = json.dumps(data)
	p = chr(len(msg) >> 8) + chr(len(msg) & 0xFF)
	sw.write(p + msg)

while True:
	r = sr.read(2)
	print "[DEBUG] Start message: " + str(r)
	l = (ord(r[0]) << 8) + ord(r[1])
	print "Received command of length " + str(l)
	srRead = sr.read(l)
	print "srRead called: " + srRead
	d = json.loads(str(srRead))
	on_receive(d)
