# Installing dependencies
```
sudo apt-get install python3-venv
```

# Setting up the virtual environment:
```
python -m venv flask
```
Or
```
python3 -m venv flask
```

To install flask:
```
flask/bin/pip3 install flask
```
# Database

```sqlite3 restserver.db < create_server_db.sql```

# To run:

```
export FLASK_APP=server.py
flask/bin/flask run -h 192.168.1.11
```

## trouble shooting
- The ip that was set will maybe not work correctly, you can change it in the makefile to one that is seen when starting up flask

To check if that IP works you can use for example```curl -v 169.254.95.194:5000```

## turn wifi on or of
### Off
sudo ifconfig wlan0 down