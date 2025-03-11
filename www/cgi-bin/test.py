#!/usr/bin/env python3

import os
import sys
import cgi
import time

# Print headers
print("Content-Type: text/html")
print("")  # Empty line to separate headers from body

# Print HTML content
print("<html>")
print("<head><title>>Python CGI</title></head>")
print("<body>")
print("<h1>Welcom to Python CGI</h1>")
print("<p>This script was executed at: {}</p>".format(time.strftime("%Y-%m-%d %H:%M:%S")))

# Process form data if this is a POST request
if os.environ.get("REQUEST_METHOD") == "POST":
    print("<h2>POST Data:</h2>")
    try:
        form = cgi.FieldStorage()
        if form:
            print("<table border='1'>")
            print("<tr><th>Field</th><th>Value</th></tr>")
            for field in form:
                print("<tr><td>{}</td><td>{}</td></tr>".format(field, form.getvalue(field)))
            print("</table>")
        else:
            print("<p>No POST data received</p>")
    except Exception as e:
        print("<p>Error parsing POST data: {}</p>".format(e))

# Show query string parameters
if os.environ.get("QUERY_STRING"):
    print("<h2>Query String Parameters:</h2>")
    print("<table border='1'>")
    print("<tr><th>Parameter</th><th>Value</th></tr>")
    for param in os.environ.get("QUERY_STRING").split("&"):
        if "=" in param:
            key, value = param.split("=", 1)
            print("<tr><td>{}</td><td>{}</td></tr>".format(key, value))
    print("</table>")
# Display environment variables
print("<h2>CGI Environment Variables:</h2>")
print("<table border='1'>")
print("<tr><th>Variable</th><th>Value</th></tr>")
for key in sorted(os.environ.keys()):
    print("<tr><td>{}</td><td>{}</td></tr>".format(key, os.environ[key]))
print("</table>")


print("</body>")
print("</html>")