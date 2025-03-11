#!/usr/bin/php-cgi
<?php
// Print headers
header("Content-Type: text/html");
echo "\n"; // Empty line to separate headers from body

// Start HTML output
echo "<!DOCTYPE html>";
echo "<html lang='en'>";
echo "<head>";
echo "    <meta charset='UTF-8'>";
echo "    <meta name='viewport' content='width=device-width, initial-scale=1.0'>";
echo "    <title>Welcome to PHP CGI</title>";
echo "    <link rel='icon' href='favicon.ico' type='image/x-icon'>";
echo "    <style>";
echo "        body {";
echo "            background: linear-gradient(135deg, #ffffff, #f4f4f4);";
echo "            color: #000;";
echo "            font-family: 'Arial', sans-serif;";
echo "            min-height: 100vh;";
echo "            margin: 0;";
echo "            padding: 20px;";
echo "            display: flex;";
echo "            flex-direction: column;";
echo "        }";
echo "        .container {";
echo "            max-width: 800px;";
echo "            margin: 0 auto;";
echo "            padding: 30px;";
echo "            background: rgba(255, 255, 255, 0.9);";
echo "            border-radius: 15px;";
echo "            box-shadow: 0 0 20px rgba(0, 0, 0, 0.1);";
echo "            flex: 1;";
echo "        }";
echo "        h1 {";
echo "            text-align: center;";
echo "            color: #00ffcc;";
echo "            text-shadow: 0 0 10px rgba(0, 255, 204, 0.5);";
echo "            margin-bottom: 40px;";
echo "        }";
echo "        .section {";
echo "            margin-bottom: 40px;";
echo "            padding: 20px;";
echo "            border-radius: 10px;";
echo "            background: rgba(0, 0, 0, 0.1);";
echo "            transition: transform 0.3s ease;";
echo "        }";
echo "        .section:hover {";
echo "            transform: translateY(-5px);";
echo "        }";
echo "        table {";
echo "            width: 100%;";
echo "            border-collapse: collapse;";
echo "            margin-top: 20px;";
echo "        }";
echo "        th, td {";
echo "            padding: 10px;";
echo "            border: 1px solid rgba(0, 255, 204, 0.3);";
echo "            text-align: left;";
echo "        }";
echo "        th {";
echo "            background: rgba(0, 255, 204, 0.2);";
echo "            color: #00ffcc;";
echo "        }";
echo "        p {";
echo "            margin: 10px 0;";
echo "        }";
echo "        #themeToggle {";
echo "            position: fixed;";
echo "            bottom: 20px;";
echo "            left: 50%;";
echo "            transform: translateX(-50%);";
echo "            padding: 10px 20px;";
echo "            background: #00ffcc;";
echo "            color: #000;";
echo "            border: none;";
echo "            border-radius: 5px;";
echo "            font-weight: bold;";
echo "            cursor: pointer;";
echo "            transition: all 0.3s ease;";
echo "        }";
echo "        #themeToggle:hover {";
echo "            transform: translateX(-50%) scale(1.05);";
echo "            box-shadow: 0 0 15px rgba(0, 255, 204, 0.5);";
echo "        }";
echo "        body.dark-theme {";
echo "            background: linear-gradient(135deg, #1a1a1a, #2d2d2d);";
echo "            color: #fff;";
echo "        }";
echo "        body.dark-theme .container {";
echo "            background: rgba(255, 255, 255, 0.1);";
echo "            box-shadow: 0 0 20px rgba(0, 0, 0, 0.3);";
echo "        }";
echo "        body.dark-theme .section {";
echo "            background: rgba(0, 0, 0, 0.2);";
echo "        }";
echo "        body.dark-theme th {";
echo "            background: rgba(0, 255, 204, 0.2);";
echo "        }";
echo "    </style>";
echo "</head>";
echo "<body>";

// Main container
echo "    <div class='container'>";
echo "        <h1>PHP CGI!</h1>";

// POST Data Section
echo "        <div class='section'>";
echo "            <h2>Your CGI Submission</h2>";
echo "            <p>Submitted at: " . date("Y-m-d H:i:s") . "</p>";
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    if (!empty($_POST)) {
        echo "            <table>";
        echo "                <tr><th>Field</th><th>Value</th></tr>";
        foreach ($_POST as $field => $value) {
            echo "                <tr><td>" . htmlspecialchars($field) . "</td><td>" . htmlspecialchars($value) . "</td></tr>";
        }
        echo "            </table>";
    } else {
        echo "            <p>No POST data received</p>";
    }
} else {
    echo "            <p>This page expects a POST request. Use the form to submit data!</p>";
}
echo "        </div>";

// Environment Variables Section
echo "        <div class='section'>";
echo "            <h2>CGI Environment Variables</h2>";
echo "            <table>";
echo "                <tr><th>Variable</th><th>Value</th></tr>";
ksort($_SERVER);
foreach ($_SERVER as $key => $value) {
    echo "                <tr><td>" . htmlspecialchars($key) . "</td><td>" . htmlspecialchars($value) . "</td></tr>";
}
echo "            </table>";
echo "        </div>";
echo "    </div>";

// Theme Toggle Button
echo "    <button id='themeToggle' onclick='toggleTheme()'>Switch to Dark Mode</button>";

// JavaScript for theme toggle
echo "    <script>";
echo "        function toggleTheme() {";
echo "            const body = document.body;";
echo "            const button = document.getElementById('themeToggle');";
echo "            body.classList.toggle('dark-theme');";
echo "            if (body.classList.contains('dark-theme')) {";
echo "                button.textContent = 'Switch to Light Mode';";
echo "                localStorage.setItem('theme', 'dark');";
echo "            } else {";
echo "                button.textContent = 'Switch to Dark Mode';";
echo "                localStorage.setItem('theme', 'light');";
echo "            }";
echo "        }";
echo "        function loadTheme() {";
echo "            const savedTheme = localStorage.getItem('theme');";
echo "            const body = document.body;";
echo "            const button = document.getElementById('themeToggle');";
echo "            if (savedTheme === 'dark') {";
echo "                body.classList.add('dark-theme');";
echo "                button.textContent = 'Switch to Light Mode';";
echo "            } else {";
echo "                body.classList.remove('dark-theme');";
echo "                button.textContent = 'Switch to Dark Mode';";
echo "            }";
echo "        }";
echo "        document.addEventListener('DOMContentLoaded', loadTheme);";
echo "    </script>";

echo "</body>";
echo "</html>";
?>