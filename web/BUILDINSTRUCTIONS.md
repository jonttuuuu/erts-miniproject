### Instructions for building the frontend software and deploying it on the Raspberry PI

1. In `/web` run `npm run build`. This will generate an optimized production build in `web/build`
2. Copy the `build` directory to the PI with for example `scp`:

```
scp -r build/ pi@<RasPi IP address>:<path to your project directory>
```

3. Navigate to the project directory on the PI
4. Install a webserver to serve the UI assets and start it:

```
sudo npm install -g serve
serve -s build -l 3000
```

5. The app can be accessed by navigating to `http://locahost:3000`
