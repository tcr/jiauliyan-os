import subprocess, shlex
import sys, re
from termcolor import colored

output = subprocess.Popen(shlex.split("img2txt -W 80 -f ansi goog.png"), stdout=subprocess.PIPE).communicate()[0]

i = 0
while i < len(output):
	m = re.match(r"\033\[s\n", output[i:])
	if m:
		i += len(m.group(0))
		continue
	else:
		sys.stdout.write(output[i])
		i += 1
