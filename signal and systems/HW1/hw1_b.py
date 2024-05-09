# Özkan Gezmiş 150200033

from scipy.io import wavfile
from scipy import signal
import numpy as np
import librosa

# Load the hidden message
message, message_rate = librosa.load('message.wav', sr=None)

# Define the system H(z) = (1 - 7/4*z^-1 - 1/2*z^-2) / (1 + 1/4*z^-1 - 1/8*z^-2)
numerator = [1, -7/4, -1/2]
denominator = [1, 1/4, -1/8]

# Apply the system to the hidden message
filtered_message = signal.lfilter(numerator, denominator, message)

# Normalize the message to the range of np.int16
message_normalized = np.int16((filtered_message / np.max(np.abs(filtered_message))) * 32767)

# Write the result back to a .wav file
wavfile.write('filtered_message.wav', message_rate, message_normalized)