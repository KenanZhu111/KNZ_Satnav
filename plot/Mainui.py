import math
import time
import Trocor
from tkinter import colorchooser
from tkinter import filedialog
from tkinter import ttk
import tkinter as tk
import matplotlib as mpl
import numpy as np
mpl.use('TkAgg')
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import (
    FigureCanvasTkAgg, NavigationToolbar2Tk)

color = "#0080FF"
def Colorchoose():
    global color
    colorvalue = tk.colorchooser.askcolor()
    color = str(colorvalue[1])

def PO_openfile():
    ENU_O = np.empty((3, 0))
    f_path = filedialog.askopenfilename()
    with open(f_path, 'r') as f:
        while 1:
            line = f.readline()
            if line.find("X-rec") >= 0:
                while 1:
                    line = f.readline()
                    if line != "":
                        E = float(line[45:55])
                        N = float(line[56:66])
                        U = float(line[67:77])
                        ENU_O = np.hstack((ENU_O, [ [E], [N], [U] ] ))
                    elif line == "":
                        ex.clear()
                        nx.clear()
                        ux.clear()
                        ex.plot(ENU_O[0, :], color=color)
                        ex.set_ylabel('E-W(m)')
                        ex.set_xlabel('Epochs')
                        ex.grid(True, linestyle='--', alpha=0.7)
                        nx.plot(ENU_O[1, :], color=color)
                        nx.set_ylabel('N-S(m)')
                        nx.set_xlabel('Epochs')
                        nx.grid(True, linestyle='--', alpha=0.7)
                        ux.plot(ENU_O[2, :], color=color)
                        ux.set_ylabel('U-D(m)')
                        ux.set_xlabel('Epochs')
                        ux.grid(True, linestyle='--', alpha=0.7)
                        canvas.draw()
                        break
            elif line == "":
                break

def SPP_calculat():
    # Const define
    C_V = 299792458
    f1 = 1575.42
    f2 = 1227.60
    ENU_O = np.empty((3, 0))
    f_path = filedialog.askopenfilename()
    with open(f_path, 'r') as f:
        while 1:
            line = f.readline()
            if line.find("APPROX POSITION XYZ") >= 0:
                apX = float(line[23:35])
                apY = float(line[36:48])
                apZ = float(line[49:61])
            elif line.find("APPROX POSITION BLH") >= 0:
                apB = float(line[23:35])
                apL = float(line[36:48])
                apH = float(line[49:61])
            elif line.find("OBS FILE PATH") >= 0:
                Obs = line[23:300]
            elif line.find("NAV FILE PATH") >= 0:
                Nav = line[23:300]
            elif line.find("IONOS OPT") >= 0:
                ION = line[23:33]
            elif line.find("TROPO OPT") >= 0:
                TRO = line[23:33]
                break
        #Get the obs file name---------------------------#
        obspath = len(Obs)
        while obspath > 0:
            flag = Obs[obspath - 1:obspath]
            if flag == "\\":
                obsfile = Obs[obspath:-4]
                break
            obspath -= 1
        # Get the file's father folder-------------------#
        pathlon = len(f_path)
        while pathlon - 3 > 0:
            flag = f_path[pathlon - 3 - 1:pathlon - 3]
            if flag == "/":
                o_path = f_path[:pathlon - 3] + obsfile +"po"
                break
            pathlon -= 1
        # -----------------------------------------------#
        with open(o_path, 'a+') as of:
            print("@ GENGRATE PROGRAM   : PySPP v0.1.1\n"
              "@ GENERATE TYPE      : Receiver Station Position\n"
              "@ GENGRATE TIME      : %s\n"
              "@ IONOS OPT          : %s"
              "@ TROPO OPT          : %s"
              "@\n\n" % (time.asctime(time.localtime()),ION,TRO), file=of)
            print("  X-rec(m)      Y-rec(m)       Z-rec(m)   Sn(n)   dE(m)      dN(m)      dU(m)     ", file=of)
            of.close()
    #Coordinate transformation matrix---------------#
        S = np.array([
            [                   -math.sin(apL),                  math.cos(apL),             0],
            [   -math.sin(apB) * math.cos(apL), -math.sin(apB) * math.sin(apL), math.cos(apB)],
            [    math.cos(apB) * math.cos(apL),  math.cos(apB) * math.sin(apL), math.sin(apB)]
            ])
    #-----------------------------------------------#

        while 1:
            line = f.readline()
            flag = line[0: 1]
            if flag == ">":
                satnum = 0
                B    = np.empty((0, 4))
                P    = np.empty((    ))
                L    = np.empty((0, 1))
                Ptem = []

                epoch = int(line[ 1: 5])
                while 1:
                    line = f.readline()
                    flag = line[0:1]
                    if flag == "G":
                            satnum += 1
                            satX = float(line[6:21])
                            satY = float(line[22:37])
                            satZ = float(line[38:53])
                            C1   = float(line[55:69])
                            C2   = float(line[71:85])
                            ma   = float(line[86:101])
                            TGD  = float(line[102:117])
                            Dt   = float(line[118:134])
                            R = math.sqrt(pow((satX - apX), 2) + pow((satY - apY), 2) + pow((satZ - apZ), 2))
                            l = -((satX - apX) / R)
                            m = -((satY - apY) / R)
                            n = -((satZ - apZ) / R)
                            B = np.vstack((B, [l, m, n, 1]))
                            Ptem.append(pow(math.sin(math.radians(ma)), 2))
                            if C2 != 0:
                                Pl = (f1 * f1 * C1) / (f1 * f1 - f2 * f2) - (f2 * f2 * C2) / (f1 * f1 - f2 * f2)
                            elif C2 == 0:
                                Pl = C1
                            L = np.vstack((L, [Pl - R + C_V * Dt - Trocor.def_tro_cor(apH, ma)]))
                    elif flag != "G":
                        if satnum >= 4:
                            P = np.diag(Ptem)
                            Q = np.linalg.inv(np.transpose(B) @ P @ B)
                            D_X = Q @ np.transpose(B) @ P @ L #Coordinate correction
                            X = D_X[0, 0] + apX
                            Y = D_X[1, 0] + apY
                            Z = D_X[2, 0] + apZ
                            DeltaXYZ = np.array([[D_X[0, 0]],[D_X[1, 0]],[D_X[2, 0]]])
                            ENU = S @ DeltaXYZ
                            ENU_O = np.hstack((ENU_O,[ [ENU[0, 0]],[ENU[1, 0]],[ENU[2, 0]] ]))
                            with open(o_path, 'a+') as of:
                                print("%12.4f %12.4f %12.4f %3d %10.5f %10.5f %10.5f" %(X, Y, Z, satnum, ENU[0, 0], ENU[1, 0], ENU[2, 0]),file = of)
                                of.close()
                            break
                        else:
                            break
            elif line.find("END") >= 0:
                ex.clear()
                nx.clear()
                ux.clear()
                ex.plot(ENU_O[0, :], color=color)
                ex.set_ylabel('E-W(m)')
                ex.grid(True, linestyle='--', alpha=0.7)
                nx.plot(ENU_O[1, :], color=color)
                nx.set_ylabel('N-S(m)')
                nx.grid(True, linestyle='--', alpha=0.7)
                ux.plot(ENU_O[2, :], color=color)
                ux.set_ylabel('U-D(m)')
                ux.set_xlabel('Epochs')
                ux.grid(True, linestyle='--', alpha=0.7)
                canvas.draw()
                f.close()
                break

root = tk.Tk()
root.title("Plot")
root.iconbitmap('C:\\Users\\34716\\source\\repos\\SPP_Calculate\\Plot.ico')
root_pos = "%dx%d+%d+%d" % (900, 450, (root.winfo_screenwidth() - 900)/2, (root.winfo_screenheight() - 450/2))
root.geometry(root_pos)

root.minsize(300,500)

frmFigure = tk.Frame(root)
frmFigure.pack(side=tk.BOTTOM, fill=tk.BOTH, expand=tk.YES)

inter = tk.Label(root,text='Interval:')
inter.pack(side=tk.LEFT, anchor='e')

cboxi = ttk.Combobox(root, width=8, height=10)
cboxi['value'] = ('0','0.05','0.1','0.2','0.25','0.5','1','5','10','15','30','60')
cboxi.current(0)
cboxi.pack(side=tk.LEFT, anchor='e')

plottype = tk.Label(root,text='Plot types:')
plottype.pack(side=tk.LEFT, anchor='e')

cboxe = ttk.Combobox(root, width=8)
cboxe['state'] = 'readonly'
cboxe['value'] = ('ENU','Satn')
cboxe.current(0)
cboxe.pack(side=tk.LEFT, anchor='e')

topmenu = tk.Menu(root)
filemenu = tk.Menu(topmenu, tearoff=False)
openmenu = tk.Menu(filemenu, tearoff=False)
openmenu.add_command(label="Open.sp", command=SPP_calculat, accelerator='Ctrl+S')
openmenu.add_command(label="Open.po", command=PO_openfile, accelerator='Ctrl+P')
filemenu.add_cascade(label="Open...", menu=openmenu)
filemenu.add_command(label="View")
filemenu.add_command(label="Exit", command=root.quit, accelerator='Ctrl+E')
topmenu.add_cascade(label="File", menu=filemenu)

optmenu = tk.Menu(topmenu, tearoff=False)
colormenu = tk.Menu(optmenu, tearoff=False)
colormenu.add_command(label="ENU Colors", command=Colorchoose, accelerator='Ctrl+M')
colormenu.add_command(label="Satnum Colors", command=Colorchoose, accelerator='Ctrl+N')
optmenu.add_cascade(label="Color options...", menu=colormenu)
topmenu.add_cascade(label="Options", menu=optmenu)

helpmenu = tk.Menu(topmenu, tearoff=False)
helpmenu.add_command(label="About")
topmenu.add_cascade(label="Help", menu=helpmenu)

root.config(menu=topmenu)

plt.rc('font',family='sans-serif')
fig, (ex, nx, ux) = plt.subplots(3, 1, figsize=(10, 0), sharex=True)
canvas = FigureCanvasTkAgg(fig,frmFigure)
canvas.draw()
canvas.get_tk_widget().pack(side=tk.BOTTOM,fill=tk.BOTH,expand=tk.YES)
toolbar = NavigationToolbar2Tk(canvas,frmFigure,pack_toolbar=False)
toolbar.update()
toolbar.pack(side=tk.LEFT)

root.protocol("WM_DELETE_WINDOW", root.quit)

root.mainloop()