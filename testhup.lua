#!/usr/bin/lua

local posix = require("posix")
local luahup = require("luahup")

local rv, err = luahup()
if not rv then
	io.stderr:write(tostring(err or "Luahup init failed\n"))
	os.exit(1)
end

counter = 0
while true do
	print("COUNTER: ", counter)
	counter = counter + 1
	posix.sleep(1)
end
