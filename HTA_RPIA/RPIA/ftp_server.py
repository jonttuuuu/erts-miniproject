import sys
import ftplib

HOSTNAME = "192.168.50.239"
USERNAME = "TTWatch"
PASSWORD = "password"
FILENAME = "hikeLogs.json"


def file_process():
    
    ftp_server = open_connection()
    try:
        download_file(ftp_server, FILENAME)
        close_connection(ftp_server)
        return True
    except KeyboardInterrupt:
        close_connection(ftp_server)
        return False


    


def process():

    # open connection
    print('Opening connection to FTP server...')
    print('Host: ', HOSTNAME)
    print('Username: ', USERNAME)
    print('Password: ', PASSWORD)

    ftp_server = open_connection()

    try:
        while(True):
            
            choice = choice()

            if choice == 1:
                filename = input('Enter filename: ')
                upload_file(ftp_server, filename)

            elif choice == 2:
                filename = input('Enter filename: ')
                download_file(ftp_server, filename)

            elif choice == 3:
                get_file_list(ftp_server)

            elif choice == 4:
                filename = input('Enter filename: ')
                delete_file(ftp_server, filename)

            elif choice == 5:
                filename = input('Enter filename: ')
                get_file_content(ftp_server, filename)

            elif choice == 6:
                close_connection(ftp_server)
                print('\nExiting...')
                break
            
    except KeyboardInterrupt:
        close_connection(ftp_server)
        sys.exit(0)


def choice():
    print('1. Upload File')
    print('2. Download File')
    print('3. Get File List')
    print('4. Delete File from server')
    print('5. Read File Content')
    print('6. Exit')
    choice = int(input('Enter your choice: '))
    return choice




def open_connection():
    # Connect FTP Server
    ftp_server = ftplib.FTP(HOSTNAME, USERNAME, PASSWORD)
    
    # force UTF-8 encoding
    ftp_server.encoding = "utf-8"
    return ftp_server

def upload_file(ftp_server, filename):
    # Read file in binary mode
    with open(filename, "rb") as file:
        # Command for Uploading the file "STOR filename"
        print(f"Uploading {filename} to FTP server...")
        ftp_server.storbinary(f"STOR {filename}", file)
    
def download_file(ftp_server, filename):
    # Write file in binary mode
    with open(filename, "wb") as file:
        # Command for Downloading the file "RETR filename"
        print(f"Downloading {filename} from FTP server...")
        ftp_server.retrbinary(f"RETR {filename}", file.write)

def get_file_list(ftp_server):
    # Get list of files
    print('File List: ')
    ftp_server.dir()

def get_file_content(ftp_server, filename):
    # Display the content of downloaded file
    file= open(filename, "r")
    print('File Content:', file.read())

def delete_file(ftp_server, filename):
    # Delete the file
    print(f"Deleting {filename} from FTP server...")
    ftp_server.delete(filename)

def close_connection(ftp_server):
    # Close the Connection
    print('Closing connection to FTP server...')
    ftp_server.quit()

#if __name__ == "__main__":
#    main()

