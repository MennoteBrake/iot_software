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


# To run:

```
export FLASK_APP=server.py
flask/bin/flask run --host=0.0.0.0
```