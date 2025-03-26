#!/usr/bin/env python3

import os
import sys
import cgi
import time

# Print headers
print("Content-Type: text/html")
print("")

# Print HTML content
print("<html>")
print("<head><title>Python CGI</title></head>")
print("<body>")
print("<h1>Welcome to Python CGI</h1>")
print("<p>This script was executed at: {}</p>".format(time.strftime("%Y-%m-%d %H:%M:%S")))

# Process GET request
if os.environ.get("REQUEST_METHOD") == "GET":
    # Parse GET data first
    print("<h2>Parsed GET Data:</h2>")
    try:
        form = cgi.FieldStorage(fp=sys.stdin, environ=os.environ)
        if form:
            print("<table border='1'>")
            print("<tr><th>Field</th><th>Value</th></tr>")
            for field in form:
                print("<tr><td>{}</td><td>{}</td></tr>".format(field, form.getvalue(field)))
            print("</table>")
            # Reconstruct raw data from form for display
            raw_data = "&".join(f"{key}={form.getvalue(key)}" for key in form)
            print("<h2>Raw GET Data:</h2>")
            print("<p>{}</p>".format(raw_data))
        else:
            print("<p>No GET data received</p>")
            print("<h2>Raw GET Data:</h2>")
            content_length = int(os.environ.get("CONTENT_LENGTH", 0))
            if content_length > 0:
                # This won’t work here because stdin is consumed, so we rely on form data above
                print("<p>Raw data not available after parsing</p>")
            else:
                print("<p>No GET data received (Content-Length is 0)</p>")
    except Exception as e:
        print("<p>Error parsing GET data: {}</p>".format(e))

# Display environment variables
print("<h2>CGI Environment Variables:</h2>")
print("<table border='1'>")
print("<tr><th>Variable</th><th>Value</th></tr>")
for key in sorted(os.environ.keys()):
    print("<tr><td>{}</td><td>{}</td></tr>".format(key, os.environ[key]))
print("</table>")

print("</body>")
print("</html>")