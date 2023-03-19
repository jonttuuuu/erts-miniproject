import ftp_server
import hike
import json
import db
from enum import Enum


Data_base = db.HubDatabase() #init database


#State = Enum('WiFi', 'BLE', 'read_json', 'last_session', 'save')

class State(Enum):
    WiFi = 1
    BLE = 2
    read_json = 3
    last_session = 4
    save = 5

def read_json():
    with open("hikeLogs.json", "r") as f:
        sessions = json.load(f)
    f.close()

    return sessions

def last_session(sessions , last_session_id):
    
    new_sessions = []

    print("session ", sessions, "len ", sessions[-1]['id'])

    r = sessions[-1]['id'] - last_session_id

    for i in range(r):
        
        last_session_id = last_session_id + 1

        s = hike.HikeSession()
        
        s.km = float(sessions[last_session_id-1]['distance'])
        s.steps = int(sessions[last_session_id-1]['stepcount'])
        s.id = int(sessions[last_session_id-1]['id'])
        s.calc_kcal()
        new_sessions.append(s)
        
        print("New session added: ", last_session_id)
        
        
    return new_sessions, last_session_id


    
    
def BLE_process():
    pass

def main():

    current_state = State.WiFi
    s = None
    last_session_id = 0
    while(True):
        ########################################    File download states
        if current_state == State.WiFi:
            try:
                ftp_server.file_process()
                print("File downloaded using WiFi")
                current_state = State.read_json
            except:
                current_state = State.BLE
                break
        if current_state == State.BLE:
            try:
                sessions = BLE_process()
                print("File downloaded using BLE")
                current_state = State.read_json
            except:
                current_state = State.WiFi
                break
        ########################################    File read state
        if current_state == State.read_json:
            try:
                sessions = read_json()
                print("File read")
                current_state = State.last_session
            except:
                current_state = State.WiFi
                break
        ########################################    Reading last session state
        if current_state == State.last_session:
            try:
                s, last_session_id= last_session(sessions, last_session_id)
                print("Last session read")
                current_state = State.save
            except:
                current_state = State.WiFi
                break
        ########################################    Saving last session state to database
        if current_state == State.save:
            try:
                #TODO   improve so it does not owerwrite the whole database
                print("s len ", len(s))
                for i in range(len(s)):
                    Data_base.save(s[i])
                print("Last session saved")
                current_state = State.WiFi
            except:
                current_state = State.WiFi
                break
        
        ########################################    End of states


  
if __name__ == "__main__":
    main()

                                          