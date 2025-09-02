# Student Tracker

This project is a comprehensive student tracking system consisting of a web application for monitoring and a firmware component for GPS data collection. The web app displays student locations on a Google Map, manages student records, and handles teacher profiles. The firmware runs on an Arduino-based device to fetch GPS coordinates and send them to Adafruit IO via MQTT.

## Main Functionalities

### Web Application
- **Map View**: Displays the teacher's location and student locations on Google Maps. Fetches real-time GPS data from Adafruit IO.
- **Geofencing**: Alerts when students are outside a defined safe radius (default 25 meters).
- **Student Management**: View, add, delete, and filter student records with status indicators (Safe/Warning).
- **Profile Management**: Update and save teacher profile information using local storage.
- **Real-time Updates**: Polls Adafruit IO every 4 seconds for location updates.

### Firmware
- **GPS Tracking**: Uses A9G module to acquire latitude and longitude.
- **MQTT Publishing**: Sends GPS data to Adafruit IO feeds for remote access.
- **Network Connectivity**: Handles GPRS connection for data transmission.

## Hardware Parts

- **ATmega328P**: Microcontroller used for main processing.
- **A9G Pudding Board**: Used for GPS and GSM functionalities.

## Prerequisites

- Node.js and npm installed (for serving the web app)
- Adafruit IO account and API keys
- Google Maps API key
- Arduino IDE (for firmware flashing)
- ATmega328P board and A9G module (for hardware setup)

## Setup

1. **Clone the repository:**

   ```bash
   git clone https://github.com/KaveeshaDharmaratne/Student-Tracker
   cd Student-Tracker
   ```

2. **Install `http-server` to serve the project locally:**

   ```bash
   npm install -g http-server
   ```

3. **Start the server:**

   ```bash
   http-server
   ```

4. **Access the application:**

   Open a web browser and navigate to `http://localhost:8080`.

## Configuration

### Adafruit IO APIs

The application fetches students' latitude, longitude, and name from Adafruit IO APIs. You need to replace the API URLs with your own Adafruit IO feed URLs.

- **Latitude API URL:**
  Replace the URL in the `fetchGPSData` function in `Frontend/Components/map.html`:
  ```javascript
  const responseLatitude = await fetch('https://io.adafruit.com/api/v2/your-username/feeds/latitude-feed');
  ```

- **Longitude API URL:**
  Replace the URL in the `fetchGPSData` function in `Frontend/Components/map.html`:
  ```javascript
  const responseLongitude = await fetch('https://io.adafruit.com/api/v2/your-username/feeds/longitude-feed');
  ```

- **Name API URL:**
  Replace the URL in the `fetchGPSData` function in `Frontend/Components/map.html`:
  ```javascript
  const responseName = await fetch('https://io.adafruit.com/api/v2/your-username/feeds/name-feed');
  ```

### Google Maps API Key

Replace the Google Maps API key in the script tag at the bottom of `Frontend/Components/map.html`:
```html
<script src="https://maps.googleapis.com/maps/api/js?key=YOUR_GOOGLE_MAPS_API_KEY" async defer></script>
```

### Firmware Configuration

In `Firmware/a9g-gps-v1.ino`, update the following placeholders:
- `<CARRIER-NAME>`: Your mobile carrier's APN (e.g., hutch3g, airtellive).
- `<ADAFRUIT-CLIENT-ID>`: Your Adafruit IO client ID.
- `<ADAFRUIT-USERNAME>`: Your Adafruit IO username.
- `<YOUR-ADAFRUIT-API-KEY>`: Your Adafruit IO API key.
- `<MQTT-END-POINT>`: The MQTT topic for publishing data (e.g., username/feeds/latitude).

Flash the firmware to your ATmega328P board using Arduino IDE.

### Geofence and Time Interval

- **Geofence Radius:**
  Define the geofence radius in meters by modifying the `safeDistance` variable in `Frontend/Components/map.html`:
  ```javascript
  const safeDistance = 25; // x meters
  ```

- **Time Interval to Check APIs:**
  Define the time interval to fetch GPS data from the APIs by modifying the interval in milliseconds in `Frontend/Components/map.html`:
  ```javascript
  setInterval(fetchGPSData, 4000); // Fetch GPS data every 4000 milliseconds (4 seconds)
  ```

## Usage

1. **Open the application:**
   Open a web browser and navigate to `http://localhost:8080`.

2. **Allow location access:**
   When prompted, allow the browser to access your location. This will set the teacher's location on the map.

3. **View student locations:**
   The map will display the teacher's location and the students' locations. The students' markers will be updated every 4 seconds based on the data fetched from the Adafruit IO APIs.

4. **Geofence alerts:**
   If a student is outside the defined geofence radius, their marker will turn red, and an alert will be displayed.

5. **Manage Students:**
   Navigate to the Students page to view, add, delete, or filter student records.

6. **Update Profile:**
   Use the Profile page to update your information.

## Mobile Access

To access the application on your mobile device, you have to use ngrok to expose your local server to the internet (HTTP localhost can't get location permission on mobile browsers. So use an HTTPS exposer like ngrok).

1. **Install ngrok:**

   Download and install ngrok from [ngrok.com](https://ngrok.com/).

2. **Expose your local server:**

   Start your local server using `http-server` and then run ngrok to expose it:
   ```sh
   http-server
   ngrok http 8080
   ```

3. **Access the application on your mobile device:**

   ngrok will provide a public URL (e.g., `https://abcd1234.ngrok.io`). Open this URL in your mobile browser to access the application.

## Troubleshooting

- **Location access denied:**
  If the browser does not prompt for location access, ensure that location services are enabled in your browser settings.

- **API errors:**
  If there are errors fetching data from the Adafruit IO APIs, check the API URLs and ensure that your Adafruit IO account and feeds are correctly configured.

- **Map not displaying:**
  If the map does not display, check the Google Maps API key and ensure that it is correctly configured and has the necessary permissions.

- **Firmware Issues:**
  Ensure the A9G module is properly connected and the carrier settings are correct. Check serial output for debugging.

## Limitations

- The ATmega328P is not powerful enough to support real-time longitude and latitude updates while processing the A9G module's responses via software-serial. This makes code optimization challenging.
- The A9G module experiences signal issues indoors but works well outdoors with minimal obstructions, achieving up to a 5-meter accuracy radius.
- Optimal polling rate or map update interval is 3-5 seconds, depending on the quality of the carrier connection.