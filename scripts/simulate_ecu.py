import serial
import random
import time

BATTERY_VOLTAGE_ADDR = 0x1404
SPEED_ADDR = 0x154B
RPM_ADDR = 0x140B
COOLANT_ADDR = 0x1405
IGNITION_ADVANCE_ADDR = 0x1489
AIRFLOW_SENSOR_ADDR = 0x1400
ENGINE_LOAD_ADDR = 0x1414
THROTTLE_ADDR = 0x1487
INJECTOR_PULSE_WIDTH_ADDR = 0x15F0
ISU_DUTY_VALVE_ADDR = 0x158A
TIMING_CORRECTION_ADDR = 0x1530
O2_AVERAGE_ADDR = 0x1403
AF_CORRECTION_ADDR = 0x1488
ATMOSPHERIC_PRESSURE_ADDR = 0x1516
MANIFOLD_PRESSURE_ADDR = 0x00BE
BOOST_SOLENOID_ADDR = 0x144D
INPUT_SWITCHES_ADDR = 0x15A8
IO_SWITCHES_ADDR = 0x15A9
ACTIVE_TROUBLE_CODE_ONE_ADDR = 0x0047
ACTIVE_TROUBLE_CODE_TWO_ADDR = 0x0048
ACTIVE_TROUBLE_CODE_THREE_ADDR = 0x0049
STORED_TROUBLE_CODE_ONE_ADDR = 0x1604
STORED_TROUBLE_CODE_TWO_ADDR = 0x1605
STORED_TROUBLE_CODE_THREE_ADDR = 0x1606

# Map addresses to names for easy lookup
ECU_ADDRESSES = {
    BATTERY_VOLTAGE_ADDR: "BATTERY_VOLTAGE",
    SPEED_ADDR: "SPEED",
    RPM_ADDR: "RPM",
    COOLANT_ADDR: "COOLANT",
    IGNITION_ADVANCE_ADDR: "IGNITION_ADVANCE",
    AIRFLOW_SENSOR_ADDR: "AIRFLOW_SENSOR",
    ENGINE_LOAD_ADDR: "ENGINE_LOAD",
    THROTTLE_ADDR: "THROTTLE",
    INJECTOR_PULSE_WIDTH_ADDR: "INJECTOR_PULSE_WIDTH",
    ISU_DUTY_VALVE_ADDR: "ISU_DUTY_VALVE",
    TIMING_CORRECTION_ADDR: "TIMING_CORRECTION",
    O2_AVERAGE_ADDR: "O2_AVERAGE",
    AF_CORRECTION_ADDR: "AF_CORRECTION",
    ATMOSPHERIC_PRESSURE_ADDR: "ATMOSPHERIC_PRESSURE",
    MANIFOLD_PRESSURE_ADDR: "MANIFOLD_PRESSURE",
    BOOST_SOLENOID_ADDR: "BOOST_SOLENOID",
    INPUT_SWITCHES_ADDR: "INPUT_SWITCHES",
    IO_SWITCHES_ADDR: "IO_SWITCHES",
    ACTIVE_TROUBLE_CODE_ONE_ADDR: "ACTIVE_TROUBLE_CODE_ONE",
    ACTIVE_TROUBLE_CODE_TWO_ADDR: "ACTIVE_TROUBLE_CODE_TWO",
    ACTIVE_TROUBLE_CODE_THREE_ADDR: "ACTIVE_TROUBLE_CODE_THREE",
    STORED_TROUBLE_CODE_ONE_ADDR: "STORED_TROUBLE_CODE_ONE",
    STORED_TROUBLE_CODE_TWO_ADDR: "STORED_TROUBLE_CODE_TWO",
    STORED_TROUBLE_CODE_THREE_ADDR: "STORED_TROUBLE_CODE_THREE"
}

ECU_PORT = "/dev/cu.usbmodem14201"
BAUD_RATE = 9600

ecu_sim = serial.Serial(port=ECU_PORT, baudrate=BAUD_RATE, bytesize=8, parity='N', stopbits=1, timeout=1)

def simulate_ecu():
    print(f"ECU Simulator running on {ECU_PORT} at {BAUD_RATE} baud...")

    while True:
        if ecu_sim.in_waiting >= 4:  # Wait for a full 4-byte request (0x78 MSB LSB 0x00)
            request = ecu_sim.read(4)  # Read the 4-byte request from the Arduino
            print(f"Received request: {request.hex()}")

            # Extract the MSB and LSB from the first 4 bytes to get the address
            msb = request[1]
            lsb = request[2]
            address = (msb << 8) | lsb  # Combine MSB and LSB to form the full address
            
            # Check if the received address matches any of the defined addresses
            if address in ECU_ADDRESSES:
                print(f"Received valid read command for address: {hex(address)} ({ECU_ADDRESSES[address]})")

                # Generate a random response value
                response_value = random.randint(0, 255)
                print(f"Generated random response value: {response_value}")

                # Send the response back to Arduino (ECU), format: msb lsb data
                response = bytes([msb, lsb, response_value])
                ecu_sim.write(response)
                print(f"Sent response: {response.hex()}")

            else:
                print(f"Invalid address: {hex(address)}")
                response_value = random.randint(0, 255)
                
        time.sleep(1)

simulate_ecu()
