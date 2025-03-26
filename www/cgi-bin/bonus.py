#!/usr/bin/env python3

import os
import sys
import time
import uuid
import http.cookies
import json
import requests

s = requests.session()
#~
#ログイン処理
#~
s.cookies#これ

import pickle

with open('cookie.dump', 'wb') as f:
    pickle.dump(s.cookies, f)

# Define session file path
SESSION_FILE = "sessions.json"

# Load sessions from file
def load_sessions():
    try:
        with open(SESSION_FILE, "r") as f:
            return json.load(f)
    except FileNotFoundError:
        return {}
    except Exception as e:
        print(f"Error loading sessions: {e}", file=sys.stderr)
        return {}

# Save sessions to file
def save_sessions(sessions):
    try:
        with open(SESSION_FILE, "w") as f:
            json.dump(sessions, f)
    except Exception as e:
        print(f"Error saving sessions: {e}", file=sys.stderr)

# Initialize sessions
sessions = load_sessions()

# Handle cookies
cookie = http.cookies.SimpleCookie()
raw_cookie = os.environ.get("HTTP_COOKIE", "")
if raw_cookie:
    cookie.load(raw_cookie)
else:
    print("Debug: No HTTP_COOKIE received", file=sys.stderr)

# Get or create session ID
session_id = cookie.get("session_id")
new_session = False
if not session_id:
    session_id = str(uuid.uuid4())
    cookie["session_id"] = session_id
    cookie["session_id"]["path"] = "/"
    cookie["session_id"]["max-age"] = 3600
    cookie["session_id"]["domain"] = "localhost"
    new_session = True
    print(f"Debug: New session ID: {session_id}", file=sys.stderr)
else:
    session_id = session_id.value

# Print headers
sys.stdout.write("Content-Type: text/html\r\n")
if new_session:
    cookie_header = cookie.output().split(": ", 1)[1]
    sys.stdout.write(f"Set-Cookie: {cookie_header}\r\n")
sys.stdout.write("\r\n")

# Update session data
if session_id not in sessions:
    sessions[session_id] = {
        "visits": 0,
        "created_at": time.strftime("%Y-%m-%d %H:%M:%S")
    }
sessions[session_id]["visits"] = sessions[session_id].get("visits", 0) + 1
session_data = sessions[session_id]

# Print HTML content
print("<html>")
print("<head><title>Python CGI with Sessions</title></head>")
print("<body>")
print("<h1>Welcome to Python CGI</h1>")
print("<p>Executed at: {}</p>".format(time.strftime("%Y-%m-d %H:%M:%S")))
print("<p>Session ID: {}</p>".format(session_id))
print("<p>Visits in this session: {}</p>".format(session_data["visits"]))
print("<p>Session created at: {}</p>".format(session_data["created_at"]))
print("<p>Raw HTTP_COOKIE: {}</p>".format(raw_cookie or "None"))
print("</body>")
print("</html>")

# Save sessions
save_sessions(sessions)