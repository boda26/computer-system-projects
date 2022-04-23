from flask import Flask
import os
import flask
app = Flask(__name__)

# Route for "/" for a web-based interface to this micro-service:
@app.route('/')
def index():
  from flask import render_template
  return render_template("index.html")

# Extract a hidden "uiuc" GIF from a PNG image:
@app.route('/extract', methods=["POST"])
def extract_hidden_gif():
  from flask import send_file, request
  # ...your code here...

  #create new directory
  directory = "temp"
  #parent_dir = "/Users/bodasong/Documents/cs240/cs240-fa21-boda2/mp6"
  #path = os.path.join(parent_dir, directory)
  os.makedirs(directory, exist_ok=True)

  #copy file
  if request.method == 'POST':
    if 'png' not in request.files:
      return "No such file", 500
    file = request.files['png']
    name = file.filename
    if name == '':
      return "Invalid file name", 500
    if file:
      file.save(os.path.join("temp", name))
    #run the program
    cmd = "./png-extractGIF temp/" + name + " temp/output.gif"
    return_value = os.system(cmd)
    print(return_value)
    if return_value != 0:
      return "png extract unsuccessful", 500
    return send_file("temp/output.gif")
    
  