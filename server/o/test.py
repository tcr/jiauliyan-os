import subprocess, shlex
import sys, re
from termcolor import colored

output = subprocess.Popen(shlex.split("img2txt -W 80 -f utf8 goog.png"), stdout=subprocess.PIPE).communicate()[0]

fg = 'white'
bg = None
attrs = []
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
				if p == '0':
					fg = 'white'
					bg = None
					attrs = ['dark']
				if int(p) >= 90 and int(p) <= 97: attrs = []
				if p == '30' or p == '90': fg = None
				if p == '31' or p == '91': fg = 'red'
				if p == '32' or p == '92': fg = 'green'
				if p == '33' or p == '93': fg = 'yellow'
				if p == '34' or p == '94': fg = 'blue'
				if p == '35' or p == '95': fg = 'magenta'
				if p == '36' or p == '96': fg = 'cyan'
				if p == '37' or p == '97': fg = 'white'
				if p == '40' or p == '100': bg = None
				if p == '41' or p == '101': bg = 'on_red'
				if p == '42' or p == '102': bg = 'on_green'
				if p == '43' or p == '103': bg = 'on_yellow'
				if p == '44' or p == '104': bg = 'on_blue'
				if p == '45' or p == '105': bg = 'on_magenta'
				if p == '46' or p == '106': bg = 'on_cyan'
				if p == '47' or p == '107': bg = 'on_white'
			i += len(m.group(0))
			continue
		print("WEIRD ERROR")
		exit()
	else:
		sys.stdout.write(colored(output[i], fg, bg, attrs))
		i += 1
