#!/usr/bin/env julia

try
	using LibSerialPort
catch
	println(stderr, "I require LibSerialPort.jl to work.")
	println(stderr, "Open 'julia' and enter into Pkg mode by hitting ]")
	println(stderr, "Then enter 'add LibSerialPort' to install the dependencies.")
	exit(1)
end

function main(args:: AbstractVector{String} = ARGS)
	if isempty(args)
		println("Usage: $(PROGRAM_FILE |> basename) <port> [clearscreen]")
		println("Available ports:")
		list_ports()
		return
	end
	
	clearscreen = parse(Bool, get(args, 2, "false"))
	
	rssi_data = Int[]
	framesize_data = Int[]
	frameid_data = Int[]
	bmp_pres_data = Float32[]
	bmp_temp_data = Float32[]
	lm35_temp_data = Float32[]
	dht11_temp_data = Float32[]
	dht11_humi_data = Float32[]
	
	open(args[1], 115200) do sp
		# in case we start listening in the middle of a frame, discard it
		line = readline(sp)
		while line != "-- END"
			line = readline(sp)
		end
		
		# start reading frames indefinitely
		while true
			readline(sp) # consume "-- START"
			
			rssi = parse(Int, readline(sp))
			framesize = parse(Int, readline(sp))
			frameid = parse(Int, readline(sp))
			bmp_pres = parse(Float32, readline(sp))
			bmp_temp = parse(Float32, readline(sp))
			lm35_temp = parse(Float32, readline(sp))
			dht11_temp = parse(Float32, readline(sp))
			dht11_humi = parse(Float32, readline(sp))
			
			push!(rssi_data, rssi)
			push!(framesize_data, framesize)
			push!(frameid_data, frameid)
			push!(bmp_pres_data, bmp_pres)
			push!(bmp_temp_data, bmp_temp)
			push!(lm35_temp_data, lm35_temp)
			push!(dht11_temp_data, dht11_temp)
			push!(dht11_humi_data, dht11_humi)
			
			clearscreen && print("\e[2J\e[;H")
			lines = [
				"Frame $frameid | $framesize bytes | RSSI $rssi",
				"BMP: $bmp_pres hPa, $bmp_temp °C",
				"LM35: $lm35_temp °C",
				"DHT11: $dht11_humi%, $dht11_temp °C"
			]
			maxlen = maximum(length, lines)
			
			println('|', '='^(maxlen+2), '|')
			for l in lines
				println("| ", rpad(l, maxlen), " |")
			end
			println('|', '='^(maxlen+2), '|')
			println()
						
			readline(sp) # consume "-- END"
		end
	end
end

try
	Base.exit_on_sigint(false)
	main()
catch e
	!isa(e, InterruptException) && rethrow()
	println()
end
