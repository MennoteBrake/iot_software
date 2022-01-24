# curl -i http://localhost:5000/allsensordata

# curl -i -H "Content-Type: application/json" -X POST -d '100 200' http://localhost:5000/data

# # curl -i -H -X POST \
# # -d 200 100 \
# # http://localhost:5000/todo/tasks

# # curl -i -X DELETE $ip

# curl -i http://localhost:5000/allsensordata

curl -i http://192.168.1.11:5000/allsensordata

curl -i -H "Content-Type: application/json" -X POST -d '100 200' http://192.168.1.11:5000/data

# curl -i -H -X POST \
# -d 200 100 \
# http://192.168.1.11:5000/todo/tasks

# curl -i -X DELETE $ip

curl -i http://192.168.1.11:5000/allsensordata