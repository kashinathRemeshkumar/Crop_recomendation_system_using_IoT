#ifndef HTML_CONTENT_H
#define HTML_CONTENT_H

const char* htmlContent = R"rawliteral(
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Crop Input Form</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #121212; /* Dark background */
            color: #ffffff; /* Light text color */
            margin: 0;
            padding: 20px;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
        }
        .container {
            background: #1e1e1e; /* Darker container */
            padding: 30px;
            border-radius: 8px;
            box-shadow: 0 4px 10px rgba(0, 0, 0, 0.5);
            width: 300px;
        }
        h1 {
            color: #ffffff;
            text-align: center;
            margin-bottom: 20px;
        }
        label {
            font-weight: bold;
            margin-top: 10px;
            display: block;
            color: #ffffff; /* Light label color */
        }
        input[type='text'], input[type='range'] {
            width: calc(100% - 20px);
            padding: 10px;
            margin: 10px 0;
            border: 1px solid #444; /* Dark border */
            border-radius: 4px;
            background-color: #333; /* Dark input background */
            color: #ffffff; /* Light input text */
        }
        input[type='submit'] {
            background-color: #6200ea; /* Purple button */
            color: white;
            padding: 10px;
            border: none;
            border-radius: 4px;
            cursor: pointer;
            width: 100%;
            font-size: 16px;
        }
        input[type='submit']:hover {
            background-color: #3700b3; /* Darker purple on hover */
        }
        .range-container {
            display: flex;
            align-items: center;
            justify-content: space-between;
        }
        .range-container input[type='text'] {
            width: 60px;
        }
    </style>
    <script>
        function showAlert(message) {
            alert(message);
            // Clear input fields after the alert
            document.getElementById('nitrogenValue').value = '';
            document.getElementById('phosphorusValue').value = '';
            document.getElementById('potassiumValue').value = '';
            document.getElementById('humidityValue').value = '';
            document.getElementById('pHInput').value = '7.0';
            document.getElementById('pHSlider').value = '7.0';
            document.getElementById('rainfallValue').value = '';
            document.getElementById('temperatureValue').value = '';
        }
    </script>
</head>
<body>
    <div class="container">
        <h1>Input Form</h1>
        <form action="/submit" method="POST" onsubmit="event.preventDefault(); submitForm();">
            <label for="nitrogenValue">N (Nitrogen):</label>
            <input type="text" id="nitrogenValue" name="nitrogenValue" required>

            <label for="phosphorusValue">P (Phosphorus):</label>
            <input type="text" id="phosphorusValue" name="phosphorusValue" required>

            <label for="potassiumValue">K (Potassium):</label>
            <input type="text" id="potassiumValue" name="potassiumValue" required>

            <label for="humidityValue">Humidity (%):</label>
            <input type="text" id="humidityValue" name="humidityValue" required>

            <label for="pHValue">pH Level:</label>
            <div class="range-container">
                <input type="range" id="pHSlider" name="pHValue" min="0" max="14" step="0.1" value="7.0" oninput="document.getElementById('pHInput').value = this.value;">
                <input type="text" id="pHInput" name="pHInput" value="7.0" oninput="document.getElementById('pHSlider').value = this.value;" required>
            </div>

            <label for="rainfallValue">Rainfall (mm):</label>
            <input type="text" id="rainfallValue" name="rainfallValue" required>

            <label for="temperatureValue">Temperature (°C):</label>
            <input type="text" id="temperatureValue" name="temperatureValue" required>

            <input type="submit" value="Submit">
        </form>
    </div>

    <script>
        function submitForm() {
            const formData = new FormData(document.querySelector('form'));
            const xhr = new XMLHttpRequest();
            xhr.open('POST', '/submit', true);
            xhr.onload = function() {
                if (xhr.status === 200) {
                    // Parse the response to get the message
                    const response = xhr.responseText;
                    showAlert(response);
                }
            };
            xhr.send(formData);
        }

        function showAlert(message) {
            alert(message);
            // Clear input fields after the alert
            document.getElementById('nitrogenValue').value = '';
            document.getElementById('phosphorusValue').value = '';
            document.getElementById('potassiumValue').value = '';
            document.getElementById('humidityValue').value = '';
            document.getElementById('pHInput').value = '7.0';
            document.getElementById('pHSlider').value = '7.0';
            document.getElementById('rainfallValue').value = '';
            document.getElementById('temperatureValue').value = '';
        }
    </script>
</body>
</html>
)rawliteral";

#endif // HTML_CONTENT_H