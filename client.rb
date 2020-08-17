#!/usr/bin/env ruby

require 'socket'

starttime = Process.clock_gettime(Process::CLOCK_MONOTONIC)

s = TCPSocket.new 'localhost', 8989
# SMP8673B IP address
# s = TCPSocket.new '10.60.1.80', 8989

s.write("test_files/#{ARGV[0]}.c\n")

s.each_line do |line|
    # puts line
end

s.close

endtime = Process.clock_gettime(Process::CLOCK_MONOTONIC)
elapsed = endtime - starttime
puts "Elapsed: #{elapsed} (#{ARGV[0]}.c) --- Connect times: #{ARGV[1]}"