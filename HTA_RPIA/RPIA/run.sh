#!/bin/bash

# this script launches the backend and frontend
echo "Launching RPIA"

# launch backend
echo "Launching backend"
cd backend/
python3 main.py &
python3 wserver.py &

# launch frontend
echo "Launching frontend"
cd ../web/
npm run start &

echo "RPIA launched!!"
