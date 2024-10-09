##Tro correct-Hopfield
import math
def def_tro_cor(H, ma):
    P0 = 1013.25
    e0 = 11.69
    T0 = 288.15

    p = P0 * pow((1.0 - 0.0068 / T0 / H) , 5)

    T = 288.15 - 0.0068 * H

    if H > 11000:
        e = 0
    if H <= 11000:
        e = e0 * pow( (1.0 - 0.0068 * H / T0) , 4)

    deltaSd = 0.00001552 * p * (40136 + 148.72 * (T - 273.16) - H) / T
    deltaSw = 0.0746512 * e * (11000 - H) / pow(T , 2)
    #Dry
    dtropd = deltaSd / math.sqrt( math.sin( math.radians( math.degrees( pow(math.radians(ma) , 2) ) + 6.25) ) )
    #Wet
    dtropw = deltaSw / math.sqrt( math.sin( math.radians( math.degrees( pow(math.radians(ma) , 2) ) + 6.25) ) )
    dtrop = dtropw + dtropd
    return dtrop