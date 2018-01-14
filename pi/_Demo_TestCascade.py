from Trekking.DebugWindow import *
from Trekking.Detection import *
import tkinter as tk
from tkinter import filedialog
import cv2

root = tk.Tk()
root.withdraw()

file_path = filedialog.askopenfilename(initialdir = "cascades/", filetypes=[("Cascade files","*.xml")])
    
window = DebugWindow(True, "debug", 640, 368)
detection = Detection(file_path, True, (255, 0, 0), 1.1, 3, 0)
window.open()

while True:
    file_path = filedialog.askopenfilename()
    frame = cv2.imread(file_path)
    objects = detection.detect(frame, (0, 0, 0), (360, 255, 255))
    print(len(objects))
    window.update(frame)
    cv2.waitKey(1000)
