import string
import random
import sqlite3
from flask import Flask, request, redirect, render_template_string, jsonify

app = Flask(__name__)

# Initialize SQLite database
def init_db():
    conn = sqlite3.connect('urls.db')
    cursor = conn.cursor()
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS url_map (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            original_url TEXT NOT NULL,
            short_code TEXT UNIQUE NOT NULL
        )
    ''')
    conn.commit()
    conn.close()

init_db()

# Function to generate a random 6-character short code
def generate_short_code():
    characters = string.ascii_letters + string.digits
    return ''.join(random.choice(characters) for _ in range(6))

# Simple HTML Interface
HTML_TEMPLATE = '''
<!DOCTYPE html>
<html>
<head>
    <title>URL Shortener</title>
    <style>
        body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; background-color: #f4f4f9; }
        .card { background: white; padding: 30px; display: inline-block; border-radius: 8px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); }
        input[type="text"] { width: 300px; padding: 10px; margin-right: 10px; border: 1px solid #ccc; border-radius: 4px; }
        button { padding: 10px 15px; background: #007bff; color: white; border: none; border-radius: 4px; cursor: pointer; }
        button:hover { background: #0056b3; }
        .result { margin-top: 20px; font-weight: bold; }
    </style>
</head>
<body>
    <div class="card">
        <h2>🔗 Simple URL Shortener</h2>
        <form method="POST" action="/shorten">
            <input type="text" name="url" placeholder="Enter long URL here..." required>
            <button type="submit">Shorten</button>
        </form>
        {% if short_url %}
        <div class="result">
            <p>Shortened URL:</p>
            <a href="{{ short_url }}" target="_blank">{{ short_url }}</a>
        </div>
        {% endif %}
    </div>
</body>
</html>
'''

# Route: Home Page with Frontend
@app.route('/')
def home():
    return render_template_string(HTML_TEMPLATE)

# Route: Shorten URL API & Form Endpoint
@app.route('/shorten', methods=['POST'])
def shorten_url():
    if request.is_json:
        long_url = request.json.get('url')
    else:
        long_url = request.form.get('url')

    if not long_url:
        return jsonify({'error': 'URL is required'}), 400

    if not (long_url.startswith('http://') or long_url.startswith('https://')):
        long_url = 'https://' + long_url

    short_code = generate_short_code()

    conn = sqlite3.connect('urls.db')
    cursor = conn.cursor()
    cursor.execute('INSERT INTO url_map (original_url, short_code) VALUES (?, ?)', (long_url, short_code))
    conn.commit()
    conn.close()

    short_url = request.host_url + short_code

    if request.is_json:
        return jsonify({'original_url': long_url, 'short_url': short_url, 'short_code': short_code})
    
    return render_template_string(HTML_TEMPLATE, short_url=short_url)

# Route: Redirect Short Code to Original URL
@app.route('/<short_code>')
def redirect_to_url(short_code):
    conn = sqlite3.connect('urls.db')
    cursor = conn.cursor()
    cursor.execute('SELECT original_url FROM url_map WHERE short_code = ?', (short_code,))
    result = cursor.fetchone()
    conn.close()

    if result:
        return redirect(result[0])
    return "<h3>Error 404: Shortened URL not found!</h3>", 404

if __name__ == '__main__':
    app.run(debug=True, port=5000)
