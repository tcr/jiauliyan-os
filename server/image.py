#!/usr/bin/python

import subprocess, shlex, json
import sys, re, base64
import urllib2, os

def convert(url):
	ext = os.path.splitext(url)[1]

	u = urllib2.urlopen(url)
	localFile = open('image.png', 'w')
	localFile.write(u.read())
	localFile.close()
	output = subprocess.Popen(shlex.split("img2txt -W 80 -H 22 -f utf8 image.png"), stdout=subprocess.PIPE).communicate()[0]

	fg = 0
	bg = 0

	def getcolor(p):
		if p == 0: return 0
		if p == 1: return 4
		if p == 2: return 2
		if p == 3: return 6
		if p == 4: return 1
		if p == 5: return 5
		if p == 6: return 3
		if p == 7: return 7
		return None

	d = ""\

	i = 0
	while i < len(output):
		if output[i] == '\033':
			m = re.match(r"\033\[s\n", output[i:])
			if m:
				print("")
				i += len(m.group(0))
				continue
			m = re.match(r"\033\[u", output[i:])
			if m:
				i += len(m.group(0))
				continue
			m = re.match(r"\033\[(.*?)m", output[i:])
			if m:
				params = m.group(1)
				for p in params.split(';'):
					p = int(p)
					if p == 0:
						fg = 15
						bg = 0
					if getcolor(p - 30) != None:
						fg = getcolor(p - 30)
					if getcolor(p - 40) != None:
						bg = getcolor(p - 40)
					if getcolor(p - 90) != None:
						fg = getcolor(p - 90) + 8
					if getcolor(p - 100) != None:
						bg = getcolor(p - 100) + 8
				i += len(m.group(0))
				continue
		elif output[1] != "\n":
			d += chr(ord('a') + bg)
			d += chr(ord('a') + fg)
			d += output[i]
			i += 1

	return d

################

import cgi, cgitb, os
cgitb.enable()

print "Content-type: text/plain"
print ""

if 'QUERY_STRING' not in os.environ:
	print "No query specified"
	exit()

url = os.environ['QUERY_STRING']
print convert(url)
