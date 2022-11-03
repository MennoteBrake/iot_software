# iot_software

## TODO

- punt 5 -> gemiddelde ophalen -> "Als gevolg van deze actie wordt de circulaire buffer geleegd."
- langdurig gemiddelde
- alle punten nalopen

## regressie
The regression is implemented based on the formulas from of the following PDF: ```http://lab.scalda.nl/pdf/kwrov.pdf``` 

In the code the formulas are split so it takes less time to calculate everything, and so the code is easier to read. 



To test the regression.py file, you can use the following main:

```
if __name__ == "__main__":
    data = [[61, 191, 447], [476, 54, 407]]
    print (regression(data))
```