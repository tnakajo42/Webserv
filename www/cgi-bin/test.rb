#!/usr/bin/ruby
require 'cgi'

cgi = CGI.new
puts "Content-Type: text/html\r\n\r\n"
puts "<!DOCTYPE html>"
puts "<html><body>"
puts "<h1>Ruby CGI Debug</h1>"
puts "<p>Script running: #{__FILE__}</p>"
puts "<p>Environment: #{ENV.to_hash.inspect}</p>"
puts "<p>Params: #{cgi.params.inspect}</p>"
puts "<p>Name: #{cgi['name'].empty? ? 'N/A' : CGI.escapeHTML(cgi['name'])}</p>"
puts "<p>Message: #{cgi['message'].empty? ? 'N/A' : CGI.escapeHTML(cgi['message'])}</p>"
puts "</body></html>"