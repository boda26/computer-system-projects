from flask import Flask, render_template, request, jsonify
import requests
app = Flask(__name__)

from pymongo import MongoClient
mongo = MongoClient('localhost', 27017)
db = mongo["mp7-state-server"]

# TODO:
# PUT /<key> – Adds a versioned object
# GET /<key> – Retrieves the latest version of a key
# GET /<key>/<version> – Retrieves a specific version of a key
# DELETE /<key> – Completely deletes a key

# curl -X PUT localhost:5000/date -d '2021-11-05' -H "Content-Type: text"
# curl localhost:5000/date
# curl localhost:5000/date/1
# curl -X DELETE localhost:5000/date

@app.route("/", methods=['GET'])
def server():
    return "Versioned State Server"


@app.route("/<key>", methods=['GET'])
def get_latest_version(key):
    #dict1['netid'] = ['boda2', 'xuningh2']
    #return jsonify(netid1 = dict1['netid'][0], netid2 = dict1['netid'][1])
    dict1 = db.dict1
    doc = dict1.find_one({"_id": key})
    if not doc:
        return "Key does not exist!", 404
    value_list = doc["value"]
    return jsonify(value = value_list[-1], version = str(len(value_list))), 200


@app.route("/<key>/<version>", methods=['GET'])
def get_spec_version(key, version):
    dict1 = db.dict1
    doc = dict1.find_one({"_id": key})
    if not doc:
        return "Key does not exist!", 404
    value_list = doc["value"]
    if int(version) > len(value_list) or int(version) < 1:
        return "Version not exist!", 404
    return jsonify(value = value_list[int(version) - 1], version = version), 200


@app.route("/<key>", methods=['PUT'])
def put(key):
    dict1 = db.dict1
    content = request.data.decode("utf-8")
    doc = dict1.find_one({"_id": key})
    if not doc:
        post = {"_id": key, "value": [content]}
        dict1.insert_one(post)
        return "Add a new key!", 200
    else:
        value_list = doc["value"]
        value_list.append(content)
        dict1.update({"_id": key}, {"value": value_list})
        return "Add a new version to existing key!", 200


@app.route("/<key>", methods=['DELETE'])
def delete(key):
    dict1 = db.dict1
    doc = dict1.find_one({"_id": key})
    if not doc:
        return "Key does not exist!", 404
    dict1.remove({"_id": key})
    return "Remove key successful", 200
    
