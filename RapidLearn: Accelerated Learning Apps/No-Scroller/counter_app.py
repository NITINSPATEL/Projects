import tkinter as tk
import pygame
import time

class CursorActivityMonitor:
    def __init__(self, root):
        self.root = root
        self.prev_x = 0
        self.prev_y = 0
        self.counter_on = False
        self.monitor_duration = 10  # Default duration set to 10 seconds
        self.last_activity_time = time.time()  # To store the last activity time

    def beep(self):
        pygame.mixer.init()
        pygame.mixer.music.load("/home/nitinspatel/Downloads/Mgs alert -Sound effects.mp3")
        pygame.mixer.music.play()

    def count(self):
        if self.counter_on:
            current_x, current_y = self.root.winfo_pointerx(), self.root.winfo_pointery()
            if current_x != self.prev_x or current_y != self.prev_y:
                self.last_activity_time = time.time()
            elapsed_time = time.time() - self.last_activity_time
            if elapsed_time >= self.monitor_duration:
                self.beep()
                self.last_activity_time = time.time()  # Reset last activity time
            self.prev_x, self.prev_y = current_x, current_y
            self.root.after(100, self.count)  # Check every 100 milliseconds
        else:
            self.counter_on = False

    def start_counter(self):
        self.counter_on = True
        self.prev_x, self.prev_y = self.root.winfo_pointerx(), self.root.winfo_pointery()
        try:
            self.monitor_duration = int(entry.get())  # Fetch user input in seconds
        except ValueError:
            # Handling invalid input, set to default if input is not a number
            self.monitor_duration = 10
            entry.delete(0, tk.END)
            entry.insert(0, "10")  # Reset entry to default value
        self.count()

    def stop_counter(self):
        self.counter_on = False

root = tk.Tk()
root.title("Cursor Activity Monitor")

monitor = CursorActivityMonitor(root)

start_button = tk.Button(root, text="Start Monitoring", command=monitor.start_counter)
start_button.pack()

stop_button = tk.Button(root, text="Stop Monitoring", command=monitor.stop_counter)
stop_button.pack()

# Entry widget for user input
entry_label = tk.Label(root, text="Enter monitoring time (in seconds):")
entry_label.pack()

entry = tk.Entry(root)
entry.pack()

root.mainloop()

