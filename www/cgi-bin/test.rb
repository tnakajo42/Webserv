#!/usr/bin/ruby
require 'cgi'
cgi = CGI.new
puts "Content-Type: text/html"
puts ""
puts "<!DOCTYPE html>"
puts "<html><body>"
puts "<h1>Ruby CGI</h1>"
puts "<p>Name: #{cgi['name'].empty? ? 'N/A' : cgi['name']}</p>"
puts "<p>Message: #{cgi['message'].empty? ? 'N/A' : cgi['message']}</p>"
puts "</body></html>"
