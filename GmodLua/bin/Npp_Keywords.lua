// outputs functions and keywords in Notepad++ xml format

local output = {}

output.libraries = {}
output.libraryMethods = {}

output.objects = {}
output.objectMethods = {}

output.globalMethods = {}

output.constants = {}

output.flags = {}

output.scriptedFunctions = {}
output.scriptedVariables = {}

output.override = {}

local xside

if ( SERVER ) then
	if file.Exists("client.txt") then
		xside = file.Read( "client.txt" )
		file.Delete("client.txt")
	end
else
	if file.Exists("server.txt") then
		xside = file.Read( "server.txt" )
		file.Delete("server.txt")
	end
end

if xside then
	for _, v in pairs(string.Explode("`", xside)) do
		local all = string.Explode("~", v)
		
		for _,g in pairs(string.Explode("%", all[2])) do
			table.insert(output[all[1]], g)
		end
	end
	for _,g in pairs({"_G", "_E", "_R", "_ENT", "color_black", "color_white", "color_transparent"}) do
		table.insert(output.globalMethods, g)
	end
end

local function GetFunctions( tab )
	local functions = {}
	local const = {}

	for k, v in pairs( tab ) do
		if ( type(v) == "function" ) then
			table.insert( functions, tostring(k) )
		elseif ( type(v) == "string" ) then
			table.insert( const, tostring(k) )
		end
	end
	
	--table.sort( functions )
	return functions, const
end

local Ignores = { "mathx", "stringx", "_G", "_R", "_E", "GAMEMODE", "g_SBoxObjects", "tablex", "color_black",
				  "color_white", "utilx", "_LOADLIB", "_LOADED", "color_transparent", "filex", "func", "DOF_Ents", 
				  "Morph", "_ENT" }

// Retrieve Everything Global
for k, v in pairs(_G) do
	if type(k) == "string" then
		if !table.HasValue( Ignores, k ) then
			if type(v) == "table" then
				--if type(v) == "table" then PrintTable(v) end
				--Msg("Library: "..tostring(v).."\n")
				if !table.HasValue(output.libraries, k) then
					table.insert(output.libraries, k)
				end
				for _,method in pairs(GetFunctions(v)) do
					if string.sub(method, 1, 2) == "__" then
						if !table.HasValue(output.flags, method) then
							table.insert(output.flags, method)
						end
					else
						if !table.HasValue(output.libraryMethods, k.."."..method) then
							table.insert(output.libraryMethods, k.."."..method)
						end
					end
				end
			elseif type(v) == "function" then
				if !table.HasValue(output.globalMethods, k) then
					table.insert(output.globalMethods, k)
				end
			else
				if !table.HasValue(output.constants, k) then
					table.insert(output.constants, k)
				end
				--if type(v) != "number" then print(k, type(v)) end
			end
		end
	end
end

// Retrieve Meta Objects
for k, v in pairs(_R) do
	if type(k) == "string" then
		if !table.HasValue( Ignores, k ) then
			if type(v) == "table" then
				--Msg("MetaTable: "..tostring(v).."\n")
				if !table.HasValue(output.objects, k) then
					table.insert(output.objects, k)
				end
				
				for _,method in pairs(GetFunctions(v)) do
					if string.sub(method, 1, 2) == "__" then
						if !table.HasValue(output.flags, method) then
							table.insert(output.flags, method)
						end
					else
						if !table.HasValue(output.objectMethods, method) then
							table.insert(output.objectMethods, method)
						end
					end
				end
			end
		end
	end
end
for k,v in pairs(weapons.Get("weapon_base")) do
	if type(k) == "string" then
		if type(v) == "function" then
			if !table.HasValue(output.scriptedFunctions, "self."..k) then
				table.insert(output.scriptedFunctions, "self."..k)
			end
			if !table.HasValue(output.scriptedFunctions, "self:"..k) then
				table.insert(output.scriptedFunctions, "self:"..k)
			end
			if !table.HasValue(output.scriptedFunctions, "SWEP."..k) then
				table.insert(output.scriptedFunctions, "SWEP."..k)
			end
			if !table.HasValue(output.scriptedFunctions, "SWEP:"..k) then
				table.insert(output.scriptedFunctions, "SWEP:"..k)
			end
		elseif type(v) == "string" or type(v) == "number" or type(v) == "boolean" then
			if !table.HasValue(output.scriptedVariables, "self."..k) then
				table.insert(output.scriptedVariables, "self."..k)
			end
			if !table.HasValue(output.scriptedVariables, "SWEP."..k) then
				table.insert(output.scriptedVariables, "SWEP."..k)
			end
		end
	end
end
for _,kind in pairs({"base_anim", "base_point", "base_brush", "base_vehicle"}) do
	if SERVER or (kind != "base_point" and kind != "base_brush") then
		for k,v in pairs(scripted_ents.Get(kind)) do
			if type(k) == "string" then
				if type(v) == "function" then
					if !table.HasValue(output.scriptedFunctions, "self."..k) then
						table.insert(output.scriptedFunctions, "self."..k)
					end
					if !table.HasValue(output.scriptedFunctions, "self:"..k) then
						table.insert(output.scriptedFunctions, "self:"..k)
					end
					if !table.HasValue(output.scriptedFunctions, "ENT."..k) then
						table.insert(output.scriptedFunctions, "ENT."..k)
					end
					if !table.HasValue(output.scriptedFunctions, "ENT:"..k) then
						table.insert(output.scriptedFunctions, "ENT:"..k)
					end
				elseif type(v) == "string" or type(v) == "number" or type(v) == "boolean" then
					if !table.HasValue(output.scriptedVariables, "self."..k) then
						table.insert(output.scriptedVariables, "self."..k)
					end
					if !table.HasValue(output.scriptedVariables, "ENT."..k) then
						table.insert(output.scriptedVariables, "ENT."..k)
					end
				end
			end
		end
	end
end
--[[  // Code to determine TOOL functions and variables
local tbl1 = {}
local tbl2 = {}
for _, t in pairs(weapons.Get('gmod_tool').Tool) do
	for k, v in pairs(t) do
		if type(v) == 'function' then
			if !table.HasValue(tbl1, k) then
				table.insert(tbl1,k)
			end
		else
			if !table.HasValue(tbl2, k) then
				table.insert(tbl2,k)
			end
		end
	end
end
table.sort(tbl1)
table.sort(tbl2)
print('\nFunctions:\n')
table.foreach(tbl1, function(_,v) print(v) end)
print('\nConstants:\n')
table.foreach(tbl2, function(_,v) print(v) end)
]]
local funct = {funct = "scriptedFunctions", const = "scriptedVariables"}
local TOOL = {} -- built from the above code manually
TOOL.funct = {"Deploy", "DrawHUD", "DrawToolScreen", "FreezeMovement", "Holster", "LeftClick", "Reload", "RightClick", "Think"}
TOOL.const = {"BuildCPanel", "AddToMenu", "AllowedCVar", "Command", "Category", "ClientConVar", "ConfigName", "LastMessage", "LeftClickAutomatic", "Message", "Mode", "Model", "Name", "RequiresTraceHit", "RightClickAutomatic", "ServerConVar", "Stage"}

for kind, tbl in pairs(TOOL) do
	for _,v in pairs(tbl) do
		if  !table.HasValue(output[funct[kind]], "self."..v)then
			table.insert(output[funct[kind]], "self."..v)
		end
		if !table.HasValue(output[funct[kind]], "self:"..v) and (kind == "funct") then
			table.insert(output[funct[kind]], "self:"..v)
		end
		if !table.HasValue(output[funct[kind]], "TOOL."..v) then
			table.insert(output[funct[kind]], "TOOL."..v)
		end
		if !table.HasValue(output[funct[kind]], "TOOL:"..v) and (kind == "funct") then
			table.insert(output[funct[kind]], "TOOL:"..v)
		end
	end
end

for k,v in pairs(GAMEMODE.BaseClass) do
	if type(k) == "string" then
		if type(v) == "function" then
			if !table.HasValue(output.scriptedFunctions, "self."..k) then
				table.insert(output.scriptedFunctions, "self."..k)
			end
			if !table.HasValue(output.scriptedFunctions, "self:"..k) then
				table.insert(output.scriptedFunctions, "self:"..k)
			end
			if !table.HasValue(output.scriptedFunctions, "GM."..k) then
				table.insert(output.scriptedFunctions, "GM."..k)
			end
			if !table.HasValue(output.scriptedFunctions, "GM:"..k) then
				table.insert(output.scriptedFunctions, "GM:"..k)
			end
			if !table.HasValue(output.scriptedFunctions, "GAMEMODE."..k) then
				table.insert(output.scriptedFunctions, "GAMEMODE."..k)
			end
			if !table.HasValue(output.scriptedFunctions, "GAMEMODE:"..k) then
				table.insert(output.scriptedFunctions, "GAMEMODE:"..k)
			end
		else--if type(v) == "string" or type(v) == "number" or type(v) == "boolean" then
			if !table.HasValue(output.scriptedVariables, "self."..k) then
				table.insert(output.scriptedVariables, "self."..k)
			end
			if !table.HasValue(output.scriptedVariables, "GM."..k) then
				table.insert(output.scriptedVariables, "GM."..k)
			end
			if !table.HasValue(output.scriptedFunctions, "GAMEMODE."..k) then
				table.insert(output.scriptedFunctions, "GAMEMODE."..k)
			end
		end
	end
end
local effect_hooks = {"Init", "Think", "Render"}
for _,hook in pairs(effect_hooks) do
	if !table.HasValue(output.scriptedFunctions, "self."..hook) then
		table.insert(output.scriptedFunctions, "self."..hook)
	end
	if !table.HasValue(output.scriptedFunctions, "self:"..hook) then
		table.insert(output.scriptedFunctions, "self:"..hook)
	end
	
	if !table.HasValue(output.scriptedFunctions, "EFFECT."..hook) then
		table.insert(output.scriptedFunctions, "EFFECT."..hook)
	end
	if !table.HasValue(output.scriptedFunctions, "EFFECT:"..hook) then
		table.insert(output.scriptedFunctions, "EFFECT:"..hook)
	end
end

local OUT = ""
if xside then
	for _,v in pairs({"__add", "__sub", "__mul", "__div", "__pow", "__unm", "__concat", "__eq", "__lt", "__le", "__index", "__newindex", "__call", "__tostring", "__gc", "__mode", "__metatable"}) do
		if !table.HasValue(output.flags, v) then
			table.insert(output.flags, v)
		end
	end
	for _,v in pairs({"Entity", "Owner", "Weapon"}) do
		if !table.HasValue(output.scriptedVariables, "self."..v) then
			table.insert(output.scriptedVariables, "self."..v)
		end
	end

	local cat = {}
	cat[1] = {output.constants}
	cat[2] = {output.globalMethods}
	cat[3] = {output.scriptedVariables}
	cat[4] = {output.scriptedFunctions}
	cat[5] = {output.libraries}
	cat[6] = {output.libraryMethods}
	cat[7] = {output.objectMethods}
	cat[8] = {output.flags}
	
	OUT = [[            <Keywords name="0">and break do else elseif end false for function if in local nil not or repeat return then true until while</Keywords>]]
	for k,tbls in pairs(cat) do
		OUT = OUT .. "\n            <Keywords name=\"" .. k .. [[">]]
		for _,tbl in pairs(tbls) do
			for _,v in pairs(tbl) do
				OUT = OUT..v.." "
			end
		end
		OUT = string.sub(OUT, 1, -2) -- chop off the extra space
		OUT = OUT .. "</Keywords>"
	end
	
	file.Write("gmod_lua.txt", OUT.."\n")
	
	print("Finished generating keywords for Notepad++.");
else
	local tab = {}
	for name, tbl in pairs(output) do
		local str = name .. "~"
		local t = {}
		for _,v in pairs(tbl) do
			table.insert(t, v)
		end
		
		table.insert(tab, str..string.Implode("%", t))
	end
	
	OUT = string.Implode("`", tab)

	
	if ( SERVER ) then
		file.Write( "server.txt", OUT )
	else
		file.Write( "client.txt", OUT )
	end
end
