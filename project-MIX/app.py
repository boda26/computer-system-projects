from flask import Flask, render_template, request, jsonify
import requests

app = Flask(__name__)

# Route for "/" (frontend):
@app.route('/')
def index():
  return render_template("index.html")


# Route for "/MIX" (middleware):
@app.route('/MIX', methods=["POST"])
def POST_weather():
  location = request.form["location"]

  return jsonify({"error": "Not implemented."}), 500
