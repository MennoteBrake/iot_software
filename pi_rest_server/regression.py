# calculates the y value of point x according to the formula. 
def calcYPoint(a, b, c, x) :
    return (a*(x**2) + (b * x) + c)

# Calculates the Determinant
def calcDeterminant(a, b, c, data, meanY) :
    error1Squared = []
    error2Squared = []
    for i in range(0, len(data[0])) :
        error1Squared.append((data[1][i] - (calcYPoint(a, b, c, data[0][i]))) ** 2)
        error2Squared.append(data[1][i] - meanY)
    
    sumError1Squared = 0
    sumError2Squared = 0
    
    for i in range(0, len(error1Squared)) :
        sumError1Squared += error1Squared[i]
        sumError2Squared += error2Squared[i]

    if sumError2Squared != 0 :
        return 1 - sumError1Squared / sumError2Squared
    else :
        return 0

# Calculates the regression
def regression(data):
    xSum = 0
    ySum = 0
    xySum = 0
    xSquareSum = 0
    xSquareYSum = 0
    ySquareSum = 0
    xToThePowerOf3Sum = 0
    xToThePowerOf4Sum = 0
    meanY = 0

    x = 0
    y = 1
    for i in range (0, len(data[x])):
        xSum += data[x][i]
        ySum += data[y][i]
        xySum += data[x][i] * data[y][i]
        xSquareSum += (data[x][i] ** 2)
        xSquareYSum += (data[x][i] ** 2 * data[y][i])
        ySquareSum += (data[y][i] ** 2)
        xToThePowerOf3Sum += (data[x][i] ** 3)
        xToThePowerOf4Sum += (data[x][i] ** 4)
        meanY += data[y][i]
    
    amountOfValues = len(data[x])
    meanY /= amountOfValues
    partA = xSquareSum - ((xSum **2) / amountOfValues)
    partC = xySum - ((xSum * ySum) / amountOfValues)
    partD = xToThePowerOf3Sum - ((xSum * xSquareSum) / amountOfValues)
    partE = xToThePowerOf4Sum - ((xSquareSum ** 2) / amountOfValues)
    partF = xSquareYSum - ((xSquareSum * ySum)/ amountOfValues)

    a = ((partC * partD) - (partA * partF)) / ((partD ** 2) - (partA * partE))
    print ("a ===")
    print (a)
    b = (partC - (a * partD)) / partA
    
    c = (ySum - (b * xSum) - (a * xSquareSum)) / amountOfValues
    coefficientOfDetermination = calcDeterminant(a, b, c, data, meanY)
    arr = [a,b,c, coefficientOfDetermination] 
    calcDeterminant(a, b, c, data, meanY)
    return arr
