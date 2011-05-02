json = require("dkjson").json

print("Loading lua code...")

--[[--------------------------------------------------------------------
Serial Communication
----------------------------------------------------------------------]]

-- serial message polling
-- format: two bytes describing message size (<65536b) then listens for
-- message; returns true if message is received and on_receive() is called
-- else, false

__message_responses = {}
__serin = ""

function handle_message(msg)
	local id = msg:byte(1)*256 + msg:byte(2)
	if not pcall(function () __message_responses[id] = json.decode(msg:sub(3)) end) then
		print("[ERROR] Bad message.")
	end
	return id
end

function poll_for_message()
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

local __send_message_id = 1;

function send_message(msg, wait, callback)
	serial.send(string.char(math.floor(msg:len() / 256), msg:len() % 256) ..
		string.char(math.floor(__send_message_id / 256), __send_message_id % 256) ..
		msg)
	if wait then
		repeat until poll_for_message() == __send_message_id
		callback(__message_responses[__send_message_id])
		__message_responses[__send_message_id] = nil
	end
	__send_message_id = __send_message_id + 1
end

--[[--------------------------------------------------------------------
HTTP lib
----------------------------------------------------------------------]]

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
Jiauliyan OS Lua Command Line
----------------------------------------------------------------------]]

-- Command line interface loop
-- Executes command as the user enters them

function cli()
	vga.setfg(vga.LIGHT_CYAN)
	print("Welcome to the Lua command line.")
	print("")
	print("Current commands:")
	print("    ls\t\t\t\tList available commands")
	print("    fetch [url]\t\t\tFetch a url over serial")
	print("    image [url]\t\t\tFetch an image over serial")
	print("    sudo make me a sandwich\tThe club cant even handle me right now")
	vga.setfg(vga.WHITE)
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
			send_http_req('GET', cmdp[2] or '', nil, nil, function (msg)
					if not msg or msg['code'] ~= "200" then
						print("Error: could not send request.")
					else
						print(msg['body'])
					end
				end)
		elseif prog == "get-tweets" then
			send_http_req('GET', 'http://identi.ca/api/statuses/friends_timeline/jiauliyan.json', nil, nil, function (msg)
					if not msg or msg['code'] ~= "200" then
						print("Error: could not receive tweets.")
					else
						for i,v in ipairs(json.decode(msg['body'])) do
							print("----------------------------------------")
							print("@" .. v['user']['screen_name'] .. ": " .. v['text'])
						end
					end
				end)
		elseif prog == "send-tweet" then
			send_http_req('POST', 'http://identi.ca/api/statuses/update.json',
				{["Authorization"]="Basic amlhdWxpeWFuOm9saW5oYXNub3RyZWVz",
				 ["Content-Type"]="application/x-www-form-urlencoded"},
				url_encode_table({status=string.sub(cmd, string.len("send-tweet "), string.len("send-tweet ") + 140)}),
				function (msg)
					if not msg or msg['code'] ~= "200" then
						print("Error: could not receive tweets.")
					else
						for i,v in ipairs(json.decode(msg['body'])) do
							print("----------------------------------------")
							print("@" .. v['user']['screen_name'] .. ": " .. v['text'])
						end
					end
				end)
		elseif prog == "image" then
			send_http_req('GET', 'http://asahina.co.cc/breeze/image.py?' .. (cmdp[2] or ''), nil, nil, function (msg)
					if not msg or msg['code'] ~= "200" then
						print('Could not display image.')
						return
					end
					
					local d = msg['body']
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
		else
			print("Unrecognized command: " .. cmd)
		end
	end
end
