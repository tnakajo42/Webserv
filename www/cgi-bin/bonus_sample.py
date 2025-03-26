#!/usr/bin/env python3

import os
import sys

# Ensure output is flushed immediately (CGI requirement)
sys.stdout = os.fdopen(sys.stdout.fileno(), 'w', buffering=1)

# Get cookies from the browser
cookie_from_browser = os.environ.get("HTTP_COOKIE", "")
has_carlos_cookie = "carlos_cookie=" in cookie_from_browser

# Headers
print("X-Test-Header: Hello")
if not has_carlos_cookie:  # Set cookie only if it doesn’t exist (first visit)
    print("Set-Cookie: carlos_cookie=muyBien; Path=/; Max-Age=3600")  # 1-hour expiration
print("Content-Type: text/html")
print()  # Blank line to end headers

# HTML content with escaped braces
html_content = """
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Cool 3D Cookie Website</title>
    <style>
        body {{
            margin: 0;
            background: linear-gradient(135deg, #1e1e1e, #3a0a6b);
            color: #fff;
            font-family: Arial, sans-serif;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            height: 100vh;
            overflow: hidden;
        }}
        h1 {{
            font-size: 3em;
            text-shadow: 0 0 10px rgba(255, 255, 255, 0.7);
        }}
        #canvas-container {{
            width: 80%;
            height: 70%;
            border: 2px solid #fff;
            border-radius: 10px;
            box-shadow: 0 0 20px rgba(255, 255, 255, 0.3);
        }}
        button {{
            padding: 10px 20px;
            font-size: 1.2em;
            background: #ff4444;
            border: none;
            border-radius: 5px;
            color: #fff;
            cursor: pointer;
        }}
        button:hover {{
            background: #cc3333;
        }}
    </style>
</head>
<body>
    <h1>{0}</h1>
    <div id="canvas-container"></div>
    <button onclick="takeCookie()">Take Carlos' Cookie</button>

    <script src="https://cdnjs.cloudflare.com/ajax/libs/three.js/r134/three.min.js"></script>
    <script>
        const scene = new THREE.Scene();
        const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
        const renderer = new THREE.WebGLRenderer({{ antialias: true }});
        const container = document.getElementById('canvas-container');
        renderer.setSize(container.clientWidth, container.clientHeight);
        container.appendChild(renderer.domElement);

        let cookie;
        const hasCookie = {1};
        if (hasCookie) {{
            const geometry = new THREE.CylinderGeometry(1, 1, 0.3, 32);
            const material = new THREE.MeshBasicMaterial({{ color: 0x8B4513, wireframe: false }});
            cookie = new THREE.Mesh(geometry, material);
            scene.add(cookie);

            const chipGeometry = new THREE.SphereGeometry(0.1, 16, 16);
            const chipMaterial = new THREE.MeshBasicMaterial({{ color: 0x4A2F1A }});
            for (let i = 0; i < 5; i++) {{
                const chip = new THREE.Mesh(chipGeometry, chipMaterial);
                chip.position.set(
                    (Math.random() - 0.5) * 1.5,
                    0.2,
                    (Math.random() - 0.5) * 1.5
                );
                cookie.add(chip);
            }}
        }}

        camera.position.z = 5;
        let isDragging = false;
        let previousMousePosition = {{ x: 0, y: 0 }};

        container.addEventListener('mousedown', (event) => {{
            isDragging = true;
        }});

        window.addEventListener('mouseup', () => {{
            isDragging = false;
        }});

        container.addEventListener('mousemove', (event) => {{
            if (isDragging && cookie) {{
                const deltaMove = {{
                    x: event.offsetX - previousMousePosition.x,
                    y: event.offsetY - previousMousePosition.y
                }};
                cookie.rotation.y += deltaMove.x * 0.01;
                cookie.rotation.x += deltaMove.y * 0.01;
            }}
            previousMousePosition = {{ x: event.offsetX, y: event.offsetY }};
        }});

        function animate() {{
            requestAnimationFrame(animate);
            if (!isDragging && cookie) {{
                cookie.rotation.x += 0.005;
                cookie.rotation.y += 0.005;
            }}
            renderer.render(scene, camera);
        }}

        window.addEventListener('resize', () => {{
            const width = container.clientWidth;
            const height = container.clientHeight;
            renderer.setSize(width, height);
            camera.aspect = width / height;
            camera.updateProjectionMatrix();
        }});

        function takeCookie() {{
            document.cookie = "carlos_cookie=; Max-Age=0; Path=/";
            if (cookie) {{
                scene.remove(cookie);
                cookie = null;
            }}
            document.querySelector('h1').innerText = "Cookie Taken!";
            setTimeout(() => location.reload(), 100);
        }}

        animate();
    </script>
</body>
</html>
""".format(
    "Cookie Taken!" if not has_carlos_cookie else "¡Esto está muy delicioso, bro!",
    "true" if has_carlos_cookie else "false"
)

# Output the HTML
print(html_content)