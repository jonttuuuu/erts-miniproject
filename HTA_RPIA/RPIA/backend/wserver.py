from flask import Flask
from flask import render_template
from flask import jsonify
from flask import Response
from flask_cors import CORS

import db as db
import hike as hike

hdb = db.HubDatabase()
app = Flask(__name__)
CORS(app)


@app.route('/')
def get_home():
    sessions = hdb.get_sessions() 
    return render_template('home.html', sessions=sessions)

@app.route('/sessions')
def get_sessions():
    sessions = hdb.get_sessions() 
    sessions = list(map(lambda s: hike.to_list(s), sessions))
    return jsonify(sessions)

@app.route('/sessions/<id>')
def get_session_by_id(id):
    session = hdb.get_session(id)
    return jsonify(hike.to_list(session))

@app.route('/sessions/<id>/delete')
def delete_session(id):
    hdb.delete(id)
    print(f'DELETED SESSION WITH ID: {id}')
    return Response(status=202)

if __name__ == "__main__":
    app.run('0.0.0.0', debug=True)