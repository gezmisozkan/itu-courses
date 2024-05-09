# Özkan Gezmiş 150200033

import numpy as np
import librosa
from scipy.io import wavfile

# Load the song
song, song_rate = librosa.load('bayrakfm.wav', sr=None)

# Divide the song into one-second parts
song_parts = np.array_split(song, len(song) // song_rate)

# Create an empty list for the message
message = []

# For each part...
for i, song_part in enumerate(song_parts):
    # Perform a Fourier transform
    song_freq = np.fft.fft(song_part)

    # Extract the second half of the transformed song
    message_freq_first_half = song_freq[len(song_freq) // 2:]

    # Create a new array for the transformed message
    message_freq = np.zeros_like(song_freq)
    message_freq[:len(message_freq_first_half)] = message_freq_first_half

    # Perform an inverse Fourier transform
    message_part = np.fft.ifft(message_freq)

    # Append the real part of the message part to the message list
    message.append(message_part.real)

# Convert the message list to a numpy array
message = np.concatenate(message)

# Normalize the message to the range of np.int16
message_normalized = np.int16((message / np.max(np.abs(message))) * 32767)

# Write the result back to a .wav file
wavfile.write('message.wav', song_rate, message_normalized)