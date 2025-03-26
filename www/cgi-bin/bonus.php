#!/usr/bin/php-cgi
<?php
// Define session file path
define('SESSION_FILE', 'sessions.json');

// Load sessions from file
function load_sessions() {
    if (file_exists(SESSION_FILE)) {
        return json_decode(file_get_contents(SESSION_FILE), true);
    }
    return [];
}

// Save sessions to file
function save_sessions($sessions) {
    file_put_contents(SESSION_FILE, json_encode($sessions));
}

// Initialize sessions
$sessions = load_sessions();

// Get or create session ID from cookie
$session_id = isset($_COOKIE['session_id']) ? $_COOKIE['session_id'] : null;
if (!$session_id) {
    $session_id = bin2hex(random_bytes(16)); // Generate a random session ID
    setcookie('session_id', $session_id, time() + 60, '/'); // 1 minute expiration
    $sessions[$session_id] = ['visits' => 0]; // Initialize new session
}

// Ensure session exists and increment visits
if (!isset($sessions[$session_id])) {
    $sessions[$session_id] = ['visits' => 0];
}
$sessions[$session_id]['visits'] = (int)$sessions[$session_id]['visits'] + 1;
$session_data = $sessions[$session_id];
$visit_count = $session_data['visits'];

// Print headers
header("Content-Type: text/html");
header("X-Cool-Header: GrokWasHere");
echo "\n";

// Start HTML output
echo "<!DOCTYPE html>";
echo "<html lang='en'>";
echo "<head>";
echo "    <meta charset='UTF-8'>";
echo "    <meta name='viewport' content='width=device-width, initial-scale=1.0'>";
echo "    <title>Super Cool PHP CGI Cookie</title>";
echo "    <style>";
echo "        body {";
echo "            margin: 0;";
echo "            background: linear-gradient(135deg, #00f7ff, #ff00cc);"; // Neon cyan to hot pink
echo "            color: #00ffff;"; // Bright cyan text
echo "            font-family: 'Courier New', monospace;";
echo "            display: flex;";
echo "            flex-direction: column;";
echo "            align-items: center;";
echo "            justify-content: center;";
echo "            height: 100vh;";
echo "            overflow: hidden;";
echo "        }";
echo "        h1 {";
echo "            font-size: 3.5em;";
echo "            text-shadow: 0 0 15px #00ffff, 0 0 25px #ff00cc;"; // Cyan and pink glow
echo "            animation: pulse 2s infinite;";
echo "        }";
echo "        p {";
echo "            font-size: 1.5em;";
echo "            background: rgba(0, 0, 0, 0.7);"; // Darker background for contrast
echo "            padding: 10px 20px;";
echo "            border-radius: 10px;";
echo "            box-shadow: 0 0 10px rgba(0, 255, 255, 0.5);"; // Cyan glow
echo "        }";
echo "        #canvas-container {";
echo "            width: 80%;";
echo "            height: 70%;";
echo "            border: 3px dashed #00ff00;"; // Neon green dashed border
echo "            border-radius: 15px;";
echo "            box-shadow: 0 0 30px rgba(0, 255, 0, 0.7);"; // Neon green glow
echo "            animation: borderPulse 3s infinite;"; // Pulsing effect
echo "        }";
echo "        @keyframes pulse {";
echo "            0% { transform: scale(1); }";
echo "            50% { transform: scale(1.05); }";
echo "            100% { transform: scale(1); }";
echo "        }";
echo "        @keyframes borderPulse {";
echo "            0% { box-shadow: 0 0 30px rgba(0, 255, 0, 0.7); }";
echo "            50% { box-shadow: 0 0 50px rgba(0, 255, 0, 1); }";
echo "            100% { box-shadow: 0 0 30px rgba(0, 255, 0, 0.7); }";
echo "        }";
echo "    </style>";
echo "</head>";
echo "<body>";
echo "    <h1>PHP CGI!</h1>";
echo "    <div id='canvas-container'></div>";
echo "    <p>Session ID: " . htmlspecialchars($session_id) . " | Visits: " . $visit_count . "</p>";
echo "    <script src='https://cdnjs.cloudflare.com/ajax/libs/three.js/r134/three.min.js'></script>";
echo "    <script>";
echo "        const scene = new THREE.Scene();";
echo "        const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);";
echo "        const renderer = new THREE.WebGLRenderer({ antialias: true });";
echo "        const container = document.getElementById('canvas-container');";
echo "        renderer.setSize(container.clientWidth, container.clientHeight);";
echo "        container.appendChild(renderer.domElement);";
echo "        const cookieGeometry = new THREE.CylinderGeometry(0.5, 0.5, 0.15, 32);";
echo "        const cookieMaterial = new THREE.MeshBasicMaterial({ color: 0x9900ff });"; // Cool purple
echo "        const chipGeometry = new THREE.SphereGeometry(0.06, 16, 16);";
echo "        const chipMaterial = new THREE.MeshBasicMaterial({ color: 0x00ff00 });"; // Neon green chips
echo "        const visitCount = " . $visit_count . ";";
echo "        const cookies = [];";
echo "        const cookieCount = (visitCount === 1) ? 1 : visitCount;"; // 1 cookie for visit 1, otherwise visitCount
echo "        for (let i = 0; i < cookieCount; i++) {";
echo "            const cookie = new THREE.Mesh(cookieGeometry, cookieMaterial);";
echo "            const angle = (i / cookieCount) * Math.PI * 2;";
echo "            cookie.position.set(Math.cos(angle) * 3, 0, Math.sin(angle) * 3);"; // Spaced out
echo "            for (let j = 0; j < 3; j++) {";
echo "                const chip = new THREE.Mesh(chipGeometry, chipMaterial);";
echo "                const chipAngle = (j / 3) * Math.PI * 2;";
echo "                chip.position.set(";
echo "                    Math.cos(chipAngle) * 0.3,";
echo "                    0.1,";
echo "                    Math.sin(chipAngle) * 0.3";
echo "                );";
echo "                cookie.add(chip);";
echo "            }";
echo "            scene.add(cookie);";
echo "            cookies.push(cookie);";
echo "        }";
echo "        camera.position.z = 7;";
echo "        let isDragging = false;";
echo "        let previousMousePosition = { x: 0, y: 0 };";
echo "        container.addEventListener('mousedown', (event) => {";
echo "            isDragging = true;";
echo "        });";
echo "        window.addEventListener('mouseup', () => {";
echo "            isDragging = false;";
echo "        });";
echo "        container.addEventListener('mousemove', (event) => {";
echo "            if (isDragging) {";
echo "                const deltaMove = {";
echo "                    x: event.offsetX - previousMousePosition.x,";
echo "                    y: event.offsetY - previousMousePosition.y";
echo "                };";
echo "                cookies.forEach(cookie => {";
echo "                    cookie.rotation.y += deltaMove.x * 0.01;";
echo "                    cookie.rotation.x += deltaMove.y * 0.01;";
echo "                });";
echo "            }";
echo "            previousMousePosition = { x: event.offsetX, y: event.offsetY };";
echo "        });";
echo "        function animate() {";
echo "            requestAnimationFrame(animate);";
echo "            if (!isDragging) {";
echo "                cookies.forEach(cookie => {";
echo "                    cookie.rotation.x += 0.007;";
echo "                    cookie.rotation.y += 0.007;";
echo "                });";
echo "            }";
echo "            renderer.render(scene, camera);";
echo "        }";
echo "        window.addEventListener('resize', () => {";
echo "            const width = container.clientWidth;";
echo "            const height = container.clientHeight;";
echo "            renderer.setSize(width, height);";
echo "            camera.aspect = width / height;";
echo "            camera.updateProjectionMatrix();";
echo "        });";
echo "        animate();";
echo "    </script>";
echo "</body>";
echo "</html>";

// Save sessions before exit
save_sessions($sessions);
?>