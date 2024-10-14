import math
import time
import Trocor
from base64 import b64decode
from tkinter import colorchooser
from tkinter import messagebox
from tkinter import filedialog
from tkinter import ttk
from PIL import ImageTk
import tkinter.font as tkFont
import tkinter as tk
import matplotlib as mpl
import numpy as np
mpl.use('TkAgg')
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import (
    FigureCanvasTkAgg, NavigationToolbar2Tk)

# Color register part
enucolor    = ["#0080FF","#0080FF","#0080FF"]
enucolormid = ["#0080FF","#0080FF","#0080FF"]
enulinew    = [   "0.80",   "0.80",   "0.80"]
satncolor   = ["#0080FF"]
satncolormid= ["#0080FF"]
satnlinew   = [   "1.50"]
# Ion & tro register part
ioncormode = ['OFF', 'Ion-Free LC', 'Broadcast']
ioncorstate = 0
trocormode = ['OFF', 'Hopfield', 'Saastamoinen']
trocorstate = 0

#Sat system register part
satsysdict = {0: "G", 1:"R", 2:"C", 3:"E", 4:"S"}
satellite_system = 0


def Colorchoose(i, label, mode):
    #Brief # choice to change the color of draw
    #Param # i : only for mode 1
    #Param # label : match the label with the button
    #Param # mode : color register type
    #               undo choice == 0 &
    #               middle choice store == 1 &
    #               final confirm == 3 &
    #               satn color == 4
    #Return# none
    global enucolor
    if mode == 0:
        enucolor = ["#0080FF", "#0080FF", "#0080FF"]
        satncolor[0] = "#0080FF"
        label.config(bg=enucolor[0])
    elif mode == 1:
        colorvalue = tk.colorchooser.askcolor()
        enucolormid[i] = str(colorvalue[1])
        label.config(bg=str(colorvalue[1]))
    elif mode == 3:
        enucolor[0] = enucolormid[0]
        enucolor[1] = enucolormid[1]
        enucolor[2] = enucolormid[2]
        satncolor[0] = satncolormid[0]
    elif mode == 4:
        colorvalue = tk.colorchooser.askcolor()
        satncolormid[0] = str(colorvalue[1])
        label.config(bg=str(colorvalue[1]))

def Linewchoose(entry0, entry1, entry2, mode):
    #Brief # input to change the width of line
    #Param # entry0 : the entry box of e
    #Param # entry1 : the entry box of n
    #Param # entry2 : the entry box of u
    #Param # mode : width of line register type
    #               undo choice == 0 &
    #               final confirm == 1
    #Return# none

    global enulinew
    if mode == 0:
        enulinew = ["0.80", "0.80", "0.80"]
        entry0.set(enulinew[0])
        entry1.set(enulinew[1])
        entry2.set(enulinew[2])

    elif mode == 1:
        if (0 < float(entry0.get()) <= 5 and
            0 < float(entry1.get()) <= 5 and
            0 < float(entry2.get()) <= 5 ):

            enulinew[0] = entry0.get()
            enulinew[1] = entry1.get()
            enulinew[2] = entry2.get()
        elif(float(entry0.get()) <= 0 or
             float(entry1.get()) <= 0 or
             float(entry2.get()) <= 0 ):
            while not messagebox.showerror("ERROR !", "The line width value is invalid !"):
                break
            enulinew = ["0.80", "0.80", "0.80"]
            entry0.set(enulinew[0])
            entry1.set(enulinew[1])
            entry2.set(enulinew[2])

        elif(5 < float(entry0.get())or
             5 < float(entry1.get())or
             5 < float(entry2.get()) ):
            msg = messagebox.askyesno("NOTICE", "The width of the line is too large\n"
                                                             "affecting the mapping.\n"
                                                             "Are you sure to continue ?")
            if msg == False:
                enulinew = ["0.80", "0.80", "0.80"]
                entry0.set(enulinew[0])
                entry1.set(enulinew[1])
                entry2.set(enulinew[2])
                pass
            elif msg == True:
                enulinew[0] = entry0.get()
                enulinew[1] = entry1.get()
                enulinew[2] = entry2.get()

def Calcu_confirm(v, cboxi, cboxt):
    #Brief # choice to change the ion & tro correction && confirm the sat system
    #Param # v sat system radio button value
    #Param # cboxi the combo box of ion correction
    #Param # cboxt the combo box of tro correction
    #Return# none
    global ioncorstate
    global trocorstate
    global satellite_system

    ioncorstate = Ion_and_Trocbox(cboxi, 0)
    trocorstate = Ion_and_Trocbox(cboxt, 1)
    satellite_system = Satsys_button(v)

    print(ioncorstate,trocorstate,satellite_system)

def Satsys_button(v):
    #Brief # get the value of radio button of sat system
    #Param # v : the value of radio button
    #Return# the sat system code
    Systemdict = {0: "0", 1: "1", 2: "2", 3: "3", 4: "4"}
    return int(Systemdict.get(v.get()))

def Ion_and_Trocbox(cbox, cortype):
    #Brief # get the value of combo box
    #Param # cortype: correction type of
    #                            ion == 0 &
    #                            tro == 1
    #Param # cbox:
    #Return# ion | tro state code
    count = -1
    if cortype == 0:
        for ionmode in ioncormode:
            count += 1
            if cbox.get() == ionmode:
                return count

    elif cortype == 1:
        for tromode in trocormode:
            count += 1
            if cbox.get() == tromode:
                return count

def Opt_windows():
    opt = tk.Toplevel(root)
    opt.title("Options")
    opt.geometry('350x200+250+200')
    opt.minsize(320,250)
    opt.maxsize(320,250)

    global  icon_img2
    icon_img2 = (b'AAABAAEAICAAAAAAIACoEAAAFgAAACgAAAAgAAAAQAAAAAEAIAAAAAAAABAAADjsAAA47AAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'ABAAAAAwAAAAAAAAAAAAAAAAAAAAAAAAADAAAAAQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAIAAAAAAAAACQAAABIAAAASAAAACQAAAAAAAAACAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAABwAAAJAAAADqAAAA7gAAAO4AAADqAAAAk'
                 b'QAAAAcAAAAAAAAAAQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAwAAAAAAAACQAAAA/wAAAPw'
                 b'AAAD/AAAA/wAAAPwAAAD/AAAAkAAAAAAAAAADAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQAAAAQAAAADAAAAAQAAAAEAAAAAA'
                 b'AAACwAAAOMAAAD/AAAA/AAAAP4AAAD+AAAA/AAAAP8AAADjAAAACgAAAAAAAAABAAAAAQAAAAMA'
                 b'AAAEAAAAAQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIAAAABAAAAAAA'
                 b'AAAAAAAABAAAABQAAAAAAAAATAAAA7QAAAP8AAAD+AAAA/wAAAP8AAAD+AAAA/wAAAO0AAAATAA'
                 b'AAAAAAAAUAAAABAAAAAAAAAAAAAAABAAAAAgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AABAAAAAAAAABEAAABTAAAAUQAAAA8AAAAAAAAAAAAAAEQAAAD/AAAA/gAAAP8AAAD/AAAA/wAA'
                 b'AP8AAAD+AAAA/wAAAEQAAAAAAAAAAAAAAA8AAABRAAAAUwAAABEAAAAAAAAAAQAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAgAAAAAAAAAwAAAA5QAAAP8AAAD/AAAA4QAAAHYAAAB2AAAA6QAAAP8AAA'
                 b'D+AAAA/wAAAP8AAAD/AAAA/wAAAP4AAAD/AAAA6QAAAHYAAAB1AAAA4QAAAP8AAAD/AAAA5QAAA'
                 b'DAAAAAAAAAAAgAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAAwAAAMoAAAD/AAAA+gAAAPwAAAD/AAAA'
                 b'/wAAAP8AAAD+AAAA/wAAAP8AAAD/AAAA/gAAAP4AAAD/AAAA/wAAAP8AAAD+AAAA/wAAAP8AAAD'
                 b'/AAAA/AAAAPoAAAD/AAAAywAAAAMAAAAAAAAAAQAAAAAAAAAAAAAAAwAAAAAAAABrAAAA/wAAAP'
                 b'oAAAD/AAAA/wAAAP0AAAD7AAAA+wAAAP4AAAD/AAAA/AAAAP0AAAD/AAAA/wAAAP0AAAD8AAAA/'
                 b'wAAAP4AAAD7AAAA+wAAAP0AAAD/AAAA/wAAAPoAAAD/AAAAawAAAAAAAAADAAAAAAAAAAEAAAAA'
                 b'AAAAAwAAANUAAAD/AAAA/QAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP0AAAD/AAAA/wAAAPI'
                 b'AAADyAAAA/wAAAP8AAAD9AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/QAAAP8AAADVAAAAAw'
                 b'AAAAAAAAABAAAAAQAAAAAAAAATAAAA7AAAAP8AAAD+AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD9A'
                 b'AAA/wAAAK8AAAA8AAAAFQAAABUAAAA8AAAArgAAAP8AAAD9AAAA/wAAAP8AAAD/AAAA/wAAAP8A'
                 b'AAD+AAAA/wAAAOwAAAATAAAAAAAAAAEAAAAAAAAAAQAAAAAAAADBAAAA/wAAAPoAAAD/AAAA/wA'
                 b'AAP8AAAD/AAAA/QAAAP8AAAB3AAAAAAAAAAIAAAAAAAAAAAAAAAIAAAAAAAAAdwAAAP8AAAD9AA'
                 b'AA/wAAAP8AAAD/AAAA/wAAAPoAAAD/AAAAwQAAAAAAAAABAAAAAAAAAAAAAAACAAAAAAAAADEAA'
                 b'ADmAAAA/wAAAP0AAAD/AAAA/wAAAPwAAAD/AAAAqwAAAAAAAAAEAAAAAwAAAAEAAAABAAAABAAA'
                 b'AAQAAAAAAAAArAAAAP8AAAD8AAAA/wAAAP8AAAD9AAAA/wAAAOYAAAAwAAAAAAAAAAIAAAAAAAA'
                 b'AAAAAAAAAAAABAAAAAAAAAA0AAACuAAAA/wAAAP0AAAD/AAAA/QAAAP8AAABCAAAAAAAAAAMAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAwAAAAAAAABCAAAA/wAAAP0AAAD/AAAA/QAAAP8AAACvAAAADQAAA'
                 b'AAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAACAAAAD4AAAA/wAAAP0AAAD/AAAA'
                 b'8gAAABgAAAAAAAAAAQAAAAAAAAAAAAAAAAAAAAAAAAABAAAAAAAAABgAAADyAAAA/wAAAP0AAAD'
                 b'/AAAA+AAAACAAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQAAAAAAAAAIAAAAP'
                 b'gAAAD/AAAA/QAAAP8AAADyAAAAGAAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAG'
                 b'AAAAPIAAAD/AAAA/QAAAP8AAAD4AAAAIAAAAAAAAAAEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAB'
                 b'AAAAAAAAAA0AAACuAAAA/wAAAP0AAAD/AAAA/QAAAP8AAABCAAAAAAAAAAMAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAwAAAAAAAABCAAAA/wAAAP0AAAD/AAAA/QAAAP8AAACuAAAADQAAAAAAAAABAAAAAA'
                 b'AAAAAAAAAAAAAAAgAAAAAAAAAxAAAA5gAAAP8AAAD9AAAA/wAAAP8AAAD8AAAA/wAAAKsAAAAAA'
                 b'AAABAAAAAMAAAABAAAAAQAAAAMAAAAEAAAAAAAAAKsAAAD/AAAA/AAAAP8AAAD/AAAA/QAAAP8A'
                 b'AADmAAAAMAAAAAAAAAACAAAAAAAAAAAAAAABAAAAAAAAAMEAAAD/AAAA+gAAAP8AAAD/AAAA/wA'
                 b'AAP8AAAD9AAAA/wAAAHcAAAAAAAAAAgAAAAAAAAAAAAAAAgAAAAAAAAB3AAAA/wAAAP0AAAD/AA'
                 b'AA/wAAAP8AAAD/AAAA+gAAAP8AAADBAAAAAAAAAAEAAAAAAAAAAQAAAAAAAAATAAAA7AAAAP8AA'
                 b'AD+AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD9AAAA/wAAAK4AAAA8AAAAFQAAABUAAAA8AAAArgAA'
                 b'AP8AAAD9AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD+AAAA/wAAAOwAAAATAAAAAAAAAAEAAAABAAA'
                 b'AAAAAAAMAAADWAAAA/wAAAP0AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD9AAAA/wAAAP8AAA'
                 b'DyAAAA8gAAAP8AAAD/AAAA/QAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP0AAAD/AAAA1QAAA'
                 b'AMAAAAAAAAAAQAAAAAAAAADAAAAAAAAAGsAAAD/AAAA+gAAAP8AAAD/AAAA/QAAAPsAAAD7AAAA'
                 b'/gAAAP8AAAD8AAAA/QAAAP8AAAD/AAAA/QAAAPwAAAD/AAAA/gAAAPsAAAD7AAAA/QAAAP8AAAD'
                 b'/AAAA+gAAAP8AAABrAAAAAAAAAAMAAAAAAAAAAAAAAAEAAAAAAAAAAwAAAMsAAAD/AAAA+gAAAP'
                 b'wAAAD/AAAA/wAAAP8AAAD+AAAA/wAAAP8AAAD/AAAA/gAAAP4AAAD/AAAA/wAAAP8AAAD+AAAA/'
                 b'wAAAP8AAAD/AAAA/AAAAPoAAAD/AAAAywAAAAMAAAAAAAAAAQAAAAAAAAAAAAAAAAAAAAIAAAAA'
                 b'AAAAMQAAAOYAAAD/AAAA/wAAAOEAAAB2AAAAdgAAAOkAAAD/AAAA/gAAAP8AAAD/AAAA/wAAAP8'
                 b'AAAD+AAAA/wAAAOkAAAB2AAAAdgAAAOEAAAD/AAAA/wAAAOYAAAAxAAAAAAAAAAIAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAEAAAAAAAAAEQAAAFQAAABRAAAADwAAAAAAAAAAAAAARAAAAP8AAAD+A'
                 b'AAA/wAAAP8AAAD/AAAA/wAAAP4AAAD/AAAARAAAAAAAAAAAAAAADwAAAFEAAABUAAAAEQAAAAAA'
                 b'AAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIAAAABAAAAAAAAAAAAAAABAAAABQA'
                 b'AAAAAAAATAAAA7QAAAP8AAAD+AAAA/wAAAP8AAAD+AAAA/wAAAO0AAAATAAAAAAAAAAUAAAABAA'
                 b'AAAAAAAAAAAAABAAAAAgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAA'
                 b'AAEAAAABAAAAAEAAAABAAAAAAAAAAsAAADjAAAA/wAAAPwAAAD+AAAA/gAAAPwAAAD/AAAA4wAA'
                 b'AAoAAAAAAAAAAQAAAAEAAAAEAAAABAAAAAEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAADAAAAAAAAAJAAAAD/AAAA/AAAAP8AAA'
                 b'D/AAAA/AAAAP8AAACQAAAAAAAAAAMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAA'
                 b'BwAAAJAAAADqAAAA7gAAAO4AAADqAAAAkQAAAAcAAAAAAAAAAQAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAACAAAAAAAAAAkAAAASAAAAEgAAAAoAAAAAAAAAAgAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAADAAAAAAAAAAAAAAAAAAAAAAAAAAM'
                 b'AAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA/9GL//'
                 b'+kJf//kAn//5AJ/+ggBBfTIATLqCAEFZAAAAkgAAAEIAAABEAAAAJAAAACIAQgBKAIEAVQClAKq'
                 b'AmQFagJkBVQClAKoAgQBSAEIARAAAACQAAAAiAAAAQgAAAEkAAACaggBBXTIATL6CAEF/+QCf//'
                 b'kAn//6Ql///Ri/8=')

    icon_img2 = b64decode(icon_img2)
    icon_img2 = ImageTk.PhotoImage(data=icon_img2)
    opt.tk.call('wm', 'iconphoto', opt._w, icon_img2)

    Optionscards = tk.ttk.Notebook(opt)

    #Calculate Options card---------------------------------------------------------#
    Main1Frame0 = tk.Frame(opt)

    #ION correction#
    TOP1Frame0 = ttk.Frame(Main1Frame0)
    TOP1Frame0.pack(side=tk.TOP, expand=tk.YES, padx='1px', pady='2px', fill=tk.X)

    ioncor = ttk.Label(TOP1Frame0, text='Ionosphere Correction')
    ioncor.pack(side=tk.LEFT, anchor='w', padx='1px')

    cionbox = ttk.Combobox(TOP1Frame0, width=15, height=4)
    cionbox['state'] = 'readonly'
    cionbox['value'] = ('OFF', 'Ion-Free LC', 'Broadcast')
    cionbox.current(ioncorstate)
    cionbox.pack(side=tk.RIGHT, anchor='e')

    #TRO correction#
    TOP2Frame0 = ttk.Frame(Main1Frame0)
    TOP2Frame0.pack(side=tk.TOP, expand=tk.YES, padx='1px', pady='2px', fill=tk.X)

    ioncor = ttk.Label(TOP2Frame0, text='Troposphere Correction')
    ioncor.pack(side=tk.LEFT, anchor='w', padx='1px')

    ctrobox = ttk.Combobox(TOP2Frame0, width=15, height=4)
    ctrobox['state'] = 'readonly'
    ctrobox['value'] = ('OFF', 'Hopfield', 'Saastamoinen')
    ctrobox.current(trocorstate)
    ctrobox.pack(side=tk.RIGHT, anchor='e')

    #GNSS Options#
    TOP5Frame0 = ttk.LabelFrame(Main1Frame0, text='Satellite system', labelanchor='nw')
    TOP5Frame0.pack(side=tk.TOP, expand=tk.YES, padx='1px', pady='2px', fill=tk.X)

    satsys = [("GPS", 0),("GLO", 1),("BeiDou", 2),("Galileo", 3),("SBAS", 4)]
    v = tk.IntVar()

    for sys, num in satsys:
        radio_button = ttk.Radiobutton(TOP5Frame0, text=sys, variable=v, value=num)
        radio_button.pack(side=tk.LEFT, padx='3px', anchor='w')

    v.set(satellite_system)

    #Drawing Options card-----------------------------------------------------------#
    Main2Frame0 = tk.Frame(opt)

    # Satnum plot options#
    Buttom1Frame1 = ttk.LabelFrame(Main2Frame0, text='Satn configs', labelanchor='nw')
    Buttom1Frame1.pack(side=tk.BOTTOM, expand=tk.YES, padx='1px', pady='1px', fill=tk.BOTH)

    SatnFrameB1 = tk.Frame(Buttom1Frame1)
    SatnFrameB1.pack(side=tk.LEFT, expand=tk.YES, padx='5px', pady='0px')
    satncolopt = ttk.Button(SatnFrameB1, text='Satn Color', width=10, command=lambda: Colorchoose(0, satncollab, 4))
    satncolopt.pack(side=tk.LEFT, expand=tk.YES, padx='5px', pady='0px')
    satncollab = tk.Label(SatnFrameB1, width=5, height=1, bg=satncolormid[0])
    satncollab.pack(side=tk.LEFT, expand=tk.YES, padx='5px', pady='0px')

    LineConB1 = tk.Frame(Buttom1Frame1)
    LineConB1.pack(side=tk.RIGHT, expand=tk.YES, padx='5px', pady='0px')
    linewre = ttk.Button(LineConB1, text='Undo', width=10,
                         command=lambda: Colorchoose(0, satncollab, 0))
    linewre.pack(side=tk.LEFT, expand=tk.YES, padx='1px')

    #Line's colors options#
    Left2Frame1 = ttk.LabelFrame(Main2Frame0, text='ENU colors', labelanchor='nw')
    Left2Frame1.pack(side=tk.LEFT, expand=tk.YES, padx='1px', pady='1px', fill=tk.Y)

    EFrameL2 = tk.Frame(Left2Frame1)
    EFrameL2.pack(side=tk.TOP, expand=tk.YES, padx='5px', pady='0px')
    ecolopt = ttk.Button(EFrameL2, text='E Color', width=10, command=lambda: Colorchoose(0, ecollab, 1))
    ecolopt.pack(side=tk.LEFT, expand=tk.YES, padx='5px', pady='0px')
    ecollab = tk.Label(EFrameL2, width=5, height=1, bg=enucolor[0])
    ecollab.pack(side=tk.LEFT, expand=tk.YES, padx='5px', pady='0px')

    NFrameL2 = tk.Frame(Left2Frame1)
    NFrameL2.pack(side=tk.TOP, expand=tk.YES, padx='5px', pady='0px')
    ncolopt = ttk.Button(NFrameL2, text='N Color', width=10, command=lambda: Colorchoose(1, ncollab,1))
    ncolopt.pack(side=tk.LEFT, expand=tk.YES, padx='5px', pady='0px')
    ncollab = tk.Label(NFrameL2, width=5, height=1, bg=enucolor[1])
    ncollab.pack(side=tk.LEFT, expand=tk.YES, padx='5px', pady='0px')

    UFrameL2 = tk.Frame(Left2Frame1)
    UFrameL2.pack(side=tk.TOP, expand=tk.YES, padx='5px', pady='0px')
    ucolopt = ttk.Button(UFrameL2, text='U Color', width=10, command=lambda: Colorchoose(2, ucollab,1))
    ucolopt.pack(side=tk.LEFT, expand=tk.YES, padx='5px', pady='0px')
    ucollab = tk.Label(UFrameL2, width=5, height=1, bg=enucolor[2])
    ucollab.pack(side=tk.LEFT, expand=tk.YES, padx='5px', pady='0px')

    ColorConL2 = tk.Frame(Left2Frame1)
    ColorConL2.pack(side=tk.TOP, expand=tk.YES, padx='5px', pady='0px')
    colorcon = ttk.Button(ColorConL2, text='Undo', width=10,
                         command=lambda: [Colorchoose(0, ecollab,0)
                                         ,Colorchoose(0, ncollab,0)
                                         ,Colorchoose(0, ucollab,0)])
    colorcon.pack(side=tk.LEFT, expand=tk.YES, padx='5px', pady='0px')

    #Line's weight options#
    Right2Frame1 = ttk.LabelFrame(Main2Frame0, text='Draw configs', labelanchor='nw')
    Right2Frame1.pack(side=tk.RIGHT, expand=tk.YES , padx='1px', pady='1px', fill=tk.Y)

    EFrameR2 = tk.Frame(Right2Frame1)
    EFrameR2.pack(side=tk.TOP, expand=tk.YES, padx='5px', pady='0px')
    elinewopt = tk.Label(EFrameR2, text='Eline:', width=5, height=1)
    elinewopt.pack(side=tk.LEFT, expand=tk.YES, padx='1px', pady='0px')
    elinewidth = ttk.Spinbox(EFrameR2, from_=0, to=10,increment=0.01, width=10)
    elinewidth.set(enulinew[0])
    elinewidth.pack(side=tk.LEFT, expand=tk.YES, padx='1px', pady='0px')

    NFrameR2 = tk.Frame(Right2Frame1)
    NFrameR2.pack(side=tk.TOP, expand=tk.YES, padx='5px', pady='0px')
    nlinewopt = tk.Label(NFrameR2, text='Nline:', width=5, height=1)
    nlinewopt.pack(side=tk.LEFT, expand=tk.YES, padx='1px', pady='0px')
    nlinewidth = ttk.Spinbox(NFrameR2, from_=0, to=10,increment=0.01, width=10)
    nlinewidth.set(enulinew[1])
    nlinewidth.pack(side=tk.LEFT, expand=tk.YES, padx='1px', pady='0px')

    UFrameR2 = tk.Frame(Right2Frame1)
    UFrameR2.pack(side=tk.TOP, expand=tk.YES, padx='5px', pady='0px')
    ulinewopt = tk.Label(UFrameR2, text='Uline:', width=5, height=1)
    ulinewopt.pack(side=tk.LEFT, expand=tk.YES, padx='1px', pady='0px')
    ulinewidth = ttk.Spinbox(UFrameR2, from_=0, to=10,increment=0.01, width=10)
    ulinewidth.set(enulinew[2])
    ulinewidth.pack(side=tk.LEFT, expand=tk.YES, padx='1px', pady='0px')

    LineConR2 = tk.Frame(Right2Frame1)
    LineConR2.pack(side=tk.TOP, expand=tk.YES, padx='5px', pady='0px')
    linewre = ttk.Button(LineConR2, text='Undo', width=10,
                         command=lambda: Linewchoose(elinewidth, nlinewidth, ulinewidth, 0))
    linewre.pack(side=tk.LEFT, expand=tk.YES, padx='1px')

    Optionscards.add(Main1Frame0, text='Calc options')
    Optionscards.add(Main2Frame0, text='Draw options')
    Optionscards.pack(fill=tk.BOTH, expand=True, padx='2px', pady='0px')

    #TOTAL Confirm#
    Bottom0Frame0 = ttk.Frame(opt)
    Bottom0Frame0.pack(side=tk.TOP, expand=tk.YES, padx='1px', pady='0px', fill=tk.Y)
    TotalCancel = ttk.Button(Bottom0Frame0, text='Cancel', width=10,
                          command=lambda: [opt.destroy()])
    TotalCancel.pack(side=tk.RIGHT, padx='1px', anchor='e')
    TotalCon = ttk.Button(Bottom0Frame0, text='Confirm', width=10,
                          command=lambda: [Colorchoose(0, ecollab,3),
                                           Linewchoose(elinewidth,nlinewidth,ulinewidth,1),
                                           Calcu_confirm(v, cionbox, ctrobox),
                                           opt.destroy()])
    TotalCon.pack(side=tk.RIGHT, padx='1px', anchor='e')

def Abo_windows():
    abo = tk.Toplevel(root)
    abo.title("About")
    abo.attributes('-toolwindow', 2)
    abo.geometry('200x200+200+200')
    abo.minsize(200, 100)
    abo.maxsize(200, 100)

    About = tk.Label(abo, text="\\(￣︶￣*\\))\n\nKNZ_plot ver 1.2.1\nCopyright (c) 2024 by KenanZhu\nAll Right Reserved.")
    About.pack(side=tk.TOP, expand=tk.YES)

def View_windows():
    view = tk.Toplevel(root)
    view.title("KNZ_View")
    view.geometry('900x600')
    view.minsize(1000, 600)

    global icon_img1
    icon_img1 = (b'AAABAAEAMDAAAAEAIACoJQAAFgAAACgAAAAwAAAAYAAAAAEAIAAAAAAAACQAAPY6AQD2OgEAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQAAAAQAAA'
                 b'ABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAEAAAAAQAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAACAAAAAgAAAAAAAAABAAAADQAAABIAAAARAAAAEQAAABEAAAARAAAAEQAAAB'
                 b'EAAAARAAAAEQAAABEAAAARAAAAEQAAABEAAAARAAAAEQAAABEAAAARAAAAEQAAABEAAAASAAAAD'
                 b'QAAAAEAAAAAAAAAAgAAAAIAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIAAAAAAAAACQAAAHEAAADMAAAA6wAAAO8'
                 b'AAADuAAAA7gAAAO4AAADuAAAA7gAAAO4AAADuAAAA7gAAAO4AAADuAAAA7gAAAO4AAADuAAAA7g'
                 b'AAAO4AAADuAAAA7gAAAO4AAADvAAAA6wAAAMwAAAByAAAACQAAAAAAAAACAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAgAAAAAA'
                 b'AAAdAAAAzwAAAP8AAAD9AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/wA'
                 b'AAP8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP0AAAD/AA'
                 b'AA0AAAAB0AAAAAAAAAAgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAABAAAAAAAAAAgAAADMAAAA/wAAAPoAAAD6AAAA/gAAAP8AAAD/AAAA/wAA'
                 b'AP8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAA'
                 b'A/wAAAP8AAAD/AAAA/gAAAPoAAAD6AAAA/wAAAMwAAAAIAAAAAAAAAAEAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAHIAAAD/AAAA+QAAA'
                 b'P8AAAD/AAAA9wAAAO0AAADuAAAA7gAAAO4AAADuAAAA7gAAAO4AAADuAAAA7gAAAO4AAADuAAAA'
                 b'7gAAAO4AAADuAAAA7gAAAO4AAADuAAAA7gAAAO4AAADtAAAA9wAAAP8AAAD/AAAA+QAAAP8AAAB'
                 b'xAAAAAAAAAAQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAABAAAAAAAAAMcAAAD/AAAA+gAAAP8AAACtAAAAGQAAABEAAAASAAAAEQAAABEAAAARAAAAE'
                 b'QAAABEAAAARAAAAEQAAABEAAAARAAAAEQAAABEAAAARAAAAEQAAABEAAAAUAAAAFgAAABUAAAAR'
                 b'AAAAGQAAAK0AAAD/AAAA+gAAAP8AAADHAAAAAAAAAAEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAADQAAAOgAAAD/AAAA/wAAAPkAAAAjAAAAAA'
                 b'AAAAIAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAACAAAAAAAAACMAAAD5AAAA/wAAAP8AAADnAAAADQAAAAAA'
                 b'AAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAEgA'
                 b'AAO0AAAD/AAAA/wAAAO0AAAATAAAAAAAAAAIAAAABAAAAAQAAAAEAAAABAAAAAQAAAAEAAAABAA'
                 b'AAAQAAAAEAAAABAAAAAQAAAAEAAAABAAAABQAAAAAAAAA/AAAAkwAAAE0AAAAAAAAAAAAAABQAA'
                 b'ADtAAAA/wAAAP8AAADtAAAAEgAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAEAAAAAAAAAEQAAAO0AAAD/AAAA/wAAAO4AAAASAAAAAAAAAAEAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAgAAAAQAAAAEAAAABAAAAAIAAAAFAAAAAAAAAFsAAA'
                 b'D/AAAA/gAAAP8AAABNAAAAAAAAABUAAADuAAAA/wAAAP8AAADtAAAAEQAAAAAAAAABAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAEgAAAO0AAAD/AAAA'
                 b'/wAAAO4AAAARAAAAAAAAAAEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIAAAADAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAIAAAAAAAAAXAAAAP8AAAD9AAAA9QAAAP8AAACIAAAAAAAAABUAAADuAAAA/wAAAP'
                 b'8AAADtAAAAEgAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAEAAAAAAAAAEgAAAO0AAAD/AAAA/wAAAO4AAAARAAAAAAAAAAEAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAgAAAAIAAAAAAAAAKgAAAGcAAAB7AAAAaAAAAB0AAABaAAAA/wAAAP0AAAD8AAAA/QAAAP8'
                 b'AAAA/AAAAAAAAABQAAADuAAAA/wAAAP8AAADtAAAAEgAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAEgAAAO0AAAD/AAAA/wAAAO4AAAARA'
                 b'AAAAAAAAAEAAAAAAAAAAAAAAAAAAAACAAAAAAAAABYAAACrAAAA/gAAAP8AAAD+AAAA/wAAAPcA'
                 b'AAD4AAAA/gAAAP0AAAD9AAAA/wAAAFwAAAAAAAAAAAAAABEAAADuAAAA/wAAAP8AAADtAAAAEgA'
                 b'AAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAA'
                 b'AAEgAAAO0AAAD/AAAA/wAAAO4AAAARAAAAAAAAAAEAAAAAAAAAAAAAAAEAAAAAAAAAGAAAANgAA'
                 b'AD/AAAA/QAAAPkAAAD2AAAA+QAAAP8AAAD9AAAA/gAAAP8AAAD/AAAAXAAAAAAAAAAFAAAAAAAA'
                 b'ABEAAADuAAAA/wAAAP8AAADtAAAAEgAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAEgAAAO0AAAD/AAAA/wAAAO4AAAARAAAAAAAAAAEAAA'
                 b'AAAAAAAAAAAAIAAAAAAAAArAAAAP8AAAD5AAAA/QAAAP8AAAD/AAAA/wAAAP0AAAD+AAAA/wAAA'
                 b'P8AAABbAAAAAAAAAAUAAAABAAAAAAAAABEAAADuAAAA/wAAAP8AAADtAAAAEgAAAAAAAAABAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAEgAAAO0AAAD'
                 b'/AAAA/wAAAO4AAAARAAAAAAAAAAEAAAAAAAAAAgAAAAAAAAAwAAAA/gAAAP0AAAD+AAAA/wAAAM'
                 b'YAAACAAAAAygAAAP8AAAD9AAAA/wAAAPoAAAAmAAAAAAAAAAIAAAABAAAAAAAAABEAAADuAAAA/'
                 b'wAAAP8AAADtAAAAEgAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAEAAAAAAAAAEgAAAO0AAAD/AAAA/wAAAO4AAAARAAAAAAAAAAEAAAAAAAAABAAAAAA'
                 b'AAABqAAAA/wAAAPkAAAD/AAAAxQAAAAMAAAAAAAAABgAAAMsAAAD/AAAA+QAAAP8AAABkAAAAAA'
                 b'AAAAQAAAABAAAAAAAAABEAAADuAAAA/wAAAP8AAADtAAAAEgAAAAAAAAABAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAEgAAAO0AAAD/AAAA/wAAAO4A'
                 b'AAARAAAAAAAAAAEAAAAAAAAABAAAAAAAAAB/AAAA/wAAAPcAAAD/AAAAggAAAAAAAAASAAAAAAA'
                 b'AAIoAAAD/AAAA9wAAAP8AAAB4AAAAAAAAAAQAAAABAAAAAAAAABEAAADuAAAA/wAAAP8AAADtAA'
                 b'AAEgAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAA'
                 b'AAAAAAAEgAAAO0AAAD/AAAA/wAAAO4AAAARAAAAAAAAAAEAAAAAAAAABAAAAAAAAABrAAAA/wAA'
                 b'APkAAAD/AAAAwQAAAAAAAAAAAAAAAwAAAMcAAAD/AAAA+QAAAP8AAABkAAAAAAAAAAQAAAABAAA'
                 b'AAAAAABEAAADuAAAA/wAAAP8AAADtAAAAEgAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAEgAAAO0AAAD/AAAA/wAAAO4AAAARAAAAAAAAA'
                 b'AEAAAAAAAAAAgAAAAAAAAAyAAAA/wAAAP0AAAD+AAAA/wAAAMEAAAB4AAAAxAAAAP8AAAD+AAAA'
                 b'/gAAAPwAAAArAAAAAAAAAAIAAAABAAAAAAAAABEAAADuAAAA/wAAAP8AAADtAAAAEgAAAAAAAAA'
                 b'BAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAEgAAAO'
                 b'0AAAD/AAAA/wAAAO4AAAARAAAAAAAAAAEAAAAAAAAAAAAAAAIAAAAAAAAAsAAAAP8AAAD5AAAA/'
                 b'QAAAP8AAAD/AAAA/wAAAP0AAAD5AAAA/wAAAKkAAAAAAAAAAgAAAAAAAAABAAAAAAAAABEAAADu'
                 b'AAAA/wAAAP8AAADtAAAAEgAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAEAAAAAAAAAEgAAAO0AAAD/AAAA/wAAAO4AAAARAAAAAAAAAAEAAAAAAAAAAA'
                 b'AAAAEAAAAAAAAAHAAAAN0AAAD/AAAA/AAAAPgAAAD2AAAA+AAAAPwAAAD/AAAA2AAAABgAAAAAA'
                 b'AAAAQAAAAAAAAABAAAAAAAAABEAAADuAAAA/wAAAP8AAADtAAAAEgAAAAAAAAABAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAEgAAAO0AAAD/AAAA/wA'
                 b'AAO4AAAARAAAAAAAAAAEAAAAAAAAAAAAAAAAAAAACAAAAAAAAABsAAAC0AAAA/wAAAP8AAAD+AA'
                 b'AA/wAAAP8AAACvAAAAGAAAAAAAAAACAAAAAAAAAAAAAAABAAAAAAAAABEAAADuAAAA/wAAAP8AA'
                 b'ADtAAAAEgAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAEAAAAAAAAAEgAAAO0AAAD/AAAA/wAAAO4AAAARAAAAAAAAAAEAAAAAAAAAAQAAAAEAAAABAAA'
                 b'AAwAAAAIAAAAAAAAAMQAAAG8AAACEAAAAbgAAAC8AAAAAAAAAAgAAAAMAAAABAAAAAQAAAAAAAA'
                 b'ABAAAAAAAAABEAAADuAAAA/wAAAP8AAADtAAAAEgAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAEgAAAO0AAAD/AAAA/wAAAO4AAAARAAAA'
                 b'AAAAAAEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAAAAAAAAABEAAADuAAAA/wAAAP8AAADtAAAAEgAAAA'
                 b'AAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAE'
                 b'gAAAO0AAAD/AAAA/wAAAO4AAAARAAAAAAAAAAEAAAABAAAAEAAAABIAAAASAAAAEgAAABIAAAAS'
                 b'AAAAFAAAABYAAAAWAAAAFgAAABQAAAASAAAAEgAAABIAAAASAAAAEAAAAAEAAAABAAAAAAAAABE'
                 b'AAADuAAAA/wAAAP8AAADtAAAAEgAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAEAAAAAAAAAEgAAAO0AAAD/AAAA/wAAAO4AAAARAAAAAAAAAAAAAAAQA'
                 b'AAA3QAAAPIAAADsAAAA7QAAAO0AAADtAAAA7QAAAO0AAADtAAAA7QAAAO0AAADtAAAA7QAAAOwA'
                 b'AADyAAAA3QAAABAAAAAAAAAAAAAAABEAAADuAAAA/wAAAP8AAADtAAAAEgAAAAAAAAABAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAEgAAAO0AAAD/AA'
                 b'AA/wAAAO4AAAARAAAAAAAAAAAAAAASAAAA7QAAAP8AAAD+AAAA/wAAAP8AAAD/AAAA/wAAAP8AA'
                 b'AD/AAAA/wAAAP8AAAD/AAAA/wAAAP4AAAD/AAAA7QAAABIAAAAAAAAAAAAAABEAAADuAAAA/wAA'
                 b'AP8AAADtAAAAEgAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAEAAAAAAAAAEgAAAO0AAAD/AAAA/wAAAO4AAAARAAAAAAAAAAAAAAASAAAA7QAAAP8AAA'
                 b'D+AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP4AAAD/AAAA7QAAA'
                 b'BIAAAAAAAAAAAAAABEAAADuAAAA/wAAAP8AAADtAAAAEgAAAAAAAAABAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAEgAAAO0AAAD/AAAA/wAAAO4AAAA'
                 b'RAAAAAAAAAAAAAAAQAAAA3QAAAPIAAADtAAAA7gAAAO4AAADuAAAA7gAAAO4AAADuAAAA7gAAAO'
                 b'4AAADuAAAA7gAAAO0AAADyAAAA3QAAABAAAAAAAAAAAAAAABEAAADuAAAA/wAAAP8AAADtAAAAE'
                 b'gAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAA'
                 b'AAAAEgAAAO0AAAD/AAAA/wAAAO4AAAARAAAAAAAAAAEAAAABAAAAEAAAABIAAAARAAAAEQAAABE'
                 b'AAAARAAAAEQAAABEAAAARAAAAEQAAABEAAAARAAAAEQAAABEAAAASAAAAEAAAAAEAAAABAAAAAA'
                 b'AAABEAAADuAAAA/wAAAP8AAADtAAAAEgAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAEgAAAO0AAAD/AAAA/wAAAO4AAAARAAAAAAAAAAEA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAABAAAAAAAAABEAAADuAAAA/wAAAP8AAADtAAAAEgAAAAAAAAABAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAEgAAAO0AA'
                 b'AD/AAAA/wAAAO4AAAARAAAAAAAAAAEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAAAAAAAAABEAAADuAAA'
                 b'A/wAAAP8AAADtAAAAEgAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAEAAAAAAAAAEgAAAO0AAAD/AAAA/wAAAO4AAAARAAAAAAAAAAEAAAABAAAAEAAAA'
                 b'BIAAAASAAAAEgAAABIAAAASAAAAEgAAABIAAAASAAAAEgAAABIAAAASAAAAEgAAABIAAAASAAAA'
                 b'EAAAAAEAAAABAAAAAAAAABEAAADuAAAA/wAAAP8AAADtAAAAEgAAAAAAAAABAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAEgAAAO0AAAD/AAAA/wAAAO'
                 b'4AAAARAAAAAAAAAAAAAAAQAAAA3QAAAPIAAADsAAAA7QAAAO0AAADtAAAA7QAAAO0AAADtAAAA7'
                 b'QAAAO0AAADtAAAA7QAAAOwAAADyAAAA3QAAABAAAAAAAAAAAAAAABEAAADuAAAA/wAAAP8AAADt'
                 b'AAAAEgAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAE'
                 b'AAAAAAAAAEgAAAO0AAAD/AAAA/wAAAO4AAAARAAAAAAAAAAAAAAASAAAA7QAAAP8AAAD+AAAA/w'
                 b'AAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP4AAAD/AAAA7QAAABIAAAAAA'
                 b'AAAAAAAABEAAADuAAAA/wAAAP8AAADtAAAAEgAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAEgAAAO0AAAD/AAAA/wAAAO4AAAARAAAAAAA'
                 b'AAAAAAAASAAAA7QAAAP8AAAD+AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/AA'
                 b'AA/wAAAP4AAAD/AAAA7QAAABIAAAAAAAAAAAAAABEAAADuAAAA/wAAAP8AAADtAAAAEgAAAAAAA'
                 b'AABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAEgAA'
                 b'AO0AAAD/AAAA/wAAAO4AAAARAAAAAAAAAAAAAAAQAAAA3QAAAPIAAADsAAAA7QAAAO0AAADtAAA'
                 b'A7QAAAO0AAADtAAAA7QAAAO0AAADtAAAA7QAAAOwAAADyAAAA3QAAABAAAAAAAAAAAAAAABEAAA'
                 b'DuAAAA/wAAAP8AAADtAAAAEgAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAEAAAAAAAAAEQAAAO0AAAD/AAAA/wAAAO4AAAASAAAAAAAAAAEAAAABAAAA'
                 b'EAAAABIAAAASAAAAEgAAABIAAAASAAAAEgAAABIAAAASAAAAEgAAABIAAAASAAAAEgAAABIAAAA'
                 b'SAAAAEAAAAAEAAAABAAAAAAAAABIAAADuAAAA/wAAAP8AAADtAAAAEQAAAAAAAAABAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAAAAAAAEgAAAO0AAAD/AAAA/'
                 b'wAAAO0AAAATAAAAAAAAAAIAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAAACAAAAAAAAABMAAADtAAAA/wAAAP8'
                 b'AAADtAAAAEgAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAEAAAAAAAAADQAAAOgAAAD/AAAA/wAAAPkAAAAjAAAAAAAAAAIAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAACAAAAAAAAACMAAAD5AAAA/wAAAP8AAADnAAAADQAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAAAAAAAAAMcAAAD/AAAA+gAAAP8AAACtAA'
                 b'AAGQAAABEAAAASAAAAEQAAABEAAAARAAAAEQAAABEAAAARAAAAEQAAABEAAAARAAAAEQAAABEAA'
                 b'AARAAAAEQAAABEAAAARAAAAEQAAABIAAAARAAAAGQAAAK0AAAD/AAAA+gAAAP8AAADHAAAAAAAA'
                 b'AAEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAAA'
                 b'AAAAAAHIAAAD/AAAA+QAAAP8AAAD/AAAA9wAAAO0AAADuAAAA7gAAAO4AAADuAAAA7gAAAO4AAA'
                 b'DuAAAA7gAAAO4AAADuAAAA7gAAAO4AAADuAAAA7gAAAO4AAADuAAAA7gAAAO4AAADtAAAA9wAAA'
                 b'P8AAAD/AAAA+QAAAP8AAABxAAAAAAAAAAQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAABAAAAAAAAAAgAAADMAAAA/wAAAPoAAAD6AAAA/gAAAP8AAAD'
                 b'/AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP'
                 b'8AAAD/AAAA/wAAAP8AAAD/AAAA/gAAAPoAAAD5AAAA/wAAAMwAAAAIAAAAAAAAAAEAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAgAAAAAAAAAd'
                 b'AAAA0AAAAP8AAAD9AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP8'
                 b'AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP8AAAD/AAAA/wAAAP0AAAD/AAAA0A'
                 b'AAAB0AAAAAAAAAAgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAIAAAAAAAAACgAAAHIAAADNAAAA6wAAAO8AAADuAAAA7gAAAO4A'
                 b'AADuAAAA7gAAAO4AAADuAAAA7gAAAO4AAADuAAAA7gAAAO4AAADuAAAA7gAAAO4AAADuAAAA7gA'
                 b'AAO4AAADvAAAA6wAAAM0AAAByAAAACgAAAAAAAAACAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACAAAAAgAAAAAAA'
                 b'AABAAAADQAAABIAAAARAAAAEQAAABEAAAARAAAAEQAAABEAAAARAAAAEQAAABEAAAARAAAAEQAA'
                 b'ABEAAAARAAAAEQAAABEAAAARAAAAEQAAABEAAAASAAAADQAAAAEAAAAAAAAAAgAAAAIAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAQAAAAQAAAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                 b'AAAAAAEAAAAEAAAAAQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAD/Rf/'
                 b'/ov8AAP6QAAAJfwAA/UAAAAK/AAD+gAAAAX8AAPkAAAAAnwAA+QAAAACfAAD5AAAAAJ8AAPoF//'
                 b'+gXwAA+gQABGBfAAD6BegIIF8AAPoF0pAgXwAA+gWkACBfAAD6BVAAYF8AAPoFIACgXwAA+gUgA'
                 b'SBfAAD6BUABIF8AAPoFQIEgXwAA+gVBQSBfAAD6BUCBIF8AAPoFQAEgXwAA+gUgAiBfAAD6BSAC'
                 b'IF8AAPoFUAUgXwAA+gQEECBfAAD6Bf//oF8AAPoEAAAgXwAA+gYAAGBfAAD6BgAAYF8AAPoGAAB'
                 b'gXwAA+gYAAGBfAAD6BAAAIF8AAPoF//+gXwAA+gX//6BfAAD6BAAAIF8AAPoGAABgXwAA+gYAAG'
                 b'BfAAD6BgAAYF8AAPoGAABgXwAA+gQAACBfAAD6BF/6IF8AAPoF//+gXwAA+QAAAACfAAD5AAAAA'
                 b'J8AAPkAAAAAnwAA/oAAAAF/AAD9QAAAAr8AAP6QAAAJfwAA/0X//6L/AAA=')

    icon_img1 = b64decode(icon_img1)
    icon_img1 = ImageTk.PhotoImage(data=icon_img1)
    view.tk.call('wm', 'iconphoto', view._w, icon_img1)

    #View window's menu
    topmenu = tk.Menu(view)
    filemenu = tk.Menu(topmenu, tearoff=False)
    filemenu.add_command(label="Open File", command=lambda: View_windowshow(text, view), accelerator='Ctrl+F')
    filemenu.add_command(label="Exit", command=view.destroy, accelerator='Ctrl+E')
    topmenu.add_cascade(label="File", menu=filemenu)

    view.config(menu=topmenu)

    ybar = tk.Scrollbar(view)
    ybar.pack(side=tk.RIGHT, fill=tk.Y)
    xbar = tk.Scrollbar(view, orient=tk.HORIZONTAL)
    xbar.pack(side=tk.BOTTOM, fill=tk.X)

    text = tk.Text(view, xscrollcommand=xbar.set, yscrollcommand=ybar.set, height=500, width=1000, undo=True, autoseparators=False, wrap='none')
    text.pack(side=tk.TOP, fill=tk.BOTH)

    ybar.config(command=text.yview)
    xbar.config(command=text.xview)

def View_windowshow(text, view):
    #Brief # get the file path of reading and show in the text ctr box
    #Param # text : the show box of file of choice
    #Param # view : the father window of text ctr box
    #Return# none
    f_path = filedialog.askopenfilename()
    view.title('KNZ_Viewing: %s' %(f_path))
    text.delete('1.0', 'end')
    with open(f_path, 'r') as f:
        lines = f.readlines()
        for line in lines:
            text.insert('insert', line)
    text.config()

def Clear_canvas():
    #Brief # clear the canvas
    #Return# none
    ex.clear()
    nx.clear()
    ux.clear()
    satn.clear()
    canvas0.draw()
    canvas1.draw()
    plotstate.config(text="None")
    root.update()

def PO_openfile():
    #Brief # read the .po file & draw in the canvas
    #Return# none
    ENU_O = np.empty((3, 0))
    #Sat numbers register part
    satnumnarry = np.empty((1, 0))
    f_path = filedialog.askopenfilename()
    if not f_path.find(".po") >= 0:
        plotstate.config(text="Error !: format is invalid.")
        root.update()
        return
    with open(f_path, 'r') as f:
        while 1:
            line = f.readline()
            if line.find("X-rec") >= 0:
                plotstate.config(text="Reading...")
                root.update()
                while 1:
                    line = f.readline()
                    if line != "" and line[1:2] != "-":
                        E = float(line[45 + 22:55 + 22])
                        N = float(line[56 + 22:66 + 22])
                        U = float(line[67 + 22:77 + 22])
                        satnum = int(line[64:67])

                        ENU_O = np.hstack((ENU_O, [ [E], [N], [U] ] ))
                        satnumnarry = np.hstack((satnumnarry, [[satnum]]))
                    if line[1:2] == "-":
                        ENU_O = np.hstack((ENU_O, [[0], [0], [0]]))
                        satnumnarry = np.hstack((satnumnarry, [[0]]))
                    if line == "":
                        ex.clear()
                        nx.clear()
                        ux.clear()
                        ex.plot(ENU_O[0, :], color=enucolor[0], linewidth=enulinew[0], marker='.', linestyle=':', ms=enulinew[0])
                        ex.set_title('E-W(m)', loc='left', fontsize=8)
                        ex.grid(True, linestyle='--', alpha=0.7)
                        nx.plot(ENU_O[1, :], color=enucolor[1], linewidth=enulinew[1], marker='.', linestyle=':', ms=enulinew[1])
                        nx.set_title('N-S(m)', loc='left', fontsize=8)
                        nx.grid(True, linestyle='--', alpha=0.7)
                        ux.plot(ENU_O[2, :], color=enucolor[2], linewidth=enulinew[2], marker='.', linestyle=':', ms=enulinew[2])
                        ux.set_title('U-D(m)', loc='left', fontsize=8)
                        ux.set_xlabel('Epochs')
                        ux.grid(True, linestyle='--', alpha=0.7)
                        canvas0.draw()

                        satn.clear()
                        satn.plot(satnumnarry[0, :], color=satncolor[0], linewidth=satnlinew[0])
                        satn.set_title('Valid Sat Numbers', loc='left', fontsize=8)
                        satn.set_xlabel('Epochs')
                        satn.grid(True, linestyle='--', alpha=0.7)
                        canvas1.draw()
                        break
            elif line == "":
                plotstate.config(text="Plot   form: %s" % (f_path))
                break

def SPP_calculat():
    # Brief # read .sp file & draw in the canvas & output the .po file
    # Return# none
    # Const define
    C_V = 299792458
    f1 = 1575.42
    f2 = 1227.60
    ENU_O = np.empty((3,0))
    #Sat numbers register part
    satnumnarry = np.empty((1, 0))
    f_path = filedialog.askopenfilename()
    if not f_path.find(".sp") >= 0:
        plotstate.config(text="Error !:  format is invalid.")
        root.update()
        return
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
                obsfile = satsysdict.get(satellite_system) + "-" + Obs[obspath:-4]
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
            print("@ GENERATE PROGRAM   : KNZ_Plot v1.2.1\n"
                  "@ GENERATE TYPE      : Receiver Station Position\n"
                  "@ GENERATE TIME      : %s\n"
                  "@ GENERATE SYS       : %s\n"
                  "@ IONOS OPT          : %s\n"
                  "@ TROPO OPT          : %s\n"
                  "@\n\n" % ( time.asctime(time.localtime())
                            , satsysdict.get(satellite_system)
                            , ioncormode[ioncorstate]
                            , trocormode[trocorstate]), file=of)
            print("GPST                     X-rec(m)     Y-rec(m)      Z-rec(m)   Sn(n)   dE(m)      dN(m)      dU(m)     ", file=of)
            of.close()
        #Coordinate transformation matrix---------------#
        S = np.array([
            [                   -math.sin(apL),                  math.cos(apL),             0],
            [   -math.sin(apB) * math.cos(apL), -math.sin(apB) * math.sin(apL), math.cos(apB)],
            [    math.cos(apB) * math.cos(apL),  math.cos(apB) * math.sin(apL), math.sin(apB)]
            ])
        #-----------------------------------------------#
        plotstate.config(text="Reading...")
        root.update()

        global epoch_last
        epoch_last = 0
        while 1:
            line = f.readline()
            if line == "":
                break
            flag = line[0: 1]
            if flag == ">":
                satnum = 0
                satX = np.empty((1, 0))
                satY = np.empty((1, 0))
                satZ = np.empty((1, 0))
                C1   = np.empty((1, 0))
                C2   = np.empty((1, 0))
                ma   = np.empty((1, 0))
                TGD  = np.empty((1, 0))
                Dt   = np.empty((1, 0))

                year  = int(line[6:10])
                month = int(line[11:13])
                day   = int(line[14:16])
                hour  = int(line[17:19])
                min   = int(line[20:22])
                sec   = float(line[23:30])
                epoch = int(line[2:6])

                count = 0
                while count < epoch - epoch_last - 1:
                    ENU_O = np.hstack(( ENU_O, [ [0], [0], [0] ] ))
                    satnumnarry = np.hstack((satnumnarry, [[0]]))
                    with open(o_path, 'a+') as of:
                        print("------------Missing data------------Missing data------------Missing data------------Missing data------------", file=of)
                        of.close()
                    count += 1
                epoch_last = epoch
                while 1:
                    line = f.readline()
                    if line == "\n":
                        satnumnarry = np.hstack(( satnumnarry, [ [satnum] ] ))
                        if satnum >= 4:
                            stop = 0
                            X = ap_X = apX
                            Y = ap_Y = apY
                            Z = ap_Z = apZ
                            while math.fabs(ap_X - X) > 1.0e-7 or ap_X == X:
                                stop += 1
                                if stop >= 4:
                                    break
                                count = 0
                                ap_X  = X
                                ap_Y  = Y
                                ap_Z  = Z
                                Ptem = []
                                B = np.empty((0, 4))
                                L = np.empty((0, 1))
                                while 1:
                                    R = math.sqrt(pow((satX[0,count] - ap_X), 2) + pow((satY[0,count] - ap_Y), 2) + pow((satZ[0,count] - ap_Z), 2))
                                    l = -((satX[0,count] - ap_X) / R)
                                    m = -((satY[0,count] - ap_Y) / R)
                                    n = -((satZ[0,count] - ap_Z) / R)
                                    B = np.vstack((B, [l, m, n, 1]))
                                    Ptem.append(pow(math.sin(math.radians(ma[0,count])), 2))
                                    if C2[0,count] != 0 and C1[0,count] != C2[0,count]:
                                        Pl = (f1 * f1 * C1[0,count]) / (f1 * f1 - f2 * f2) - (f2 * f2 * C2[0,count]) / (f1 * f1 - f2 * f2)
                                    else:
                                        Pl = C1[0,count]
                                    L = np.vstack((L, [Pl - R + C_V * Dt[0,count] - Trocor.def_tro_cor(apH, ma[0,count])]))

                                    count += 1
                                    if count == satnum:
                                        break

                                P = np.diag(Ptem)
                                BTP = np.dot(np.transpose(B) , P)
                                BTPB = np.dot( BTP , B)
                                Q = np.linalg.inv( BTPB )
                                D_X = np.dot( np.dot( np.dot( Q , np.transpose(B) ) , P ) , L ) #Coordinate correction
                                X = D_X[0, 0] + ap_X
                                Y = D_X[1, 0] + ap_Y
                                Z = D_X[2, 0] + ap_Z

                            DeltaXYZ = np.array([[X - apX],[Y - apY],[Z - apZ]])
                            ENU = S @ DeltaXYZ
                            if ENU[0, 0] > 25 or ENU[1, 0] > 25 or ENU[2, 0] > 50:
                                ENU_O = np.hstack((ENU_O, [[0], [0], [0]]))
                            else:
                                ENU_O = np.hstack((ENU_O,[ [ENU[0, 0]],[ENU[1, 0]],[ENU[2, 0]] ]))
                            with open(o_path, 'a+') as of:
                                print("%4d\\%02d\\%02d\\%02d\\%02d\\%04.1f %12.4f %12.4f %12.4f %3d %10.5f %10.5f %10.5f"
                                      %(year, month, day, hour, min, sec, X, Y, Z, satnum, ENU[0, 0], ENU[1, 0], ENU[2, 0]),file = of)
                                of.close()
                            break
                        else:
                            break

                    flag = line[0:1]
                    if flag == satsysdict.get(satellite_system):
                        satnum += 1
                        satX = np.hstack(( satX, [ [float(line[6:21])]    ] ))
                        satY = np.hstack(( satY, [ [float(line[22:37])]   ] ))
                        satZ = np.hstack(( satZ, [ [float(line[38:53])]   ] ))
                        C1   = np.hstack(( C1,   [ [float(line[55:69])]   ] ))
                        C2   = np.hstack(( C2,   [ [float(line[71:85])]   ] ))
                        ma   = np.hstack(( ma,   [ [float(line[86:101])]  ] ))
                        Dt   = np.hstack(( Dt,   [ [float(line[102:118])] ] ))


            elif line.find("END") >= 0:
                ex.clear()
                nx.clear()
                ux.clear()
                ex.plot(ENU_O[0, :], color=enucolor[0], linewidth=enulinew[0], marker='.', linestyle=':', ms=enulinew[0])
                ex.set_title('E-W(m)', loc='left', fontsize=8)
                ex.grid(True, linestyle='--', alpha=0.7)
                nx.plot(ENU_O[1, :], color=enucolor[1], linewidth=enulinew[1], marker='.', linestyle=':', ms=enulinew[1])
                nx.set_title('N-S(m)', loc='left', fontsize=8)
                nx.grid(True, linestyle='--', alpha=0.7)
                ux.plot(ENU_O[2, :], color=enucolor[2], linewidth=enulinew[2], marker='.', linestyle=':', ms=enulinew[2])
                ux.set_title('U-D(m)', loc='left', fontsize=8)
                ux.set_xlabel('Epochs')
                ux.grid(True, linestyle='--', alpha=0.7)
                canvas0.draw()

                satn.clear()
                satn.plot(satnumnarry[0, :], color=satncolor[0], linewidth=satnlinew[0])
                satn.set_title('Valid Sat Numbers', loc='left', fontsize=8)
                satn.set_xlabel('Epochs')
                satn.grid(True, linestyle='--', alpha=0.7)
                canvas1.draw()

                f.close()
                plotstate.config(text="Plot   form: %s" % (f_path))
                break

root = tk.Tk()
root.title("KNZ_Plot")
root.geometry('900x450')
root.minsize(500,500)

icon_img = (b'AAABAAEAICAAAAEAIACoEAAAFgAAACgAAAAgAAAAQAAAAAEAIAAAAAAAABAAANIAAADSAAAAAAA'
            b'AAAAAAAAEAQOcBAEDtQQBA6UEAQN6BAEDPgQBAwgEAQMABAEDAQQBAwMEAQMBBAEDAAQBAwAEAQ'
            b'MAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
            b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAQBA60EAQP/BAED/QQBA/8EAQP+BAED3AQBA4cEAQMeBAED'
            b'AAQBAwMEAQMCBAEDAAQBAwAEAQMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
            b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAEDBwQBA8wEAQP/BAED+gQBA/'
            b'4EAQP+BAED/wQBA/cEAQOCBAEDBgQCAwAEAQMBBAEDAAQBAwAAAAAAAAAAAAAAAAAAAAAAAAAAA'
            b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAQMA'
            b'BAEDVQQBA/8EAQP8BAED/wQBA/0EAQP7BAED/AQBA/8EAQPEBAEDFAQBAwAEAQMBBAEDAAAAAAA'
            b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
            b'AAAAAAAAAAAAAAAAQBAwAEAQMOBAED5wQBA/8EAQP+BAED/wQBA/8EAQP+BAED+QQBA/8EAQO7B'
            b'AEDAAQBAwAEAQMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
            b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAEDAgQBAwAEAQO5BAED/wQBA/wEAQP/BAED/wQ'
            b'BA/8EAQP/BAED/AQBA/8EAQMzBAEDAAQBAwIAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
            b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAQMEBAEDAAQBA5MEA'
            b'QP/BAED+wQBA/8EAQP/BAED/wQBA/8EAQP9BAED/wQBAz8EAQMABAEDBQQBAwAEAQMABAEDAAAA'
            b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
            b'AAAQBAwQEAQMABAEDaAQBA/8EAQP7BAED/wQBA/8EAQP/BAED/gQBA/8EAQPqBAEDFAQBAwAEAQ'
            b'MBBAEDAwQBAwAEAQMABAEDAAYAAQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
            b'AAAAAAAAAAAAAAAAAAAAAAABAEDAgQBAwAEAQMnBAED+QQBA/4EAQP8BAED/gQBA/wEAQP5BAED'
            b'/wQBA38EAQMABAEDOgQBAzsEAQMABAEDBAQBAwEEAQMABAEDAAMBAgAAAAAAAAAAAAAAAAAAAAA'
            b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAQMABAEDAwQBAwAEAQOIBAED/wQBA/'
            b'wEAQP/BAED/QQBA/8EAQOhBAIDAAMBBAEEAQPIBAED/wQBA2UEAQMABAEDAwQBAwEEAQMABAEDA'
            b'AQBBAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQBAwAEAQMA'
            b'BAEDAQUAAgAEAQN8BAED6QQBA/QEAQPVBAEDZgQBAwMEAQMABAEDkAQBA/8EAQP5BAED/wQBA5Q'
            b'EAwUABAEDAgQBAwEEAQMABAEDAAQBAwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
            b'AAAAAAAAAABAEDAAQBAwAEAQMABAEDAgQBAwAEAQMLBAEDGAQBAwQEAQMABAEDBAQBA5QEAQP/B'
            b'AED/AQBA/4EAQP6BAED/wQBA6oEAQMDBAEDAQQBAwEEAQMABAEDAAQABAAAAAAAAAAAAAAAAAAA'
            b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAEDAAQBAwAEAQMBBAEDAwQBAwAEAQMCBAEDAAQ'
            b'BAz8EAQPVBAED/wQBA/wEAQP/BAED/wQBA/8EAQP6BAED/wQBA64EAQMDBAEDAQQBAwEEAQMABA'
            b'EDAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAEDAAQBAgAEA'
            b'QIABAEDAQQBAwQEAQMABAEDRQQBA/8EAQP6BAED/gQBA/8EAQP/BAED/wQBA/8EAQP6BAED/wQB'
            b'A6YEAQQABAEDAgQBAwEEAQMABAEDAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
            b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAABAEDAAQBAwMEAQMABAEDbAQBA/8EAQP7BAED/wQBA/8EAQ'
            b'P/BAED/wQBA/8EAQP6BAED/wQBA5UEAQMABAEDAwQBAwAEAQMABAEDAAAAAAAAAAAAAAAAAAAAA'
            b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQBAwAEAQMABAEDAAQBAwMEAQMABAED'
            b'kQQBA/8EAQP6BAED/wQBA/8EAQP/BAED/wQBA/8EAQP7BAED/wQBA38EAQMABAEDBAQAAAAEAQM'
            b'ABAEDAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQBAw'
            b'AEAQMABAEDAQQBAwIEAQMABAEDnwQBA/8EAQP6BAED/wQBA/8EAQP/BAED/wQBA/8EAQP8BAED/'
            b'wQBA2QEAQMABAEDBAQBAwAEAQMABAEDAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
            b'AAAAAAAAAAAAAAAAAAAAAAQBAwAEAQMABAEDAQQBAwIHAQQABAEDnQQBA/8EAQP6BAED/gQBA/8'
            b'EAQP/BAED/wQBA/4EAQP+BAED/QQBA0cEAQMABAEDBAQBAwAEAQMABAECAAAAAAAAAAAAAAAAAA'
            b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAgEBAAQBAwAEAQMABAEDAQQBAwMEAQMAB'
            b'AEDjAQBA/8EAQP7BAED/gQBA/8EAQP/BAED/wQBA/0EAQP/BAED7gQBAysEAQMABAEDAwQBAwAE'
            b'AQMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAgECAAQ'
            b'BAwAEAQMABAEDAQQBAwQEAQMABAEDcwQBA/8EAQP9BAED/QQBA/8EAQP/BAED/wQBA/wEAQP/BA'
            b'ED1gQBAxIEAQMABAEDAgQBAwAEAQMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
            b'AAAAAAAAAAAAAAAAAAAAwECAAQBAwAEAQMABAEDAQQBAwQEAQMABAEDVQQBA/cEAQP/BAED/AQB'
            b'A/8EAQP/BAED/wQBA/sEAQP/BAEDtQAAAAAEAQMCBAEDAQQBBAAEAQMAAAAAAAAAAAAAAAAAAAA'
            b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQBAwAEAQMABAEDAAQBAwQEAQ'
            b'MABAEDNgQBA+QEAQP/BAED+wQBA/8EAQP/BAED/wQBA/sEAQP/BAEDjAQBAwAEAQMEBAEEAAQBA'
            b'wAEAQMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
            b'AAQBAwAEAQMABAADAAQBAwQEAQMABAEDGgQBA8cEAQP/BAED+wQBA/4EAQP/BAED/wQBA/wEAQP'
            b'/BAEDXgQBAwAEAQMEBAEDAAQBAwAEAAMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
            b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAQBAwAEAQMABAEDAAQBAwMEAwIABAEDBQQBA54EAQP/BAED/'
            b'AQBA/0EAQP/BAED/gQBA/8EAQP0BAEDMQQBAwAEAQMDBAEDAAQBAwAAAAAAAAAAAAAAAAAAAAAA'
            b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQBAwAEAQMABAEDAAQBAwI'
            b'EAQMDBAEDAAQBA20EAQP8BAED/wQBA/wEAQP/BAED/AQBA/8EAQPTBAEDDQQBAwAEAQMBBAEDAA'
            b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
            b'AAAAAQBAwAEAQMABAEDAAQBAwEEAQMEBAEDAAQBAz0EAQPkBAED/wQBA/sEAQP+BAED+wQBA/8E'
            b'AQOgBAEDAAQBAwMEAQMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
            b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAMBAgAEAQMABAEDAAQBAwAEAQMEBAEDAAQBAxUEAQO4BA'
            b'ED/wQBA/sEAQP9BAED/AQBA/8EAQNhBAEDAAQBAwQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
            b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAQMABAEDAAQB'
            b'AwAEAQMDBAEDAQQBBAAEAQN6BAED/QQBA/8EAQP6BAED/wQBA+8EAQMiBAEDAAAAAAAAAAAAAAA'
            b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
            b'AAAAAAAAAAAAAEAQMABAEDAAQBAwAEAQMCBAEDBAQBAwAEAQM6BAED2wQBA/8EAQP4BAED/wQBA'
            b'60EAQMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
            b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAEAQMABAEDAAQBAwAEAQMBBAEDBAQBAwAEAQM'
            b'MBAEDlgQBA/8EAQP+BAED/wQBA1UAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
            b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAEDA'
            b'AQBAwAEAQMABAEDAwQBAwMEAQMABAEDPAQBA8kEAQP/BAED1wAAAAAAAAAAAAAAAAAAAAAAAAAA'
            b'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
            b'AAAAAAAAAAAAAAAAAAAAABAEDAAQBAwAEAQMABAEDAQQBAwQEAQMABAEDAwQBA18EAQOvAi///w'
            b'CX//8AC///gBP//4AT//9AC///QAp//0AIv/9AEl//ICEv/wAgl/9IgAv/oQAF/9kAEv/8gAl//'
            b'EAEf/0gAr/+kAFf/0gAr/+kAFf/0gAD/+kAK//0gBX/+QAK//6QBP//SAT//6QC///JAf//9IH/'
            b'//pA///8kP///0g=')

icon_img = b64decode(icon_img)
icon_img = ImageTk.PhotoImage(data=icon_img)
root.tk.call('wm', 'iconphoto', root._w, icon_img)

Drawcards = tk.ttk.Notebook(root)

default_font = tkFont.nametofont("TkDefaultFont")
default_font.configure(family="Segoe UI", size=9)

stateframe = tk.Frame(root)
stateframe.pack(side=tk.BOTTOM, fill=tk.X)
plotstate = ttk.Label(stateframe,text="None")
plotstate.pack(side=tk.LEFT, anchor='w')

Tooptions = tk.Frame(root)
Tooptions.pack(side=tk.TOP, fill=tk.X)

topmenu = tk.Menu(root)
filemenu = tk.Menu(topmenu, tearoff=False)
openmenu = tk.Menu(filemenu, tearoff=False)
openmenu.add_command(label="Open.sp", command=SPP_calculat, accelerator='Ctrl+S')
openmenu.add_command(label="Open.po", command=PO_openfile, accelerator='Ctrl+P')
filemenu.add_cascade(label="Open...", menu=openmenu)
filemenu.add_command(label="Clear", command=Clear_canvas)
filemenu.add_command(label="View", command=View_windows)
filemenu.add_command(label="Exit", command=root.quit, accelerator='Ctrl+E')
topmenu.add_cascade(label="File", menu=filemenu)

optmenu = tk.Menu(topmenu, tearoff=False)
optmenu.add_cascade(label="Options...", command=Opt_windows)
topmenu.add_cascade(label="Options", menu=optmenu)

helpmenu = tk.Menu(topmenu, tearoff=False)
helpmenu.add_command(label="About", command=Abo_windows)
topmenu.add_cascade(label="Help", menu=helpmenu)

root.config(menu=topmenu)

frmFigure0 = tk.Frame(root)
frmFigure0.pack(side=tk.BOTTOM, fill=tk.BOTH, expand=tk.YES)

frmFigure1 = tk.Frame(root)
frmFigure1.pack(side=tk.BOTTOM, fill=tk.BOTH, expand=tk.YES)

plt.rc('font',family='Segoe UI')
fig, (ex, nx, ux) = plt.subplots(3, 1, figsize=(10, 0) ,sharex=True)
fig.subplots_adjust(left=0.06, bottom=0.115, right=0.97, top=0.95)
canvas0 = FigureCanvasTkAgg(fig,frmFigure0)
canvas0.draw()
canvas0.get_tk_widget().pack(side=tk.BOTTOM,fill=tk.BOTH,expand=tk.YES)

toolbar = NavigationToolbar2Tk(canvas0,frmFigure0,pack_toolbar=False)
toolbar.update()
toolbar.pack(side=tk.LEFT)

plt.rc('font',family='Segoe UI')
fig, (satn) = plt.subplots(1, 1, figsize=(10, 0) ,sharex=True)
fig.subplots_adjust(left=0.06, bottom=0.115, right=0.97, top=0.95)
canvas1 = FigureCanvasTkAgg(fig,frmFigure1)
canvas1.draw()
canvas1.get_tk_widget().pack(side=tk.BOTTOM,fill=tk.BOTH,expand=tk.YES)

toolbar = NavigationToolbar2Tk(canvas1,frmFigure1,pack_toolbar=False)
toolbar.update()
toolbar.pack(side=tk.LEFT)

Drawcards.add(frmFigure0, text='ENU-draw')
Drawcards.add(frmFigure1, text='Sat num')
Drawcards.pack(fill=tk.BOTH, expand=True, padx='2px', pady='0px')

root.protocol("WM_DELETE_WINDOW", root.quit)

root.mainloop()