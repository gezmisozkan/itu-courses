# Özkan Gezmiş 150200033

import numpy as np
import soundcard as sc
from scipy.signal import find_peaks
import pyautogui
import time

# Define the movement commands
move_up = 'w'
move_down = 's'
move_left = 'a'
move_right = 'd'

mined_tile = True  # The flag to indicate whether the tile is mined or not
last_move = move_up  # Initialize the last move
next_move = move_up  # The next move to perform
next_double_move = None  # The next double move to perform
double_move_flag = False  # The flag to indicate whether the next move is a double move
current_move_index = 0 # Initialize the current move index

mics = sc.all_microphones(include_loopback=True)
default_mic = mics[1]

while True:  # Infinite loop
    print("Recording audio...")
    with default_mic.recorder(samplerate=148000) as mic:
        data = mic.record(numframes=1000000)
    print("Audio recorded.")
    print("Analyzing audio...")
    # Convert the audio data to a one-dimensional array
    data = data.flatten()

    # Perform a Fourier transform
    freq_data = np.fft.fft(data)

    # Find the peaks in the frequency domain signal
    peaks, _ = find_peaks(np.abs(freq_data))

    # Define the moves
    moves = [move_up, move_right, move_left, move_down]

    # # Analyze the peaks to determine whether the tile is clear or not
    if len(peaks) > 1:
        mined_tile = True
        # The monster is near a mined tile, decide the direction based on the last move
        if last_move == move_up:
            next_move = move_down
        elif last_move == move_right:
            next_move = move_left
        elif last_move == move_left:
            next_move = move_right
        elif last_move == move_down:
            next_move = move_up

    else:
        # The tile is clear, move in the next direction in the sequence
        if mined_tile == True:
            next_move = moves[current_move_index]
            current_move_index = (current_move_index + 1) % len(moves)  # Update the current move index
            mined_tile = False
        else:
        # The monster is on a safe spot, move in the same direction as the last move
            next_double_move = moves[current_move_index-1]
            current_move_index = 0
            double_move_flag = True

    if not double_move_flag:  # If the next move is not a double move
        # Perform the move
        pyautogui.keyDown(next_move)
        time.sleep(0.4)
        pyautogui.keyUp(next_move)

    else:  # If the next move is a double move
        # Perform the double move
        pyautogui.keyDown(next_double_move)
        time.sleep(1.75)
        pyautogui.keyUp(next_double_move)
        double_move_flag = False