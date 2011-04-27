require "json"

print("Loading lua code...")

-- sends a serial message with two bytes for length and then input

function send_message(msg)
	serial_send(string.char(math.floor(msg:len() / 256), msg:len() % 256) .. msg)
end

-- serial message polling
-- format: two bytes describing message size (<65536b) then listens for
-- message; returns true if message is received and on_receive() is called
-- else, false

local serin = ""

function check_serial_input()
	serin = serin .. serial_receive_all()
	if serin:len() > 2 then
		local l = serin:byte(1)*256 + serin:byte(2)
		if serin:len() >= l + 2 then
			on_receive(serin:sub(3, l + 3))
			serin = serin:sub(l + 3)
			return true
		end
	end
	return false
end

--[[--------------------------------------------------------------------
Jiauliyan OS Lua Command Line
----------------------------------------------------------------------]]

-- on serial message received
-- msg has been decoded from JSON

function on_receive(msg)
	print("Received message:\n" .. msg)
end

-- Command line interface loop
-- Executes command as the user enters them

function cli()
	print("Welcome to the Lua command line.")
	print("")
	print("Current commands:")
	print("    ls\t\t\t\tList available commands")
	print("    fetch [url]\t\t\tFetch a url over serial")
	print("    image [url]\t\t\tFetch an image over serial")
	print("    sudo make me a sandwich\tThe club cant even handle me right now")
	print("")
	
	io.write("Launch HTTP server, then press RETURN...")
	io.flush()
	io.read()
	
	local cmd
	local serin = ""
	while true do
		io.write("> ")
		io.flush()
		cmd = trim(io.read())
		
		local cmdp = {}
		for v in cmd:gmatch("%S+") do
			table.insert(cmdp, v)
		end
		local prog = cmdp[1] or ''
		if prog == "ls" then
			print("Directory listing whoo")
		elseif cmd == "sudo make me a sandwich" then
			print("Make it yoself")
		elseif prog == "fetch" then
			send_message(json.encode({action='httpreq', method='get', url=cmdp[2] or ''}))
			repeat until check_serial_input()
		elseif prog == "image" then
			send_message(json.encode({action='httpimage', method='get', url=cmdp[2] or ''}))
			repeat until check_serial_input()
		else
			print("Unrecognized command: " .. cmd)
		end
	end
end
