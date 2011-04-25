print("Launched Lua kernel.")

function foo()
	print("Let's assume a kernel were running here. Wheeeee!")
	
	for i = 1, 5 do
		print(i)
    end
    
    print("Apples " .. 45 .. " pears")
end