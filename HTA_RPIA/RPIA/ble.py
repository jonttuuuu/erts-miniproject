from bluepy.btle import Peripheral, UUID, DefaultDelegate
import json



# Set the UUID of the characteristic you want to read/write/subscribe to
CHARACTERISTIC_UUID = UUID('a8c339e5-1d37-4ff6-90ff-a8edeed316e6')


# Set the MAC address of the BLE device you want to connect to
DEVICE_MAC_ADDRESS = '08:3A:F2:69:AB:CE'



def ble_process():
    print("Trying to connect to BLE device")
    dev = Peripheral(DEVICE_MAC_ADDRESS)
    characteristics = dev.getCharacteristics()
    print("Connected to BLE device")
    s = None
    try:
        for char in characteristics:
            
            if(char.uuid == CHARACTERISTIC_UUID ):
        
                s = char.read()
                

        s = s.decode("utf-8")
        s = json.loads(s)
        print("File downloaded using BLE ", s)
    except:
        print("Failed to download file using BLE")
    return s
    
