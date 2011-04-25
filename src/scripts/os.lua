print("Init Lua kernel.")

function foo()
	print("Lua kernel start.")
	
	--[[
	local answer
	repeat
		io.write("continue with this operation (y/n)? ")
		io.flush()
		answer=io.read()
	until answer=="y" or answer=="n"]]
end