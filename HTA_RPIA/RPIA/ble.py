from bluepy.btle import Peripheral, UUID, DefaultDelegate
import time


class NotificationDelegate(DefaultDelegate):
    def handleNotification(self, handle, value):
        notification_callback(handle, value)


# Set the UUID of the characteristic you want to read/write/subscribe to
CHARACTERISTIC_UUID = UUID('xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx')

# Set the MAC address of the BLE device you want to connect to
DEVICE_MAC_ADDRESS = 'XX:XX:XX:XX:XX:XX'

# Subscribe to notifications from a characteristic
def notification_callback(handle, value):
    print(f'Received notification: {value}')


def main():
    # Connect to the device
    device = Peripheral(DEVICE_MAC_ADDRESS)

    # Read from a characteristic
    value = device.readCharacteristic(CHARACTERISTIC_UUID)
    print(f'Received value: {value}')

    # Write to a characteristic
    device.writeCharacteristic(CHARACTERISTIC_UUID, b'Hello, device!')
    print('Value written.')
    

    device.withDelegate(NotificationDelegate())
    device.writeCharacteristic(CHARACTERISTIC_UUID + 1, b'\x01\x00') # Enable notifications
    print('Notifications subscribed.')

    # Wait for some time
    time.sleep(10)

    # Unsubscribe from notifications
    device.writeCharacteristic(CHARACTERISTIC_UUID + 1, b'\x00\x00') # Disable notifications
    print('Notifications unsubscribed.')

    # Disconnect from the device
    device.disconnect()
    print('Disconnected.')

if __name__ == '__main__':  
    main()