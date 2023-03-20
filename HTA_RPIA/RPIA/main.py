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
    
    if ls['id'] == sessions[-1]['id']:
        print("no new session")
         
    else:
        print("new session",ls['id'], sessions[-1]['id'])
        
        if ls['id'] < sessions[-1]['id']: #if new session is added
            print("##########")            
            print("ls ", ls)
            r = sessions[-1]['id'] - ls['id'] #
            print("r ", r)
            
            for i in range(r):
                s = hike.HikeSession()
                
                s.id = int(sessions[ls['id']]['id'])
                
                s.steps = int(sessions[ls['id']]['stepcount'])
                
                s.km = int(sessions[ls['id']]['distance'])
               
                s.calc_kcal()
                
                new_sessions.append(s)
                
                ls = sessions[ls['id']]
                ls['id'] = s.id
                print("new session ", ls , s)              
                               
                
        
        ########################################    If new session is not added
        else:
            print("new session not added")
            r = sessions[-1]['id']
            for i in range(r):
                s = hike.HikeSession()
                
                s.id = int(sessions[ls['id']]['id'])
                
                s.steps = int(sessions[ls['id']]['stepcount'])
                
                s.km = int(sessions[ls['id']]['distance'])
               
                s.calc_kcal()
                
                new_sessions.append(s)
                
                ls = sessions[ls['id']]
                ls['id'] = s.id
                print("new session ", ls , s)    
        
                
    return new_sessions, ls
            
            

    
    
def BLE_process():
    s = ble.ble_process()
    return s

def string_to_json(s):
    json_string = json.loads(s)
    return json_string

def main():

    current_state = State.WiFi
    s = None
    
    ls = string_to_json('{"id": 0, "stepcount": 0, "distance": 0}')
    
    sessions = None
    
    while(True):
        ########################################    File download states
        if current_state == State.WiFi:
            try:
                print("Trying to download file using WiFi")
                response = ftp_server.file_process()
                print("response ", response)
                if response == True:                    
                    print("File downloaded using WiFi")
                    current_state = State.read_json
                else:
                    current_state = State.BLE
                    
            except:
                current_state = State.BLE
                
        if current_state == State.BLE:
            try:
                print("Trying to download file using BLE")
                sessions = BLE_process()
                if sessions != None:
                    print("File downloaded using BLE ", sessions)
                    current_state = State.last_session
                else:
                #print("File downloaded using BLE ", sessions)
                    current_state = State.last_session
            except:
                print("No BLE")
                current_state = State.WiFi
                
        ########################################    File read state
        if current_state == State.read_json:
            try:
                print("Trying to read file")
                sessions = read_json()
                print("File read")
                current_state = State.last_session
            except:
                current_state = State.WiFi
                
        ########################################    Reading last session state
        if current_state == State.last_session:
            try:
                print("Trying to read last session")
                s, ls= last_session_extract(sessions, ls)                
                print("Last session read", len(s))
                if len(s) > 0:
                    current_state = State.save
                else:
                    current_state = State.WiFi
            except:
                current_state = State.WiFi
                
        ########################################    Saving last session state to database
        if current_state == State.save:
            try:
                print("Trying to save last session")
                #TODO   improve so it does not owerwrite the whole database
                print("s len ", len(s))
                for i in range(len(s)):
                    Data_base.save(s[i])
                    print("Session saved", s[i])
                print("Last session saved")
                s = None
                current_state = State.BLE
            except:
                current_state = State.BLE
                
        
        ########################################    End of states



if __name__ == "__main__":
    main()

                                          