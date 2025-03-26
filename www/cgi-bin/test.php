#!/usr/bin/php-cgi
<?php
ob_start();

// Set headers
header("Content-Type: text/html");
echo "\r\n";

// Get raw POST data
$raw_post_data = file_get_contents("php://input");

// Start HTML output
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Welcome to PHP CGI</title>
    <link rel="icon" href="favicon.ico" type="image/x-icon">
    <style>
        /* Your existing CSS unchanged */
        body {
            background: linear-gradient(135deg, #ffffff, #f4f4f4);
            color: #000;
            font-family: 'Arial', sans-serif;
            min-height: 100vh;
            margin: 0;
            padding: 20px;
            display: flex;
            flex-direction: column;
        }
        .container {
            max-width: 800px;
            margin: 0 auto;
            padding: 30px;
            background: rgba(255, 255, 255, 0.9);
            border-radius: 15px;
            box-shadow: 0 0 20px rgba(0, 0, 0, 0.1);
            flex: 1;
        }
        h1 {
            text-align: center;
            color: #00ffcc;
            text-shadow: 0 0 10px rgba(0, 255, 204, 0.5);
            margin-bottom: 40px;
        }
        .section {
            margin-bottom: 40px;
            padding: 20px;
            border-radius: 10px;
            background: rgba(0, 0, 0, 0.1);
            transition: transform 0.3s ease;
        }
        .section:hover {
            transform: translateY(-5px);
        }
        table {
            width: 100%;
            border-collapse: collapse;
            margin-top: 20px;
        }
        th, td {
            padding: 10px;
            border: 1px solid rgba(0, 255, 204, 0.3);
            text-align: left;
        }
        th {
            background: rgba(0, 255, 204, 0.2);
            color: #00ffcc;
        }
        p {
            margin: 10px 0;
        }
        #themeToggle {
            position: fixed;
            bottom: 20px;
            left: 50%;
            transform: translateX(-50%);
            padding: 10px 20px;
            background: #00ffcc;
            color: #000;
            border: none;
            border-radius: 5px;
            font-weight: bold;
            cursor: pointer;
            transition: all 0.3s ease;
        }
        #themeToggle:hover {
            transform: translateX(-50%) scale(1.05);
            box-shadow: 0 0 15px rgba(0, 255, 204, 0.5);
        }
        body.dark-theme {
            background: linear-gradient(135deg, #1a1a1a, #2d2d2d);
            color: #fff;
        }
        body.dark-theme .container {
            background: rgba(255, 255, 255, 0.1);
            box-shadow: 0 0 20px rgba(0, 0, 0, 0.3);
        }
        body.dark-theme .section {
            background: rgba(0, 0, 0, 0.2);
        }
        body.dark-theme th {
            background: rgba(0, 255, 204, 0.2);
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>PHP CGI!</h1>

        <!-- Submission Section -->
        <div class="section">
            <h2>Your CGI Submission</h2>
            <?php
            if ($_SERVER["REQUEST_METHOD"] == "POST") {
                // Parsed POST Data
                echo "<h3>Parsed POST Data:</h3>";
                if (!empty($_POST)) {
                    echo "<table>";
                    echo "<tr><th>Field</th><th>Value</th></tr>";
                    foreach ($_POST as $field => $value) {
                        echo "<tr><td>" . htmlspecialchars($field) . "</td><td>" . htmlspecialchars($value) . "</td></tr>";
                    }
                    echo "</table>";
                } else {
                    // Fallback: Manually parse raw data if $_POST is empty
                    $parsed_data = [];
                    parse_str($raw_post_data, $parsed_data);
                    if (!empty($parsed_data)) {
                        echo "<table>";
                        echo "<tr><th>Field</th><th>Value</th></tr>";
                        foreach ($parsed_data as $field => $value) {
                            echo "<tr><td>" . htmlspecialchars($field) . "</td><td>" . htmlspecialchars($value) . "</td></tr>";
                        }
                        echo "</table>";
                    } else {
                        echo "<p>No parsed POST data received</p>";
                    }
                }
            } else {
                echo "<p>This page expects a POST request. Use the form to submit data!</p>";
            }
            ?>
        </div>

        <!-- Environment Variables Section -->
        <div class="section">
            <h2>CGI Environment Variables</h2>
            <p>Submitted at: <?php echo date("Y-m-d H:i:s"); ?></p>
            <table>
                <tr><th>Variable</th><th>Value</th></tr>
                <?php
                ksort($_SERVER);
                foreach ($_SERVER as $key => $value) {
                    echo "<tr><td>" . htmlspecialchars($key) . "</td><td>" . htmlspecialchars($value) . "</td></tr>";
                }
                ?>
            </table>
        </div>

        <!-- Back to Form Link -->
        <div class="section">
            <a href="/cgi.html" style="display: inline-block; padding: 10px 20px; background: #00ffcc; color: #000; text-decoration: none; border-radius: 5px; transition: all 0.3s ease;">Back to Form</a>
        </div>
    </div>

    <!-- Theme Toggle Button -->
    <button id="themeToggle" onclick="toggleTheme()">Switch to Dark Mode</button>

    <!-- JavaScript for Theme Toggle -->
    <script>
        function toggleTheme() {
            const body = document.body;
            const button = document.getElementById('themeToggle');
            body.classList.toggle('dark-theme');
            if (body.classList.contains('dark-theme')) {
                button.textContent = 'Switch to Light Mode';
                localStorage.setItem('theme', 'dark');
            } else {
                button.textContent = 'Switch to Dark Mode';
                localStorage.setItem('theme', 'light');
            }
        }
        function loadTheme() {
            const savedTheme = localStorage.getItem('theme');
            const body = document.body;
            const button = document.getElementById('themeToggle');
            if (savedTheme === 'dark') {
                body.classList.add('dark-theme');
                button.textContent = 'Switch to Light Mode';
            } else {
                body.classList.remove('dark-theme');
                button.textContent = 'Switch to Dark Mode';
            }
        }
        document.addEventListener('DOMContentLoaded', loadTheme);
    </script>
</body>
</html>
<?php
ob_end_flush();
?>