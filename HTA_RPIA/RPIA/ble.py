from bluepy.btle import Peripheral, UUID, DefaultDelegate
import json



# Set the UUID of the characteristic you want to read/write/subscribe to
CHARACTERISTIC_UUID = UUID('a8c339e5-1d37-4ff6-90ff-a8edeed316e6')


# Set the MAC address of the BLE device you want to connect to
DEVICE_MAC_ADDRESS = '08:3A:F2:69:AB:CE'



def ble_process():
    dev = Peripheral(DEVICE_MAC_ADDRESS)
    characteristics = dev.getCharacteristics()

    s = None

    for char in characteristics:
        
        if(char.uuid == CHARACTERISTIC_UUID ):
        # print("=== !CHARACTERISTIC_UUID matched! ==")
            #print(char)
            #print(dir(char))
            #print(char.getDescriptors)
            #print(char.propNames)
            #print(char.properties)
            #print(type(char.read()))
            s = char.read()
            #print(char.read())

    s = s.decode("utf-8")
    s = json.loads(s)
    
    return s
    
