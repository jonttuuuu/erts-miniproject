import ftp_server
import hike
import json
import db
from enum import Enum
import ble

Data_base = db.HubDatabase() #init database


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

def last_session_extract(sessions , ls):
    
    print('last session', ls)
    
    
    new_sessions = []

    print("session ", sessions, "len ", sessions[-1]['id'])
    
    if ls == sessions[-1]:
        print("no new session")
        return None, ls  
    else:
        print("new session",ls['id'], sessions[-1]['id'])
        
        if ls['id'] < sessions[-1]['id']: #if new session is added
            print("new sessions added")
            ls = sessions[-1]
            
            r = sessions[-1]['id'] - ls['id']
            
            for i in range(r):
                s = hike.HikeSession()
                
                s.id = int(sessions[ls['id'] + 1]['id'])
                s.stepcount = int(sessions[ls['id'] + 1]['stepcount'])
                s.km = float(sessions[ls['id'] + 1]['distance'])
                s.calc_kcal()
                new_sessions.append(s)
                ls = sessions[ls['id'] + 1]
                print("new session added", new_sessions)
                    
           
        ########################################    If new session is not added
        else:
            print("new session not added")
            r = sessions[-1]['id']
            for i in range(r):
                s = hike.HikeSession()
                
                s.id = int(sessions[ls['id'] + 1]['id'])
                s.stepcount = int(sessions[ls['id'] + 1]['stepcount'])
                s.km = float(sessions[ls['id'] + 1]['distance'])
                s.calc_kcal()
                new_sessions.append(s)
                ls = sessions[ls['id'] + 1]
                
        return new_sessions, ls
            
            

    
    
def BLE_process():
    s = ble.ble_process()
    return s

def string_to_json(s):
    json_string = json.loads(s)
    return json_string

def main():

    current_state = State.BLE
    s = None
    ls = hike.HikeSession() #needs to be a json object
    ls.id = 0
    ls.stepcount = 0
    ls.km = 0
    ls.kcal = 0
    
    sessions = None
    
    while(True):
        ########################################    File download states
        if current_state == State.WiFi:
            try:
                print("Trying to download file using WiFi")
                ftp_server.file_process()
                print("File downloaded using WiFi")
                current_state = State.read_json
            except:
                current_state = State.BLE
                break
        if current_state == State.BLE:
            try:
                print("Trying to download file using BLE")
                sessions = BLE_process()
                print("File downloaded using BLE ", sessions)
                current_state = State.last_session
            except:
                current_state = State.WiFi
                break
        ########################################    File read state
        if current_state == State.read_json:
            try:
                print("Trying to read file")
                sessions = read_json()
                print("File read")
                current_state = State.last_session
            except:
                current_state = State.WiFi
                break
        ########################################    Reading last session state
        if current_state == State.last_session:
            try:
                print("Trying to read last session")
                s, ls= last_session_extract(sessions, ls)
                print("Last session read")
                current_state = State.save
            except:
                current_state = State.WiFi
                break
        ########################################    Saving last session state to database
        if current_state == State.save:
            try:
                print("Trying to save last session")
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

                                          