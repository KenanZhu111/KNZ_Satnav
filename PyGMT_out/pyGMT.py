import os

from pygmt import project

os.environ["GMT_LIBRARY_PATH"] = "C:/programs/gmt6/bin"

import pygmt

fig = pygmt.Figure()
fig.coast(region="d",projection="Cyl_stere/12c",land="white",water="#C6E2FF",borders="1/0.2p",shorelines="1/0.2p",frame="ag")

f = open("LLA_result.txt","r")
f.readline()
string = f.readline()
epoch = 0
while epoch < 2880:
    flag = string[0:1]
    epoch = int(string[1:5])
    if flag == ">":
        gpsnum = 0
        while gpsnum != 36:
            string = f.readline()
            flag = string[0:1]
            if flag =="G":
                gpsnum = gpsnum + 1
                sPRN = int(string[1:3])
                B = float(string[10:20])
                L = float(string[21:31])
                if sPRN == 1:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 2:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 3:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 4:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 5:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 6:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 7:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 8:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 9:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 10:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 11:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 12:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 13:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 14:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 15:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 16:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 17:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 18:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 19:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 20:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 21:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 22:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 23:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 24:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 25:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 26:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 27:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 28:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 29:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 30:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 31:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
                if sPRN == 32:
                    fig.plot(x = L,y = B,style = "c0.015c",fill = "blue",pen = "blue")
            if flag !="G":
                break
    elif flag != ">":
        break

fig.show()
