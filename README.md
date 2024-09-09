# UART Full-Duplex Communication Implementation

## Overview

This repository contains an implementation of a Full-Duplex communication scheme using UART (Universal Asynchronous Receiver Transmitter). The project involves two main functions for transmitting (`uart_tx()`) and receiving (`uart_rx()`) data between devices, where each data transmission is framed with start, stop, and parity bits. The implementation ensures continuous data exchange without using busy waiting or sleep/delay mechanisms.

## Implementation Details

### Communication Scheme

In asynchronous UART communication, data is framed with specific start, stop, and parity bits:

1. **Start Bit**: '0' to signify the beginning of a frame.
2. **Stop Bit**: '1' to signify the end of a frame. The stop bit duration is `BIT_TIME` (1/50 seconds), resulting in a frequency of 50Hz, similar to the previous lab.
3. **Parity Bit (PB)**: Odd parity, which ensures the number of ones in the data byte is odd.
4. **Data**: 8 bits of data transmitted serially, with the Least Significant Bit (LSB) sent first. The number of bits can vary (up to 12 bits) and should be configurable using `#define` directives.

### Functions

Two functions handle the transmission and reception of framed data:

1. **`uart_tx()`**: This function sends data to the other device using the structure described above.
   - The function works with 1 Byte (8 bits) of data, transmitting the LSB first.
   - After transmitting the frame (start bit, data bits, parity bit, and stop bit), the transmitter waits for a random amount of time before sending the next frame.
   - Note: No sleep, delay, while, or for loops are allowed in this function to prevent busy waiting.

2. **`uart_rx()`**: This function reads the channel pin to detect the start of a transmission.
   - It looks for the first '0' (start bit) to identify the start of a frame.
   - Each bit should be sampled `SAMP_NUM = 3` times for reliable reading.
   - If the frame is corrupt (e.g., wrong parity, incorrect start or stop bits), the frame is dropped, and the channel is ignored for the remainder of the frame time.

### Usage

The UART communication should run indefinitely in a loop, alternating between sending and receiving data. A typical implementation might look like this:

```c
void loop() {
    uart_tx();
    uart_rx();
}
```

### How to Use
1. **Clone the Repository:**
   ```bash
   git clone https://github.com/your-username/uart-full-duplex.git
   cd uart-full-duplex
   ```
   
2. **Compile and Upload to the Device:**
* Use your preferred IDE or toolchain to compile the code.
* Upload the compiled binary to your microcontroller or device.

3. **Run the Code:**
After uploading, the devices will begin communicating using the 'uart_tx()' and 'uart_rx()' functions in a continuous loop.
