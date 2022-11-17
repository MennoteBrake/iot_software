import requests_raw
import time

arduinoAddress = "http://192.168.1.21"
rpiAddress = "http://192.168.1.11:5000"

actualSensor1 = 0
actualSensor2 = 0

def getActualDataSensor1() :
    req = "GET /sensors/1/actual HTTP/1.0\r\n\r\n"
    response = requests_raw.raw(url=arduinoAddress, data=req)
    if response.status_code == 200:
        actualSensor1 = response.content.decode('ascii')
        print("actual " + actualSensor1)
    else :
        print ("ERROR. It gives the following status code: " + str(response.status_code))

def getActualDataSensor2() :
    req = "GET /sensors/2/actual HTTP/1.0\r\n\r\n"
    response = requests_raw.raw(url=arduinoAddress, data=req)
    if response.status_code == 200:
        actualSensor2 = response.content.decode('ascii')
        print("actual " + actualSensor2)
    else :
        print ("ERROR. It gives the following status code: " + str(response.status_code))

def getSTDEV() : 
    req = "GET /sensors/1/stdev HTTP/1.0\r\n\r\n"
    response = requests_raw.raw(url=arduinoAddress, data=req)

    if response.status_code == 200:
        print("stdev: " + response.content.decode('ascii'))
    else :
        print ("ERROR. It gives the following status code: " + str(response.status_code))

def getAvg() : 
    req = "GET /sensors/1/avg HTTP/1.0\r\n\r\n"
    response = requests_raw.raw(url=arduinoAddress, data=req)
    if response.status_code == 200:
        print("avg: " + response.content.decode('ascii'))
    else :
        print ("ERROR. It gives the following status code: " + str(response.status_code))

def postDataSetToPy() :
    contentLength = len(str(actualSensor1)) + len(str(actualSensor2)) + 1
    req = "\
POST /data HTTP/1.0\r\n\
Content-Length: " + str(contentLength) + "\r\n\
\r\n\
"+ str(actualSensor1) + " " + str(actualSensor2)
    print (req)
    response = requests_raw.raw(url=rpiAddress, data=req)
    if response.status_code != 201 : 
        print ("ERROR. It gives the following status code: " + str(response.status_code))

def getStatistics() :
    req = "GET /statistics HTTP/1.0\r\n\r\n"
    response = requests_raw.raw(url=rpiAddress, data=req)
    if response.status_code == 200 :
        b0, b1, b2, R2 = response.content.decode('ascii').split(' ')
        print ("b0 = " + str(b0))
        print ("b1 = " + str(b1))
        print ("b2 = " + str(b2))
        print ("R2 = " + str(R2))
    else :
        print ("ERROR. It gives the following status code: " + str(response.status_code))  

if __name__ == "__main__":
    while True : 
        getActualDataSensor1()
        getActualDataSensor2()
        postDataSetToPy()
        getSTDEV()
        getAvg()
        getStatistics()
        time.sleep(12)