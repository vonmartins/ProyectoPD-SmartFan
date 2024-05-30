document.addEventListener('DOMContentLoaded', () => {
    const lockUnlockElement = document.getElementById('lockUnlockElement');
    const AutoModeElement = document.getElementById('AutoModeElement');
    const NightModeElement = document.getElementById('NightModeElement');
    const StartStopElement = document.getElementById('StartStopElement');

    function setButtonOn(button) {
        button.classList.add('on');
    }

    function setButtonOff(button) {
        button.classList.remove('on');
    }

    function setLocked() {
        lockUnlockElement.classList.add('locked');
    }

    function setUnlocked() {
        lockUnlockElement.classList.remove('locked');
    }

    function sendLockRequest() {
        fetch('/setLocked')
            .then(response => {
                if (response.ok) {
                    setLocked();
                }
            })
            .catch(error => console.error('Error:', error));
    }

    function sendUnlockRequest() {
        fetch('/setUnlocked')
            .then(response => {
                if (response.ok) {
                    setUnlocked();
                }
            })
            .catch(error => console.error('Error:', error));
    }

    function sendFanOnRequest() {
        fetch('/turnOnFan')
            .then(response => {
                if (response.ok) {
                    console.log('Fan turned on');
                }
            })
            .catch(error => console.error('Error:', error));
    }

    function sendFanOffRequest() {
        fetch('/turnOffFan')
            .then(response => {
                if (response.ok) {
                    console.log('Fan turned off');
                }
            })
            .catch(error => console.error('Error:', error));
    }

    function sendAutoOnRequest() {
        fetch('/activateAutoMode')
            .then(response => {
                if (response.ok) {
                    console.log('Auto mode activated');
                }
            })
            .catch(error => console.error('Error:', error));
    }

    function sendAutoOffRequest() {
        fetch('/deactivateAutoMode')
            .then(response => {
                if (response.ok) {
                    console.log('Auto mode deactivated');
                }
            })
            .catch(error => console.error('Error:', error));
    }

    function sendNightOnRequest() {
        fetch('/activateNightMode')
            .then(response => {
                if (response.ok) {
                    console.log('Night mode activated');
                }
            })
            .catch(error => console.error('Error:', error));
    }

    function sendNightOffRequest() {
        fetch('/deactivateNightMode')
            .then(response => {
                if (response.ok) {
                    console.log('Night mode deactivated');
                }
            })
            .catch(error => console.error('Error:', error));
    }

    function sendIncreaseSpeedRequest() {
        fetch('/increaseSpeed')
            .then(response => {
                if (response.ok) {
                    console.log('Speed increased');
                }
            })
            .catch(error => console.error('Error:', error));
    }

    function sendDecreaseSpeedRequest() {
        fetch('/decreaseSpeed')
            .then(response => {
                if (response.ok) {
                    console.log('Speed decreased');
                }
            })
            .catch(error => console.error('Error:', error));
    }

    document.getElementById('lockButton').addEventListener('click', sendLockRequest);
    document.getElementById('unlockButton').addEventListener('click', sendUnlockRequest);
    document.getElementById('startButton').addEventListener('click', sendFanOnRequest);
    document.getElementById('stopButton').addEventListener('click', sendFanOffRequest);
    document.getElementById('autoOnButton').addEventListener('click', sendAutoOnRequest);
    document.getElementById('autoOffButton').addEventListener('click', sendAutoOffRequest);
    document.getElementById('nightOnButton').addEventListener('click', sendNightOnRequest);
    document.getElementById('nightOffButton').addEventListener('click', sendNightOffRequest);
    document.getElementById('speedUpButton').addEventListener('click', sendIncreaseSpeedRequest);
    document.getElementById('speedDownButton').addEventListener('click', sendDecreaseSpeedRequest);

    async function fetchSensorData() {
        try {
            const response = await fetch('/data');
            const data = await response.json();
            const roundedTemperature = Math.round(data.temperature);
            document.getElementById('temperature').textContent = `${roundedTemperature}º`;
            document.getElementById('humidity').textContent = `${data.humidity}%`;
        } catch (error) {
            console.error('Error fetching sensor data:', error);
        }
    }

    // Llama a la función para obtener datos del sensor
    fetchSensorData();
    // Puedes llamar a fetchSensorData periódicamente si deseas actualizar los datos continuamente
    setInterval(fetchSensorData, 60000); // Actualiza cada 60 segundos

    async function updateDateTime() {
        const now = new Date();
        const dateOptions = { weekday: 'short', year: 'numeric', month: 'short', day: 'numeric' };
        const timeOptions = { hour: '2-digit', minute: '2-digit'};
    
        const dateStr = now.toLocaleDateString('en-US', dateOptions);
        const timeStr = now.toLocaleTimeString('en-US', timeOptions);
    
        document.getElementById('current-date').textContent = dateStr;
        document.getElementById('current-time').textContent = timeStr;
      }
    
      setInterval(updateDateTime, 30000);
      updateDateTime();
});
