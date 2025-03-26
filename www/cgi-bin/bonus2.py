#!/usr/bin/env python3

import os

# Set HTTP headers
print("Content-Type: text/html")
# print("Set-Cookie: taka_cookie=OiShe; Path=/; Max-Age=3600")
print("X-Cool-Header: GrokWasHere")
print()  # Blank line to separate headers from body

# Get the cookie from the browser (if it exists)
cookie_from_browser = os.environ.get("HTTP_COOKIE", "")

# Base HTML content
html_content = """
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Super Cool Cookie Test</title>
    <style>
        body {
            margin: 0;
            background: linear-gradient(135deg, #2a0b4a, #ff6f61);
            color: #fff;
            font-family: 'Courier New', monospace;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            height: 100vh;
            overflow: hidden;
        }
        h1 {
            font-size: 3.5em;
            text-shadow: 0 0 15px #ff6f61, 0 0 25px #fff;
            animation: pulse 2s infinite;
        }
        p {
            font-size: 1.5em;
            background: rgba(0, 0, 0, 0.5);
            padding: 10px 20px;
            border-radius: 10px;
            box-shadow: 0 0 10px rgba(255, 255, 255, 0.3);
        }
        #canvas-container {
            width: 60%;
            height: 50%;
            border: 3px dashed #fff;
            border-radius: 15px;
            box-shadow: 0 0 30px rgba(255, 111, 97, 0.7);
        }
        @keyframes pulse {
            0% { transform: scale(1); }
            50% { transform: scale(1.05); }
            100% { transform: scale(1); }
        }
    </style>
</head>
<body>
    <h1>Do you want my Cookie?</h1>
"""

# Add conditional content based on cookie presence
if cookie_from_browser:
    html_content += f"""
    <div id="canvas-container"></div>
    <p>Cookie received: {cookie_from_browser}</p>

    <!-- Load Three.js from CDN -->
    <script src="https://cdnjs.cloudflare.com/ajax/libs/three.js/r134/three.min.js"></script>
    <script>
        // Set up the scene, camera, and renderer
        const scene = new THREE.Scene();
        const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
        const renderer = new THREE.WebGLRenderer({{ antialias: true }});
        
        // Set renderer size and append to the container
        const container = document.getElementById('canvas-container');
        renderer.setSize(container.clientWidth, container.clientHeight);
        container.appendChild(renderer.domElement);

        // Create a 3D cookie (cylinder)
        const geometry = new THREE.CylinderGeometry(1, 1, 0.3, 32);
        const material = new THREE.MeshBasicMaterial({{ color: 0xD2691E, wireframe: false }});
        const cookie = new THREE.Mesh(geometry, material);
        scene.add(cookie);

        // Add "chocolate chips" (small spheres)
        const chipGeometry = new THREE.SphereGeometry(0.12, 16, 16);
        const chipMaterial = new THREE.MeshBasicMaterial({{ color: 0x3C2F2F }});
        for (let i = 0; i < 6; i++) {{
            const chip = new THREE.Mesh(chipGeometry, chipMaterial);
            chip.position.set(
                (Math.random() - 0.5) * 1.6,
                0.2,
                (Math.random() - 0.5) * 1.6
            );
            cookie.add(chip);
        }}

        // Position the camera
        camera.position.z = 5;

        // Mouse interaction variables
        let isDragging = false;
        let previousMousePosition = {{ x: 0, y: 0 }};

        // Mouse down event
        container.addEventListener('mousedown', (event) => {{
            isDragging = true;
        }});

        // Mouse up event
        window.addEventListener('mouseup', () => {{
            isDragging = false;
        }});

        // Mouse move event
        container.addEventListener('mousemove', (event) => {{
            if (isDragging) {{
                const deltaMove = {{
                    x: event.offsetX - previousMousePosition.x,
                    y: event.offsetY - previousMousePosition.y
                }};
                cookie.rotation.y += deltaMove.x * 0.01;
                cookie.rotation.x += deltaMove.y * 0.01;
            }}
            previousMousePosition = {{ x: event.offsetX, y: event.offsetY }};
        }});

        // Animation function
        function animate() {{
            requestAnimationFrame(animate);
            if (!isDragging) {{
                cookie.rotation.x += 0.007;
                cookie.rotation.y += 0.007;
            }}
            renderer.render(scene, camera);
        }}

        // Adjust renderer size on window resize
        window.addEventListener('resize', () => {{
            const width = container.clientWidth;
            const height = container.clientHeight;
            renderer.setSize(width, height);
            camera.aspect = width / height;
            camera.updateProjectionMatrix();
        }});

        // Start the animation
        animate();
    </script>
    """
else:
    html_content += """
    <p>No cookie received yet. Carlos, did you steal mine again??</p>
    """

# Close the HTML
html_content += """
</body>
</html>
"""

# Output the HTML
print(html_content)