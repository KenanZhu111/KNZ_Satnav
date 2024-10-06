import math
import time
import Trocor
from base64 import b64decode
from tkinter import colorchooser
from tkinter import filedialog
from tkinter import ttk
from PIL import Image, ImageTk
import tkinter.font as tkFont
import tkinter as tk
import matplotlib as mpl
import numpy as np
mpl.use('TkAgg')
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import (
    FigureCanvasTkAgg, NavigationToolbar2Tk)

ecolor = "#0080FF"
ncolor = "#0080FF"
ucolor = "#0080FF"

def eColorchoose():
    global ecolor
    colorvalue = tk.colorchooser.askcolor()
    ecolor = str(colorvalue[1])

def nColorchoose():
    global ncolor
    colorvalue = tk.colorchooser.askcolor()
    ncolor = str(colorvalue[1])

def uColorchoose():
    global ucolor
    colorvalue = tk.colorchooser.askcolor()
    ucolor = str(colorvalue[1])

def Opt_windows():
    opt = tk.Toplevel(root)
    opt.title("Options")
    opt.attributes("-toolwindow", 2)
    opt.geometry('200x200+200+200')
    opt.minsize(100,250)
    opt.maxsize(100,250)
    enucolopt = tk.Button(opt,text='E Color',width=10, height=1,command=eColorchoose)
    enucolopt.pack(side=tk.TOP, expand=tk.YES)
    enucollab = tk.Label(opt, width=11 , height=1 ,bg=ecolor)
    enucollab.pack(side=tk.TOP, expand=tk.YES)
    enucolopt = tk.Button(opt, text='N Color', width=10, height=1, command=nColorchoose)
    enucolopt.pack(side=tk.TOP, expand=tk.YES)
    enucollab = tk.Label(opt, width=11, height=1, bg=ncolor)
    enucollab.pack(side=tk.TOP, expand=tk.YES)
    enucolopt = tk.Button(opt, text='U Color', width=10, height=1, command=uColorchoose)
    enucolopt.pack(side=tk.TOP, expand=tk.YES)
    enucollab = tk.Label(opt, width=11, height=1, bg=ucolor)
    enucollab.pack(side=tk.TOP, expand=tk.YES)
    enuconfirm = tk.Button(opt, text='Confirm', width=10, height=1, command=opt.destroy)
    enuconfirm.pack(side=tk.TOP, expand=tk.YES)

def Abo_windows():
    abo = tk.Toplevel(root)
    abo.title("About")
    abo.attributes("-toolwindow", 2)
    abo.geometry('200x200+200+200')
    abo.minsize(200, 190)
    abo.maxsize(200, 190)

    global photo
    photo = ImageTk.PhotoImage(Image.open('Plot.ico'))
    label = tk.Label(abo, image=photo)
    label.pack(side=tk.TOP, expand=tk.YES)

    About = tk.Label(abo, text='\nSPP_plot ver 0.1.4\nCopyright (c) 2024 by KenanZhu\nAll Right Reserved.')
    About.pack(side=tk.TOP, expand=tk.YES)

def View_windows():
    f_path = filedialog.askopenfilename()
    view = tk.Toplevel()
    view.title('Viewing: %s'%(f_path))
    view.attributes("-toolwindow", 2)
    view.geometry('900x600')
    view.minsize(1000, 600)
    ybar = tk.Scrollbar(view)
    ybar.pack(side=tk.RIGHT, fill=tk.Y)
    text = tk.Text(view, height=500, undo=True, autoseparators=False)
    text.pack(fill=tk.BOTH)
    with open(f_path, 'r') as f:
        lines = f.readlines()
        for line in lines:
            text.insert("insert", line)

    text.config(state=tk.DISABLED)
    ybar.config(command=text.yview)

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
                        E = float(line[45 + 22:55 + 22])
                        N = float(line[56 + 22:66 + 22])
                        U = float(line[67 + 22:77 + 22])
                        ENU_O = np.hstack((ENU_O, [ [E], [N], [U] ] ))
                    elif line == "":
                        ex.clear()
                        nx.clear()
                        ux.clear()
                        ex.plot(ENU_O[0, :], color=ecolor)
                        ex.set_title('E-W(m)', loc='left', fontsize=8)
                        ex.grid(True, linestyle='--', alpha=0.7)
                        nx.plot(ENU_O[1, :], color=ncolor)
                        nx.set_title('N-S(m)', loc='left', fontsize=8)
                        nx.grid(True, linestyle='--', alpha=0.7)
                        ux.plot(ENU_O[2, :], color=ucolor)
                        ux.set_title('U-D(m)', loc='left', fontsize=8)
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
    ENU_O = np.empty((3,0))
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

        # Generate the data header-----------------------#
        with open(o_path, 'w+') as of:# Initialize-------#
            of.close()
        with open(o_path, 'a+') as of:
            print("@ GENGRATE PROGRAM   : PySPP v0.1.1\n"
              "@ GENERATE TYPE      : Receiver Station Position\n"
              "@ GENGRATE TIME      : %s\n"
              "@ IONOS OPT          : %s"
              "@ TROPO OPT          : %s"
              "@\n\n" % (time.asctime(time.localtime()),ION,TRO), file=of)
            print("GPST                     X-rec(m)     Y-rec(m)      Z-rec(m)   Sn(n)   dE(m)      dN(m)      dU(m)     ", file=of)
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

                y     = int(line[6:10])
                m     = int(line[11:13])
                d     = int(line[14:16])
                h     = int(line[17:19])
                min   = int(line[20:22])
                sec   = float(line[23:30])
                while 1:
                    line = f.readline()
                    flag = line[0:1]
                    if flag == "G":
                            satnum += 1
                            sPEN = int(line[1:3])
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
                                print("%4d\\%02d\\%02d\\%02d\\%02d\\%04.1f %12.4f %12.4f %12.4f %3d %10.5f %10.5f %10.5f"
                                      %(y, m, d, h, min, sec, X, Y, Z, satnum, ENU[0, 0], ENU[1, 0], ENU[2, 0]),file = of)
                                of.close()
                            break
                        else:
                            break
            elif line.find("END") >= 0:
                ex.clear()
                nx.clear()
                ux.clear()
                ex.plot(ENU_O[0, :], color=ecolor)
                ex.set_title('E-W(m)', loc='left', fontsize=8)
                ex.grid(True, linestyle='--', alpha=0.7)
                nx.plot(ENU_O[1, :], color=ncolor)
                nx.set_title('N-S(m)', loc='left', fontsize=8)
                nx.grid(True, linestyle='--', alpha=0.7)
                ux.plot(ENU_O[2, :], color=ucolor)
                ux.set_title('U-D(m)', loc='left', fontsize=8)
                ux.set_xlabel('Epochs')
                ux.grid(True, linestyle='--', alpha=0.7)
                canvas.draw()
                f.close()
                break

root = tk.Tk()
root.title("SPP_plot")
root.geometry('900x450')
root.minsize(300,500)

icon_img = b'AAABAAEAICAAAAEAIACoEAAAFgAAACgAAAAgAAAAQAAAAAEAIAAAAAAAABAAANIAAADSAAAAAAAAAAAAAAAEAQOcBAEDtQQBA6UEAQN6BAEDPgQBAwgEAQMABAEDAQQBAwMEAQMBBAEDAAQBAwAEAQMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQBA60EAQP/BAED/QQBA/8EAQP+BAED3AQBA4cEAQMeBAEDAAQBAwMEAQMCBAEDAAQBAwAEAQMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAEDBwQBA8wEAQP/BAED+gQBA/4EAQP+BAED/wQBA/cEAQOCBAEDBgQCAwAEAQMBBAEDAAQBAwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAQMABAEDVQQBA/8EAQP8BAED/wQBA/0EAQP7BAED/AQBA/8EAQPEBAEDFAQBAwAEAQMBBAEDAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQBAwAEAQMOBAED5wQBA/8EAQP+BAED/wQBA/8EAQP+BAED+QQBA/8EAQO7BAEDAAQBAwAEAQMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAEDAgQBAwAEAQO5BAED/wQBA/wEAQP/BAED/wQBA/8EAQP/BAED/AQBA/8EAQMzBAEDAAQBAwIAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAQMEBAEDAAQBA5MEAQP/BAED+wQBA/8EAQP/BAED/wQBA/8EAQP9BAED/wQBAz8EAQMABAEDBQQBAwAEAQMABAEDAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQBAwQEAQMABAEDaAQBA/8EAQP7BAED/wQBA/8EAQP/BAED/gQBA/8EAQPqBAEDFAQBAwAEAQMBBAEDAwQBAwAEAQMABAEDAAYAAQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAEDAgQBAwAEAQMnBAED+QQBA/4EAQP8BAED/gQBA/wEAQP5BAED/wQBA38EAQMABAEDOgQBAzsEAQMABAEDBAQBAwEEAQMABAEDAAMBAgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAQMABAEDAwQBAwAEAQOIBAED/wQBA/wEAQP/BAED/QQBA/8EAQOhBAIDAAMBBAEEAQPIBAED/wQBA2UEAQMABAEDAwQBAwEEAQMABAEDAAQBBAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQBAwAEAQMABAEDAQUAAgAEAQN8BAED6QQBA/QEAQPVBAEDZgQBAwMEAQMABAEDkAQBA/8EAQP5BAED/wQBA5QEAwUABAEDAgQBAwEEAQMABAEDAAQBAwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAEDAAQBAwAEAQMABAEDAgQBAwAEAQMLBAEDGAQBAwQEAQMABAEDBAQBA5QEAQP/BAED/AQBA/4EAQP6BAED/wQBA6oEAQMDBAEDAQQBAwEEAQMABAEDAAQABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAEDAAQBAwAEAQMBBAEDAwQBAwAEAQMCBAEDAAQBAz8EAQPVBAED/wQBA/wEAQP/BAED/wQBA/8EAQP6BAED/wQBA64EAQMDBAEDAQQBAwEEAQMABAEDAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAEDAAQBAgAEAQIABAEDAQQBAwQEAQMABAEDRQQBA/8EAQP6BAED/gQBA/8EAQP/BAED/wQBA/8EAQP6BAED/wQBA6YEAQQABAEDAgQBAwEEAQMABAEDAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAEDAAQBAwMEAQMABAEDbAQBA/8EAQP7BAED/wQBA/8EAQP/BAED/wQBA/8EAQP6BAED/wQBA5UEAQMABAEDAwQBAwAEAQMABAEDAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQBAwAEAQMABAEDAAQBAwMEAQMABAEDkQQBA/8EAQP6BAED/wQBA/8EAQP/BAED/wQBA/8EAQP7BAED/wQBA38EAQMABAEDBAQAAAAEAQMABAEDAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQBAwAEAQMABAEDAQQBAwIEAQMABAEDnwQBA/8EAQP6BAED/wQBA/8EAQP/BAED/wQBA/8EAQP8BAED/wQBA2QEAQMABAEDBAQBAwAEAQMABAEDAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQBAwAEAQMABAEDAQQBAwIHAQQABAEDnQQBA/8EAQP6BAED/gQBA/8EAQP/BAED/wQBA/4EAQP+BAED/QQBA0cEAQMABAEDBAQBAwAEAQMABAECAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAgEBAAQBAwAEAQMABAEDAQQBAwMEAQMABAEDjAQBA/8EAQP7BAED/gQBA/8EAQP/BAED/wQBA/0EAQP/BAED7gQBAysEAQMABAEDAwQBAwAEAQMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAgECAAQBAwAEAQMABAEDAQQBAwQEAQMABAEDcwQBA/8EAQP9BAED/QQBA/8EAQP/BAED/wQBA/wEAQP/BAED1gQBAxIEAQMABAEDAgQBAwAEAQMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAwECAAQBAwAEAQMABAEDAQQBAwQEAQMABAEDVQQBA/cEAQP/BAED/AQBA/8EAQP/BAED/wQBA/sEAQP/BAEDtQAAAAAEAQMCBAEDAQQBBAAEAQMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQBAwAEAQMABAEDAAQBAwQEAQMABAEDNgQBA+QEAQP/BAED+wQBA/8EAQP/BAED/wQBA/sEAQP/BAEDjAQBAwAEAQMEBAEEAAQBAwAEAQMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQBAwAEAQMABAADAAQBAwQEAQMABAEDGgQBA8cEAQP/BAED+wQBA/4EAQP/BAED/wQBA/wEAQP/BAEDXgQBAwAEAQMEBAEDAAQBAwAEAAMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQBAwAEAQMABAEDAAQBAwMEAwIABAEDBQQBA54EAQP/BAED/AQBA/0EAQP/BAED/gQBA/8EAQP0BAEDMQQBAwAEAQMDBAEDAAQBAwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQBAwAEAQMABAEDAAQBAwIEAQMDBAEDAAQBA20EAQP8BAED/wQBA/wEAQP/BAED/AQBA/8EAQPTBAEDDQQBAwAEAQMBBAEDAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQBAwAEAQMABAEDAAQBAwEEAQMEBAEDAAQBAz0EAQPkBAED/wQBA/sEAQP+BAED+wQBA/8EAQOgBAEDAAQBAwMEAQMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAMBAgAEAQMABAEDAAQBAwAEAQMEBAEDAAQBAxUEAQO4BAED/wQBA/sEAQP9BAED/AQBA/8EAQNhBAEDAAQBAwQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAQMABAEDAAQBAwAEAQMDBAEDAQQBBAAEAQN6BAED/QQBA/8EAQP6BAED/wQBA+8EAQMiBAEDAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAQMABAEDAAQBAwAEAQMCBAEDBAQBAwAEAQM6BAED2wQBA/8EAQP4BAED/wQBA60EAQMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAQMABAEDAAQBAwAEAQMBBAEDBAQBAwAEAQMMBAEDlgQBA/8EAQP+BAED/wQBA1UAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAEDAAQBAwAEAQMABAEDAwQBAwMEAQMABAEDPAQBA8kEAQP/BAED1wAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAEDAAQBAwAEAQMABAEDAQQBAwQEAQMABAEDAwQBA18EAQOvAi///wCX//8AC///gBP//4AT//9AC///QAp//0AIv/9AEl//ICEv/wAgl/9IgAv/oQAF/9kAEv/8gAl//EAEf/0gAr/+kAFf/0gAr/+kAFf/0gAD/+kAK//0gBX/+QAK//6QBP//SAT//6QC///JAf//9IH///pA///8kP///0g='
icon_img = b64decode(icon_img)
icon_img = ImageTk.PhotoImage(data=icon_img)
root.tk.call('wm', 'iconphoto', root._w, icon_img)

default_font = tkFont.nametofont("TkDefaultFont")
default_font.configure(family="Segoe UI", size=9)

frmFigure = tk.Frame(root)
frmFigure.pack(side=tk.BOTTOM, fill=tk.BOTH, expand=tk.YES)

inter = tk.Label(root,text=' Interval:')
inter.pack(side=tk.LEFT, anchor='e')

cboxi = ttk.Combobox(root, width=8, height=10)
cboxi['value'] = ('0','0.05','0.1','0.2','0.25','0.5','1','5','10','15','30','60')
cboxi.current(0)
cboxi.pack(side=tk.LEFT, anchor='e')

plottype = tk.Label(root,text='  Plot types:')
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
filemenu.add_command(label="View", command=View_windows)
filemenu.add_command(label="Exit", command=root.quit, accelerator='Ctrl+E')
topmenu.add_cascade(label="File", menu=filemenu)

optmenu = tk.Menu(topmenu, tearoff=False)
optmenu.add_cascade(label="Color options...", command=Opt_windows)
topmenu.add_cascade(label="Options", menu=optmenu)

helpmenu = tk.Menu(topmenu, tearoff=False)
helpmenu.add_command(label="About", command=Abo_windows)
topmenu.add_cascade(label="Help", menu=helpmenu)

root.config(menu=topmenu)

plt.rc('font',family='Segoe UI')
fig, (ex, nx, ux) = plt.subplots(3, 1, figsize=(10, 0) ,sharex=True)
fig.subplots_adjust(left=0.045, bottom=0.12, right=0.97, top=0.95)
canvas = FigureCanvasTkAgg(fig,frmFigure)
canvas.draw()
canvas.get_tk_widget().pack(side=tk.BOTTOM,fill=tk.BOTH,expand=tk.YES)
toolbar = NavigationToolbar2Tk(canvas,frmFigure,pack_toolbar=False)
toolbar.update()
toolbar.pack(side=tk.LEFT)

root.protocol("WM_DELETE_WINDOW", root.quit)

root.mainloop()