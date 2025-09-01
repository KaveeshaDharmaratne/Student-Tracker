# Student Tracker

This project is a web application that tracks the location of students and displays it on a Google Map. The teacher's location is obtained using the device's geolocation, and the students' locations are fetched from Adafruit IO APIs.

## Prerequisites

- Node.js and npm installed
- Adafruit IO account and API keys
- Google Maps API key

## Setup

1. **Clone the repository:**

   git clone <repository-url>
   cd Student-Tracker

2. **Install `http-server` to serve the project locally:**

   npm install -g http-server

3. **Start the server:**

   http-server

4. **Access the application:**

   Open a web browser and navigate to `http://localhost:8080`.

## Configuration

### Adafruit IO APIs

The application fetches students' latitude, longitude, and name from Adafruit IO APIs. You need to replace the API URLs with your own Adafruit IO feed URLs.

- **Latitude API URL:**
  Replace the URL in the `fetchGPSData` function:
  ```javascript
  const responseLatitude = await fetch('https://io.adafruit.com/api/v2/your-username/feeds/latitude-feed');
  ```

- **Longitude API URL:**
  Replace the URL in the `fetchGPSData` function:
  ```javascript
  const responseLongitude = await fetch('https://io.adafruit.com/api/v2/your-username/feeds/longitude-feed');
  ```

- **Name API URL:**
  Replace the URL in the `fetchGPSData` function:
  ```javascript
  const responseName = await fetch('https://io.adafruit.com/api/v2/your-username/feeds/name-feed');
  ```

### Google Maps API Key

Replace the Google Maps API key in the script tag at the bottom of the map.html file:
```html
<script src="https://maps.googleapis.com/maps/api/js?key=YOUR_GOOGLE_MAPS_API_KEY" async defer></script>
```

### Geofence and Time Interval

- **Geofence Radius:**
  Define the geofence radius in meters by modifying the `safeDistance` variable:
  ```javascript
  const safeDistance = 25; // x meters
  ```

- **Time Interval to Check APIs:**
  Define the time interval to fetch GPS data from the APIs by modifying the interval in milliseconds:
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

## Mobile Access

To access the application on your mobile device, you have to use ngrok to expose your local server to the internet.(http local hosts cant get location permission on mobile browsers. So use a https exposer like ngrok)

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
