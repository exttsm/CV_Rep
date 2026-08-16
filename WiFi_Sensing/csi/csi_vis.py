import serial
import numpy as np
import math
import matplotlib

# --- NEW: Force PyCharm to use a standalone interactive window ---
matplotlib.use('TkAgg')
# -----------------------------------------------------------------

import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time

# --- CHANGE THIS TO YOUR CURRENT PORT ---
COM_PORT = "COM3"
# ----------------------------------------
BAUD_RATE = 115200

# ... [The rest of your code remains exactly the same from here down] ...


def process_csi_payload(payload_string):
    raw_data = list(map(int, payload_string.strip().split(',')))
    amplitudes = []

    for i in range(0, len(raw_data), 2):
        if i + 1 >= len(raw_data):
            break
        imaginary = raw_data[i]
        real = raw_data[i + 1]
        amplitude = math.sqrt((real ** 2) + (imaginary ** 2))
        amplitudes.append(amplitude)

    structured_vector = np.fft.fftshift(amplitudes)

    if len(structured_vector) > 0:
        center_index = len(structured_vector) // 2
        structured_vector[center_index] = 0

    return structured_vector


def main():
    print(f"Starting Live CSI Analyzer on {COM_PORT}...")

    fig, ax = plt.subplots(figsize=(10, 6))
    line, = ax.plot([], [], lw=2, color='cyan')

    fig.patch.set_facecolor('#1e1e1e')
    ax.set_facecolor('#1e1e1e')
    ax.tick_params(colors='white')
    ax.xaxis.label.set_color('white')
    ax.yaxis.label.set_color('white')
    ax.title.set_color('white')

    ax.set_title("Live Wi-Fi CSI Amplitude")
    ax.set_xlabel("Subcarrier Index (Negative to Positive)")
    ax.set_ylabel("Amplitude")
    ax.set_ylim(0, 100)

    try:
        ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=0.1)
        ser.setDTR(False)
        ser.setRTS(False)
    except Exception as e:
        print(f"\n[CRITICAL ERROR] Could not open {COM_PORT}.")
        print(f"Details: {e}")
        print("Please check your COM port number and ensure it is not being used by another program.")
        time.sleep(5)  # Pause for 5 seconds so you can read the error before the terminal closes
        return

    def update(frame):
        try:
            line_data = ""

            while ser.in_waiting > 0:
                line_data = ser.readline().decode('utf-8', errors='ignore').strip()

            if ',' in line_data:
                amplitude_vector = process_csi_payload(line_data)
                num_subcarriers = len(amplitude_vector)

                if num_subcarriers > 0:
                    half_point = num_subcarriers // 2
                    x_data = np.arange(-half_point, half_point)

                    line.set_data(x_data, amplitude_vector)
                    ax.set_xlim(-half_point, half_point - 1)

                    max_amp = max(amplitude_vector)
                    new_ceiling = max(50, max_amp + (max_amp * 0.2))
                    ax.set_ylim(0, new_ceiling)

        except Exception:
            pass

        return line,

    ani = animation.FuncAnimation(fig, update, interval=20, blit=False, cache_frame_data=False)

    try:
        # Added block=True to forcefully prevent the window from closing instantly
        plt.show(block=True)
    except KeyboardInterrupt:
        print("\nAnalysis stopped by user.")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("Serial port closed safely.")


if __name__ == '__main__':
    main()