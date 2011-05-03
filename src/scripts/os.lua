--[[--------------------------------------------------------------------
--  Jiauliyan OS - Released under the MIT License
--  Copyright (C) 2011 Paul Booth, Jialiya Huang, Tim Ryan
--  https://github.com/timcameronryan/jiauliyan
--]]--------------------------------------------------------------------

json = require("dkjson").json

print("Loading lua code...")

--[[--------------------------------------------------------------------
--  Serial Communication
--]]--------------------------------------------------------------------

-- kitshy loading bar

local __load_bar = {"|", "\\", "-", "/"}
local __load_bar_i = 1

function load_bar()
	vga.setbg(vga.BLACK)
	vga.setfg(vga.LIGHT_RED)
	vga.placechar(__load_bar[__load_bar_i], 79, 0)
	vga.setbg(vga.DARK_GREY)
	vga.setfg(vga.WHITE)
	__load_bar_i = (__load_bar_i % 4) + 1
end

-- serial message polling
-- format: two bytes describing message size (<65536b) then listens for
-- message; returns true if message is received and on_receive() is called
-- else, false

__message_headers = {}
__message_responses = {}
__serin = ""

function read_segments(msg)
	local m = {}
	local i = 1
	while i <= msg:len() and msg:len() >= 2 do
		local len = msg:byte(i)*256 + msg:byte(i + 1)
		table.insert(m, msg:sub(i + 2, i + 2 + len - 1))
		i = i + 2 + len
	end
	return m
end

function handle_message(msg)
	local id = msg:byte(1)*256 + msg:byte(2)
	if not pcall(function ()
		local m = read_segments(msg:sub(3))
		__message_headers[id], len, status = json.decode(m[1])
		__message_responses[id] = m[2]
	end) then
		print("[ERROR] Bad message.")
	elseif not __message_headers[id] then
		print(status)
	end
	return id
end

function poll_for_message()
	load_bar()
	__serin = __serin .. serial.receive_all()
	if __serin:len() > 2 then
		local l = __serin:byte(1)*256 + __serin:byte(2)
		if __serin:len() >= l + 2 then
			retid = handle_message(__serin:sub(3, l + 2))
			__serin = __serin:sub(l + 3)
			return retid
		end
	end
	return -1
end

-- sends a serial message with two bytes for length and then input

local __send_message_id = 1

function send_message(msg, wait, callback)
	serial.send(string.char(math.floor(msg:len() / 256), msg:len() % 256) ..
		string.char(math.floor(__send_message_id / 256), __send_message_id % 256) ..
		msg)
	if wait then
		repeat until poll_for_message() == __send_message_id
		callback(__message_headers[__send_message_id], __message_responses[__send_message_id])
		__message_headers[__send_message_id] = nil
		__message_responses[__send_message_id] = nil
	end
	__send_message_id = __send_message_id + 1
end

--[[--------------------------------------------------------------------
--  HTTP lib
--]]--------------------------------------------------------------------

-- http requests over serial

function send_http_req(method, url, headers, body, callback)
	send_message(json.encode({action='httpreq', method=method, headers=headers, body=body, url=url}), true, callback)
end

function url_encode(s)
	s = string.gsub(s, "([&=+%c])", function (c)
		return string.format("%%%02X", string.byte(c))
	end)
	s = string.gsub(s, " ", "+")
	return s
end

function url_encode_table(t)
	local s = ""
	for k,v in pairs(t) do
		s = s .. "&" .. url_encode(k) .. "=" .. url_encode(v)
	end
	return string.sub(s, 2)     -- remove first `&'
end

--[[--------------------------------------------------------------------
--  VGA Effects
--]]--------------------------------------------------------------------

function temp_screen(callback)
	local vgad = vga.getdata()
	
	callback(vgad)

	for x=1,80 do
		for y=1,25 do
			local i = y*vga.SCREEN_WIDTH + x
			vga.setfg(vgad.fg[i])
			vga.setbg(vgad.bg[i])
			vga.placechar(vgad.chars:sub(i,i), x-1, y-1)
		end
	end
	
	vga.setfg(vga.WHITE)
	vga.setbg(vga.DARK_GREY)
end

--[[--------------------------------------------------------------------
--  Jiauliyan OS Lua Command Line
--]]--------------------------------------------------------------------

--remove backspace (hack)
function removebackspace(cmd)
	 cmd, count = string.gsub(cmd, ".\b","",1)
	 if (count > 0) then 
	    	return removebackspace(cmd) 
	 else
		return cmd 
	 end
end

-- register commands

local commands = {}
local command_list = ""

function register_command(prog, desc, callback)
	commands[prog] = callback
	command_list = command_list .. "    " .. prog .. (" "):rep(30 - prog:len()) .. desc .. "\n"
end

-- list command

register_command("ls", "Displays a directory listing... sure.", function (args, line)
	print("/\t\tThere's only one file! It's the root file!")
end)

-- sudo command

register_command("sudo make me a sandwich", "Makes a sandwich.", function (args, line)
	print("/\t\tJk, make it yoself.")
end)

-- fetch webpage and display source

register_command("fetch", "Fetches a webpage, displays source.", function (args, line)
	send_http_req('GET', args[2] or '', nil, nil,
		function (headers, body)
			if not headers or headers['code'] ~= "200" then
				print("Error: could not send request.")
			else
				print(body)
			end
		end)
end)

-- tweeting through identi.ca

register_command("get-tweets", "Displays tweet timeline.", function (args, line)
	send_http_req('GET', 'http://identi.ca/api/statuses/friends_timeline/jiauliyan.json?count=5', nil, nil,
		function (headers, body)
			if not headers or headers['code'] ~= "200" then
				print("Error: could not receive tweets.")
			else
				data, code, status = json.decode(body)
				if not data then
					print(status)
				else
					for i,v in ipairs(data) do
						print("----------------------------------------")
						print("@" .. v['user']['screen_name'] .. ": " .. v['text'])
					end
				end
			end
		end)
end)

register_command("send-tweet", "Specify and send a tweet.", function (args, line)
	send_http_req('POST', 'http://identi.ca/api/statuses/update.json',
		{["Authorization"]="Basic amlhdWxpeWFuOm9saW5oYXNub3RyZWVz",
		 ["Content-Type"]="application/x-www-form-urlencoded"},
		url_encode_table({status=string.sub(line, ("send-tweet "):len(), ("send-tweet "):len() + 140)}),
		function (headers, body)
			if not headers or headers['code'] ~= "200" then
				print("Error: could not receive tweets.")
			else
				for i,v in ipairs(json.decode(body)) do
					print("----------------------------------------")
					print("@" .. v['user']['screen_name'] .. ": " .. v['text'])
				end
			end
		end)
end)

-- display a color image from a URL

register_command("image", "Specify a URL of an image to display.", function (args, line)
	send_http_req('GET', 'http://asahina.co.cc/breeze/image.py?' .. (args[2] or ''), nil, nil,
		function (headers, body)
			if not headers or headers['code'] ~= "200" then
				print('Could not display image.')
				return
			end
			
			local d = body
			for i=3,#d,3 do
				ch = d:sub(i,i)
				anc = string.byte("a", 1)
				bg = d:byte(i-2) - anc
				fg = d:byte(i-1) - anc
				if ch ~= "\n" then
					vga.setbg(bg)
					vga.setfg(fg)
					io.write(ch)
				end
			end
		
			vga.setbg(vga.DARK_GREY)
			vga.setfg(vga.WHITE)
		end)
end)

-- alert command

register_command("alert", "Throws a wrench in our VGA driver.", function ()
	temp_screen(function (vgad)
		local dx, dy, dw, dh = 2, 2, 74, 20
		
		-- display all available characters
		for i=1,255 do
			vga.setfg(vga.LIGHT_RED)
			vga.setbg(vga.BLACK)
			vga.placechar(string.char(i), (i-1) % 80, math.floor((i-1)/80))
		end
		
		-- modal dialog
		vga.setfg(vga.WHITE)
		vga.setbg(vga.BLUE)
		for y=dy,dy+dh do vga.placechar(string.char(186), dx, y) end
		for y=dy,dy+dh do vga.placechar(string.char(186), dx+dw, y) end
		for x=dx,dx+dw do vga.placechar(string.char(205), x, dy) end
		for x=dx,dx+dw do vga.placechar(string.char(205), x, dy+dh) end
		vga.placechar(string.char(201), dx, dy)
		vga.placechar(string.char(187), dx+dw, dy)
		vga.placechar(string.char(200), dx, dy+dh)
		vga.placechar(string.char(188), dx+dw, dy+dh)
		for x=dx+1,dx+dw-1 do 
			for y=dy+1,dy+dh-1 do vga.placechar(' ', x, y) end
		end
		
		local str = "Something awesome is happening!"
		for x=1,dw-4 do
			vga.placechar(str:sub(x,x), x+dx+2, dy+4)
		end
		
		for y=dy+1,dy+dh+1 do
			local i = y*vga.SCREEN_WIDTH + dx+dw+1
			vga.setfg(vgad.fg[i] % 8)
			vga.setbg(vgad.bg[i] % 8)
			vga.placechar(vgad.chars:sub(i, i), dx+dw+1, y)
		end
		for x=dx+1,dx+dw+1 do
			local i = (dy+dh+1)*vga.SCREEN_WIDTH + x
			vga.setfg(vgad.fg[i] % 8)
			vga.setbg(vgad.bg[i] % 8)
			vga.placechar(vgad.chars:sub(i, i), x, dy+dh+1)
		end
		
		vga.setbg(vga.DARK_GREY)
		vga.setfg(vga.WHITE)
		
		--wait
		io.read()
	end)
end)

-- Command line interface loop
-- Executes command as the user enters them

function cli()
	io.write("Launch HTTP server, then press RETURN...")
	io.flush()
	io.read()

	vga.setfg(vga.LIGHT_CYAN)
	print("")
	print("Welcome to the Jiauliyan OS command line.")
	print("")
	print("Available commands:")
	print(command_list)
	vga.setfg(vga.WHITE)
	
	local cmd
	local serin = ""
	while true do
		io.write("> ")
		io.flush()
		cmd = trim(io.read())
		cmd = removebackspace(cmd)
		
		local args = {}
		for v in cmd:gmatch("%S+") do
			table.insert(args, v)
		end
		local prog = args[1] or ''
		if commands[prog] then
			commands[prog](args, cmd)
		else
			print("Unrecognized command: " .. prog)
		end
	end
end
