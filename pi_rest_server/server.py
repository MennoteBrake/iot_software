from flask import Flask, jsonify, abort, make_response, request, url_for, g

from sqlite3 import dbapi2 as sqlite3

app = Flask(__name__)

def get_restserver_db():
    if not hasattr(g, 'sqlite_db'):
        g.sqlite_db = connect_restserver_db()
    return g.sqlite_db

def connect_restserver_db():
    rv = sqlite3.connect('restserver.db')
    rv.row_factory = sqlite3.Row
    return rv

@app.teardown_appcontext
def close_db(error):
    if hasattr(g, 'sqlite_db'):
        g.sqlite_db.close()

@app.route('/allsensordata', methods=['GET'])
def get_allsensordata():
    db = get_restserver_db()
    cur = db.execute('SELECT id, dataSensor1, dataSensor2 FROM data ORDER BY id')
    rows = cur.fetchall()
    tasks = []
    for row in rows:
        tasks.append({'dataSensor1' : row[1], 'dataSensor2' : row[2]})

    return jsonify({'tasks': tasks})

@app.route('/statistics', methods=['GET'])
def get_statistics():
    # needs to calculate b0, b1, b2 en R2 when there are more than 3 sets of data
    # needs to be non json and seperated with a space
    task_id = 0
    tasks = []
    task = [task for task in tasks if task['id'] == task_id]
    if len(task) == 0:
        abort(404)
    return jsonify({'task': task[0]})

@app.route('/data', methods=['POST'])
def add_data():
    recieved_data = request.data.decode("utf-8")
    data_values = recieved_data.split()
    db = get_restserver_db()
    db.execute('INSERT INTO DATA (dataSensor1, dataSensor2) VALUES(' + data_values[0] + ',' + data_values[1] + ')')
    db.commit()

    return make_response("", 201)

@app.route('/statistics', methods=['DELETE'])
def delete_statistics():
    db = get_restserver_db()
    db.execute('DELETE FROM data')
    db.commit()
    return make_response("", 201)

@app.errorhandler(400)
def bad_request(error):
    return make_response(jsonify({'error': 'Bad request'}), 400)

@app.errorhandler(404)
def not_found(error):
    return make_response(jsonify({'error' : 'Not found'}), 404)

if __name__ == '__main__':
    app.run(debug=True)