print("Loading lua code...")

function cli()
	print("Welcome to the Lua command line.")
	print("")
	print("Current commands:")
	print("    ls\t\t\t\tList available commands")
	print("    sudo make me a sandwich\tThe club cant even handle me right now")
	print("")
	
	local cmd
	while true do
		io.write("> ")
		io.flush()
		cmd = trim(io.read())
		
		if cmd == "ls" then
			print("Directory listing whoo")
		elseif cmd == "sudo make me a sandwich" then
			print("Make it yoself bitch")
		else
			print("Unrecognized command: " .. cmd)
		end
	end
end