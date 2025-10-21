import csv
import sys
import math

def read_csv(filename):
    """Read oscilloscope CSV file (skip 2 headers)."""
    times, ch1, ch2 = [], [], []
    with open(filename, "r") as f:
        reader = csv.reader(f)
        # skip two header lines
        next(reader)
        next(reader)
        for row in reader:
            if len(row) < 3:
                continue
            try:
                t = float(row[0])
                v1 = float(row[1]) if row[1] != "" else None
                v2 = float(row[2]) if row[2] != "" else None
                times.append(t)
                ch1.append(v1)
                ch2.append(v2)
            except ValueError:
                continue
    return times, ch1, ch2


def decode_uart(time, voltages, baud=250000, vth=None):
    """Decode one UART channel from sampled waveform."""
    if all(v is None for v in voltages):
        return []

    # Determine threshold if not provided
    if vth is None:
        valid = [v for v in voltages if v is not None]
        vmin, vmax = min(valid), max(valid)
        vth = (vmin + vmax) / 2.0

    # Convert voltages to logic (1/0)
    logic = []
    for v in voltages:
        if v is None:
            logic.append(logic[-1] if logic else 1)  # assume idle high
        else:
            logic.append(1 if v > vth else 0)

    bit_time = 1.0 / baud
    decoded = []
    i = 0
    while i < len(logic) - 1:
        # look for falling edge = start bit
        if logic[i] == 1 and logic[i+1] == 0:
            t0 = time[i+1]
            bits = []
            for n in range(1, 9):  # data bits
                sample_t = t0 + (n + 0.5) * bit_time
                # find nearest sample
                idx = min(range(len(time)), key=lambda j: abs(time[j] - sample_t))
                bits.append(logic[idx])
            # stop bit sample
            stop_t = t0 + (9.5) * bit_time
            idx = min(range(len(time)), key=lambda j: abs(time[j] - stop_t))
            stop_bit = logic[idx]

            if stop_bit == 1:  # valid stop bit
                value = 0
                for n, b in enumerate(bits):
                    value |= (b << n)
                decoded.append(value)
                # advance i to end of frame
                i = idx
            else:
                i += 1
        else:
            i += 1
    return decoded


def main(filename):
    time, ch1, ch2 = read_csv(filename)

    data1 = decode_uart(time, ch1)
    data2 = decode_uart(time, ch2)

    print("Channel 1:", " ".join(f"{b:02X}" for b in data1))
    print("Channel 2:", " ".join(f"{b:02X}" for b in data2))


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python decode_uart.py <oscilloscope.csv>")
        sys.exit(1)
    main(sys.argv[1])
